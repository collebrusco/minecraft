#include "Application.h"
#include <flgl/logger.h>
#include "render/OutlineRenderer.h"
#include "render/BasicRenderer.h"
#include "render/EntityRenderer.h"
#include "game/components.h"
using namespace glm;
LOG_MODULE(mc);


Application::Application() : state(wgen) {
    const float kb = (static_cast<float>(sizeof(blockID)) * CHUNK_SIZE_F * CHUNK_SIZE_F * MAX_HEIGHT_F) / 1024.f;
    const float mb = kb / 1024.f;
    LOG_INF("minecraft init");
    LOG_INF("chunk size: %.0f KB (%.2f MB)", kb, mb);
    LOG_INF("world %.1f MB (%.2f GB)", mb * RENDER_DISTANCE_F * RENDER_DISTANCE_F, mb * RENDER_DISTANCE_F * RENDER_DISTANCE_F * (1./1024.));
}

void Application::user_create() {
    gl.init();
    window.create("minecraft", 1280, 720);

    dbui.init();

    wrenderer.init();

    OutlineRenderer::init();

    PointRenderer::init();

    WorldPointRenderer::init();

    WorldAxesRenderer::init();

    EntityRenderer::init();

    UIRenderer::init("ui");

    driver.init(state);
}

void Application::user_update(float dt, Keyboard const& kb, Mouse const& mouse) { (void)kb; (void)mouse;
    dbui.tall.start();
    if (kb[GLFW_KEY_ESCAPE].down) this->close();

    static bool wf = false;
    if (kb[GLFW_KEY_K].pressed){
        wf = !wf;
    }
    gl.wireframe(wf);

    if (window.keyboard[GLFW_KEY_E].pressed) {
        static bool mg = true;
        mg = !mg;
        state.set_playerlook(mg);
    }

    if (window.keyboard[GLFW_KEY_C].pressed) {
        state.set_creative(!state.creative);
    }

    driver.tickA(state, dt);
    dbui.teng.start();
    driver.tickB(state, dt);
    dbui.teng.stop();
    driver.tickC(state, dt);

    ui.tick(dt, mouse);
}

void Application::user_render() {
    gl.set_clear_color(135./255., 206./255., 235./255.);
    gl.clear();

        glFinish();
        dbui.tcpu.start();
    wrenderer.update(state);
        dbui.tcpu.stop();

        dbui.tbuf.start();
    wrenderer.buffer(state);
        glFinish();
        dbui.tbuf.stop();
    wrenderer.sync(state.camera);

        dbui.tren.start();
    wrenderer.render();
        glFinish();
        dbui.tren.stop();



    World::RaycastResult const& cast = state.getComp<c_LineOfSight>(state.player).cast;
    if (cast.hit()) {
        OutlineRenderer::sync(state.camera);
        OutlineRenderer::draw(cast.bpos);
        WorldAxesRenderer::sync(state.camera);
        WorldAxesRenderer::render(cast.pos);
    }

    PointRenderer::render();

        dbui.tall.stop();
        dbui.tick(dt(), state.camera.readPos());

    UIRenderer::prepare(ui);
    UIRenderer::render();

}

void Application::user_destroy() {
    driver.destroy();
    dbui.destroy();
    TextRenderer::destroy_text_rendering();
    ChunkRenderer::destroy_chunk_rendering();
    PointRenderer::destroy();
    WorldPointRenderer::destroy();
    WorldAxesRenderer::destroy();
    EntityRenderer::destroy();
    UIRenderer::destroy();
    wrenderer.destroy();
}
