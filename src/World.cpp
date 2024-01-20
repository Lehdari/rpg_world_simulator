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


World::World() :
    _size   (10.0)
{
    constexpr int nNPCs = 8;
    for (int i=0; i<nNPCs; ++i) {
        _npcs.emplace_back(Vec2f(5.0*cos(2.0*M_PI*((float)i/nNPCs)), 5.0*sin(2.0*M_PI*((float)i/nNPCs))));
    }
}

void World::update()
{
    for (auto& npc : _npcs)
        npc.update(this);

    checkCollisions();
}

void World::render(SpriteRenderer* renderer)
{
    for (auto& npc : _npcs)
        npc.render(renderer);
}

double World::getSize() const
{
    return _size;
}

void World::checkCollisions()
{
    for (decltype(_npcs)::size_type i=0; i<_npcs.size(); ++i) {
        auto& npc1 = _npcs[i];
        for (decltype(_npcs)::size_type j=0; j<_npcs.size(); ++j) {
            if (j >= i) continue;
            auto& npc2 = _npcs[j];

            if (npc1._orientation.checkCollision(npc2._orientation)) {
                npc1.collision(this, &npc2);
            }
        }
    }
}
