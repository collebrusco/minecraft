/** 
 * EntityRenderer.h t
 * minecraft
 * created 04/24/25 by frank collebrusco
 */
#ifndef ENTITY_RENDERER_H
#define ENTITY_RENDERER_H
#include "standard.h"
#include <flgl.h>
struct EntityRenderer{
    static void init();
    static void destroy();

    static void render();
    static VertexArray vao;
    static VertexBuffer<Vt_pun> vbo;
    static ElementBuffer ibo;

    static Shader entity_shader;
};

#endif /* ENTITY_RENDERER_H */
