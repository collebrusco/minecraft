#include "standard.h"

const glm::vec3 DIRECTIONS[] = {
    (glm::vec3{ 0.f, 1.f, 0.f}),
    (glm::vec3{ 0.f,-1.f, 0.f}),
    (glm::vec3{ 0.f, 0.f, 1.f}),
    (glm::vec3{ 0.f, 0.f,-1.f}),
    (glm::vec3{ 1.f, 0.f, 0.f}),
    (glm::vec3{-1.f, 0.f, 0.f})
};

const glm::ivec3 IDIRECTIONS[] = {
    (glm::ivec3{ 0, 1, 0}),
    (glm::ivec3{ 0,-1, 0}),
    (glm::ivec3{ 0, 0, 1}),
    (glm::ivec3{ 0, 0,-1}),
    (glm::ivec3{ 1, 0, 0}),
    (glm::ivec3{-1, 0, 0})
};

/** convert block position to absolute position. the position returned is the min corner of the block by default or offset by the offset parameter */
pos_t bpos_to_pos(bpos_t const& bp, pos_t const& offset) {
    return ((pos_t)bp) + offset;
}

/** convert block position the the absolute position of its center */
pos_t bpos_to_pos_center(bpos_t const& bpos) {
    return bpos_to_pos(bpos, pos_t{0.5,0.5,0.5});
}

/** convert absolute position to block position, i.e. the position of the block that the given point lies within. */
bpos_t pos_to_bpos(pos_t const& pos) {
    return static_cast<bpos_t>(glm::floor(pos));
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
bpos_t cpos_to_bpos(cpos_t const& cpos, bpos_t const& offset) {
    return bpos_t{(cpos.x * CHUNK_SIZE) + offset.x, offset.y, (cpos.y * CHUNK_SIZE) + offset.z};
}

/** convert absolute pos to chunk pos, i.e. the position of the chunk that the given point lies within */
cpos_t pos_to_cpos(pos_t const& pos) {
    return bpos_to_cpos(pos_to_bpos(pos));
}

/** convert chunk pos to absolute pos. this returns the pos of the min corner of the chunk, or offset from that by the optional parameter */
pos_t cpos_to_pos(cpos_t const& cpos, pos_t const& offset) {
    return bpos_to_pos(cpos_to_bpos(cpos)) + offset;
}

/** checks if the given block position is a valid local chunk coord */
bool bpos_is_local(bpos_t const &bpos) {
    return (bpos.x >= 0) && (bpos.y >= 0) && (bpos.z >= 0) &&
           (bpos.x < CHUNK_SIZE) && (bpos.z < CHUNK_SIZE) && bpos.y < MAX_HEIGHT;
}

bool bpos_is_valid(bpos_t const &bpos) {
    return (bpos.y >= 0) && (bpos.y < MAX_HEIGHT);
}

bool pos_is_valid(pos_t const &pos) {
    return (pos.y >= 0.f) && (pos.y < MAX_HEIGHT_F);
}

bpos_t bpos_to_local(bpos_t const &global) {
    return {index_mod(global.x, CHUNK_SIZE), global.y, index_mod(global.z, CHUNK_SIZE)};
}
