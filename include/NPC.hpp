//
// Project: rpg_world_simulator
// File: NPC.hpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#pragma once

#include "Orientation.hpp"
#include "Sprite.hpp"


class SpriteRenderer;
class World;


class NPC {
public:
    NPC(const Vec2f& position);

    void update(World* world);

    void collision(World* world, NPC* other);

    void render(SpriteRenderer* renderer);

    friend class World;

private:
    Orientation _orientation;
    Sprite      _sprite;
    double      _speed;
};