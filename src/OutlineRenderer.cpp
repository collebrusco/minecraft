#include "OutlineRenderer.h"

Shader OutlineRenderer::shader;
VertexArray OutlineRenderer::vao;
VertexBuffer<glm::vec3> OutlineRenderer::vbo;
ElementBuffer OutlineRenderer::ibo;

#define QUAD (0.5f)

void OutlineRenderer::init() {

    const glm::vec3 verts[] = {
        {-QUAD, -QUAD, -QUAD},
        {-QUAD,  QUAD, -QUAD},
        { QUAD,  QUAD, -QUAD},
        { QUAD, -QUAD, -QUAD},
        {-QUAD, -QUAD,  QUAD},
        {-QUAD,  QUAD,  QUAD},
        { QUAD,  QUAD,  QUAD},
        { QUAD, -QUAD,  QUAD}
    };

    const uint32_t elems[] = {
        0,1, 1,2, 2,3, 3,0,
        4,5, 5,6, 6,7, 7,4,
        0,4, 1,5, 2,6, 3,7
    };

    shader = Shader::from_source("outline_vert", "outline_frag");
    
    vao.create_bind();

    vbo.create_bind();
    vao.attach(vbo);
    vbo.buffer_data(8, verts);

    ibo.create_bind();
    ibo.buffer_data(24, elems);

    vao.unbind();
    vbo.unbind();
    ibo.unbind();
}

void OutlineRenderer::destroy() {
    vao.destroy();
    vbo.destroy();
    ibo.destroy();
    shader.destroy();
}

void OutlineRenderer::sync(Camera &cam) {
    shader.uViewProj(cam.view(), cam.proj());
}

void OutlineRenderer::draw(bpos_t pos) {
    glm::mat4 model = genModelMat3d(bpos_to_pos(pos));
    shader.uMat4("uModel", model);
    vao.bind();
    glDisable(GL_CULL_FACE);
    gl.draw_vao_ibo(ibo, GL_LINES);
}
