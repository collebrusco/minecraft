#include "World.h"
#include "RNG.h"

void BasicWorldGen::gen_chunk(cpos_t cpos, Chunk *target) const {
    bpos_t pos = cpos_to_bpos(cpos);
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            int x = pos.x + i; int z = pos.z + j;
            int y = glm::floor(RNG::random_scalar_octave(x, z, 6) * 6.);
            target->blockAt(bpos_t{i, y, j})->type = &Blocks::stone;
        }
    }
}

World::ChunkStore::~ChunkStore() {
    for (auto pair : map) {
        delete pair.second;
    }
    map.clear();
}

Chunk *World::ChunkStore::get(cpos_t pos) {
    map_it_t it = map.find(pos);
    if (it == map.end()) return 0;
    return it->second;
}

Chunk *World::ChunkStore::gen(cpos_t pos, WorldGenerator const &gen) {
    map_it_t it = map.find(pos);
    if (it != map.end()) return 0;
    Chunk* chunk = new Chunk(pos);
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

World::World(WorldGenerator& g) : generator(g) {
    for (ITER_WORLD_BUFXY(i)) {
        for (ITER_WORLD_BUFXY(j)) {
            cpos_t pos(i - (RENDER_DISTANCE/2) + 1, j - (RENDER_DISTANCE/2) + 1);
            chunks[j + i*RENDER_DISTANCE] = store.gen(pos, generator);
        }
    }
}

World::~World() {
}

Chunk *World::chunkAt(cpos_t pos) {
    return nullptr;
}

Chunk const *World::chunkAt(cpos_t pos) const {
    return nullptr;
}

Block *World::blockAt(bpos_t pos) {
    return nullptr;
}

Block const *World::blockAt(bpos_t pos) const {
    return nullptr;
}

