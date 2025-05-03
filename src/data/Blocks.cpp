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
static const BlockType iron_ore = {*Blocks::IRON_ORE,
                                   {{Faces::IRON_ORE, 0},
                                    {Faces::IRON_ORE, 0},
                                    {Faces::IRON_ORE, 0},
                                    {Faces::IRON_ORE, 0},
                                    {Faces::IRON_ORE, 0},
                                    {Faces::IRON_ORE, 0}}};

static const BlockType diamond_ore ={*Blocks::DIAMOND_ORE,
                                    {{Faces::DIAMOND_ORE, 0},
                                    {Faces::DIAMOND_ORE, 0},
                                    {Faces::DIAMOND_ORE, 0},
                                    {Faces::DIAMOND_ORE, 0},
                                    {Faces::DIAMOND_ORE, 0},
                                    {Faces::DIAMOND_ORE, 0}}};
static const BlockType oaklog ={*Blocks::OAK_LOG,
                             {{Faces::OAK_LOG_INTERNAL, 0},
                             {Faces::OAK_LOG_INTERNAL, 0},
                             {Faces::OAK_LOG_SIDE, 0},
                             {Faces::OAK_LOG_SIDE, 0},
                             {Faces::OAK_LOG_SIDE, 0},
                             {Faces::OAK_LOG_SIDE, 0}}};
static const BlockType leaf = {*Blocks::LEAF,
                              {{Faces::LEAF, 0},
                              {Faces::LEAF, 0},
                              {Faces::LEAF, 0},
                              {Faces::LEAF, 0},
                              {Faces::LEAF, 0},
                              {Faces::LEAF, 0}}};
/** declare new blocks here... */

BlockType const *
BlockType::get(blockID id)
{
    static const BlockType* types[] = {
        0,        
        &stone, 
        &grass, 
        &dirt,
        &iron_ore,
        &diamond_ore,
        &oaklog,
        &leaf
        /** register new blocks here... */
    };
    return (types[id]);
}

BlockType const* BlockType::get(Blocks id) {
    return get(*id);
}