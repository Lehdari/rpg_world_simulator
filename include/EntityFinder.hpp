//
// Project: rpg_world_simulator
// File: EntityFinder.hpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#pragma once

#include "Entity.hpp"
#include "Entities.hpp"

#include <gut_utils/MathTypes.hpp>


class Label;
class Orientation;


struct EntityFinder {
    Vec2f                                       point           {0.0f, 0.0f};
    double                                      radius          {0.0};
    std::vector<std::pair<EntityId, TypeId>>*   entityHandles   {nullptr};

    void operator()(EntityId id, Label& label, Orientation& orientation);
};