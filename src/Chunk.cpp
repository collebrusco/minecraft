#include "Chunk.h"
#include "RNG.h"
LOG_MODULE(chunk);

Chunk::BlockStore::BlockStore() {
    for (size_t i = 0; i < BlockStore::n(); i++) {
        data[i] = Block();
    }
}

Block *Chunk::BlockStore::get(bpos_t local) {
    return &this->data[flatten_idx(local)];
}

Block const *Chunk::BlockStore::read(bpos_t local) const {
    return &this->data[flatten_idx(local)];
}

size_t Chunk::BlockStore::flatten_idx(bpos_t local) {
    return local.y * CHUNK_SIZE * CHUNK_SIZE +
           local.z * CHUNK_SIZE +
           local.x;
}

Block::Block(BlockType const *const typ) : type(typ) {
}

Chunk::Chunk(cpos_t p) : pos(p), flag(true), store() {
}

Block *Chunk::blockAt(bpos_t local) {
    Block* b = this->store.get(local);
    return b;
}

Block const *Chunk::blockAt(bpos_t local) const {
    Block const* b = this->store.read(local);
    return b;
}

