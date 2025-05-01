#include "Chunk.h"
LOG_MODULE(chunk);

Chunk::BlockStore::BlockStore() {
    memset(data, 0, bytes());
}

blockID *Chunk::BlockStore::get(bpos_t local) {
    return &this->data[flatten_idx(local)];
}

blockID Chunk::BlockStore::read(bpos_t local) const {
    return this->data[flatten_idx(local)];
}

size_t Chunk::BlockStore::flatten_idx(bpos_t local) {
    size_t r = (local.y * CHUNK_SIZE * CHUNK_SIZE) + (local.z * CHUNK_SIZE) + local.x;
    if (!(r < n()) || !bpos_is_local(local)) {
        LOG_ERR("trying to access block store with non-local %d,%d,%d", local.x, local.y, local.z);
    }
    return r;
}

Chunk::Chunk(cpos_t p) : pos(p), model(genModelMat3d(cpos_to_pos(p), glm::vec3(0.f), glm::vec3(1.))), flag(true), store() {
}

blockID *Chunk::blockAt(bpos_t local) {
    return this->store.get(local);
}

blockID Chunk::blockAt(bpos_t local) const {
    return this->store.read(local);
}

