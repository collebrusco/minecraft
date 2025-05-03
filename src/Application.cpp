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
    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window.id(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    

    dbui.init();

    wrenderer.init();

    OutlineRenderer::init();

    PointRenderer::init();

    WorldPointRenderer::init();

    WorldAxesRenderer::init();

    EntityRenderer::init();

    UIRenderer::init("ui");
    SkyboxRenderer::init();

    driver.init(state);

    ui.init();
    ui.inventory.disabled = true;
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
        ui.inventory.disabled = mg;
    }

    if (window.keyboard[GLFW_KEY_C].pressed) {
        state.set_creative(!state.creative);
    }

    driver.tickA(state, dt);
    dbui.teng.start();
    driver.tickB(state, dt);
    dbui.teng.stop();
    driver.tickC(state, dt);

    c_PlayerInventory& inv = state.getComp<c_PlayerInventory>(state.player);
    ui.hotbar.read_inv(inv);
    ui.inventory.read_inv(inv);
    ui.tick(dt, mouse);
    ui.inventory.write_inv(inv);
    ui.hotbar.write_inv(inv);
}

void Application::user_render() {
    gl.set_clear_color(135./255., 206./255., 235./255.);
    gl.clear();

    SkyboxRenderer::sync(state.camera);
    SkyboxRenderer::render();

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
    ui.draw();

}

void Application::user_destroy() {
    driver.destroy();
    dbui.destroy();
    ui.destroy();
    TextRenderer::destroy_text_rendering();
    ChunkRenderer::destroy_chunk_rendering();
    PointRenderer::destroy();
    WorldPointRenderer::destroy();
    WorldAxesRenderer::destroy();
    EntityRenderer::destroy();
    UIRenderer::destroy();
    SkyboxRenderer::destroy();
    wrenderer.destroy();
}
