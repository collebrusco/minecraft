#include "Chunk.h"

Block *BlockStore::get(bpos_t local) {
    return &this->data[flatten_idx(local)];
}

Block const *BlockStore::read(bpos_t local) const {
    return &this->data[flatten_idx(local)];
}

size_t BlockStore::flatten_idx(bpos_t local) {
    return local.y * CHUNK_SIZE * CHUNK_SIZE +
           local.z * CHUNK_SIZE +
           local.x;
}

Block::Block(BlockType const *const typ) : type(typ) {
}

Block *Chunk::blockAt(bpos_t local) {
    Block* b = this->store.get(local);
    if (b->empty()) return 0;
    return b;
}

Block const *Chunk::blockAt(bpos_t local) const {
    Block const* b = this->store.read(local);
    if (b->empty()) return 0;
    return b;
}

World::World() {
    data = new Chunk[RENDER_DISTANCE * RENDER_DISTANCE];
}

World::~World() {
}
