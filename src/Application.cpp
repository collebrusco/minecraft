#include "Application.h"
#include <flgl/logger.h>
#include "render/OutlineRenderer.h"
#include "render/BasicRenderer.h"
using namespace glm;
LOG_MODULE(mc);


Application::Application() : state(wgen) {
    const float kb = (static_cast<float>(sizeof(Block)) * CHUNK_SIZE_F * CHUNK_SIZE_F * MAX_HEIGHT_F) / 1024.f;
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

    state.camera.enable_look();
    state.camera.getPos() = vec3{14.f, 16.f, 14.f};
    state.camera.getFar() = 700.f;
    state.camera.getNear() = 0.01f;
    state.camera.update();
}

void Application::user_update(float dt, Keyboard const& kb, Mouse const& mouse) { (void)kb; (void)mouse;
    static bool wf = false;
    dbui.tall.start();
    if (kb[GLFW_KEY_ESCAPE].down) this->close();
    if (kb[GLFW_KEY_K].pressed) wf = !wf;
    gl.wireframe(wf);

    vec3 move = vec3(0.);
    vec3 fwd = state.camera.readLook(); fwd.y = 0;
    vec3 rht = cross(state.camera.readLook(), state.camera.readUp());
    float speed = 4.;
    if (kb[GLFW_KEY_PERIOD].down) {
        speed *= 5.;
    }
    if (kb[GLFW_KEY_COMMA].down) {
        speed *= 5.;
    }
    if (kb[GLFW_KEY_W].down) {
        move += fwd;
    }
    if (kb[GLFW_KEY_A].down) {
        move -= rht;
    }
    if (kb[GLFW_KEY_S].down) {
        move -= fwd;
    }
    if (kb[GLFW_KEY_D].down) {
        move += rht;
    }
    if (kb[GLFW_KEY_L].pressed) {
        static bool mg = true;
        mg = !mg;
        window.set_mouse_grab(mg);
    }
    if (length(move) > 0.) {
        move = normalize(move);
    }
    move *= speed * dt;
    if (kb[GLFW_KEY_SPACE].down) {
        move += (V3_UP * speed * dt);
    }
    if (kb[GLFW_KEY_LEFT_SHIFT].down) {
        move -= (V3_UP * speed * dt);        
    }


    pos_t oldpos = state.camera.readPos();
    pos_t newpos = oldpos + move;

    if (!state.cyl_collide(newpos)) {
        state.camera.getPos() = newpos;
    } else {
        pos_t attempt = oldpos;

        attempt.x += move.x;
        if (!state.cyl_collide(attempt)) {
            state.camera.getPos() = attempt;
        }

        attempt = state.camera.readPos();
        attempt.z += move.z;
        if (!state.cyl_collide(attempt)) {
            state.camera.getPos() = attempt;
        }

        attempt = state.camera.readPos();
        attempt.y += move.y;
        if (!state.cyl_collide(attempt)) {
            state.camera.getPos() = attempt;
        }
    }


    cpos_t cpos = pos_to_cpos(state.camera.readPos());
    if (cpos != state.center()) {
        state.shift_to(cpos);
    }

    cast = state.raycast(Ray(state.camera.readPos(), state.camera.readLook()));

    if (mouse.left().pressed && cast.hit()) {
        Block* b = state.blockAt(cast.bpos);
        *b = Block::null();
    }

    if (mouse.right().pressed && cast.hit()) {
        Block* place = state.blockAt(cast.bpos + IDIRECTIONS[cast.face]);
        place->type = &Blocks::stone;
    }
    
    state.camera.update();
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


    if (cast.hit()) {
        OutlineRenderer::sync(state.camera);
        OutlineRenderer::draw(cast.bpos);
        WorldAxesRenderer::sync(state.camera);
        WorldAxesRenderer::render(cast.pos);
    }

    PointRenderer::render();

        dbui.tall.stop();
        dbui.tick(dt(), state.camera.readPos());

}

void Application::user_destroy() {
    dbui.destroy();
    TextRenderer::destroy_text_rendering();
    ChunkRenderer::destroy_chunk_rendering();
    PointRenderer::destroy();
    WorldPointRenderer::destroy();
    WorldAxesRenderer::destroy();
    wrenderer.destroy();
}
