//
// Project: rpg_world_simulator
// File: World.hpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#pragma once

#include "NPC.hpp"
#include "SpriteRenderer.hpp"

#include <vector>


class World {
public:
    World();

    void update();
    void render(SpriteRenderer* renderer);

    double getSize() const;

private:
    double              _size;  // radius around origin

    std::vector<NPC>    _npcs;

    void checkCollisions();
};
