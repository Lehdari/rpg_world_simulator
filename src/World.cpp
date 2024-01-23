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
#include "SpriteRenderer.hpp"
#include "CollisionHandler.hpp"


World::World(ComponentPool<COMPONENT_TYPES>* componentPool) :
    _size           (15.0),
    _componentPool  (componentPool)
{
    constexpr int nNPCs = 8;
    constexpr int nFood = 24;
    for (int i=0; i<nNPCs; ++i) {
        _npcs.emplace_back(_componentPool->createEntity<NPC>(Vec2f(
            5.0*cos(2.0*M_PI*((float)i/nNPCs)),
            5.0*sin(2.0*M_PI*((float)i/nNPCs)))));
    }
    for (int i=0; i<nFood; ++i) {
        _food.emplace_back(_componentPool->createEntity<Food>(Vec2f(
            10.0*cos(2.0*M_PI*((float)i/nFood)),
            10.0*sin(2.0*M_PI*((float)i/nFood)))));
    }
}

void World::update(CollisionHandler* collisionHandler)
{
    for (auto& npc : _npcs)
        npc.update(this);

    _componentPool->runSystem<CollisionHandler, CollisionBody, Orientation>(collisionHandler);
}

void World::render(SpriteRenderer* renderer)
{
    _componentPool->runSystem<SpriteRenderer, Sprite, Orientation>(renderer);
}

double World::getSize() const
{
    return _size;
}
