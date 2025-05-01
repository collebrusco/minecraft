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

struct Chunk {
    Chunk(cpos_t pos);
    cpos_t pos;
    blockID blockAt(bpos_t local) const;
    inline glm::mat4 const& get_model() const {return model;}

    enum State {
        CLEAN = 0,
        DIRTY,
        NEED_BUFFER,
    };
    State get_state() const {return state;}

    inline void mark() {state = DIRTY;}
    inline void clean_cpu() {if (state == DIRTY) state = NEED_BUFFER;}
    inline void clean_gpu() {if (state == NEED_BUFFER) state = CLEAN;}

private:
friend struct World;
    State state;
    blockID* blockAt(bpos_t local);
    const glm::mat4 model;
    bool flag;
    struct BlockStore {
        friend struct Chunk;
        BlockStore();
        blockID* get(bpos_t local);
        blockID read(bpos_t local) const;
        static inline size_t n() {return CHUNK_SIZE * CHUNK_SIZE * MAX_HEIGHT;}
        static inline size_t bytes() {return n() * sizeof(blockID);}
    private:
        static size_t flatten_idx(bpos_t local);
        blockID data[CHUNK_SIZE * CHUNK_SIZE * MAX_HEIGHT];
    };
    BlockStore store;
};

#endif /* CHUNK_H */
