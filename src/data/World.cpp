#include "World.h"
#include "game/WorldGen.h"
using namespace glm;
LOG_MODULE(world);

size_t nallocs = 0;

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


Chunk *World::ChunkStore::gen(cpos_t pos, World& world) {
    map_it_t it = map.find(pos);
    if (it != map.end()) return 0;
    Chunk* chunk = new Chunk(pos);
    nallocs++;
    map.insert({pos, chunk});
    world.generator.gen_chunk(pos, chunk, world);
    chunk->mark();
    return chunk;
}

Chunk *World::ChunkStore::get_or_gen(cpos_t pos, World& world) {
    Chunk* r = this->get(pos);
    if (!r) return this->gen(pos, world);
    r->mark();
    return r;
}

#define ITER_WORLD_BUF(iter) int iter = 0; iter < RENDER_DISTANCE * RENDER_DISTANCE; iter++
#define ITER_WORLD_BUFXY(iter) int iter = 0; iter < RENDER_DISTANCE; iter++

World::World(WorldGenerator& g) : generator(g), _center(0) {
    memset(chunks, 0, sizeof(Chunk*) * RENDER_DISTANCE * RENDER_DISTANCE);
    for (ITER_WORLD_BUFXY(i)) {
        for (ITER_WORLD_BUFXY(j)) {
            cpos_t pos(i - (RENDER_DISTANCE_R), j - (RENDER_DISTANCE_R));
            desired_chunks[cpos_to_idx(pos)] = pos;
            // chunks[cpos_to_idx(pos)] = store.gen(pos, *this);
            // chunks[cpos_to_idx(pos)]->mark();
        }
    }
}

World::~World() {
}

Chunk *World::chunkAt(cpos_t pos) {
    Chunk* chunk = store.get(pos);
    if (chunk) chunk->mark();
    return chunk;
}

Chunk const *World::read_chunkAt(cpos_t pos) const {
    return store.get(pos);
}

blockID *World::blockAt(bpos_t pos) {
    if (!bpos_is_valid(pos)) return 0;
    Chunk* chunk = this->chunkAt(bpos_to_cpos(pos));
    if (!chunk) return 0;
    return blockAtLocal(bpos_to_local(pos), chunk);
}

void World::mark_adjacent_if_solid(bpos_t world_pos, Chunk* self_chunk) {

    cpos_t cpos = bpos_to_cpos(world_pos);
    Chunk* neighbor = chunkAt(cpos);
    if (!neighbor) {
        return;
    }

    if (neighbor == self_chunk) {
        return;
    }

    bpos_t local = bpos_to_local(world_pos);

    blockID* b = neighbor->blockAt(local);
    if (!b) {
        return;
    }

    neighbor->mark();
}

/** marks chunk and any adjacent if needed */
blockID* World::blockAtLocal(bpos_t lpos, Chunk* chunk) {
    if (!bpos_is_local(lpos)) {
        LOG_ERR("non local pos to World::blockAtLocal");
        return 0;
    }
    if (!bpos_is_valid(lpos)) return 0;
    chunk->mark();

    const bpos_t wpos = cpos_to_bpos(chunk->pos) + lpos;

    if (lpos.x == 0)         mark_adjacent_if_solid(wpos + bpos_t{-1, 0, 0}, chunk);
    else if (lpos.x == CHUNK_SIZE-1) mark_adjacent_if_solid(wpos + bpos_t{ 1, 0, 0}, chunk);
    
    if (lpos.z == 0)         mark_adjacent_if_solid(wpos + bpos_t{0, 0, -1}, chunk);
    else if (lpos.z == CHUNK_SIZE-1) mark_adjacent_if_solid(wpos + bpos_t{0, 0,  1}, chunk);

    return chunk->blockAt(lpos);
}




blockID World::read_blockAt(bpos_t pos) const {
    if (!bpos_is_valid(pos)) return 0;
    const Chunk* chunk = this->read_chunkAt(bpos_to_cpos(pos));
    if (!chunk) return 0;
    return chunk->blockAt(bpos_to_local(pos));
}

