//
// Project: rpg_world_simulator
// File: Entity.hpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#pragma once

#include <vector>
#include <tuple>


#define ENTITY_CONSTRUCTOR(ENTITY, ...) \
    ENTITY::ENTITY(EntityType&& entity, __VA_ARGS__) :\
        EntityType(std::forward<EntityType>(entity))


template <typename... T_Components>
class ComponentPool;

using EntityId = std::vector<void*>::size_type;


template <typename... T_Components>
class Entity {
public:
    using EntityType = Entity<T_Components...>;

    Entity() :
        _pool       (nullptr),
        _id         (0),
        _destroy    (nullptr),
        _move       (nullptr),
        _copy       (nullptr)
    {
    }

    template <typename... T_EntityPoolComponents>
    Entity(ComponentPool<T_EntityPoolComponents...>* pool, EntityId id) :
        _pool       (pool),
        _id         (id),
        // EntityPool constructs the components by itself
        _destroy    (&destroyEntity<T_EntityPoolComponents...>),
        _move       (&moveEntity<T_EntityPoolComponents...>),
        _copy       (&copyEntity<T_EntityPoolComponents...>)
    {
    }

    Entity(Entity&& other) :
        _pool       (other._pool),
        _id         (other._id),
        _components (other._components),
        _destroy    (other._destroy),
        _move       (other._move),
        _copy       (other._copy)
    {
        if (_move != nullptr)
            _move(_pool, _id, this);

        other._pool = nullptr;
        other._destroy = nullptr;
        other._move = nullptr;
        other._copy = nullptr;
    }

    Entity(const Entity& other) :
        _pool       (other._pool),
        _destroy    (other._destroy),
        _move       (other._move),
        _copy       (other._copy)
    {
        if (_copy != nullptr)
            _copy(_pool, other, this);
    }

    Entity& operator=(Entity&& other)
    {
        if (this == &other)
            return *this;

        if (_destroy != nullptr)
            _destroy(_pool, _id);

        _pool       = other._pool;
        _id         = other._id;
        _components = other._components;
        _destroy    = other._destroy;
        _move       = other._move;
        _copy       = other._copy;

        if (_move != nullptr)
            _move(_pool, _id, this);

        other._pool = nullptr;
        other._destroy = nullptr;
        other._move = nullptr;
        other._copy = nullptr;

        return *this;
    }
    Entity& operator=(const Entity& other)
    {
        _pool       = other._pool;
        _destroy    = other._destroy;
        _move       = other._move;
        _copy       = other._copy;

        if (_copy != nullptr)
            _copy(_pool, other, this);

        return *this;
    }

    ~Entity()
    {
        if (_destroy != nullptr)
            _destroy(_pool, _id);
    }

    template <typename T_Component>
    T_Component& component()
    {
        return *std::get<T_Component*>(_components);
    }

    const EntityId& entityId() const
    {
        return _id;
    }

    template <typename... T_EntityPoolComponents>
    friend class ComponentPool;

private:
    void*                           _pool;
    EntityId                        _id;
    std::tuple<T_Components*...>    _components;

    void(*_destroy)(void*, EntityId);
    void(*_move)(void*, EntityId, void*);
    void(*_copy)(void*, const Entity<T_Components...>&, Entity<T_Components...>*);

    template <typename... T_EntityPoolComponents>
    static void destroyEntity(void* entityPool, EntityId entityId)
    {
        static_cast<ComponentPool<T_EntityPoolComponents...>*>(entityPool)->destroyEntity(entityId);
    }

    template <typename... T_EntityPoolComponents>
    static void moveEntity(void* entityPool, EntityId entityId, void* newLocation)
    {
        static_cast<ComponentPool<T_EntityPoolComponents...>*>(entityPool)->moveEntity(entityId, newLocation);
    }

    template <typename... T_EntityPoolComponents>
    static void copyEntity(
        void* entityPool,
        const Entity<T_Components...>& oldEntity,
        Entity<T_Components...>* newEntity)
    {
        static_cast<ComponentPool<T_EntityPoolComponents...>*>(entityPool)->copyEntity(oldEntity, newEntity);
    }
};
