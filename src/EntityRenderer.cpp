#include "EntityRenderer.h"
#define QUAD (0.5f)
 VertexArray EntityRenderer::vao;
 VertexBuffer<Vt_pun> EntityRenderer::vbo;
 ElementBuffer EntityRenderer::ibo;

 Shader EntityRenderer::entity_shader;
 Texture texture; 
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
   texture = Texture::from_file("1.8_Skin_Template");
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
std::vector<glm::mat4> getSteveModelMatrices(float t, bool isHead = true) {
    std::vector<glm::mat4> parts;

    glm::vec3 headSize(0.4f, 0.4f, 0.4f);
    glm::vec3 bodySize(0.5f, 0.66f, 0.25f);
    glm::vec3 armSize(0.2f, 0.7f, 0.25f);
    glm::vec3 legSize(0.25f, 0.7f, 0.25f);

    float maxAngle = glm::pi<float>() / 4.0f;
    float leftArmAngle  =  std::sin(t) * maxAngle;
    float rightArmAngle = -std::sin(t) * maxAngle;
    float leftLegAngle  = -std::sin(t) * maxAngle;
    float rightLegAngle =  std::sin(t) * maxAngle;
    float rise = 0.0f;

    glm::mat4 head = glm::scale(glm::mat4(1.0f), headSize);

    glm::mat4 body = glm::translate(glm::mat4(1.0f),
        glm::vec3(0.0f, -((headSize.y / 2.0f) + (bodySize.y / 2.0f)) + rise, 0.0f));
    body = glm::scale(body, bodySize);

    glm::mat4 leftArm = glm::translate(glm::mat4(1.0f),
        glm::vec3(-((armSize.x / 2.0f) + (bodySize.x / 2.0f)), -0.57f + rise, 0.0f));
    leftArm = glm::translate(leftArm, glm::vec3(0.0f, 0.35f, 0.0f));
    leftArm = leftArm * glm::toMat4(glm::angleAxis(leftArmAngle, glm::vec3(1, 0, 0)));
    leftArm = glm::translate(leftArm, glm::vec3(0.0f, -0.35f, 0.0f));
    leftArm = glm::scale(leftArm, armSize);

    glm::mat4 rightArm = glm::translate(glm::mat4(1.0f),
        glm::vec3((armSize.x / 2.0f) + (bodySize.x / 2.0f), -0.57f + rise, 0.0f));
    rightArm = glm::translate(rightArm, glm::vec3(0.0f, 0.35f, 0.0f));
    rightArm = rightArm * glm::toMat4(glm::angleAxis(rightArmAngle, glm::vec3(1, 0, 0)));
    rightArm = glm::translate(rightArm, glm::vec3(0.0f, -0.35f, 0.0f));
    rightArm = glm::scale(rightArm, armSize);

    glm::mat4 leftLeg = glm::translate(glm::mat4(1.0f),
        glm::vec3(-legSize.x / 2.0f, -(bodySize.y + headSize.y / 2.0f + legSize.y / 2.0f) + rise + 0.025f, 0.0f));
    leftLeg = glm::translate(leftLeg, glm::vec3(0.0f, 0.35f, 0.0f));
    leftLeg = leftLeg * glm::toMat4(glm::angleAxis(leftLegAngle, glm::vec3(1, 0, 0)));
    leftLeg = glm::translate(leftLeg, glm::vec3(0.0f, -0.35f, 0.0f));
    leftLeg = glm::scale(leftLeg, legSize);

    glm::mat4 rightLeg = glm::translate(glm::mat4(1.0f),
        glm::vec3(legSize.x / 2.0f, -(bodySize.y + headSize.y / 2.0f + legSize.y / 2.0f) + rise + 0.025f, 0.0f));
    rightLeg = glm::translate(rightLeg, glm::vec3(0.0f, 0.35f, 0.0f));
    rightLeg = rightLeg * glm::toMat4(glm::angleAxis(rightLegAngle, glm::vec3(1, 0, 0)));
    rightLeg = glm::translate(rightLeg, glm::vec3(0.0f, -0.35f, 0.0f));
    rightLeg = glm::scale(rightLeg, legSize);

    if (isHead)
        parts.push_back(head);
    parts.push_back(body);
    parts.push_back(leftArm);
    parts.push_back(rightArm);
    parts.push_back(leftLeg);
    parts.push_back(rightLeg);

    return parts;
}

void EntityRenderer::render(Stopwatch const& timer,glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale){
    auto modelMats = getSteveModelMatrices(timer.read());
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    gl.enable_depth_test();
    entity_shader.bind();
    glm::mat4 modelmat = genModelMat3d(pos, rotation, scale);
    entity_shader.uMat4("uModel", modelmat);
    vao.bind();
    for(glm::mat4 const& m : modelMats){
        entity_shader.uMat4("uModelPart", m);
        gl.draw_elements(GL_TRIANGLES, ibo.num_elements());
    }
}

void EntityRenderer::sync(Camera &cam){
    entity_shader.bind();
    entity_shader.uViewProj(cam.view(), cam.proj());
}
