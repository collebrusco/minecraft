#include "Minecraft.h"
#include <flgl/logger.h>
LOG_MODULE(mc);


Minecraft::Minecraft() {
    const float kb = (static_cast<float>(sizeof(Block)) * CHUNK_SIZE_F * CHUNK_SIZE_F * MAX_HEIGHT_F) / 1024.f;
    const float mb = kb / 1024.f;
    LOG_INF("minecraft init");
    LOG_INF("chunk size: %.0f KB (%.1f MB);", kb, mb);
    LOG_INF("world %.0f MB (%.1f GB)", mb * RENDER_DISTANCE_F * RENDER_DISTANCE_F, mb * RENDER_DISTANCE_F * RENDER_DISTANCE_F * (1./1024.));
}


void Minecraft::user_create() {
    
}

void Minecraft::user_update(float dt, Keyboard const& kb, Mouse const& mouse) {

}

void Minecraft::user_render() {

}

void Minecraft::user_destroy() {

}
