//
// Project: rpg_world_simulator
// File: Orientation.cpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "Orientation.hpp"


Orientation::Orientation(
    const Vec2f& position,
    float rotation,
    float scale
    ) :
    _position   (position),
    _rotation   (rotation),
    _rotSin     (std::sin(_rotation)),
    _rotCos     (std::cos(_rotation)),
    _scale      (scale)
{
    updateOrientation();
}

void Orientation::setPosition(const Vec2f& position)
{
    _position = position;
    updateOrientation();
}

void Orientation::setRotation(float rotation)
{
    _rotation = rotation;
    _rotSin = std::sin(_rotation);
    _rotCos = std::cos(_rotation);
    updateOrientation();
}

void Orientation::setScale(float scale)
{
    _scale = scale;
    updateOrientation();
}

void Orientation::translate(const Vec2f& translation)
{
    _position += translation;
    updateOrientation();
}

void Orientation::rotate(float rotation)
{
    _rotation += rotation;
    updateOrientation();
}

void Orientation::scale(float scale)
{
    _scale *= scale;
    updateOrientation();
}

const Vec2f& Orientation::getPosition() const
{
    return _position;
}

float Orientation::getRotation() const
{
    return _rotation;
}

float Orientation::getScale() const
{
    return _scale;
}

const Mat3f& Orientation::getOrientation() const
{
    return _orientation;
}

void Orientation::updateOrientation()
{
    _orientation <<
        _rotCos*_scale, -_rotSin*_scale,    _position(0),
        _rotSin*_scale, _rotCos*_scale,     _position(1),
        0.0f,           0.0f,               1.0f;
}
