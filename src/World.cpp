#include "World.h"
#include "RNG.h"
using namespace glm;
LOG_MODULE(world);

size_t nallocs = 0;

void BasicWorldGen::gen_chunk(cpos_t cpos, Chunk *target) const {
    bpos_t pos = cpos_to_bpos(cpos);
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            int x = pos.x + i; int z = pos.z + j;
            int y = glm::floor(RNG::random_scalar_octave(x, z, 6) * 6.);
            target->blockAt(bpos_t{i, y, j})->type = &Blocks::grass;
        }
    }
}


World::ChunkStore::~ChunkStore() {
    for (auto pair : map) {
        nallocs--;
        delete pair.second;
    }
    map.clear();
    LOG_INF("chunks freed: %lu allocs leftover", nallocs);
}

Chunk *World::ChunkStore::get(cpos_t pos) {
    map_it_t it = map.find(pos);
    if (it == map.end()) return 0;
    return it->second;
}

Chunk const* World::ChunkStore::get(cpos_t pos) const {
    cmap_it_t it = map.find(pos);
    if (it == map.end()) return nullptr;
    return it->second;
}


Chunk *World::ChunkStore::gen(cpos_t pos, WorldGenerator const &gen) {
    map_it_t it = map.find(pos);
    if (it != map.end()) return 0;
    Chunk* chunk = new Chunk(pos);
    nallocs++;
    map.insert({pos, chunk});
    gen.gen_chunk(pos, chunk);
    return chunk;
}

Chunk *World::ChunkStore::get_or_gen(cpos_t pos, WorldGenerator const &gen) {
    Chunk* r = this->get(pos);
    if (!r) return this->gen(pos, gen);
    return r;
}

#define ITER_WORLD_BUF(iter) int iter = 0; iter < RENDER_DISTANCE * RENDER_DISTANCE; iter++
#define ITER_WORLD_BUFXY(iter) int iter = 0; iter < RENDER_DISTANCE; iter++

World::World(WorldGenerator& g) : generator(g), _center(0) {
    for (ITER_WORLD_BUFXY(i)) {
        for (ITER_WORLD_BUFXY(j)) {
            cpos_t pos(i - (RENDER_DISTANCE_R), j - (RENDER_DISTANCE_R));
            chunks[cpos_to_idx(pos)] = store.gen(pos, generator);
        }
    }
}

World::~World() {
}

Chunk *World::chunkAt(cpos_t pos) {
    Chunk* chunk = store.get(pos);
    chunk->mark();
    return chunk;
}

Chunk const *World::read_chunkAt(cpos_t pos) const {
    return store.get(pos);
}

Block *World::blockAt(bpos_t pos) {
    Chunk* chunk = this->chunkAt(bpos_to_cpos(pos));
    chunk->mark();
    return chunk->blockAt(bpos_to_local(pos));
}

Block const *World::read_blockAt(bpos_t pos) const {
    const Chunk* chunk = this->read_chunkAt(bpos_to_cpos(pos));
    return chunk->blockAt(bpos_to_local(pos));
}

World::RaycastResult World::raycast(Ray const& r, float maxlen) {
    const glm::vec3 origin = r.pos;
    const glm::vec3 dir = glm::normalize(r.dir);

    bpos_t blockPos = glm::floor(origin);

    glm::ivec3 step(
        dir.x > 0 ? 1 : (dir.x < 0 ? -1 : 0),
        dir.y > 0 ? 1 : (dir.y < 0 ? -1 : 0),
        dir.z > 0 ? 1 : (dir.z < 0 ? -1 : 0)
    );

    glm::vec3 tMax(
        (step.x != 0) ? ((glm::floor(origin.x) + (step.x > 0 ? 1.0f : 0.0f)) - origin.x) / dir.x : FLT_MAX,
        (step.y != 0) ? ((glm::floor(origin.y) + (step.y > 0 ? 1.0f : 0.0f)) - origin.y) / dir.y : FLT_MAX,
        (step.z != 0) ? ((glm::floor(origin.z) + (step.z > 0 ? 1.0f : 0.0f)) - origin.z) / dir.z : FLT_MAX
    );

    glm::vec3 tDelta(
        (step.x != 0) ? 1.0f / glm::abs(dir.x) : FLT_MAX,
        (step.y != 0) ? 1.0f / glm::abs(dir.y) : FLT_MAX,
        (step.z != 0) ? 1.0f / glm::abs(dir.z) : FLT_MAX
    );

    float dist_traveled = 0.0f;
    orientation_e hit_face = TOP;

    while (dist_traveled <= maxlen) {
        const Block* block = read_blockAt(blockPos);
        if (block && !block->empty()) {
            glm::vec3 collision_pos = origin + dir * dist_traveled;
            return {block, blockPos, collision_pos, hit_face, dist_traveled};
        }

        if (tMax.x < tMax.y) {
            if (tMax.x < tMax.z) {
                blockPos.x += step.x;
                hit_face = (step.x > 0) ? WEST : EAST;
                dist_traveled = tMax.x;
                tMax.x += tDelta.x;
            } else {
                blockPos.z += step.z;
                hit_face = (step.z > 0) ? SOUTH : NORTH;
                dist_traveled = tMax.z;
                tMax.z += tDelta.z;
            }
        } else {
            if (tMax.y < tMax.z) {
                blockPos.y += step.y;
                hit_face = (step.y > 0) ? BOT : TOP;
                dist_traveled = tMax.y;
                tMax.y += tDelta.y;
            } else {
                blockPos.z += step.z;
                hit_face = (step.z > 0) ? SOUTH : NORTH;
                dist_traveled = tMax.z;
                tMax.z += tDelta.z;
            }
        }
    }

    return {nullptr, blockPos, origin + dir * maxlen, TOP, maxlen};
}


void World::shift(int dx, int dy) {
    if (dx > 1 || dx < -1 || dy > 1 || dy < -1) {
        LOG_ERR("invalid shift %d,%d !!", dx, dy);
        return;
    }
    if (dx == 0 && dy == 0) {
        return;
    }
    _center.x += dx;
    if (dx) {
        int starty = _center.y - (RENDER_DISTANCE_R);
        for (int i = 0; i < RENDER_DISTANCE; i++) {
            int y = starty + i;
            int x = _center.x + ((dx*(RENDER_DISTANCE_R)) - (dx>0));
            size_t idx = cpos_to_idx(cpos_t(x,y));
            chunks[idx]->mark();
            chunks[idx] = store.get_or_gen(cpos_t(x,y), generator);
        }
    }
    _center.y += dy;
    if (dy) {
        int startx  = _center.x - (RENDER_DISTANCE_R);
        for (int i = 0; i < RENDER_DISTANCE; i++) {
            int x = startx + i;
            int y = _center.y + ((dy*(RENDER_DISTANCE_R)) - (dy>0));
            size_t idx = cpos_to_idx(cpos_t(x,y));
            chunks[idx]->mark();
            chunks[idx] = store.get_or_gen(cpos_t(x,y), generator);
        }
    }
}

void World::shift_to(cpos_t center) {
    int dx = center.x - _center.x;
    int dy = center.y - _center.y;
    this->shift(dx, dy);
}

size_t World::cpos_to_idx(cpos_t cpos) {
    ivec2 idxs = index_mod(cpos, RENDER_DISTANCE);
    return idxs.x + idxs.y * RENDER_DISTANCE;
}
