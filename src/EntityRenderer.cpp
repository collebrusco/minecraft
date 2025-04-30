#include "EntityRenderer.h"
#define QUAD (0.5f)

VertexArray EntityRenderer::vao;
VertexBuffer<Vt_pun> EntityRenderer::vbo;
ElementBuffer EntityRenderer::ibo;

Shader EntityRenderer::entity_shader;
Texture EntityRenderer::texture; 
SteveModel EntityRenderer::steve;

void EntityRenderer::init(){
   const Vt_pun cube_verts[] = {
       // Front face (+Z)
       {{-QUAD, -QUAD, QUAD}, {0, 0}, {0, 0, 1}}, // 0
       {{QUAD, -QUAD, QUAD}, {1, 0}, {0, 0, 1}},  // 1
       {{QUAD, QUAD, QUAD}, {1, 1}, {0, 0, 1}},   // 2
       {{-QUAD, QUAD, QUAD}, {0, 1}, {0, 0, 1}},  // 3

       // Back face (-Z)
       {{QUAD, -QUAD, -QUAD}, {0, 0}, {0, 0, -1}},  // 4
       {{-QUAD, -QUAD, -QUAD}, {1, 0}, {0, 0, -1}}, // 5
       {{-QUAD, QUAD, -QUAD}, {1, 1}, {0, 0, -1}},  // 6
       {{QUAD, QUAD, -QUAD}, {0, 1}, {0, 0, -1}},   // 7

       // Left face (-X)
       {{-QUAD, -QUAD, -QUAD}, {0, 0}, {-1, 0, 0}}, // 8
       {{-QUAD, -QUAD, QUAD}, {1, 0}, {-1, 0, 0}},  // 9
       {{-QUAD, QUAD, QUAD}, {1, 1}, {-1, 0, 0}},   // 10
       {{-QUAD, QUAD, -QUAD}, {0, 1}, {-1, 0, 0}},  // 11

       // Right face (+X)
       {{QUAD, -QUAD, QUAD}, {0, 0}, {1, 0, 0}},  // 12
       {{QUAD, -QUAD, -QUAD}, {1, 0}, {1, 0, 0}}, // 13
       {{QUAD, QUAD, -QUAD}, {1, 1}, {1, 0, 0}},  // 14
       {{QUAD, QUAD, QUAD}, {0, 1}, {1, 0, 0}},   // 15

       // Top face (+Y)
       {{-QUAD, QUAD, QUAD}, {0, 0}, {0, 1, 0}},  // 16
       {{QUAD, QUAD, QUAD}, {1, 0}, {0, 1, 0}},   // 17
       {{QUAD, QUAD, -QUAD}, {1, 1}, {0, 1, 0}},  // 18
       {{-QUAD, QUAD, -QUAD}, {0, 1}, {0, 1, 0}}, // 19

       // Bottom face (-Y)
       {{-QUAD, -QUAD, -QUAD}, {0, 0}, {0, -1, 0}}, // 20
       {{QUAD, -QUAD, -QUAD}, {1, 0}, {0, -1, 0}},  // 21
       {{QUAD, -QUAD, QUAD}, {1, 1}, {0, -1, 0}},   // 22
       {{-QUAD, -QUAD, QUAD}, {0, 1}, {0, -1, 0}},  // 23
   };
   const uint32_t cube_elems[] = {
       // Front face
       0, 1, 2, 0, 2, 3,
       // Back face
       4, 5, 6, 4, 6, 7,
       // Left face
       8, 9, 10, 8, 10, 11,
       // Right face
       12, 13, 14, 12, 14, 15,
       // Top face
       16, 17, 18, 16, 18, 19,
       // Bottom face
       20, 21, 22, 20, 22, 23};
   entity_shader = Shader::from_source("entity_vert", "entity_frag");
   // TODO break out into its own thing
   texture = Texture::from_file("steve_template");
   texture.pixelate();
   vao.create_bind();
   vbo.create_bind();
   vao.attach(vbo);
   vbo.buffer_data(24, cube_verts);

   ibo.create_bind();
   ibo.buffer_data(36, cube_elems);

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
    texture.destroy();
}

void EntityRenderer::render(Stopwatch const& timer,glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale){
   // auto modelMats = getSteveModelMatrices(timer.read());
    Model modelMats = steve.get();
    steve.setT(timer.read());
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    gl.enable_depth_test();
    entity_shader.bind();
    texture.bind();

    glm::mat4 modelmat = genModelMat3d(pos, rotation, scale);
    entity_shader.uMat4("uModel", modelmat);
    vao.bind();
    for (ModelPart const &m : modelMats){
        entity_shader.uMat4("uModelPartMat", m.mat);

        glm::vec2 uvsMin[6];
        glm::vec2 uvsMax[6];
        for (int i = 0; i <= ORIENTATION_LAST; i++)
        {
            uvsMin[i] = m.uvs[i].mi; // Get the min corner
            uvsMax[i] = m.uvs[i].mx; // Get the max corner
        }
        entity_shader.uVec2Arr("uUVMin", 6, &uvsMin[0]);
        entity_shader.uVec2Arr("uUVMax", 6, &uvsMax[0]);

        gl.draw_elements(GL_TRIANGLES, ibo.num_elements());
    }
}

void EntityRenderer::sync(Camera &cam){
    entity_shader.bind();
    entity_shader.uViewProj(cam.view(), cam.proj());
}
