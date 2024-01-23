//
// Project: rpg_world_simulator
// File: Food.cpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "Food.hpp"


Food::Food(EntityType&& entity, const Vec2f& position) :
    EntityType(std::forward<EntityType>(entity))
{
    component<Orientation>().setPosition(position);

    component<Sprite>().setSpriteId(1);
    component<Sprite>().setOrigin(Vec2f(64.0f, 64.0f));
    component<Sprite>().setScale(Vec2f(1.0f/64.0f, 1.0f/64.0f));

    component<CollisionBody>().setRadius(1.0f);
    component<CollisionBody>().setEntityType<Food>();
}

void Food::update(World* world)
{

}
