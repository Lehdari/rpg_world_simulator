//
// Project: rpg_world_simulator
// File: EntityFinder.cpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "EntityFinder.hpp"
#include "Label.hpp"
#include "Orientation.hpp"


void EntityFinder::operator()(EntityId id, Label& label, Orientation& orientation)
{
    if ((point-orientation.getPosition()).squaredNorm() <= radius*radius)
        entityHandles->emplace_back(id, label.entityTypeId);
}
