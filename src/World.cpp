//
// Project: rpg_world_simulator
// File: World.cpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "World.hpp"


World::World()
{
    _npcs.emplace_back(Vec2f(0.0f, 0.0f));
    _npcs.emplace_back(Vec2f(2.0f, 0.0f));
}

void World::update()
{
    for (auto& npc : _npcs)
        npc.update();
}

void World::render(SpriteRenderer* renderer)
{
    for (auto& npc : _npcs)
        npc.render(renderer);
}
