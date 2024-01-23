//
// Project: rpg_world_simulator
// File: CollisionBody.cpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "CollisionBody.hpp"


CollisionBody::CollisionBody(float radius) :
    _radius (radius)
{
}

void CollisionBody::setRadius(float radius)
{
    _radius = radius;
}
