#include "WorldGen.h"


void BasicWorldGen::gen_chunk(cpos_t cpos, Chunk *target) const {
    bpos_t pos = cpos_to_bpos(cpos);
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            int x = pos.x + i; int z = pos.z + j;
            int y = glm::floor(noise.value_octave(x, z, 6) * 6.);
            target->blockAt(bpos_t{i, y, j})->type = &Blocks::grass;
        }
    }
}




