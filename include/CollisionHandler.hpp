//
// Project: rpg_world_simulator
// File: CollisionHandler.hpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#pragma once

#include "Components.hpp"
#include "Entity.hpp"
#include "Entities.hpp"


class Orientation;
class CollisionBody;
template <typename... T_Components> class ComponentPool;


class CollisionHandler {
public:
    CollisionHandler(ComponentPool<COMPONENT_TYPES>* componentPool);

    void operator()(EntityId id, CollisionBody& collisionBody, Orientation& orientation);

    static void handleCollision(NPC* npc1, NPC* npc2);
    static void handleCollision(NPC* npc1, Food* food2);
    static void handleCollision(Food* food1, NPC* npc2);
    static void handleCollision(Food* food1, Food* food2);

    template <typename T_Entity1, typename T_Entity2>
    static void handleCollision(void* entity1, void* entity2);

    using CollisionCallback = void(*)(void*, void*);
    using CollisionCallBackArray = std::array<CollisionCallback, N_ENTITY_TYPES*N_ENTITY_TYPES>;

    template<typename T_First1, typename... T_Rest1>
    struct CallbacksInitializer {
        template<typename T_First2, typename... T_Rest2>
        static void callbacksInitializer(CollisionCallback* callback)
        {
            *callback = handleCollision<T_First1, T_First2>;
            if constexpr (sizeof...(T_Rest2) > 0)
                callbacksInitializer<T_Rest2...>(callback+1);
        }

        template <typename... T_Rest2>
        static void callbacksInitializerOuter(CollisionCallback* callback)
        {
            callbacksInitializer<T_Rest2...>(callback);
            if constexpr (sizeof...(T_Rest1) > 0)
                CallbacksInitializer<T_Rest1...>::template callbacksInitializer<T_Rest2...>(callback+N_ENTITY_TYPES);
        }
    };

private:
    class CollisionSubHandler {
    public:
        ComponentPool<COMPONENT_TYPES>* componentPool;
        EntityId        outerId;
        CollisionBody*  outerCollisionBody;
        Orientation*    outerOrientation;

        void operator()(EntityId id, CollisionBody& collisionBody, Orientation& orientation);
    };

    ComponentPool<COMPONENT_TYPES>* _componentPool;
    CollisionSubHandler             _subHandler;
    static CollisionCallBackArray   _collisionCallbacks;
};
