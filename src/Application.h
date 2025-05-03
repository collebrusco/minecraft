/** 
 * Application.h 
 * created 04/16/25 by frank collebrusco
 */
#ifndef MINECRAFT_H
#define MINECRAFT_H
#include <flgl.h>
#include <flgl/driver.h>
#include <flgl/ecs.h>
#include <flgl/glm.h>
#include "standard.h"
#include "data/Chunk.h"
#include "data/World.h"
#include "data/State.h"
#include "game/state_driver.h"
#include "render/ChunkRenderer.h"
#include "render/WorldRenderer.h"
#include "util/DebugUI.h"
#include "game/WorldGen.h"
#include "render/UI.h"
#include "render/SkyboxRenderer.h"

struct Application : public Driver {

    Application();
    virtual void user_create() override final;
    virtual void user_update(float dt, Keyboard const& kb, Mouse const& mouse) override final;
    virtual void user_render() override final;
    virtual void user_destroy() override final;

    PerlinWorldGen wgen;

    State state;
    StateDriver driver;

    WorldRenderer wrenderer;

    SkyboxRenderer srenderer;

    DebugUI dbui;

    MinecraftUI ui;

};


#endif /* MINECRAFT_H */
