/** 
 * WorldGen.h 
 * minecraft
 * created 04/30/25 by frank collebrusco
 */
#ifndef WORLD_GEN_H
#define WORLD_GEN_H
#include "standard.h"
#include "data/World.h"
#include "util/Noise.h"


struct BasicWorldGen : public WorldGenerator {
    Noise noise;
    virtual void gen_chunk(cpos_t, Chunk* target) const override final;
};


#endif /* WORLD_GEN_H */
