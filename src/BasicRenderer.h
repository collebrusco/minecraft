/** 
 * PointRenderer.h 
 * minecraft
 * created 04/25/25 by frank collebrusco
 */
#ifndef BASIC_RENDERER_H
#define BASIC_RENDERER_H
#include "standard.h"

struct PointRenderer {
    static VertexArray vao;
    static VertexBuffer<glm::vec2> vbo;
    static Shader shader;
    static void init();
    static void destroy();

    static void render();
};

struct WorldPointRenderer {
    static VertexArray vao;
    static VertexBuffer<glm::vec3> vbo;
    static Shader shader;
    static void init();
    static void destroy();

    static void sync(Camera& cam);
    static void render(pos_t pos);
};

#endif /* BASIC_RENDERER_H */
