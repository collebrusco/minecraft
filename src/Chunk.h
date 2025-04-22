/** 
 * Chunk.h 
 * minecraft
 * created 04/17/25 by frank collebrusco
 */
#ifndef CHUNK_H
#define CHUNK_H
#include "standard.h"
#include <flgl/ecs.h>

struct BlockType {
    blockID id;
};

struct Blocks {
    inline static blockID newID() {return idct++;}
    static const BlockType stone;
private:
    static blockID idct;
};

struct Block {
    BlockType const* type;
    Block(BlockType const*const type = 0);
    inline bool empty() const {return !type;}
};

struct Chunk {
    Chunk(cpos_t pos);
    cpos_t pos;
    Block* blockAt(bpos_t local);
    Block const* blockAt(bpos_t local) const;
    inline void mark(bool m = true) {flag = m;}
    inline bool is_marked() const {return flag;}
private:
    bool flag;
    struct BlockStore {
        BlockStore();
        Block* get(bpos_t local);
        Block const* read(bpos_t local) const;
        static inline size_t n() {return CHUNK_SIZE * CHUNK_SIZE * MAX_HEIGHT;}
    private:
        static size_t flatten_idx(bpos_t local);
        Block data[CHUNK_SIZE * CHUNK_SIZE * MAX_HEIGHT];
    };
    BlockStore store;
};

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
struct ChunkStore {
    ~ChunkStore();
    typedef std::unordered_map<cpos_t, Chunk*>::iterator map_it_t;
    std::unordered_map<cpos_t, Chunk*> map;
    Chunk* get(cpos_t);
    Chunk* gen(cpos_t, WorldGenerator const& gen);
    Chunk* get_or_gen(cpos_t, WorldGenerator const& gen);
};

struct World : public ECS<> {
    World(WorldGenerator&);
    ~World();
    Chunk* chunkAt(cpos_t pos);
    Chunk const* chunkAt(cpos_t pos) const;
    Block* blockAt(bpos_t pos);
    Block const* blockAt(bpos_t pos) const;
    inline Chunk** renderdata() const {return (Chunk**)chunks;}
private:
    ChunkStore store;
    WorldGenerator& generator;
    Chunk* chunks[RENDER_DISTANCE * RENDER_DISTANCE];
};

#include "ChunkRenderer.h"
struct WorldRenderer {
    ChunkRenderer renderers[RENDER_DISTANCE * RENDER_DISTANCE];
    void init();
    void destroy();

    void update(World const& world);
    void buffer(World const& world);
    void sync(Camera const& cam);

    void render() const;
};

#endif /* CHUNK_H */
