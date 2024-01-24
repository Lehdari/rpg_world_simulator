//
// Project: rpg_world_simulator
// File: Food.hpp
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


class World;


class Food : public Entity<Orientation, Sprite, CollisionBody> {
public:
    Food(EntityType&& entity, const Vec2f& position);

    void update(World* world);
    void updateRadius();

    friend class CollisionHandler;

private:
    double  _nutritionalValue;
};