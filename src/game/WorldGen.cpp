#include "WorldGen.h"
LOG_MODULE(wgen);
using namespace glm;

void WorldGenerator::gen_chunk(cpos_t cpos, Chunk *target, World &world) const {
    Chunk* c;
    c = world.chunkAt(cpos+IV2_NORTH);
    if (c) c->mark();
    c = world.chunkAt(cpos+IV2_SOUTH);
    if (c) c->mark();
    c = world.chunkAt(cpos+IV2_EAST);
    if (c) c->mark();
    c = world.chunkAt(cpos+IV2_WEST);
    if (c) c->mark();
    this->abs_gen_chunk(cpos, target, world);
}

void BasicWorldGen::abs_gen_chunk(cpos_t cpos, Chunk *target, World &world) const
{
    bpos_t pos = cpos_to_bpos(cpos);
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            int x = pos.x + i; int z = pos.z + j;
            int y = glm::floor(noise.value_octave(x, z, 6) * 6.);
            *(world.blockAtLocalNoFlag(bpos_t{i, y, j}, target)) = *Blocks::GRASS;
        }
    }
}

float trilinear_sample_cave(const float* noise, int nx, int ny, int nz,
    int x, int y, int z, int res, int stride_y, int stride_z) {
    int xi = x / res, yi = y / res, zi = z / res;
    float fx = float(x % res) / res;
    float fy = float(y % res) / res;
    float fz = float(z % res) / res;

    int idx000 = xi * stride_y * stride_z + yi * stride_z + zi;
    int idx100 = (xi+1) * stride_y * stride_z + yi * stride_z + zi;
    int idx010 = xi * stride_y * stride_z + (yi+1) * stride_z + zi;
    int idx110 = (xi+1) * stride_y * stride_z + (yi+1) * stride_z + zi;
    int idx001 = xi * stride_y * stride_z + yi * stride_z + (zi+1);
    int idx101 = (xi+1) * stride_y * stride_z + yi * stride_z + (zi+1);
    int idx011 = xi * stride_y * stride_z + (yi+1) * stride_z + (zi+1);
    int idx111 = (xi+1) * stride_y * stride_z + (yi+1) * stride_z + (zi+1);

    float c00 = glm::mix(noise[idx000], noise[idx100], fx);
    float c10 = glm::mix(noise[idx010], noise[idx110], fx);
    float c01 = glm::mix(noise[idx001], noise[idx101], fx);
    float c11 = glm::mix(noise[idx011], noise[idx111], fx);
    float c0 = glm::mix(c00, c10, fy);
    float c1 = glm::mix(c01, c11, fy);
    return glm::mix(c0, c1, fz);
}


#define ITER_CHUNK_XZ(I) for (int (I) = 0; (I) < CHUNK_SIZE; (I)++)

