/** 
 * Minecraft.h 
 * created 04/16/25 by frank collebrusco
 */
#ifndef MINECRAFT_H
#define MINECRAFT_H
#include <flgl.h>
#include <flgl/driver.h>
#include <flgl/ecs.h>
#include <flgl/glm.h>
#include "standard.h"
#include "Chunk.h"
#include "ChunkRenderer.h"
#include "WorldRenderer.h"
#include "util/DebugUI.h"

struct Minecraft : public Driver {

    Minecraft();
    virtual void user_create() override final;
    virtual void user_update(float dt, Keyboard const& kb, Mouse const& mouse) override final;
    virtual void user_render() override final;
    virtual void user_destroy() override final;

    MousePerspectiveCamera camera;

    BasicWorldGen wgen;
    World world;
    WorldRenderer wrenderer;

    DebugUI dbui;

};


#endif /* MINECRAFT_H */
