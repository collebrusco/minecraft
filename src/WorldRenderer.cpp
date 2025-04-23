#include "WorldRenderer.h"

#define ITER_WORLD_BUF(iter) int iter = 0; iter < RENDER_DISTANCE * RENDER_DISTANCE; iter++
#define ITER_WORLD_BUFXY(iter) int iter = 0; iter < RENDER_DISTANCE; iter++

WorldRenderer::WorldRenderer() {
    for (ITER_WORLD_BUF(i)) {
        renderers[i] = ChunkRenderer();
    }
}

void WorldRenderer::init() {
    ChunkRenderer::init_chunk_rendering();
    for (ITER_WORLD_BUF(i)) {
        renderers[i].init();
    }
}

void WorldRenderer::destroy() {
    ChunkRenderer::destroy_chunk_rendering();
    for (ITER_WORLD_BUF(i)) {
        renderers[i].destroy();
    }
}

void WorldRenderer::update(World const& world) {
    Chunk*const* buf = world.renderdata();
    for (ITER_WORLD_BUF(i)) {
        if (buf[i]->is_marked()) {
            renderers[i].update(*(buf[i]));
        }
    }
}

void WorldRenderer::buffer(World const& world) {
    Chunk*const* buf = world.renderdata();
    for (ITER_WORLD_BUF(i)) {
        if (buf[i]->is_marked()) {
            buf[i]->mark(false);
            renderers[i].buffer();
        }
    }
}

void WorldRenderer::sync(Camera const &cam) {
    ChunkRenderer::sync(cam);
    Shader::unbind();
}

void WorldRenderer::render() const {
    ChunkRenderer::shader.bind();
    ChunkRenderer::vao.bind();
    for (ITER_WORLD_BUF(i)) {
        ChunkRenderer::shader.uMat4("uModel", renderers[i].model);
        renderers[i].attach();
        ChunkRenderer::vao.bind();
        renderers[i].render();
    }
    Shader::unbind();
    VertexArray::unbind();
}

