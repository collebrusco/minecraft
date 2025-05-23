/** 
 * ChunkRenderer.h 
 * minecraft
 * created 04/20/25 by frank collebrusco
 */
#ifndef CHUNK_RENDERER_H
#define CHUNK_RENDERER_H
#include <vector>
#include <flgl.h>
#include "data/Chunk.h"
#include "data/World.h"
#include "data/Blocks.h"

struct ChunkRenderer {

    static VertexArray vao;
    static VertexBuffer<Vt_pun> vbo;
    static ElementBuffer ibo;
    static Shader shader;
    static Texture texture;

    glm::mat4 model;
    std::vector<uint32_t> instance_data;
    InstanceBuffer<iAttr_u32> instance_buffer;

    static void init_chunk_rendering();
    static void destroy_chunk_rendering();

    void init();
    void destroy();

    void attach() const;
    bool update(Chunk const& chunk, World const& world);
    void buffer();
    static void sync(Camera const& cam);

    void render() const;

    void emit_cube(int x, int y, int z);
    void emit_face(orientation_e o, Faces face, int x, int y, int z);
};


#endif /* CHUNK_RENDERER_H */
