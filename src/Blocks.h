/** 
 * Blocks.h 
 * minecraft
 * created 04/23/25 by frank collebrusco
 */
#ifndef BLOCKS_H
#define BLOCKS_H
#include "standard.h"

typedef enum {
    STONE = 0,
    GRASS,
    GRASS_SIDE,
    DIRT,
} face_e;

struct BlockFace {
    face_e face     : 12;
    uint16_t aux    : 4;
};

struct BlockType {
    blockID id;
    BlockFace faces[ORIENTATION_LAST + 1];
};

struct Blocks {
    inline static blockID newID() {return idct++;}
    static const BlockType stone;
    static const BlockType grass;
    static const BlockType dirt;
private:
    static blockID idct;
};

#endif /* BLOCKS_H */
