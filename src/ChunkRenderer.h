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

struct ChunkRenderer {

    static VertexArray vao;
    static VertexBuffer<Vt_pun> vbo;
    static ElementBuffer ibo;
    static Shader shader;

    std::vector<uint32_t> instance_data;
    InstanceBuffer<iAttr_u32> instance_buffer;

    static void init_chunk_rendering();
    static void destroy_chunk_rendering();

    void init();
    void destroy();

    void attach() const;
    void update(Chunk const& chunk);
    void sync(Camera const& cam);
};


#endif /* CHUNK_RENDERER_H */
