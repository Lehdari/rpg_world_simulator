//
// Project: rpg_world_simulator
// File: TypeId.hpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#pragma once

#include <cstdint>
#include <type_traits>


using TypeId = uint64_t;


template <typename T_First, typename... T_Rest>
struct TypeIdGenerator {
    template <typename T>
    static consteval TypeId typeId(TypeId prev=0)
    {
        if constexpr (std::is_same_v<T_First, T>)
            return prev;
        else
            return TypeIdGenerator<T_Rest...>::template typeId<T>(prev+1);
    }
};


template <typename... T_Types>
struct TypeCounter {
    static constexpr uint64_t value = sizeof...(T_Types);
};
#define N_ENTITY_TYPES TypeCounter<ENTITY_TYPES>::value
