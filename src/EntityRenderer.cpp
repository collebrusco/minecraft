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

struct ModelPart {
    glm::mat4 mat;
    struct {
        glm::vec2 mi;
        glm::vec2 mx;
    } uvs[ORIENTATION_LAST + 1];
};

/** make this \/ return those /\ */
std::vector<ModelPart>getSteveModelMatrices(float t, bool isHead = true) {
    std::vector<ModelPart> parts;

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

    ModelPart head;
    glm::mat4 hd = glm::scale(glm::mat4(1.0f), headSize);
    head.mat = hd;
    head.uvs[TOP].mi = glm::vec2(0.125f, 0.875f);
    head.uvs[TOP].mx = glm::vec2(0.25f, 1.0f);
    head.uvs[BOT].mi = glm::vec2(0.25f, 0.875f);
    head.uvs[BOT].mx = glm::vec2(0.375f, 1.0f);
    head.uvs[WEST].mi = glm::vec2(0.0f, 0.75f);
    head.uvs[WEST].mx = glm::vec2(0.125f, 0.875f);
    head.uvs[NORTH].mi = glm::vec2(0.125f, 0.75f);
    head.uvs[NORTH].mx = glm::vec2(0.25f, 0.875f);
    head.uvs[EAST].mi = glm::vec2(0.25f, 0.75f);
    head.uvs[EAST].mx = glm::vec2(0.375f, 0.875f);
    head.uvs[SOUTH].mi = glm::vec2(0.375f, 0.75f);
    head.uvs[SOUTH].mx = glm::vec2(0.5f, 0.875f);
    
    ModelPart body;
    glm::mat4 bodyMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -((headSize.y / 2.0f) + (bodySize.y / 2.0f)) + rise, 0.0f));
    bodyMat = glm::scale(bodyMat, bodySize);
    body.mat = bodyMat;
    body.uvs[TOP].mi = glm::vec2(0.3125f, 0.6875f);
    body.uvs[TOP].mx = glm::vec2(0.4375f, 0.75f);
    body.uvs[BOT].mi = glm::vec2(0.4375f, 0.6875f);
    body.uvs[BOT].mx = glm::vec2(0.5625f, 0.75f);
    body.uvs[WEST].mi = glm::vec2(0.25f, 0.5f);
    body.uvs[WEST].mx = glm::vec2(0.3125f, 0.6875f);
    body.uvs[NORTH].mi = glm::vec2(0.3125f, 0.5f);
    body.uvs[NORTH].mx = glm::vec2(0.4375f, 0.6875f);
    body.uvs[EAST].mi = glm::vec2(0.4375f, 0.5f);
    body.uvs[EAST].mx = glm::vec2(0.5f, 0.6875f);
    body.uvs[SOUTH].mi = glm::vec2(0.5f, 0.5f);
    body.uvs[SOUTH].mx = glm::vec2(0.625f, 0.6875f);
    
    ModelPart rightArm;
    glm::mat4 rightArmMat = glm::translate(glm::mat4(1.0f), glm::vec3((armSize.x / 2.0f) + (bodySize.x / 2.0f), -0.57f + rise, 0.0f));
    rightArmMat = glm::translate(rightArmMat, glm::vec3(0.0f, 0.35f, 0.0f));
    rightArmMat = rightArmMat * glm::toMat4(glm::angleAxis(rightArmAngle, glm::vec3(1, 0, 0)));
    rightArmMat = glm::translate(rightArmMat, glm::vec3(0.0f, -0.35f, 0.0f));
    rightArmMat = glm::scale(rightArmMat, armSize);
    rightArm.mat = rightArmMat;
    rightArm.uvs[TOP].mi = glm::vec2(0.6875f, 0.6875f);
    rightArm.uvs[TOP].mx = glm::vec2(0.75f, 0.75f);
    rightArm.uvs[BOT].mi = glm::vec2(0.75f, 0.6875f);
    rightArm.uvs[BOT].mx = glm::vec2(0.8125f, 0.75f);
    rightArm.uvs[WEST].mi = glm::vec2(0.625f, 0.5f);
    rightArm.uvs[WEST].mx = glm::vec2(0.6875f, 0.6875f);
    rightArm.uvs[NORTH].mi = glm::vec2(0.6875f, 0.5f);
    rightArm.uvs[NORTH].mx = glm::vec2(0.75f, 0.6875f);
    rightArm.uvs[EAST].mi = glm::vec2(0.75f, 0.5f);
    rightArm.uvs[EAST].mx = glm::vec2(0.8125f, 0.6875f);
    rightArm.uvs[SOUTH].mi = glm::vec2(0.8125f, 0.5f);
    rightArm.uvs[SOUTH].mx = glm::vec2(0.875f, 0.6875f);
    
    ModelPart leftArm;
    glm::mat4 leftArmMat = glm::translate(glm::mat4(1.0f), glm::vec3(-((armSize.x / 2.0f) + (bodySize.x / 2.0f)), -0.57f + rise, 0.0f));
    leftArmMat = glm::translate(leftArmMat, glm::vec3(0.0f, 0.35f, 0.0f));
    leftArmMat = leftArmMat * glm::toMat4(glm::angleAxis(leftArmAngle, glm::vec3(1, 0, 0)));
    leftArmMat = glm::translate(leftArmMat, glm::vec3(0.0f, -0.35f, 0.0f));
    leftArmMat = glm::scale(leftArmMat, armSize);
    leftArm.mat = leftArmMat;
    leftArm.uvs[TOP].mi = glm::vec2(0.5625f, 0.1875f);
    leftArm.uvs[TOP].mx = glm::vec2(0.625f, 0.25f);
    leftArm.uvs[BOT].mi = glm::vec2(0.625f, 0.1875f);
    leftArm.uvs[BOT].mx = glm::vec2(0.6875f, 0.25f);
    leftArm.uvs[WEST].mi = glm::vec2(0.5f, 0.0f);
    leftArm.uvs[WEST].mx = glm::vec2(0.5625f, 0.1875f);
    leftArm.uvs[NORTH].mi = glm::vec2(0.5625f, 0.0f);
    leftArm.uvs[NORTH].mx = glm::vec2(0.625f, 0.1875f);
    leftArm.uvs[EAST].mi = glm::vec2(0.625f, 0.0f);
    leftArm.uvs[EAST].mx = glm::vec2(0.6875f, 0.1875f);
    leftArm.uvs[SOUTH].mi = glm::vec2(0.6875f, 0.0f);
    leftArm.uvs[SOUTH].mx = glm::vec2(0.75f, 0.1875f);
    
    ModelPart rightLeg;
    glm::mat4 rightLegMat = glm::translate(glm::mat4(1.0f), glm::vec3(legSize.x / 2.0f, -(bodySize.y + headSize.y / 2.0f + legSize.y / 2.0f) + rise + 0.025f, 0.0f));
    rightLegMat = glm::translate(rightLegMat, glm::vec3(0.0f, 0.35f, 0.0f));
    rightLegMat = rightLegMat * glm::toMat4(glm::angleAxis(rightLegAngle, glm::vec3(1, 0, 0)));
    rightLegMat = glm::translate(rightLegMat, glm::vec3(0.0f, -0.35f, 0.0f));
    rightLegMat = glm::scale(rightLegMat, legSize);
    rightLeg.mat = rightLegMat;
    rightLeg.uvs[TOP].mi = glm::vec2(0.0625f, 0.6875f);
    rightLeg.uvs[TOP].mx = glm::vec2(0.125f, 0.75f);
    rightLeg.uvs[BOT].mi = glm::vec2(0.125f, 0.6875f);
    rightLeg.uvs[BOT].mx = glm::vec2(0.1875f, 0.75f);
    rightLeg.uvs[WEST].mi = glm::vec2(0.0f, 0.5f);
    rightLeg.uvs[WEST].mx = glm::vec2(0.0625f, 0.6875f);
    rightLeg.uvs[NORTH].mi = glm::vec2(0.0625f, 0.5f);
    rightLeg.uvs[NORTH].mx = glm::vec2(0.125f, 0.6875f);
    rightLeg.uvs[EAST].mi = glm::vec2(0.125f, 0.5f);
    rightLeg.uvs[EAST].mx = glm::vec2(0.1875f, 0.6875f);
    rightLeg.uvs[SOUTH].mi = glm::vec2(0.1875f, 0.5f);
    rightLeg.uvs[SOUTH].mx = glm::vec2(0.25f, 0.6875f);
    
    ModelPart leftLeg;
    glm::mat4 leftLegMat = glm::translate(glm::mat4(1.0f), glm::vec3(-legSize.x / 2.0f, -(bodySize.y + headSize.y / 2.0f + legSize.y / 2.0f) + rise + 0.025f, 0.0f));
    leftLegMat = glm::translate(leftLegMat, glm::vec3(0.0f, 0.35f, 0.0f));
    leftLegMat = leftLegMat * glm::toMat4(glm::angleAxis(leftLegAngle, glm::vec3(1, 0, 0)));
    leftLegMat = glm::translate(leftLegMat, glm::vec3(0.0f, -0.35f, 0.0f));
    leftLegMat = glm::scale(leftLegMat, legSize);
    leftLeg.mat = leftLegMat;
    leftLeg.uvs[TOP].mi = glm::vec2(0.3125f, 0.1875f);
    leftLeg.uvs[TOP].mx = glm::vec2(0.375f, 0.25f);
    leftLeg.uvs[BOT].mi = glm::vec2(0.375f, 0.1875f);
    leftLeg.uvs[BOT].mx = glm::vec2(0.4375f, 0.25f);
    leftLeg.uvs[WEST].mi = glm::vec2(0.25f, 0.0f);
    leftLeg.uvs[WEST].mx = glm::vec2(0.3125f, 0.1875f);
    leftLeg.uvs[NORTH].mi = glm::vec2(0.3125f, 0.0f);
    leftLeg.uvs[NORTH].mx = glm::vec2(0.375f, 0.1875f);
    leftLeg.uvs[EAST].mi = glm::vec2(0.375f, 0.0f);
    leftLeg.uvs[EAST].mx = glm::vec2(0.4375f, 0.1875f);
    leftLeg.uvs[SOUTH].mi = glm::vec2(0.4375f, 0.0f);
    leftLeg.uvs[SOUTH].mx = glm::vec2(0.5f, 0.1875f);
    
     

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
