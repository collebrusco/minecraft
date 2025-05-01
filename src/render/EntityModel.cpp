#include "EntityModel.h"

Texture SteveModel::texture;

void EntityModel::init() {
    SteveModel::init();
    CreeperModel::init();
}

void EntityModel::destroy() {
    SteveModel::destroy();
    CreeperModel::destroy();
}

void SteveModel::init() {
    texture = Texture::from_file("Creeper_T-Shirt");
    texture.pixelate();
}

void SteveModel::destroy() {
    texture.destroy();
}

void SteveModel::prepare_render() const {
    texture.bind();
}

Model SteveModel::get() const {
    Model parts;

    glm::vec3 headSize(0.5f, 0.5f, 0.5f); // 8 * 1/16 etc. 
    glm::vec3 bodySize(0.5f, 0.75f, 0.25f);
    glm::vec3 armSize(0.25f, 0.75f, 0.25f);
    glm::vec3 legSize(0.25f, 0.75f, 0.25f);
    

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
    
     

    parts.push_back(head);
    parts.push_back(body);
    parts.push_back(leftArm);
    parts.push_back(rightArm);
    parts.push_back(leftLeg);
    parts.push_back(rightLeg);

    return parts;

}

Texture CreeperModel::texture;

void CreeperModel::init() {
    texture = Texture::from_file("creeper");
    texture.pixelate();
}

void CreeperModel::destroy() {
    texture.destroy();
}

void CreeperModel::prepare_render() const {
    texture.bind();
}


Model CreeperModel::get() const{
    Model parts;

    glm::vec3 headSize(8 * 0.0625f, 8 * 0.0625f, 8 * 0.0625f); // 8x8x8
    glm::vec3 bodySize(8 * 0.0625f, 12 * 0.0625f, 4 * 0.0625f); // 8x12x4
    glm::vec3 legSize(4 * 0.0625f, 6 * 0.0625f, 4 * 0.0625f);  // 4x6x4
    
    float maxAngle = glm::pi<float>() / 8.0f;
    float frontLeftBackRightAngle  =  std::sin(t) * maxAngle;
    float frontRightBackLeftAngle = -std::sin(t) * maxAngle;
    
    ModelPart head;
    glm::vec3 headOffset = glm::vec3(0.0f, legSize.y + bodySize.y + headSize.y / 2.0f, 0.0f);
    head.mat = glm::translate(glm::mat4(1.0f), headOffset);
    head.mat = glm::scale(head.mat, headSize);
    head.uvs[TOP]   = { glm::vec2(0.125f, 0.75f), glm::vec2(0.25f, 1.0f) };
    head.uvs[BOT]   = { glm::vec2(0.25f, 0.75f), glm::vec2(0.375f, 1.0f) };
    head.uvs[WEST]  = { glm::vec2(0.0f, 0.5f), glm::vec2(0.125f, 0.75f) };
    head.uvs[NORTH] = { glm::vec2(0.125f, 0.5f), glm::vec2(0.25f, 0.75f) };
    head.uvs[EAST]  = { glm::vec2(0.25f, 0.5f), glm::vec2(0.375f, 0.75f) };
    head.uvs[SOUTH] = { glm::vec2(0.375f, 0.5f), glm::vec2(0.5f, 0.75f) };
    parts.push_back(head); 

    ModelPart body;
    glm::vec3 bodyOffset = glm::vec3(0.0f, legSize.y + bodySize.y / 2.0f, 0.0f);
    body.mat = glm::translate(glm::mat4(1.0f), bodyOffset);
    body.mat = glm::scale(body.mat, bodySize);
    body.uvs[TOP]   = { glm::vec2(0.3125f, 0.375f), glm::vec2(0.4375f, 0.5f) };
    body.uvs[BOT]   = { glm::vec2(0.4375f, 0.375f), glm::vec2(0.5625f, 0.5f) };
    body.uvs[WEST]  = { glm::vec2(0.25f, 0.0f), glm::vec2(0.3125f, 0.375f) };
    body.uvs[NORTH] = { glm::vec2(0.3125f, 0.0f), glm::vec2(0.4375f, 0.375f) };
    body.uvs[EAST]  = { glm::vec2(0.4375f, 0.0f), glm::vec2(0.5f, 0.375f) };
    body.uvs[SOUTH] = { glm::vec2(0.5f, 0.0f), glm::vec2(0.625f, 0.375f) };
    parts.push_back(body);

    std::vector<glm::vec3> legOffsets = {
        {-0.125f, legSize.y / 2.0f,  0.25f},  // Front Left
        { 0.125f, legSize.y / 2.0f,  0.25f},  // Front Right
        {-0.125f, legSize.y / 2.0f, -0.25f},  // Back Left
        { 0.125f, legSize.y / 2.0f, -0.25f},  // Back Right
    };

    for (int i = 0; i < 4; ++i) {
        float angle = (i == 0 || i == 3) ? frontLeftBackRightAngle : frontRightBackLeftAngle;
        ModelPart leg;
        glm::mat4 legMat = glm::translate(glm::mat4(1.0f), legOffsets[i]);
        legMat = glm::translate(legMat, glm::vec3(0.0f, legSize.y / 2.0f, 0.0f));
        legMat = legMat * glm::toMat4(glm::angleAxis(angle, glm::vec3(1, 0, 0)));
        legMat = glm::translate(legMat, glm::vec3(0.0f, -legSize.y / 2.0f, 0.0f));
        legMat = glm::scale(legMat, legSize);
        leg.mat = legMat;
        leg.uvs[TOP]   = { glm::vec2(0.0625f, 0.375f), glm::vec2(0.125f, 0.5f) };
        leg.uvs[BOT]   = { glm::vec2(0.125f, 0.375f), glm::vec2(0.1875f, 0.5f) };
        leg.uvs[WEST]  = { glm::vec2(0.0f, 0.1875f), glm::vec2(0.0625f, 0.375f) };
        leg.uvs[NORTH] = { glm::vec2(0.0625f, 0.1875f), glm::vec2(0.125f, 0.375f) };
        leg.uvs[EAST]  = { glm::vec2(0.125f, 0.1875f), glm::vec2(0.1875f, 0.375f) };
        leg.uvs[SOUTH] = { glm::vec2(0.1875f, 0.1875f), glm::vec2(0.25f, 0.375f) };
        parts.push_back(leg);
    }

    return parts;
}
