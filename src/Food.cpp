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


ENTITY_CONSTRUCTOR(Food, const Vec2f& position),
    _nutritionalValue   (0.15 + rnd(0.0, 0.35))
{
    double radius = std::sqrt(_nutritionalValue*0.25);

    component<Orientation>().setPosition(position);

    component<Sprite>().setSpriteId(1);
    component<Sprite>().setOrigin(Vec2f(64.0f, 64.0f));
    component<Sprite>().setScale(Vec2f(radius/64.0f, radius/64.0f));
    component<Sprite>().setColor(Vec3f(0.3f, 0.7f, 0.05f));

    component<CollisionBody>().setRadius(radius);
    component<CollisionBody>().setEntityType<Food>();
}

void Food::update(World* world)
{
    _nutritionalValue += rnd(0.0, 0.01);
    double radius = std::sqrt(_nutritionalValue*0.25);
    component<Sprite>().setScale(Vec2f(radius/64.0f, radius/64.0f));
    component<CollisionBody>().setRadius(radius);
}
