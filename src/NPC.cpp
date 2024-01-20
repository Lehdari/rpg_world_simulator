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
#include "World.hpp"

#include <algorithm>


NPC::NPC(const Vec2f& position)
{
    _orientation.setPosition(position);
    _orientation.setRotation(rnd(0.0f, 2.0f*M_PI));

    _sprite.setSpriteId(0);
    _sprite.setOrigin(Vec2f(64.0f, 64.0f));
    _sprite.setScale(Vec2f(1.0f/64.0f, 1.0f/64.0f));

    _speed = rnd(-0.002, 0.02);
}

void NPC::update(World* world)
{
    // Random movement (for now)
    _speed = std::clamp(_speed+rnd(-0.0001, 0.001), -0.005, 0.05);
    _orientation.rotate(rnd(-0.01, 0.01));

    // Collision check with world boundary
    auto& position = _orientation.getPosition();
    if (position.squaredNorm() >= world->getSize()*world->getSize()) {
        _orientation.setRotation(atan2f(-position(1), -position(0))); // turn towards origin
        if (_speed <= 0.0)
            _speed = 0.001; // force forward movement
    }

    // Move
    _orientation.translate((_orientation.getOrientation() * Vec3f((float)_speed, 0.0f, 0.0f)).block<2,1>(0,0));
}

void NPC::render(SpriteRenderer* renderer)
{
    _sprite.render(renderer, _orientation);
}
