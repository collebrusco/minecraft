/** 
 * Blocks.h 
 * minecraft
 * created 04/23/25 by frank collebrusco
 */
#ifndef BLOCKS_H
#define BLOCKS_H
#include "standard.h"

enum class Faces {
    STONE = 0,
    GRASS,
    GRASS_SIDE,
    DIRT,
    IRON_ORE,
    DIAMOND_ORE,
    OAK_LOG_INTERNAL,
    OAK_LOG_SIDE,
    LEAF
};

struct BlockFace {
    Faces face     : 12;
    uint16_t aux    : 4;
};

enum class Blocks {
    AIR = 0,
    STONE,
    GRASS,
    DIRT,
    IRON_ORE,
    DIAMOND_ORE,
    OAK_LOG,
    LEAF,
    LAST
};
static_assert((size_t)Blocks::LAST < (1ULL << (sizeof(blockID)*8)) && "too many blocks for blockID integer type");
static inline blockID operator*(Blocks b_e) {return (blockID)b_e;}

struct BlockType {
    blockID id;
    BlockFace faces[ORIENTATION_LAST];
    static BlockType const* get(Blocks id);
    static BlockType const* get(blockID id);
    /** onBreak(), onUpdate(), other oop behavioral stuff here */

};


#endif /* BLOCKS_H */
