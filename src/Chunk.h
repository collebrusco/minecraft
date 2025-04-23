/** 
 * Chunk.h 
 * minecraft
 * created 04/17/25 by frank collebrusco
 */
#ifndef CHUNK_H
#define CHUNK_H
#include "standard.h"
#include <flgl/ecs.h>
#include "Blocks.h"

struct Block {
    BlockType const* type;
    Block(BlockType const*const type = 0);
    inline bool empty() const {return !type;}
};

struct Chunk {
    Chunk(cpos_t pos);
    cpos_t pos;
    Block* blockAt(bpos_t local);
    Block const* blockAt(bpos_t local) const;
    inline void mark(bool m = true) {flag = m;}
    inline bool is_marked() const {return flag;}
    inline glm::mat4 const& get_model() const {return model;}
private:
    const glm::mat4 model;
    bool flag;
    struct BlockStore {
        BlockStore();
        Block* get(bpos_t local);
        Block const* read(bpos_t local) const;
        static inline size_t n() {return CHUNK_SIZE * CHUNK_SIZE * MAX_HEIGHT;}
    private:
        static size_t flatten_idx(bpos_t local);
        Block data[CHUNK_SIZE * CHUNK_SIZE * MAX_HEIGHT];
    };
    BlockStore store;
};

#endif /* CHUNK_H */
