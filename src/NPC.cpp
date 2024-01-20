//
// Project: rpg_world_simulator
// File: NPC.cpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "NPC.hpp"
#include "SpriteRenderer.hpp"


NPC::NPC(const Vec2f& position)
{
    _orientation.setPosition(position);
    _sprite.setSpriteId(0);
}

void NPC::render(SpriteRenderer* renderer)
{
    _sprite.render(renderer, _orientation);
}