PerlinWorldGen::PerlinWorldGen() {
    params.terrain_layers.push_back({30.f, 200.f});
    params.terrain_layers.push_back({15.f, 32.f});
    params.terrain_layers.push_back({3.f, 8.f});
    params.base_height = 36.f;

    params.cave_layers.push_back({
        0.6f,
        glm::vec3{64.f, 16.f, 64.f}
    });
    params.cave_layers.push_back({
        0.4f,
        glm::vec3{16.f, 8.f, 16.f}
    });

    params.cave_threshold = 0.1f;    // lower = more caves
    params.cave_resolution = 8;      // 8-block spacing in each dim
    params.terrain_resolution = 4;   // 4-block spacing for heightmap
    init();
}
void PerlinWorldGen::init() {
    terrain_nx = CHUNK_SIZE / params.terrain_resolution + 1;
    cave_nx = CHUNK_SIZE / params.cave_resolution + 1;
    cave_ny = MAX_HEIGHT / params.cave_resolution + 1;
    cave_nz = CHUNK_SIZE / params.cave_resolution + 1;

    terrain_lowres.resize(terrain_nx * terrain_nx);
    cave_field.resize(cave_nx * cave_ny * cave_nz);
}
void PerlinWorldGen::abs_gen_chunk(cpos_t cpos, Chunk* target, World& world) const {
    constexpr int CH = CHUNK_SIZE;
    constexpr int HMAX = MAX_HEIGHT;

    static int nch = 0;
    LOG_INF("chunk %d", nch++);

    bpos_t base = cpos_to_bpos(cpos);
    const int terrain_res = params.terrain_resolution;
    const int cave_res = params.cave_resolution;

    // --- 1. Fill 2D terrain lowres grid ---
    for (int i = 0; i < terrain_nx; ++i) {
        for (int j = 0; j < terrain_nx; ++j) {
            float x = float(base.x + i * terrain_res);
            float z = float(base.z + j * terrain_res);
            float h = 0.f;
            for (const auto& layer : params.terrain_layers) {
                h += layer.amplitude *
                     perlin2.perlin(glm::vec2{x, z}, 3.f, glm::vec2{layer.frequency});
            }
            h += params.base_height;
            terrain_at(i, j) = h;
        }
    }

    // --- 2. Fill 3D cave field ---
    for (int i = 0; i < cave_nx; ++i) {
        for (int j = 0; j < cave_ny; ++j) {
            for (int k = 0; k < cave_nz; ++k) {
                float x = float(base.x + i * cave_res);
                float y = float(j * cave_res);
                float z = float(base.z + k * cave_res);
                float val = 0.f;
                for (const auto& layer : params.cave_layers) {
                    val += layer.amplitude *
                           perlin3.perlin(glm::vec3{x, y, z}, glm::vec3{0.f}, glm::vec3{layer.frequency});
                }
                cave_at(i, j, k) = val;
            }
        }
    }

    // --- 3. Generate blocks ---
    for (int i = 0; i < CH; ++i) {
        for (int k = 0; k < CH; ++k) {
            int i0 = i / terrain_res;
            int k0 = k / terrain_res;
            float tx = float(i % terrain_res) / terrain_res;
            float tz = float(k % terrain_res) / terrain_res;

            float h00 = terrain_at(i0, k0);
            float h10 = terrain_at(i0 + 1, k0);
            float h01 = terrain_at(i0, k0 + 1);
            float h11 = terrain_at(i0 + 1, k0 + 1);

            float h0 = glm::mix(h00, h10, tx);
            float h1 = glm::mix(h01, h11, tx);
            float terrain_h = glm::mix(h0, h1, tz);
            int surface_h = static_cast<int>(terrain_h);

            for (int j = 0; j < HMAX; ++j) {
                // Trilinear cave noise interpolation
                int xi = i / cave_res;
                int yi = j / cave_res;
                int zi = k / cave_res;

                float fx = float(i % cave_res) / cave_res;
                float fy = float(j % cave_res) / cave_res;
                float fz = float(k % cave_res) / cave_res;

                float c000 = cave_at(xi,     yi,     zi);
                float c100 = cave_at(xi + 1, yi,     zi);
                float c010 = cave_at(xi,     yi + 1, zi);
                float c110 = cave_at(xi + 1, yi + 1, zi);
                float c001 = cave_at(xi,     yi,     zi + 1);
                float c101 = cave_at(xi + 1, yi,     zi + 1);
                float c011 = cave_at(xi,     yi + 1, zi + 1);
                float c111 = cave_at(xi + 1, yi + 1, zi + 1);

                float c00 = glm::mix(c000, c100, fx);
                float c10 = glm::mix(c010, c110, fx);
                float c01 = glm::mix(c001, c101, fx);
                float c11 = glm::mix(c011, c111, fx);
                float c0 = glm::mix(c00, c10, fy);
                float c1 = glm::mix(c01, c11, fy);
                float cave_val = glm::mix(c0, c1, fz);

                if (cave_val < params.cave_threshold)
                    continue;

                bpos_t pos = {i, j, k};
                blockID* blk = world.blockAtLocalNoFlag(pos, target);

                if (j > surface_h)
                    continue;
                else if (j == surface_h)
                    *blk = *Blocks::GRASS;
                else if (j > surface_h - 4)
                    *blk = *Blocks::DIRT;
                else
                    *blk = *Blocks::STONE;
            }
        }
    }
}
