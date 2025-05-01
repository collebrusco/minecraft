#include "WorldRenderer.h"
LOG_MODULE(wrend);

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

void WorldRenderer::update(World const& world, size_t batch) {
    Chunk*const* buf = world.renderdata();
    for (ITER_WORLD_BUF(i)) {
        if (buf[i] && buf[i]->get_state() == Chunk::DIRTY) {
            buf[i]->clean_cpu();
            renderers[i].update(*(buf[i]), world);
            if (!(--batch)) return;
        }
    }
}

void WorldRenderer::buffer(World const& world, size_t batch) {
    Chunk*const* buf = world.renderdata();
    for (ITER_WORLD_BUF(i)) {
        if (buf[i] && buf[i]->get_state() == Chunk::NEED_BUFFER) {
            buf[i]->clean_gpu();
            renderers[i].buffer();
            if (!(--batch)) return;
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

