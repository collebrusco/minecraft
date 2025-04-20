#include "Minecraft.h"
#include <flgl/logger.h>
using namespace glm;
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

    camera.enable_look();
    camera.update();
}

void Minecraft::user_update(float dt, Keyboard const& kb, Mouse const& mouse) {
    if (window.keyboard[GLFW_KEY_ESCAPE].down) this->close();
    static bool wf = false; if (window.keyboard[GLFW_KEY_K].pressed) wf = !wf;
    gl.wireframe(wf);

    vec3 move = vec3(0.);
    vec3 fwd = camera.readLook(); fwd.y = 0;
    vec3 rht = cross(camera.readLook(), camera.readUp());
    const float speed = 2.;
    if (window.keyboard[GLFW_KEY_W].down) {
        move += fwd;
    }
    if (window.keyboard[GLFW_KEY_A].down) {
        move -= rht;
    }
    if (window.keyboard[GLFW_KEY_S].down) {
        move -= fwd;
    }
    if (window.keyboard[GLFW_KEY_D].down) {
        move += rht;
    }
    if (length(move) > 0.) move = normalize(move);
    move *= speed * dt;
    if (window.keyboard[GLFW_KEY_SPACE].down) {
        move += (V3_UP * speed * dt);
    }
    if (window.keyboard[GLFW_KEY_LEFT_SHIFT].down) {
        move -= (V3_UP * speed * dt);        
    }

    camera.getPos() += move;
    
    camera.update();
}

void Minecraft::user_render() {
    gl.set_clear_color(0.23, 0.01, 0.33);
    gl.clear();

    crenderer.sync(camera);
    crenderer.render();
}

void Minecraft::user_destroy() {
    ChunkRenderer::destroy_chunk_rendering();
}
