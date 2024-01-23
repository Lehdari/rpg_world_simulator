//
// Project: rpg_world_simulator
// File: Entities.hpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#pragma once

#include "TypeId.hpp"


// List all Entities here, remember also to add forward declaration below
#define ENTITY_TYPES NPC, Food


class NPC;
class Food;


template <typename T_Entity>
consteval TypeId entityTypeId()
{
    return TypeIdGenerator<ENTITY_TYPES>::typeId<T_Entity>();
}
