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
    for (int i=0; i<nNPCs; ++i) {
        _npcs.emplace_back(_componentPool->createEntity<NPC>(Vec2f(
            5.0*cos(2.0*M_PI*((float)i/nNPCs)),
            5.0*sin(2.0*M_PI*((float)i/nNPCs)))));
    }
}

void World::update(CollisionHandler* collisionHandler)
{
    spawnFood();

    for (auto& npc : _npcs)
        npc.update(this);

    for (auto& food : _food)
        food.update(this);

    _componentPool->runSystem<CollisionHandler, Label, CollisionBody, Orientation>(collisionHandler);
}

void World::render(SpriteRenderer* renderer)
{
    _componentPool->runSystem<SpriteRenderer, Sprite, Orientation>(renderer);
}

void World::removeNPC(NPC* npc)
{
    // This is utter carbage but it'll do for now
    _npcs.erase(_npcs.begin() + (npc - _npcs.data()));
}

void World::removeFood(Food* food)
{
    // This is utter carbage but it'll do for now
    _food.erase(_food.begin() + (food - _food.data()));
}

void World::spawnFood()
{
    size_t maxFood = static_cast<size_t>((M_PI*_size*_size) / (5*5));
    if (_food.size() >= maxFood)
        return;

    double nNewFood = rnd(0.0, (M_PI*_size*_size)/(64*64));
    long nNewFoodDiscrete = static_cast<long>(nNewFood);
    for (long i=0; i<nNewFoodDiscrete; ++i) {
        if (_food.size() >= maxFood)
            return;

        Vec2f p;
        do  // Rejection sample inside the _size radius
            p << rnd((float)-_size, (float)_size), rnd((float)-_size, (float)_size);
        while (p.squaredNorm() > _size*_size);

        _food.emplace_back(_componentPool->createEntity<Food>(p));
    }
    if (rnd(0.0, 1.0) < nNewFood-static_cast<double>(nNewFoodDiscrete)) {
        Vec2f p;
        do
            p << rnd((float)-_size, (float)_size), rnd((float)-_size, (float)_size);
        while (p.squaredNorm() > _size*_size);

        _food.emplace_back(_componentPool->createEntity<Food>(p));
    }
}

double World::getSize() const
{
    return _size;
}
