#include "ChunkRenderer.h"
using namespace glm;
LOG_MODULE(crend);

VertexArray ChunkRenderer::vao;
VertexBuffer<Vt_pun> ChunkRenderer::vbo;
ElementBuffer ChunkRenderer::ibo;
Shader ChunkRenderer::shader;

typedef enum {
    TOP     = 0,
    BOT     = 1,
    NORTH   = 2,
    SOUTH   = 3,
    EAST    = 4,
    WEST    = 5
} orientation_e;

#define HEIGHT_BITS (8)
#define X_BITS      (4)
#define Y_BITS      (4)
#define ORIENT_BITS (3)
#define TEX_BITS (32 - HEIGHT_BITS - X_BITS - Y_BITS - ORIENT_BITS)
static_assert((HEIGHT_BITS + X_BITS + Y_BITS + ORIENT_BITS + TEX_BITS) == 32);
static_assert((1<<X_BITS) == (1<<Y_BITS) && (1<<X_BITS) == CHUNK_SIZE);
static_assert((1<<HEIGHT_BITS) == MAX_HEIGHT);

union instance_u {
    struct _PACKED_ instance_data {
        uint32_t x                  : X_BITS;
        uint32_t y                  : Y_BITS;
        uint32_t height             : HEIGHT_BITS;
        orientation_e orientation   : ORIENT_BITS;
        uint32_t tex                : TEX_BITS;
    } f;
    static_assert(sizeof(instance_data) == sizeof(uint32_t));
    uint32_t val;
};

#define QUAD (0.51)

void ChunkRenderer::init_chunk_rendering() {

    shader = Shader::from_source("chunk_vert", "chunk_frag");

    const Vt_pun verts[] = {
        {{-QUAD,-QUAD,-QUAD}, {0.,0.}, {0.,0.,-1.}},
        {{-QUAD, QUAD,-QUAD}, {0.,1.}, {0.,0.,-1.}},
        {{ QUAD, QUAD,-QUAD}, {1.,1.}, {0.,0.,-1.}},
        {{ QUAD,-QUAD,-QUAD}, {1.,0.}, {0.,0.,-1.}}
    };

    const uint32_t elems[] = {
        0, 2, 1,
        0, 3, 2
    };

    vao.create_bind();
    
    vbo.create_bind();
    vbo.buffer_data(4, verts);
    vao.attach(vbo);

    ibo.create_bind();
    ibo.buffer_data(6, elems);

    vao.unbind();
    vbo.unbind();
    ibo.unbind();
}
void ChunkRenderer::destroy_chunk_rendering() {
    vao.destroy();
    vbo.destroy();
    ibo.destroy();
    shader.destroy();
}

void ChunkRenderer::init() {
    this->instance_buffer.create_bind();
}
void ChunkRenderer::destroy() {
    this->instance_buffer.destroy();
}

void ChunkRenderer::attach() const {
    vao.bind();
    instance_buffer.bind();
    vao.attach(instance_buffer, 3);
    vao.unbind();
    instance_buffer.unbind();
}

void ChunkRenderer::update(Chunk const &chunk) {
    instance_data.clear();
    for (int y = 0; y < MAX_HEIGHT; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                bpos_t local{x,y,z};
                Block const* b = chunk.blockAt(local);
                if (b && !b->empty()) {
                    emit_cube(x,y,z);
                }
            }
        }
    }

    instance_buffer.bind();
    instance_buffer.buffer(instance_data);
    instance_buffer.unbind();
}
void ChunkRenderer::sync(Camera const& cam) {
    shader.uViewProj(cam.view(), cam.proj());
    shader.uMat4("uModel", glm::identity<glm::mat4>());
    shader.uVec3("uLightdir", glm::normalize(vec3{-1., -8., -2.}));
}

void ChunkRenderer::render() const {
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    gl.enable_depth_test();
    vao.bind();
    shader.bind();
    gl.draw_vao_ibo_instanced(ibo, instance_data.size());
}

void ChunkRenderer::emit_cube(int sx, int sy, int sz) {
    uint32_t x = sx; uint32_t y = sy; uint32_t z = sz;
    instance_data.push_back((instance_u){.f.x = x, .f.height = y, .f.y = z, .f.orientation = TOP}.val);
    instance_data.push_back((instance_u){.f.x = x, .f.height = y, .f.y = z, .f.orientation = BOT}.val);
    instance_data.push_back((instance_u){.f.x = x, .f.height = y, .f.y = z, .f.orientation = NORTH}.val);
    instance_data.push_back((instance_u){.f.x = x, .f.height = y, .f.y = z, .f.orientation = SOUTH}.val);
    instance_data.push_back((instance_u){.f.x = x, .f.height = y, .f.y = z, .f.orientation = EAST}.val);
    instance_data.push_back((instance_u){.f.x = x, .f.height = y, .f.y = z, .f.orientation = WEST}.val);
}
