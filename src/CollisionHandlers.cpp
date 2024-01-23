//
// Project: rpg_world_simulator
// File: CollisionHandlers.cpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

void CollisionHandler::handleCollision(World* world, NPC* npc1, NPC* npc2)
{
    {   // Physics collision
        Vec2f fromOther = npc1->component<Orientation>().getPosition() - npc2->component<Orientation>().getPosition();
        float distToOther = fromOther.norm();
        Vec2f fromOtherUnit = fromOther / distToOther;

        // Push the NPCs from inside each other
        float overlap = npc1->component<Orientation>().getScale() + npc2->component<Orientation>().getScale() -
            distToOther + 0.001f;
        npc1->component<Orientation>().translate(fromOtherUnit * overlap * 0.5f);
        npc2->component<Orientation>().translate(-fromOtherUnit * overlap * 0.5f);

        // Bounce
        Vec2f newVelocity, otherNewVelocity;

        float dot1 = npc1->_velocity.dot(fromOtherUnit);
        Vec2f proj1 = dot1 * fromOtherUnit;
        if (dot1 < 0.0f)
            newVelocity = npc1->_velocity - 2.0f * proj1;
        else
            newVelocity = npc1->_velocity + 2.0f * proj1;
        npc1->component<Orientation>().setRotation(atan2f(newVelocity(1), newVelocity(0)));

        float dot2 = npc2->_velocity.dot(fromOtherUnit);
        Vec2f proj2 = dot2 * fromOtherUnit;
        if (dot2 > 0.0f)
            otherNewVelocity = npc2->_velocity - 2.0f * proj2;
        else
            otherNewVelocity = npc2->_velocity + 2.0f * proj2;
        npc2->component<Orientation>().setRotation(atan2f(otherNewVelocity(1), otherNewVelocity(0)));
    }
}

void CollisionHandler::handleCollision(World* world, NPC* npc, Food* food)
{
//    {   // Physics collision
//        Vec2f fromOther = npc->component<Orientation>().getPosition() - food->component<Orientation>().getPosition();
//        float distToOther = fromOther.norm();
//        Vec2f fromOtherUnit = fromOther / distToOther;
//
//        // Push the objects from inside each other
//        float overlap = npc->component<Orientation>().getScale() + food->component<Orientation>().getScale() -
//            distToOther + 0.001f;
//        npc->component<Orientation>().translate(fromOtherUnit * overlap * 0.5f);
//        food->component<Orientation>().translate(-fromOtherUnit * overlap * 0.5f);
//    }
    world->removeFood(food);
}

void CollisionHandler::handleCollision(World* world, Food* food1, Food* food2)
{
    {   // Physics collision
        Vec2f fromOther = food1->component<Orientation>().getPosition() - food2->component<Orientation>().getPosition();
        float distToOther = fromOther.norm();
        Vec2f fromOtherUnit = fromOther / distToOther;

        // Push the objects from inside each other
        float overlap = food1->component<Orientation>().getScale() + food2->component<Orientation>().getScale() -
            distToOther + 0.001f;
        food1->component<Orientation>().translate(fromOtherUnit * overlap * 0.5f);
        food2->component<Orientation>().translate(-fromOtherUnit * overlap * 0.5f);
    }
}
