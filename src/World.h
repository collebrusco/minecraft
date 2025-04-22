/** 
 * World.h 
 * minecraft
 * created 04/22/25 by frank collebrusco
 */
#ifndef WORLD_H
#define WORLD_H
#include "standard.h"
#include "Chunk.h"

typedef size_t seed_t;

struct WorldGenerator {
    WorldGenerator(seed_t s = 0x4200FEED) : _seed(s) {}
    virtual ~WorldGenerator() = default;
    virtual void gen_chunk(cpos_t, Chunk* target) const = 0;
    seed_t seed() const {return _seed;}
private:
    seed_t _seed;
};

struct BasicWorldGen : public WorldGenerator {
    virtual void gen_chunk(cpos_t, Chunk* target) const override final;
};

#include <functional>
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

#include <unordered_map>
struct World : public ECS<> {
    World(WorldGenerator&);
    ~World();
    Chunk* chunkAt(cpos_t pos);
    Chunk const* chunkAt(cpos_t pos) const;
    Block* blockAt(bpos_t pos);
    Block const* blockAt(bpos_t pos) const;
    inline Chunk*const* renderdata() const {return &chunks[0];}
private:
    struct ChunkStore {
        ~ChunkStore();
        typedef std::unordered_map<cpos_t, Chunk*>::iterator map_it_t;
        std::unordered_map<cpos_t, Chunk*> map;
        Chunk* get(cpos_t);
        Chunk* gen(cpos_t, WorldGenerator const& gen);
        Chunk* get_or_gen(cpos_t, WorldGenerator const& gen);
    };
    ChunkStore store;
    WorldGenerator& generator;
    Chunk* chunks[RENDER_DISTANCE * RENDER_DISTANCE];
};


#endif /* WORLD_H */
