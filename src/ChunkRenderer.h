/** 
 * ChunkRenderer.h 
 * minecraft
 * created 04/20/25 by frank collebrusco
 */
#ifndef CHUNK_RENDERER_H
#define CHUNK_RENDERER_H
#include <vector>
#include <flgl.h>
#include "Chunk.h"

typedef enum {
    TOP     = 0,
    BOT     = 1,
    NORTH   = 2,
    SOUTH   = 3,
    EAST    = 4,
    WEST    = 5
} orientation_e;

struct ChunkRenderer {

    static VertexArray vao;
    static VertexBuffer<Vt_pun> vbo;
    static ElementBuffer ibo;
    static Shader shader;

    glm::mat4 model;
    std::vector<uint32_t> instance_data;
    InstanceBuffer<iAttr_u32> instance_buffer;

    static void init_chunk_rendering();
    static void destroy_chunk_rendering();

    void init();
    void destroy();

    void attach() const;
    bool update(Chunk const& chunk);
    void buffer();
    static void sync(Camera const& cam);

    void render() const;

    void emit_cube(int x, int y, int z);
    void emit_face(orientation_e o, int x, int y, int z);
};


#endif /* CHUNK_RENDERER_H */
