//
// Project: rpg_world_simulator
// File: ComponentPool.hpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#pragma once

#include "Entity.hpp"
#include <cstdint>


template <typename... T_Components>
class ComponentPool
{
public:
    ComponentPool(uint64_t preallocation = 0) :
        _entityHandles      (preallocation, nullptr),
        _componentMovers    (preallocation, nullptr)
    {
        std::apply([this](auto&&... components) {((components.resize(_entityHandles.size())), ...);}, _components);
    }

    ComponentPool(ComponentPool&&) = delete;
    ComponentPool(const ComponentPool&&) = delete;
    ComponentPool& operator=(ComponentPool&&) = delete;
    ComponentPool& operator=(const ComponentPool&&) = delete;

    template <typename T_Entity, typename... T_Args>
    T_Entity createEntity(T_Args&&... args)
    {
        T_Entity* dummy = nullptr; // required for type deduction in constructEntity
        T_Entity entity(constructEntity(dummy), std::forward<T_Args>(args)...);
        return entity;
    }

    template <typename... T_EntityComponents>
    friend class Entity;

private:
    void destroyEntity(EntityId entityId)
    {
        _entityHandles[entityId] = nullptr;
    }

    void moveEntity(EntityId entityId, void* newLocation)
    {
        _entityHandles[entityId] = newLocation;
    }

    template <typename... T_EntityComponents>
    void copyEntity(const Entity<T_EntityComponents...>& oldEntity, Entity<T_EntityComponents...>* newEntity)
    {
        newEntity->_id = findFreeEntityId();
        _componentMovers[newEntity->_id] = &ComponentPool<T_Components...>::moveComponents<T_EntityComponents...>;
        copyComponent<Entity<T_EntityComponents...>, T_EntityComponents...>(oldEntity, newEntity);
        _entityHandles[newEntity->_id] = newEntity;
    }

    EntityId findFreeEntityId()
    {
        for (EntityId id=0; id<_entityHandles.size(); ++id) {
            if (_entityHandles[id] == nullptr)
                return id;
        }

        _entityHandles.push_back(nullptr);
        _componentMovers.push_back(nullptr);
        resizeComponentStorage(_entityHandles.size());
        return _entityHandles.size()-1;
    }

    template <typename... T_EntityComponents>
    Entity<T_EntityComponents...> constructEntity(Entity<T_EntityComponents...>* dummy) // dummy needed for component type deduction
    {
        return constructEntity<T_EntityComponents...>();
    }

    template <typename... T_EntityComponents>
    Entity<T_EntityComponents...> constructEntity()
    {
        auto entity = Entity<T_EntityComponents...>(this, findFreeEntityId());
        _componentMovers[entity._id] = &ComponentPool<T_Components...>::moveComponents<T_EntityComponents...>;
        constructComponent<Entity<T_EntityComponents...>, T_EntityComponents...>(&entity);
        return entity;
    }

    template <typename T_Entity, typename T_FirstComponent, typename... T_RestComponents>
    void constructComponent(T_Entity* entity)
    {
        auto& componentVector = std::get<std::vector<T_FirstComponent>>(_components);
        std::get<T_FirstComponent*>(entity->_components) = &componentVector[entity->_id];
        if constexpr (sizeof...(T_RestComponents) > 0)
            constructComponent<T_Entity, T_RestComponents...>(entity);
    }

    template <typename T_Entity, typename T_FirstComponent, typename... T_RestComponents>
    void copyComponent(const T_Entity& oldEntity, T_Entity* newEntity)
    {
        auto& componentVector = std::get<std::vector<T_FirstComponent>>(_components);
        std::get<T_FirstComponent*>(newEntity->_components) = &componentVector[newEntity->_id];
        componentVector[newEntity->_id] = *std::get<T_FirstComponent*>(oldEntity._components);
        if constexpr (sizeof...(T_RestComponents) > 0)
            copyComponent<T_Entity, T_RestComponents...>(oldEntity, newEntity);
    }

    void resizeComponentStorage(std::size_t size)
    {
        // Resize all component vectors
        std::apply([size](auto&&... components) {((components.resize(size)), ...);}, _components);

        // Reallocate all entity components
        for (EntityId id=0; id<_entityHandles.size(); ++id) {
            if (_entityHandles[id] == nullptr)
                continue;

            if (_componentMovers[id] == nullptr) {
                // TODO throw an error
            }

            (this->*_componentMovers[id])(_entityHandles[id]);
        }
    }

    template <typename... T_EntityComponents>
    void moveComponents(void* entityHandle)
    {
        moveComponents<Entity<T_EntityComponents...>, T_EntityComponents...>(
        static_cast<Entity<T_EntityComponents...>*>(entityHandle));
    }

    template <typename T_Entity, typename T_FirstComponent, typename... T_RestComponents>
    inline void moveComponents(T_Entity* entity)
    {
        std::get<T_FirstComponent*>(entity->_components) =
            &std::get<std::vector<T_FirstComponent>>(_components)[entity->_id];

        if constexpr (sizeof...(T_RestComponents) > 0)
            moveComponents<T_Entity, T_RestComponents...>(entity);
    }

    using ComponentMover = void(ComponentPool<T_Components...>::*)(void*);

    std::vector<void*>                          _entityHandles;
    std::vector<ComponentMover>                 _componentMovers;
    std::tuple<std::vector<T_Components>...>    _components;
};
