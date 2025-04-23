/** 
 * standard.h 
 * minecraft
 * created 04/17/25 by frank collebrusco
 */
#ifndef STANDARD_H
#define STANDARD_H
#include <flgl/glm.h>
#include <flgl/logger.h>
#include <flgl/tools.h>

typedef size_t blockID;

#define BLOCK_PIX (16)
#define BLOCK_PIX_F (static_cast<float>(BLOCK_PIX))
#define CHUNK_SIZE (16)
#define CHUNK_SIZE_F (static_cast<float>(CHUNK_SIZE))
#define MAX_HEIGHT (256)
#define MAX_HEIGHT_F (static_cast<float>(MAX_HEIGHT))
#define RENDER_DISTANCE_R (16)
#define RENDER_DISTANCE_MIDPT RENDER_DISTANCE_R
#define RENDER_DISTANCE (RENDER_DISTANCE_R * 2)
#define RENDER_DISTANCE_F (static_cast<float>(RENDER_DISTANCE))

typedef glm::vec3  pos_t;   /* absolute floating point position */
typedef glm::ivec3 bpos_t;  /* integer block position, same scale as pos_t. when literally translated to absolute position, the bpos of a block is its minimum corner. */
typedef glm::ivec2 cpos_t;  /* integer chunk position, CHUNK_SIZE times coarser than pos_t. i.e., chunk at cpos=(0,0) contains all bpos w/ x,z in [0, CHUNK_SIZE) chunk positions are in the xz plane only because each chunk contains all y values. Literally translated, chunk pos is the x and z coord of the minimum corner of the chunk */

pos_t bpos_to_pos(bpos_t const& bp, pos_t const& offset = pos_t{0.,0.,0.});
pos_t bpos_to_pos_center(bpos_t const& bpos);
bpos_t pos_to_bpos(pos_t const& pos);
cpos_t bpos_to_cpos(bpos_t const& bpos);
bpos_t cpos_to_bpos(cpos_t const& cpos, bpos_t const& offset = bpos_t{0,0,0});
cpos_t pos_to_cpos(pos_t const& pos);
pos_t cpos_to_pos(cpos_t const& cpos, pos_t const& offset = pos_t{0.,0.,0.});

bool bpos_local(bpos_t const& bpos);

#define V3_UP      (glm::vec3{ 0.f, 1.f, 0.f})
#define V3_DOWN    (glm::vec3{ 0.f,-1.f, 0.f})
#define V3_NORTH   (glm::vec3{ 0.f, 0.f, 1.f})
#define V3_SOUTH   (glm::vec3{ 0.f, 0.f,-1.f})
#define V3_EAST    (glm::vec3{ 1.f, 0.f, 0.f})
#define V3_WEST    (glm::vec3{-1.f, 0.f, 0.f})

#define V2_NORTH   (glm::vec2{ 0.f,  1.f})
#define V2_SOUTH   (glm::vec2{ 0.f, -1.f})
#define V2_EAST    (glm::vec2{ 1.f,  0.f})
#define V2_WEST    (glm::vec2{-1.f,  0.f})

static inline int index_mod(int i, int m) {
    const int r = i % m; return (r < 0) ? r + m : r;
}

static inline glm::ivec2 index_mod(glm::ivec2 i, int m) {
    const int x = i.x % m;
    const int y = i.y % m;
    return glm::ivec2{(x<0)?x+m:x, (y<0)?y+m:y};
}

static inline glm::ivec3 index_mod(glm::ivec3 i, int m) {
    const int x = i.x % m;
    const int y = i.y % m;
    const int z = i.z % m;
    return glm::ivec3{(x<0)?x+m:x, (y<0)?y+m:y, (z<0)?z+m:z};
}


#endif /* STANDARD_H */
