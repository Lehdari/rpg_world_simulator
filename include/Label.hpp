//
// Project: rpg_world_simulator
// File: Label.hpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#pragma once

#include "Entities.hpp"


// Component for identifying entities
struct Label {
    TypeId  entityTypeId;
};
