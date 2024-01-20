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

#include <algorithm>


NPC::NPC(const Vec2f& position)
{
    _orientation.setPosition(position);
    _orientation.setRotation(rnd(0.0f, 2.0f*M_PI));

    _sprite.setSpriteId(0);
    _sprite.setOrigin(Vec2f(64.0f, 64.0f));
    _sprite.setScale(Vec2f(1.0f/64.0f, 1.0f/64.0f));

    _speed = rnd(-0.001, 0.01);
}

void NPC::update()
{
    _speed = std::clamp(_speed+rnd(-0.0001, 0.001), -0.002, 0.02);

    _orientation.translate((_orientation.getOrientation() * Vec3f(_speed, 0.0f, 0.0f)).block<2,1>(0,0));
    _orientation.rotate(rnd(-0.01, 0.01));
}

void NPC::render(SpriteRenderer* renderer)
{
    _sprite.render(renderer, _orientation);
}