blockID* World::blockAtNoFlag(bpos_t pos) {
    if (!bpos_is_valid(pos)) return 0;
    Chunk* chunk = store.get(bpos_to_cpos(pos));
    if (!chunk) return 0;
    return chunk->blockAt(bpos_to_local(pos));
}

blockID* World::blockAtLocalNoFlag(bpos_t lpos, Chunk *chunk) {
    if (!bpos_is_valid(lpos)) return 0;
    return chunk->blockAt(lpos);
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
        blockID block = read_blockAt(blockPos);
        if (block) {
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

    return {0, blockPos, origin + dir * maxlen, TOP, maxlen};
}


bool World::disc_collide(pos_t pos, float rad) const {
    const int bx = static_cast<int>(std::floor(pos.x));
    const int bz = static_cast<int>(std::floor(pos.z));
    const int by = static_cast<int>(std::floor(pos.y));

    const float lx = pos.x - (bx + 0.5f);
    const float lz = pos.z - (bz + 0.5f);

    const float half = 0.5f;
    const float rr   = rad * rad;

    const bool center      = read_blockAt({bx,     by, bz});

    const bool west        = (lx < -(half - rad)) && read_blockAt({bx - 1, by, bz});
    const bool east        = (lx >  (half - rad)) && read_blockAt({bx + 1, by, bz});
    const bool north       = (lz >  (half - rad)) && read_blockAt({bx,     by, bz + 1});
    const bool south       = (lz < -(half - rad)) && read_blockAt({bx,     by, bz - 1});

    const bool north_west  = ( (lx + half)*(lx + half) + (lz - half)*(lz - half) < rr ) && read_blockAt({bx - 1, by, bz + 1});
    const bool north_east  = ( (lx - half)*(lx - half) + (lz - half)*(lz - half) < rr ) && read_blockAt({bx + 1, by, bz + 1});
    const bool south_west  = ( (lx + half)*(lx + half) + (lz + half)*(lz + half) < rr ) && read_blockAt({bx - 1, by, bz - 1});
    const bool south_east  = ( (lx - half)*(lx - half) + (lz + half)*(lz + half) < rr ) && read_blockAt({bx + 1, by, bz - 1});

    return center || west || east || north || south || north_west || north_east || south_west || south_east;
}

bool World::cyl_collide(pos_t pos, float rad, float height, float piv) const {
    const float step = 0.5f;
    float ySlice = piv;
    while (ySlice >= piv - height) {
        pos_t slicePos = pos;
        slicePos.y += ySlice;
        if (disc_collide(slicePos, rad))
            return true;
        ySlice -= step;
    }
    if (ySlice < (piv - height)) {
        if (disc_collide({pos.x, pos.y + piv - height, pos.z}, rad))
            return true;
    }
    return false;
}



void World::shift(int dx, int dy) {
    if (dx == 0 && dy == 0) return;
    _center += cpos_t{dx, dy};
    const int my = _center.y - RENDER_DISTANCE_R;
    const int My = _center.y + RENDER_DISTANCE_R;
    const int mx = _center.x - RENDER_DISTANCE_R;
    const int Mx = _center.x + RENDER_DISTANCE_R;
    for (int y = my; y < My; y++) {
        for (int x = mx; x < Mx; x++) {
            cpos_t desired{x,y};
            desired_chunks[cpos_to_idx(desired)] = desired;
        }
    }
}

void World::shift_to(cpos_t center) {
    int dx = center.x - _center.x;
    int dy = center.y - _center.y;
    this->shift(dx, dy);
}

void World::load_batch(size_t n) {
    for (ITER_WORLD_BUF(i)) {
        if (!chunks[i] || chunks[i]->pos != desired_chunks[i]) {
            chunks[i] = store.get_or_gen(desired_chunks[i], *this);
            if (!(--n)) return;
        }
    }
}

size_t World::cpos_to_idx(cpos_t cpos) {
    ivec2 idxs = index_mod(cpos, RENDER_DISTANCE);
    return idxs.x + idxs.y * RENDER_DISTANCE;
}
