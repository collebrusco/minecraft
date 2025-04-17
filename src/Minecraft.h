/** 
 * Minecraft.h 
 * created 04/16/25 by frank collebrusco
 */
#ifndef MINECRAFT_H
#define MINECRAFT_H
#include <flgl.h>
#include <flgl/driver.h>
#include <flgl/ecs.h>
#include <flgl/glm.h>

typedef size_t blockID;

// #define index_mod(vec, m) \
// (ivec2((vec.x%m)<0?(vec.x%m)+m:vec.x%m, (vec.y%m)<0?(vec.y%m)+m:vec.y%m))

static inline int index_mod(int i, int m) {
    const int r = i % m; return (r < 0) ? r + m : r;
}

static inline glm::ivec3 index_mod(glm::ivec3 i, int m) {
    const int x = i.x % m;
    const int y = i.y % m;
    const int z = i.z % m;
    return glm::ivec3{(x<0)?x+m:x, (y<0)?y+m:y, (z<0)?z+m:z};
}

#define BLOCK_PIX (16)
#define BLOCK_PIX_F (static_cast<float>(BLOCK_PIX))
#define CHUNK_SIZE (16)
#define CHUNK_SIZE_F (static_cast<float>(CHUNK_SIZE))
#define MAX_HEIGHT (512)
#define MAX_HEIGHT_F (static_cast<float>(MAX_HEIGHT))

typedef glm::vec3  pos_t;   /* absolute floating point position */
typedef glm::ivec3 bpos_t;  /* integer block position, same scale as pos_t. when literally translated to absolute position, the bpos of a block is its minimum corner. */
typedef glm::ivec2 cpos_t;  /* integer chunk position, CHUNK_SIZE times coarser than pos_t. i.e., chunk at cpos=(0,0) contains all bpos w/ x,z in [0, CHUNK_SIZE) chunk positions are in the xz plane only because each chunk contains all y values. Literally translated, chunk pos is the x and z coord of the minimum corner of the chunk */

/** convert block position to absolute position. the position returned is the min corner of the block by default or offset by the offset parameter */
pos_t bpos_to_pos(bpos_t const& bp, pos_t const& offset = pos_t{0.,0.,0.}) {
    return ((pos_t)bp) + offset;
}

/** convert block position the the absolute position of its center */
pos_t bpos_to_pos_center(bpos_t const& bpos) {
    return bpos_to_pos(bpos, pos_t{0.5,0.5,0.5});
}

/** convert absolute position to block position, i.e. the position of the block that the given point lies within. */
bpos_t pos_to_bpos(pos_t const& pos) {
    return glm::floor(pos);
}

/** convert block position to chunk position, i.e. the position of the chunk that the given block lies within. */
cpos_t bpos_to_cpos(bpos_t const& bpos) {
    return static_cast<cpos_t>(
        glm::floor(
            (static_cast<glm::vec2>(bpos.xz())) / CHUNK_SIZE_F
        )
    );
}

/** convert chunk pos to block pos. this returns the block in the chunk with local chunk coords (0,0,0) by default or given by the offset parameter. */
bpos_t cpos_to_bpos(cpos_t const& cpos, bpos_t const& offset = bpos_t{0,0,0}) {
    return bpos_t{(cpos.x * CHUNK_SIZE) + offset.x, offset.y, (cpos.y * CHUNK_SIZE) + offset.z};
}

/** convert absolute pos to chunk pos, i.e. the position of the chunk that the given point lies within */
cpos_t pos_to_cpos(pos_t const& pos, pos_t) {
    return bpos_to_cpos(pos_to_bpos(pos));
}

/** convert chunk pos to absolute pos. this returns the pos of the min corner of the chunk, or offset from that by the optional parameter */
pos_t cpos_to_pos(cpos_t const& cpos, pos_t const& offset = pos_t{0.,0.,0.}) {
    return bpos_to_pos(cpos_to_bpos(cpos)) + offset;
}



class BlockType {
    blockID id;
};

struct Block {
    BlockType const* type;
    bpos_t pos;
    Block(BlockType const& type, bpos_t p = bpos_t(0.f));
};

struct Chunk {
    
};

struct World : public ECS<> {

};

class Minecraft : public Driver {
    virtual void user_create() override final;
    virtual void user_update(float dt, Keyboard const& kb, Mouse const& mouse) override final;
    virtual void user_render() override final;
    virtual void user_destroy() override final;



};


void Minecraft::user_create() {

}

void Minecraft::user_update(float dt, Keyboard const& kb, Mouse const& mouse) {

}

void Minecraft::user_render() {

}

void Minecraft::user_destroy() {

}


#endif /* MINECRAFT_H */
