#include "Minecraft.h"
#include <flgl/logger.h>
#include "render/OutlineRenderer.h"
#include "render/BasicRenderer.h"
using namespace glm;
LOG_MODULE(mc);


Minecraft::Minecraft() : game(camera, wgen) {
    const float kb = (static_cast<float>(sizeof(Block)) * CHUNK_SIZE_F * CHUNK_SIZE_F * MAX_HEIGHT_F) / 1024.f;
    const float mb = kb / 1024.f;
    LOG_INF("minecraft init");
    LOG_INF("chunk size: %.0f KB (%.2f MB)", kb, mb);
    LOG_INF("world %.1f MB (%.2f GB)", mb * RENDER_DISTANCE_F * RENDER_DISTANCE_F, mb * RENDER_DISTANCE_F * RENDER_DISTANCE_F * (1./1024.));
}

void Minecraft::user_create() {
    gl.init();
    window.create("minecraft", 1280, 720);

    dbui.init();

    wrenderer.init();

    OutlineRenderer::init();

    PointRenderer::init();

    WorldPointRenderer::init();

    WorldAxesRenderer::init();

    camera.enable_look();
    camera.getPos() = vec3{14.f, 16.f, 14.f};
    camera.getFar() = 700.f;
    camera.getNear() = 0.01f;
    camera.update();
}

void Minecraft::user_update(float dt, Keyboard const& kb, Mouse const& mouse) { (void)kb; (void)mouse;
    static bool wf = false;
    dbui.tall.start();
    if (window.keyboard[GLFW_KEY_ESCAPE].down) this->close();
    if (window.keyboard[GLFW_KEY_K].pressed) wf = !wf;
    gl.wireframe(wf);

    game.tick(dt);

    camera.update();
}

void Minecraft::user_render() {
    gl.set_clear_color(135./255., 206./255., 235./255.);
    gl.clear();

        glFinish();
        dbui.tcpu.start();
    wrenderer.update(game);
        dbui.tcpu.stop();

        dbui.tbuf.start();
    wrenderer.buffer(game);
        glFinish();
        dbui.tbuf.stop();
    wrenderer.sync(camera);

        dbui.tren.start();
    wrenderer.render();
        glFinish();
        dbui.tren.stop();


    if (game.cast.hit()) {
        OutlineRenderer::sync(camera);
        OutlineRenderer::draw(game.cast.bpos);
        WorldAxesRenderer::sync(camera);
        WorldAxesRenderer::render(game.cast.pos);
    }

    PointRenderer::render();

        dbui.tall.stop();
        dbui.tick(dt(), camera.readPos());

}

void Minecraft::user_destroy() {
    dbui.destroy();
    TextRenderer::destroy_text_rendering();
    ChunkRenderer::destroy_chunk_rendering();
    PointRenderer::destroy();
    WorldPointRenderer::destroy();
    WorldAxesRenderer::destroy();
    wrenderer.destroy();
}
