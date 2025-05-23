/** 
 * WorldRenderer.h 
 * minecraft
 * created 04/22/25 by frank collebrusco
 */
#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H
#include "standard.h"
#include "data/World.h"
#include "ChunkRenderer.h"

struct WorldRenderer {
    WorldRenderer();
    ChunkRenderer renderers[RENDER_DISTANCE * RENDER_DISTANCE];
    void init();
    void destroy();

    void update(World const& world, size_t batch = 2);
    void buffer(World const& world, size_t batch = 3);
    void sync(Camera const& cam);

    void render() const;
};


#endif /* WORLD_RENDERER_H */
