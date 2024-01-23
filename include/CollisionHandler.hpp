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
class World;


class CollisionHandler {
public:
    CollisionHandler(ComponentPool<COMPONENT_TYPES>* componentPool, World* world);

    void operator()(EntityId id, CollisionBody& collisionBody, Orientation& orientation);

    #include "CollisionHandlers.inl"

    // Function template that injects the type information, _collisionCallbacks is filled with pointers
    // (CollisionCallback) to this function
    template <typename T_Entity1, typename T_Entity2>
    static void handleCollision(World* world, void* entity1, void* entity2);

    using CollisionCallback = void(*)(World*, void*, void*);
    // Array of function pointers for all collision type pairs
    using CollisionCallBackArray = std::array<CollisionCallback, N_ENTITY_TYPES*N_ENTITY_TYPES>;

    // Initialization machinery for _collisionCallbacks
    template<typename T_First1, typename... T_Rest1>
    struct CallbacksInitializer {
        template<typename T_First2, typename... T_Rest2>
        static void callbacksInitializer(CollisionCallback* callback)
        {
            // Check that the function for these types have been implemented
            constexpr bool hasHandleCollisionForTheTypes = requires(World* w, T_First1* a, T_First2* b) {
                CollisionHandler::handleCollision(w, a, b);
            };
            if constexpr (!hasHandleCollisionForTheTypes || entityTypeId<T_First1>() > entityTypeId<T_First2>())
                *callback = nullptr; // omit these so that mirrored handleCollision function definitions are not needed
            else
                *callback = handleCollision<T_First1, T_First2>;
            if constexpr (sizeof...(T_Rest2) > 0)
                callbacksInitializer<T_Rest2...>(callback+1);
        }

        template <typename... T_Rest2>
        static void callbacksInitializerOuter(CollisionCallback* callback)
        {
            callbacksInitializer<T_Rest2...>(callback);
            if constexpr (sizeof...(T_Rest1) > 0)
                CallbacksInitializer<T_Rest1...>::template callbacksInitializerOuter<T_Rest2...>(
                    callback+N_ENTITY_TYPES);
        }
    };

    friend class CollisionSubHandler;

private:
    class CollisionSubHandler {
    public:
        ComponentPool<COMPONENT_TYPES>* componentPool;
        World*                          world;
        EntityId                        outerId;
        CollisionBody*                  outerCollisionBody;
        Orientation*                    outerOrientation;

        void operator()(EntityId id, CollisionBody& collisionBody, Orientation& orientation);
    };

    ComponentPool<COMPONENT_TYPES>* _componentPool;
    CollisionSubHandler             _subHandler;
    static CollisionCallBackArray   _collisionCallbacks;
};
