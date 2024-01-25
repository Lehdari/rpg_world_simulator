//
// Project: rpg_world_simulator
// File: NPC.cpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "NPC.hpp"
#include "SpriteRenderer.hpp"
#include "World.hpp"
#include "Food.hpp"

#include <algorithm>


static constexpr double maxHealth = 100.0;


ENTITY_CONSTRUCTOR(NPC, const Vec2f& position),
    _speed              (rnd(-0.002, 0.02)),
    _health             (maxHealth),
    _maxEnergy          (100.0),
    _energy             (_maxEnergy),
    _inventoryWeightCap (1.0),
    _foodInInventory    (0.0)
{
    component<Label>().entityTypeId = entityTypeId<NPC>();

    component<Orientation>().setPosition(position);
    component<Orientation>().setRotation(rnd<float>(0.0f, 2.0f*M_PI));

    component<Sprite>().setSpriteId(0);
    component<Sprite>().setOrigin(Vec2f(64.0f, 64.0f));
    component<Sprite>().setScale(Vec2f(1.0f/64.0f, 1.0f/64.0f));

    component<CollisionBody>().setRadius(1.0f);
}

void NPC::update(World* world)
{
    auto& position = component<Orientation>().getPosition();

    // Find nearest food
    static std::vector<std::pair<EntityId, TypeId>> nearbyEntities;
    nearbyEntities.clear();
    world->getEntitiesWithinRadius(position, 4.0, &nearbyEntities);
    float distanceSqrToNearest = -1.0f;
    Food* nearestFood = nullptr;
    for (const auto& eInfo : nearbyEntities) {
        if (eInfo.second == entityTypeId<Food>()) {
            auto food = static_cast<Food*>(world->componentPool->getEntityHandle(eInfo.first));
            float distanceSqr = (food->component<Orientation>().getPosition() - position).squaredNorm();
            if (distanceSqrToNearest < 0.0f || distanceSqr < distanceSqrToNearest) {
                nearestFood = food;
                distanceSqrToNearest = distanceSqr;
            }
        }
    }

    _speed = std::clamp(_speed + rnd(-0.001, 0.0011), -0.005, 0.05);
    if (nearestFood == nullptr) {
        // Random movement (for now)
        component<Orientation>().rotate(rnd(-0.05, 0.05));
    }
    else {
        // Move towards the nearest food
        Vec2f toFood = nearestFood->component<Orientation>().getPosition() - position;
        component<Orientation>().setRotation(atan2(toFood(1), toFood(0)));
    }

    // Collision check with world boundary
    if (position.squaredNorm() >= world->getSize()*world->getSize()) {
        component<Orientation>().setRotation(atan2f(-position(1), -position(0))); // turn towards origin
        if (_speed <= 0.0)
            _speed = 0.001; // force forward movement
    }

    // Move
    _velocity = (component<Orientation>().getOrientation() * Vec3f((float)_speed, 0.0f, 0.0f)).block<2,1>(0,0);
    component<Orientation>().translate(_velocity);

    // Energy consumption
    _energy -= 100.0*_speed*_speed;
    if (_energy <= 0.0 && _foodInInventory > 0.0) { // eating
        constexpr double foodToEnergyConversionRatio = 100.0;
        double amountToEat = std::min(_foodInInventory*foodToEnergyConversionRatio, _maxEnergy) / foodToEnergyConversionRatio;
        _energy += amountToEat*foodToEnergyConversionRatio;
        _foodInInventory -= amountToEat;
    }
    if (_health < maxHealth) {
        double delta = std::min(_energy, maxHealth-_health);
        _health += delta;
        _energy -= delta;
    }
    if (_energy <= 0.0) { // health degrades if no energy left
        _health += _energy;
        _energy = 0.0;
    }

    // Sprite update (cyan: max energy, max health; green: 0 energy, max health; red: 0 energy, 0 health)
    component<Sprite>().setColor(Vec3f(std::sqrt((100.0-_health)*0.01), std::sqrt(_health*0.01), _energy / _maxEnergy));

    // Death
    if (_health <= 0.0) {
        world->removeNPC(this);
    }
}
