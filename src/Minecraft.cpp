#include "Minecraft.h"
#include <flgl/logger.h>
using namespace glm;
LOG_MODULE(mc);


Minecraft::Minecraft() : world(wgen) {
    const float kb = (static_cast<float>(sizeof(Block)) * CHUNK_SIZE_F * CHUNK_SIZE_F * MAX_HEIGHT_F) / 1024.f;
    const float mb = kb / 1024.f;
    LOG_INF("minecraft init");
    LOG_INF("chunk size: %.0f KB (%.1f MB);", kb, mb);
    LOG_INF("world %.0f MB (%.1f GB)", mb * RENDER_DISTANCE_F * RENDER_DISTANCE_F, mb * RENDER_DISTANCE_F * RENDER_DISTANCE_F * (1./1024.));
}

void Minecraft::user_create() {
    gl.init();
    window.create("minecraft", 1280, 720);

    dbui.init();

    wrenderer.init();

    camera.enable_look();
    camera.getPos() = vec3{14.f, 16.f, 14.f};
    camera.getFar() = 700.f;
    camera.getNear() = 0.01f;
    camera.update();
}

void Minecraft::user_update(float dt, Keyboard const& kb, Mouse const& mouse) {
    static bool wf = false;
    dbui.tall.start();
    if (window.keyboard[GLFW_KEY_ESCAPE].down) this->close();
    if (window.keyboard[GLFW_KEY_K].pressed) wf = !wf;
    gl.wireframe(wf);

    vec3 move = vec3(0.);
    vec3 fwd = camera.readLook(); fwd.y = 0;
    vec3 rht = cross(camera.readLook(), camera.readUp());
    float speed = 4.;
    if (window.keyboard[GLFW_KEY_PERIOD].down) {
        speed *= 5.;
    }
    if (window.keyboard[GLFW_KEY_COMMA].down) {
        speed *= 5.;
    }
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

    cpos_t cpos = pos_to_cpos(camera.readPos());
    if (cpos != world.center()) {
        world.shift_to(cpos);
    }
    
    camera.update();
}

void Minecraft::user_render() {
    gl.set_clear_color(0.23, 0.01, 0.33);
    gl.clear();

        dbui.tcpu.start();
    wrenderer.update(world);
        dbui.tcpu.stop();

        dbui.tbuf.start();
    wrenderer.buffer(world);
        glFinish();
        dbui.tbuf.stop();
    wrenderer.sync(camera);

        dbui.tren.start();
    wrenderer.render();
        glFinish();
        dbui.tren.stop();

        dbui.tall.stop();
        dbui.tick(dt());

}

void Minecraft::user_destroy() {
    dbui.destroy();
    TextRenderer::destroy_text_rendering();
    ChunkRenderer::destroy_chunk_rendering();
    wrenderer.destroy();
}
