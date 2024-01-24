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
#include "CollisionBody.hpp"
#include "Entity.hpp"


class SpriteRenderer;
class World;
class Food;


class NPC : public Entity<Orientation, Sprite, CollisionBody> {
public:
    NPC(EntityType&& entity, const Vec2f& position);

    void update(World* world);

    friend class CollisionHandler;

private:
    double  _speed;
    Vec2f   _velocity; // computed from orientation and _speed

    double  _health;
    double  _maxEnergy;
    double  _energy;
    double  _inventoryWeightCap; // maximum amount of weight that the NPC can carry
    double  _foodInInventory;
};
