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
#include "Food.hpp"

#include <algorithm>


NPC::NPC(EntityType&& entity, const Vec2f& position) :
    EntityType(std::forward<EntityType>(entity))
{
    component<Orientation>().setPosition(position);
    component<Orientation>().setRotation(rnd(0.0f, 2.0f*M_PI));

    component<Sprite>().setSpriteId(0);
    component<Sprite>().setOrigin(Vec2f(64.0f, 64.0f));
    component<Sprite>().setScale(Vec2f(1.0f/64.0f, 1.0f/64.0f));

    component<CollisionBody>().setRadius(1.0f);
    component<CollisionBody>().setEntityType<NPC>();

    _speed = rnd(-0.002, 0.02);
}

void NPC::update(World* world)
{
    // Random movement (for now)
    _speed = std::clamp(_speed+rnd(-0.001, 0.0011), -0.005, 0.05);
    component<Orientation>().rotate(rnd(-0.01, 0.01));

    // Collision check with world boundary
    auto& position = component<Orientation>().getPosition();
    if (position.squaredNorm() >= world->getSize()*world->getSize()) {
        component<Orientation>().setRotation(atan2f(-position(1), -position(0))); // turn towards origin
        if (_speed <= 0.0)
            _speed = 0.001; // force forward movement
    }

    // Move
    _velocity = (component<Orientation>().getOrientation() * Vec3f((float)_speed, 0.0f, 0.0f)).block<2,1>(0,0);
    component<Orientation>().translate(_velocity);
}
