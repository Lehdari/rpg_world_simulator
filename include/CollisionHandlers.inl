//
// Project: rpg_world_simulator
// File: CollisionHandlers.inl
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

// Definitions for all collision handling functions
// Make sure to have the types in the same order as they are listed in ENTITY_TYPES (see Entities.hpp)
static void handleCollision(NPC* npc1, NPC* npc2);
static void handleCollision(NPC* npc, Food* food);
static void handleCollision(Food* food1, Food* food2);
