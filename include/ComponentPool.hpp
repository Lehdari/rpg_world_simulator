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
        _componentMasks     (preallocation, 0x0000000000000000),
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

    template <typename T_System, typename... T_SystemComponents>
    void runSystem(T_System* system)
    {
        constexpr auto mask = componentMask<T_SystemComponents...>();
        for (EntityId id=0; id<_entityHandles.size(); ++id) {
            if ((mask & _componentMasks[id]) == mask) {
                (*system)(id, std::get<std::vector<T_SystemComponents>>(_components)[id]...);
            }
        }
    }

    void* getEntityHandle(EntityId id)
    {
        return _entityHandles[id];
    }

    template <typename... T_MaskComponents>
    static consteval uint64_t componentMask()
    {
        if constexpr (sizeof...(T_MaskComponents) > 0)
            return (componentMaskRecurse<T_MaskComponents, T_Components...>(0) | ...);
        else
            return 0x0000000000000000;
    }

    template <typename... T_EntityComponents>
    friend class Entity;

private:
    void destroyEntity(EntityId entityId)
    {
        _entityHandles[entityId] = nullptr;
        _componentMasks[entityId] = 0x0000000000000000;
    }

    void moveEntity(EntityId entityId, void* newLocation)
    {
        _entityHandles[entityId] = newLocation;
    }

    template <typename... T_EntityComponents>
    void copyEntity(const Entity<T_EntityComponents...>& oldEntity, Entity<T_EntityComponents...>* newEntity)
    {
        newEntity->_id = findFreeEntityId();
        _componentMasks[newEntity->_id] = componentMask<T_EntityComponents...>();
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
        _componentMasks.push_back(0x0000000000000000);
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
        _componentMasks[entity._id] = componentMask<T_EntityComponents...>();
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

        for (EntityId id=0; id<_entityHandles.size(); ++id) {
            if (_entityHandles[id] == nullptr)
                continue;

            if (_componentMovers[id] == nullptr) {
                std::runtime_error("_componentMovers[" + std::to_string(id) + "] is nullptr when resizing component storage. This is indicates a bug in ComponentPool implementation");
            }

            // Reassign all entity component pointers to point to (potentially) new component locations
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

    template <typename T_Component, typename T_FirstComponent, typename... T_RestComponents>
    static consteval uint64_t componentMaskRecurse(uint64_t id)
    {
        if constexpr (std::is_same_v<T_Component, T_FirstComponent>)
            return (uint64_t)1 << id;
        else if constexpr (sizeof...(T_RestComponents) > 0)
            return componentMaskRecurse<T_Component, T_RestComponents...>(id+1);
        else
            []<bool flag = false>() // Lambda prevents the ill-formed case with static_assert
                { static_assert(flag, "Requested component not enabled in the ComponentPool"); }();

        return 0x0000000000000000;
    }

    using ComponentMover = void(ComponentPool<T_Components...>::*)(void*);

    std::vector<void*>                          _entityHandles;
    std::vector<uint64_t>                       _componentMasks;
    std::vector<ComponentMover>                 _componentMovers;
    std::tuple<std::vector<T_Components>...>    _components;
};
