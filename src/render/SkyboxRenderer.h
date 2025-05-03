/** 
 * SkyboxRenderer.h 
 * minecraft
 * created 05/01/25 by frank collebrusco
 */
#ifndef SKYBOX_RENDERER_H
#define SKYBOX_RENDERER_H
#include "standard.h"
#include <flgl.h>

struct SkyboxRenderer {

    static Shader shader;
    static Texture texture;
    static VertexArray vao;
    static VertexBuffer<Vertex_3f> vbo;
    
    static void init();
    static void destroy();

    static void sync(Camera& cam);
    static void render();
};


#endif /* SKYBOX_RENDERER_H */
