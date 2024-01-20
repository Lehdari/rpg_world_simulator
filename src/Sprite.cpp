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
#include "SpriteRenderer.hpp"
#include "Orientation.hpp"


Sprite::Sprite(SpriteSheetId sheetId, int spriteId) :
    _dirty          (true),
    _spriteSheetId  (sheetId),
    _spriteId       (spriteId),
    _origin         (0.0f, 0.0f),
    _color          (1.0f, 1.0f, 1.0f),
    _scale          (1.0f, 1.0f),
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

void Sprite::setScale(const Vec2f& scale)
{
    _scale = scale;
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

const Vec2f& Sprite::getScale() const
{
    return _scale;
}

void Sprite::render(SpriteRenderer* renderer, const Orientation& orientation)
{
    if (_dirty) {
        auto& spriteSheet = renderer->_spriteSheets[_spriteSheetId];
        int sw, sh;
        float uvLeft, uvRight, uvTop, uvBottom;
        spriteSheet.getDimensions(_spriteId, sw, sh);
        spriteSheet.getUVCoordinates(_spriteId, uvLeft, uvRight, uvTop, uvBottom);

        _positions[0] << -_origin(0), -_origin(1), 1.0f;
        _positions[1] << sw-_origin(0), -_origin(1), 1.0f;
        _positions[2] << sw-_origin(0), sh-_origin(1), 1.0f;
        _positions[3] << -_origin(0), sh-_origin(1), 1.0f;

        _texCoords[0] << uvLeft, uvTop;
        _texCoords[1] << uvRight, uvTop;
        _texCoords[2] << uvRight, uvBottom;
        _texCoords[3] << uvLeft, uvBottom;

        _dirty = false;
    }

    auto& vertexPositions = renderer->_spriteVertexPositions[_spriteSheetId];
    Mat3f orientationMatrix = orientation.getOrientation();
    orientationMatrix(0,0) *= _scale(0);
    orientationMatrix(1,0) *= _scale(0);
    orientationMatrix(0,1) *= _scale(1);
    orientationMatrix(1,1) *= _scale(1);
    vertexPositions.emplace_back((orientationMatrix * _positions[0]).block<2,1>(0,0));
    vertexPositions.emplace_back((orientationMatrix * _positions[1]).block<2,1>(0,0));
    vertexPositions.emplace_back((orientationMatrix * _positions[3]).block<2,1>(0,0));
    vertexPositions.emplace_back((orientationMatrix * _positions[3]).block<2,1>(0,0));
    vertexPositions.emplace_back((orientationMatrix * _positions[1]).block<2,1>(0,0));
    vertexPositions.emplace_back((orientationMatrix * _positions[2]).block<2,1>(0,0));

    auto& vertexTexCoords = renderer->_spriteVertexTexCoords[_spriteSheetId];
    vertexTexCoords.emplace_back(_texCoords[0]);
    vertexTexCoords.emplace_back(_texCoords[1]);
    vertexTexCoords.emplace_back(_texCoords[3]);
    vertexTexCoords.emplace_back(_texCoords[3]);
    vertexTexCoords.emplace_back(_texCoords[1]);
    vertexTexCoords.emplace_back(_texCoords[2]);

    auto& vertexColors = renderer->_spriteVertexColors[_spriteSheetId];
    vertexColors.emplace_back(_color);
    vertexColors.emplace_back(_color);
    vertexColors.emplace_back(_color);
    vertexColors.emplace_back(_color);
    vertexColors.emplace_back(_color);
    vertexColors.emplace_back(_color);
}
