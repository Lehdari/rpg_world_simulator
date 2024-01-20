//
// Project: rpg_world_simulator
// File: Sprite.cpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "Sprite.hpp"


Sprite::Sprite(SpriteSheetId sheetId, int spriteId) :
    _dirty          (true),
    _spriteSheetId  (sheetId),
    _spriteId       (spriteId),
    _origin         (0.0f, 0.0f),
    _color          (1.0f, 1.0f, 1.0f),
    _positions      {Vec3f(0.0f, 0.0f, 1.0f)},
    _texCoords      {Vec2f(0.0f, 0.0f)}
{
}

void Sprite::setSpriteSheet(SpriteSheetId sheetId)
{
    _spriteSheetId = sheetId;
    _dirty = true;
}

void Sprite::setSpriteId(int spriteId)
{
    _spriteId = spriteId;
    _dirty = true;
}

void Sprite::setOrigin(const Vec2f& origin)
{
    _origin = origin;
    _dirty = true;
}

void Sprite::setColor(const Vec3f& color)
{
    _color = color;
    _dirty = true;
}

SpriteSheetId Sprite::getSpriteSheet() const
{
    return _spriteSheetId;
}

int Sprite::getSprite() const
{
    return _spriteId;
}

const Vec2f& Sprite::getOrigin() const
{
    return _origin;
}

const Vec3f& Sprite::getColor() const
{
    return _color;
}
