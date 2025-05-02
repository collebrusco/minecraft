#include "SkyboxRenderer.h"

Shader SkyboxRenderer::shader;
Texture SkyboxRenderer::texture(GL_TEXTURE_CUBE_MAP);
VertexArray SkyboxRenderer::vao;
VertexBuffer<Vertex_3f> SkyboxRenderer::vbo;

void SkyboxRenderer::init() {
    Vertex_3f cubepos[] = {
        // positions          
        {-1.0f,  1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f},
         {1.0f, -1.0f, -1.0f},
         {1.0f, -1.0f, -1.0f},
         {1.0f,  1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},
    
        {-1.0f, -1.0f,  1.0f},
        {-1.0f, -1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},
        {-1.0f,  1.0f,  1.0f},
        {-1.0f, -1.0f,  1.0f},
    
         {1.0f, -1.0f, -1.0f},
         {1.0f, -1.0f,  1.0f},
         {1.0f,  1.0f,  1.0f},
         {1.0f,  1.0f,  1.0f},
         {1.0f,  1.0f, -1.0f},
         {1.0f, -1.0f, -1.0f},
    
        {-1.0f, -1.0f,  1.0f},
        {-1.0f,  1.0f,  1.0f},
         {1.0f,  1.0f,  1.0f},
         {1.0f,  1.0f,  1.0f},
         {1.0f, -1.0f,  1.0f},
        {-1.0f, -1.0f,  1.0f},
    
        {-1.0f,  1.0f, -1.0f},
         {1.0f,  1.0f, -1.0f},
         {1.0f,  1.0f,  1.0f},
         {1.0f,  1.0f,  1.0f},
        {-1.0f,  1.0f,  1.0f},
        {-1.0f,  1.0f, -1.0f},
    
        {-1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f,  1.0f},
         {1.0f, -1.0f, -1.0f},
         {1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f,  1.0f},
         {1.0f, -1.0f,  1.0f}
    };
    vao.create_bind();
    vbo.create_bind();
    vao.attach(vbo);
    vbo.buffer_data(36, cubepos);

    shader = Shader::from_source("skybox_vert", "skybox_frag");
    vao.unbind();
    vbo.unbind();

    /*
    0 → NEGATIVE_X
    1 → NEGATIVE_Z
    2 → POSITIVE_x
    3 → POSITIVE_Z
    4 → POSITIVE_Y
    5 → NEGATIVE_Y
    */
    texture = Texture::from_cubemap_files("cubemap/rcubemap_0", "cubemap/rcubemap_1","cubemap/rcubemap_2","cubemap/rcubemap_3","cubemap/rcubemap_4","cubemap/rcubemap_5");


}

void SkyboxRenderer::destroy() {
    vao.destroy();
    vbo.destroy();
    shader.destroy();
    texture.destroy();
}


void SkyboxRenderer::sync(Camera& cam) {
    shader.bind();
    glm::mat4 view = glm::mat4(glm::mat3(cam.view()));
    shader.uViewProj(view, cam.proj());
}

void SkyboxRenderer::render() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    shader.bind();
    texture.bind();
    vao.bind();
    gl.draw_arrays(GL_TRIANGLES, 0, 36);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

}


