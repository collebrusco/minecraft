/** 
 * EntityRenderer.h t
 * minecraft
 * created 04/24/25 by frank collebrusco
 */
#ifndef ENTITY_RENDERER_H
#define ENTITY_RENDERER_H
#include "standard.h"
#include <flgl.h>
#include "EntityModel.h"

struct EntityRenderer{
    static void init();
    static void destroy();

    static void render(EntityModel& model, glm::vec3 pos, glm::vec3 rotation=glm::vec3(0.), glm::vec3 scale=glm::vec3(1.));
    static VertexArray vao;
    static VertexBuffer<Vt_pun> vbo;
    static ElementBuffer ibo;

    static Shader entity_shader;
    static void sync(Camera& cam);
};

#endif /* ENTITY_RENDERER_H */
