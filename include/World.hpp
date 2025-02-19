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

#include "Components.hpp"
#include "ComponentPool.hpp"
#include "NPC.hpp"
#include "Food.hpp"
#include "EntityFinder.hpp"

#include <vector>


class SpriteRenderer;
class CollisionHandler;


class World {
public:
    World(ComponentPool<COMPONENT_TYPES>* componentPool);

    void update(CollisionHandler* handler);
    void render(SpriteRenderer* renderer);

    void removeNPC(NPC* npc);
    void removeFood(Food* food);
    void spawnFood();

    void getEntitiesWithinRadius(const Vec2f& point, double radius,
        std::vector<std::pair<EntityId, TypeId>>* entityHandles);
    double getSize() const;

    ComponentPool<COMPONENT_TYPES>* componentPool;

private:
    double                          _size;  // radius around origin

    std::vector<NPC>                _npcs;
    std::vector<Food>               _food;

    EntityFinder                    _entityFinder;
};
