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

#include <vector>


class SpriteRenderer;
class CollisionHandler;


class World {
public:
    World(ComponentPool<COMPONENT_TYPES>* componentPool);

    void update(CollisionHandler* handler);
    void render(SpriteRenderer* renderer);

    double getSize() const;

private:
    double                          _size;  // radius around origin

    ComponentPool<COMPONENT_TYPES>* _componentPool;
    std::vector<NPC>                _npcs;

};
