/** 
 * OutlineRenderer.h 
 * minecraft
 * created 04/24/25 by frank collebrusco
 */
#ifndef OUTLINE_RENDERER_H
#define OUTLINE_RENDERER_H
#include "standard.h"

struct OutlineRenderer {

    static Shader shader;
    static VertexArray vao;
    static VertexBuffer<glm::vec3> vbo;
    static ElementBuffer ibo;

    static void init();
    static void destroy();

    static void sync(Camera& cam);
    static void draw(bpos_t pos);

};


#endif /* OUTLINE_RENDERER_H */
