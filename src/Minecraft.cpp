#include "Minecraft.h"
#include <flgl/logger.h>
LOG_MODULE(mc);


Minecraft::Minecraft() {
    const float kb = (static_cast<float>(sizeof(Block)) * CHUNK_SIZE_F * CHUNK_SIZE_F * MAX_HEIGHT_F) / 1024.f;
    const float mb = kb / 1024.f;
    LOG_INF("minecraft init");
    LOG_INF("chunk size: %.0f KB (%.1f MB);", kb, mb);
    LOG_INF("world %.0f MB (%.1f GB)", mb * RENDER_DISTANCE_F * RENDER_DISTANCE_F, mb * RENDER_DISTANCE_F * RENDER_DISTANCE_F * (1./1024.));
}


void Minecraft::user_create() {
    gl.init();
    window.create("minecraft", 1280, 720);

    ChunkRenderer::init_chunk_rendering();
    crenderer.init();
    crenderer.update(chunk);
    crenderer.attach();
}

void Minecraft::user_update(float dt, Keyboard const& kb, Mouse const& mouse) {
    if (window.keyboard[GLFW_KEY_ESCAPE].down) this->close();
    
    camera.update();
}

void Minecraft::user_render() {
    gl.clear();

    crenderer.sync(camera);
    crenderer.render();
}

void Minecraft::user_destroy() {
    ChunkRenderer::destroy_chunk_rendering();
}
