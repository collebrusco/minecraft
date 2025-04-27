#include "BasicRenderer.h"
using namespace glm;

/** POINT RENDERER */

VertexArray PointRenderer::vao;
VertexBuffer<glm::vec2> PointRenderer::vbo;
Shader PointRenderer::shader;

void PointRenderer::init() {
    const vec2 vert = vec2(0);
    shader = Shader::from_source("cross_vert", "cross_frag");
    vao.create_bind();
    vbo.create_bind();
    vao.attach(vbo);
    vbo.buffer_data(1, &vert);

    vao.unbind();
    vbo.unbind();
}

void PointRenderer::destroy() {
    vao.destroy();
    shader.destroy();
    vbo.destroy();
}

void PointRenderer::render() {
    vao.bind();
    shader.bind();
    glEnable(GL_PROGRAM_POINT_SIZE);
    gl.draw_arrays(GL_POINTS, 0, 1);
    glDisable(GL_PROGRAM_POINT_SIZE);
}

/** WORLD POINT RENDERER */

VertexArray WorldPointRenderer::vao;
VertexBuffer<glm::vec3> WorldPointRenderer::vbo;
Shader WorldPointRenderer::shader;

void WorldPointRenderer::init() {
    shader = Shader::from_source("point_vert", "cross_frag");
    vao.create_bind();
    vbo.create_bind();
    vao.attach(vbo);
    vao.unbind();
    vbo.unbind();
}

void WorldPointRenderer::destroy() {
    vao.destroy();
    vbo.destroy();
    shader.destroy();
}

void WorldPointRenderer::sync(Camera &cam) {
    shader.uViewProj(cam.view(), cam.proj());
    shader.uVec3("uCampos", cam.readPos());
}

void WorldPointRenderer::render(pos_t pos)
{
    vao.bind();
    vbo.bind();
    vbo.buffer_data(1, &pos);
    vbo.unbind();

    vao.bind();
    glEnable(GL_PROGRAM_POINT_SIZE);
    gl.draw_arrays(GL_POINTS, 0, 1);
    glDisable(GL_PROGRAM_POINT_SIZE);
}

/** WORLD AXES RENDERER */

VertexArray WorldAxesRenderer::vao;
VertexBuffer<Vt_pn> WorldAxesRenderer::vbo;
Shader WorldAxesRenderer::shader;

void WorldAxesRenderer::init() {
    shader = Shader::from_source("axes_vert", "axes_frag");
    vao.create_bind();
    vbo.create_bind();
    vao.attach(vbo);
    vao.unbind();
    vbo.unbind();
}

void WorldAxesRenderer::destroy() {
    vao.destroy();
    vbo.destroy();
    shader.destroy();
}

void WorldAxesRenderer::sync(Camera &cam) {
    shader.uViewProj(cam.view(), cam.proj());
    shader.uVec3("uCampos", cam.readPos());
}

void WorldAxesRenderer::render(pos_t pos) {
    const Vt_pn verts[] = {
        {pos,               V3_POSX},
        {pos + V3_POSX/2.f, V3_POSX},
        {pos,               V3_POSY},
        {pos + V3_POSY/2.f, V3_POSY},
        {pos,               V3_POSZ},
        {pos + V3_POSZ/2.f, V3_POSZ}
    };

    vao.bind();
    vbo.bind();
    vbo.buffer_data(6, verts);
    vbo.unbind();

    vao.bind();
    gl.draw_arrays(GL_LINES, 0, 6);
}
