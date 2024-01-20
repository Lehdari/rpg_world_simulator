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
    _speed = std::clamp(_speed+rnd(-0.001, 0.0011), -0.005, 0.05);
    _orientation.rotate(rnd(-0.01, 0.01));

    // Collision check with world boundary
    auto& position = _orientation.getPosition();
    if (position.squaredNorm() >= world->getSize()*world->getSize()) {
        _orientation.setRotation(atan2f(-position(1), -position(0))); // turn towards origin
        if (_speed <= 0.0)
            _speed = 0.001; // force forward movement
    }

    // Move
    _velocity = (_orientation.getOrientation() * Vec3f((float)_speed, 0.0f, 0.0f)).block<2,1>(0,0);
    _orientation.translate(_velocity);
}

void NPC::collision(World* world, NPC* other)
{
    {   // Physics collision
        Vec2f fromOther = _orientation.getPosition() - other->_orientation.getPosition();
        float distToOther = fromOther.norm();
        Vec2f fromOtherUnit = fromOther / distToOther;

        // Push the NPCs from inside each other
        float overlap = _orientation.getScale() + other->_orientation.getScale() - distToOther + 0.001f;
        _orientation.translate(fromOtherUnit * overlap * 0.5f);
        other->_orientation.translate(-fromOtherUnit * overlap * 0.5f);

        // Bounce
        Vec2f newVelocity, otherNewVelocity;

        float dot1 = _velocity.dot(fromOtherUnit);
        Vec2f proj1 = dot1 * fromOtherUnit;
        if (dot1 < 0.0f)
            newVelocity = _velocity - 2.0f * proj1;
        else
            newVelocity = _velocity + 2.0f * proj1;
        _orientation.setRotation(atan2f(newVelocity(1), newVelocity(0)));

        float dot2 = other->_velocity.dot(fromOtherUnit);
        Vec2f proj2 = dot2 * fromOtherUnit;
        if (dot2 > 0.0f)
            otherNewVelocity = other->_velocity - 2.0f * proj2;
        else
            otherNewVelocity = other->_velocity + 2.0f * proj2;
        other->_orientation.setRotation(atan2f(otherNewVelocity(1), otherNewVelocity(0)));
    }
}

void NPC::render(SpriteRenderer* renderer)
{
    _sprite.render(renderer, _orientation);
}
