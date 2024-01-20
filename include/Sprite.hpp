//
// Project: rpg_world_simulator
// File: Sprite.hpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#pragma once


#include "SpriteSheet.hpp"

#include <gut_utils/MathUtils.hpp>


class SpriteRenderer;
class Orientation;


class Sprite {
public:
    Sprite(SpriteSheetId sheetId = 0, int spriteId = 0);

    void setSpriteSheet(SpriteSheetId sheetId);
    void setSpriteId(int spriteId);
    void setOrigin(const Vec2f& origin);
    void setColor(const Vec3f& color);

    SpriteSheetId getSpriteSheet() const;
    int getSprite() const;
    const Vec2f& getOrigin() const;
    const Vec3f& getColor() const;

    void render(SpriteRenderer* renderer, const Orientation& orientation);

    friend class SpriteRenderer;

private:
    bool            _dirty;
    SpriteSheetId   _spriteSheetId;
    int             _spriteId;
    Vec2f           _origin;
    Vec3f           _color;
    Vec3f           _positions[4]; // homogeneous vector
    Vec2f           _texCoords[4];
};