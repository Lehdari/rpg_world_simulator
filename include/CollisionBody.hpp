//
// Project: rpg_world_simulator
// File: CollisionBody.hpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#pragma once

#include "Entities.hpp"


class CollisionBody {
public:
    CollisionBody(float radius = 0.0f);

    void setRadius(float radius);

    template <typename T_Entity>
    void setEntityType()
    {
        _entityTypeId = entityTypeId<T_Entity>();
    }

    friend class CollisionHandler;

private:
    float   _radius;
    TypeId  _entityTypeId;
};
