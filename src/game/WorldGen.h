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

typedef size_t seed_t;

struct World;

struct WorldGenerator {
    WorldGenerator(seed_t s = 0x4200FEED) : _seed(s) {}
    virtual ~WorldGenerator() = default;
    void gen_chunk(cpos_t cpos, Chunk* target, World& world) const;
    seed_t seed() const {return _seed;}
private:
    virtual void abs_gen_chunk(cpos_t cpos, Chunk* target, World& world) const = 0;
    seed_t _seed;
};

struct BasicWorldGen : public WorldGenerator {
    Noise noise;
    virtual void abs_gen_chunk(cpos_t, Chunk* target, World& world) const override final;
};


struct PerlinWorldGen : public WorldGenerator {
    PerlinWorldGen();
    struct PerlinWorldGenParams {
        // Terrain heightmap layers (2D)
        struct TerrainLayer {
            float amplitude;
            float frequency;
        };
        std::vector<TerrainLayer> terrain_layers;
        float base_height = 36.f;
    
        // Cave noise octaves (3D)
        struct CaveLayer {
            float amplitude;
            glm::vec3 frequency; // anisotropic: x, y, z
        };
        std::vector<CaveLayer> cave_layers;
    
        // Cave behavior
        float cave_threshold = 0.3f; // lower = more caves
        int cave_resolution = 8;     // controls interpolation block size
    
        // Terrain heightmap sampling
        int terrain_resolution = 4;
    } params;
    Noise noise;
    Perlin2 perlin2;
    Perlin3 perlin3;
    void init();    
    virtual void abs_gen_chunk(cpos_t pos, Chunk* target, World& world) const override final;
private:
    int terrain_nx = 0;
    int cave_nx = 0, cave_ny = 0, cave_nz = 0;

    mutable std::vector<float> terrain_lowres;
    mutable std::vector<float> cave_field;

    inline float& terrain_at(int i, int j) const {
        return terrain_lowres[i * terrain_nx + j];
    }
    
    inline float& cave_at(int i, int j, int k) const {
        return cave_field[i * cave_ny * cave_nz + j * cave_nz + k];
    }    

};


#endif /* WORLD_GEN_H */
