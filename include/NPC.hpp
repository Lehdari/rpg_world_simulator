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


class NPC {
public:
    NPC(const Vec2f& position);

    void render(SpriteRenderer* renderer);

private:
    Orientation _orientation;
    Sprite      _sprite;
};