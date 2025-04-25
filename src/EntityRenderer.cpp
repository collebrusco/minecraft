#include "EntityRenderer.h"
#define QUAD (0.5f)
 VertexArray EntityRenderer::vao;
 VertexBuffer<Vt_pun> EntityRenderer::vbo;
 ElementBuffer EntityRenderer::ibo;

 Shader EntityRenderer::entity_shader;

void EntityRenderer::init(){
    const Vt_pun verts[] = { // Square
        {{-QUAD, -QUAD, 0 }, {0,0}, {0,0,0}},
       {{-QUAD,  QUAD, 0 }, {0,0}, {0,0,0}},
       {{ QUAD,  QUAD, 0 }, {0,0}, {0,0,0}},
       {{ QUAD, -QUAD, 0 }, {0,0}, {0,0,0}},
    };
    const uint32_t elems[] = {
        0,1, 1,2, 2,3, 0,3 
    };
    entity_shader = Shader::from_source("entity_vert", "entity_frag");
    vao.create_bind();
    vbo.create_bind();
    vao.attach(vbo);
    vbo.buffer_data(4, verts);

    ibo.create_bind();
    ibo.buffer_data(8, elems);

    vao.unbind();
    vbo.unbind();
    ibo.unbind();
}


void EntityRenderer::destroy(){
    // vao, vbo, ibo are created. now we destory
    vao.destroy();
    vbo.destroy();
    ibo.destroy();
    entity_shader.destroy();
}

void EntityRenderer::render(){
    entity_shader.bind();
    vao.bind();
    gl.draw_elements(GL_LINES, ibo.num_elements());
}
