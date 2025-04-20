/** 
 * Chunk.h 
 * minecraft
 * created 04/17/25 by frank collebrusco
 */
#ifndef CHUNK_H
#define CHUNK_H
#include "standard.h"
#include <flgl/ecs.h>

struct BlockType {
    blockID id;
};

struct Blocks {
    inline static blockID newID() {return idct++;}
    static const BlockType stone;
private:
    static blockID idct;
};

struct Block {
    BlockType const* type;
    Block(BlockType const*const type = 0);
    inline bool empty() const {return !type;}
};

struct BlockStore {
    BlockStore();
    Block* get(bpos_t local);
    Block const* read(bpos_t local) const;
    static size_t n();
private:
    static size_t flatten_idx(bpos_t local);
    Block data[CHUNK_SIZE * CHUNK_SIZE * MAX_HEIGHT];
    NO_COPY_OR_MOVE(BlockStore);
};


struct Chunk {
    cpos_t pos;
    BlockStore store;
    Block* blockAt(bpos_t local);
    Block const* blockAt(bpos_t local) const;
};

struct World : public ECS<> {
    World();
    ~World();
    Chunk* chunkAt(cpos_t pos);
    Chunk const* chunkAt(cpos_t pos) const;
    Block* blockAt(bpos_t pos);
    Block const* blockAt(bpos_t pos) const;

private:
    Chunk* data;
};

#endif /* CHUNK_H */
