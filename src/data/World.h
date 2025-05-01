/** 
 * World.h 
 * minecraft
 * created 04/22/25 by frank collebrusco
 */
#ifndef WORLD_H
#define WORLD_H
#include <functional>
#include <unordered_map>
#include "standard.h"
#include "Chunk.h"

struct WorldGenerator;

namespace std {
    template<>
    struct hash<cpos_t> {
        size_t operator()(const cpos_t& v) const noexcept {
            uint64_t x = static_cast<uint32_t>(v.x);
            uint64_t y = static_cast<uint32_t>(v.y);
            return (x << 32) | y;
        }
    };
}

struct World : public ECS<> {
    World(WorldGenerator&);
    virtual ~World();
    NO_COPY_OR_MOVE(World);

    Chunk* chunkAt(cpos_t pos);
    Chunk const* read_chunkAt(cpos_t pos) const;
    blockID* blockAt(bpos_t pos);
    blockID* blockAtLocal(bpos_t lpos, Chunk* chunk);
    blockID read_blockAt(bpos_t pos) const;

    blockID* blockAtNoFlag(bpos_t pos);
    blockID* blockAtLocalNoFlag(bpos_t lpos, Chunk* chunk);

    struct RaycastResult {
        blockID block;
        bpos_t bpos;
        pos_t pos;
        orientation_e face;
        float len;
        inline bool miss() const {return !block;}
        inline bool hit() const {return block;}
    };

    RaycastResult raycast(Ray const& r, float maxlen = 5.f);

    bool disc_collide(pos_t pos, float rad = 0.4f) const;
    bool cyl_collide(pos_t pos, float rad = 0.4f, float height = 1.72, float piv = 0.1f) const;

    inline cpos_t center() const {return _center;}
    void shift(int dx, int dy);
    void shift_to(cpos_t center);
    void load_batch(size_t n);

    inline Chunk*const* renderdata() const {return &chunks[0];}
private:
    static size_t cpos_to_idx(cpos_t cpos);
    void mark_adjacent_if_solid(bpos_t world_pos, Chunk* self_chunk);
    struct ChunkStore {
        ChunkStore() = default;
        ~ChunkStore();
        NO_COPY_OR_MOVE(ChunkStore);
        typedef std::unordered_map<cpos_t, Chunk*>::iterator map_it_t;
        typedef std::unordered_map<cpos_t, Chunk*>::const_iterator cmap_it_t;
        std::unordered_map<cpos_t, Chunk*> map;
        Chunk* get(cpos_t);
        Chunk const* get(cpos_t) const;
        Chunk* gen(cpos_t, World& world);
        Chunk* get_or_gen(cpos_t, World& world);
    };
friend struct ChunkStore;
    ChunkStore store;
    WorldGenerator& generator;
    cpos_t _center;
    Chunk* chunks[RENDER_DISTANCE * RENDER_DISTANCE];
    cpos_t desired_chunks[RENDER_DISTANCE * RENDER_DISTANCE];
};


#endif /* WORLD_H */
