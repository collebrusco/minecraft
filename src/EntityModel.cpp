#include "EntityModel.h"

Model SteveModel::get() const{
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

Model CreeperModel::get() const{
    Model parts;
    // Head
    ModelPart head;
    head.mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.85f, 0.0f));
    head.mat = glm::scale(head.mat, glm::vec3(0.4f, 0.4f, 0.4f));
    // Set head UVs here
    parts.push_back(head);

    // Body
    ModelPart body;
    body.mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.4f, 0.0f));
    body.mat = glm::scale(body.mat, glm::vec3(0.4f, 0.6f, 0.2f));
    // Set body UVs here
    parts.push_back(body);

    // Legs
    float legY = -0.3f;
    float legX = 0.15f;
    float legZ = 0.1f;
    for (int i = 0; i < 4; ++i)
    {
        ModelPart leg;
        float x = (i % 2 == 0) ? -legX : legX;
        float z = (i / 2 == 0) ? -legZ : legZ;
        leg.mat = glm::translate(glm::mat4(1.0f), glm::vec3(x, legY, z));
        leg.mat = glm::scale(leg.mat, glm::vec3(0.2f, 0.3f, 0.2f));
        // Set leg UVs here
        parts.push_back(leg);
    }

    return parts;
}
