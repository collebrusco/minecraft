#include "standard.h"
#include "Chunk.h"
#include "Blocks.h"

static const BlockType stone = {*Blocks::STONE,  
                                {{Faces::STONE, 0},
                                {Faces::STONE, 0},
                                {Faces::STONE, 0},
                                {Faces::STONE, 0},
                                {Faces::STONE, 0},
                                {Faces::STONE, 0}}};

static const BlockType grass = {*Blocks::GRASS,  
                                {{Faces::GRASS, 0},
                                {Faces::DIRT, 0},
                                {Faces::GRASS_SIDE, 0},
                                {Faces::GRASS_SIDE, 0},
                                {Faces::GRASS_SIDE, 0},
                                {Faces::GRASS_SIDE, 0}}};

static const BlockType dirt  = {*Blocks::DIRT,
                                {{Faces::DIRT, 0},
                                {Faces::DIRT, 0},
                                {Faces::DIRT, 0},
                                {Faces::DIRT, 0},
                                {Faces::DIRT, 0},
                                {Faces::DIRT, 0}}};

/** declare new blocks here... */

BlockType const* BlockType::get(blockID id) {
    static const BlockType* types[] = {
        0,        
        &stone, 
        &grass, 
        &dirt,
        /** register new blocks here... */
    };
    return (types[id]);
}

BlockType const* BlockType::get(Blocks id) {
    return get(*id);
}