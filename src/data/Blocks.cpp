#include "standard.h"
#include "Chunk.h"

blockID Blocks::idct = 0;

const BlockType Blocks::stone = {Blocks::newID(),  {{STONE, 0},
                                                    {STONE, 0},
                                                    {STONE, 0},
                                                    {STONE, 0},
                                                    {STONE, 0},
                                                    {STONE, 0}}};

const BlockType Blocks::grass = {Blocks::newID(),  {{GRASS, 0},
                                                    {DIRT, 0},
                                                    {GRASS_SIDE, 0},
                                                    {GRASS_SIDE, 0},
                                                    {GRASS_SIDE, 0},
                                                    {GRASS_SIDE, 0}}};

const BlockType Blocks::dirt  = {Blocks::newID(),  {{DIRT, 0},
                                                    {DIRT, 0},
                                                    {DIRT, 0},
                                                    {DIRT, 0},
                                                    {DIRT, 0},
                                                    {DIRT, 0}}};
