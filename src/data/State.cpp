#include "State.h"
#include "game/components.h"

State::State(WorldGenerator &wgen) : World(wgen), creative(false), player_look(true) {
}

void State::set_creative(bool c) {
    creative = c;
    getComp<c_Physics>(player).mass = creative ? 0.f : 1.f;
}

void State::set_playerlook(bool p) {
    player_look = p;
    window.set_mouse_grab(player_look);
}
