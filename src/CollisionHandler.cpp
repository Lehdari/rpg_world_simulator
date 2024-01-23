//
// Project: rpg_world_simulator
// File: CollisionHandler.cpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "CollisionHandler.hpp"
#include "ComponentPool.hpp"
#include "World.hpp"
#include "NPC.hpp"
#include "Food.hpp"


CollisionHandler::CollisionCallBackArray CollisionHandler::_collisionCallbacks =
    []()
{
    CollisionCallBackArray callbacks{};
    CallbacksInitializer<ENTITY_TYPES>::callbacksInitializerOuter<ENTITY_TYPES>(callbacks.data());
    return callbacks;
}();


CollisionHandler::CollisionHandler(ComponentPool<COMPONENT_TYPES>* componentPool, World* world) :
    _componentPool  (componentPool)
{
    _subHandler.componentPool = componentPool;
    _subHandler.world = world;
}

void CollisionHandler::operator()(EntityId id, CollisionBody& collisionBody, Orientation& orientation)
{
    _subHandler.outerId = id;
    _subHandler.outerCollisionBody = &collisionBody;
    _subHandler.outerOrientation = &orientation;
    _componentPool->runSystem<CollisionSubHandler, CollisionBody, Orientation>(&_subHandler);
}

template<typename T_Entity1, typename T_Entity2>
void CollisionHandler::handleCollision(World* world, void* entity1, void* entity2)
{
    handleCollision(world, static_cast<T_Entity1*>(entity1), static_cast<T_Entity2*>(entity2));
}

void CollisionHandler::CollisionSubHandler::operator()(
    EntityId id,
    CollisionBody& collisionBody,
    Orientation& orientation
) {
    if (id >= outerId)
        return;

    float dist = (outerOrientation->getPosition() - orientation.getPosition()).squaredNorm();
    float totalRadius = outerCollisionBody->_radius + collisionBody._radius;
    if (dist < totalRadius * totalRadius) {
        // Pick the collision function so that mirrored handleCollision function definitions are not needed
        // (for example handleCollision(NPC*, Food*) and handleCollision(Food*, NPC*))
        uint64_t functionId;
        if (outerCollisionBody->_entityTypeId < collisionBody._entityTypeId) {
            functionId = collisionBody._entityTypeId + outerCollisionBody->_entityTypeId*N_ENTITY_TYPES;
            // If you're getting segfault here it's likely that you forgot to overload handleCollision for
            // the entity types that collided
            CollisionHandler::_collisionCallbacks[functionId](world,
                componentPool->getEntityHandle(outerId), componentPool->getEntityHandle(id));
        }
        else {
            functionId = outerCollisionBody->_entityTypeId + collisionBody._entityTypeId*N_ENTITY_TYPES;
            // If you're getting segfault here it's likely that you forgot to overload handleCollision for
            // the entity types that collided
            CollisionHandler::_collisionCallbacks[functionId](world,
                componentPool->getEntityHandle(id), componentPool->getEntityHandle(outerId));
        }
    }
}

// Looks weird but helps to keep the code a bit more clean as this file contains much of the abstract machinery
#include "CollisionHandlers.cpp"
