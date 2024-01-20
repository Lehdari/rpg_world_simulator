//
// Project: rpg_world_simulator
// File: Viewport.cpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include <Viewport.hpp>


Viewport::Viewport(float windowWidth, float windowHeight, const Vec2f& position, float scale) :
    _windowWidth    (windowWidth),
    _windowHeight   (windowHeight),
    _initialScale   (scale)
{
    _viewport <<
        scale,  0.0f,   position(0),
        0.0f,   -scale, position(1),
        0.0f,   0.0f,   1.0f;
}

void Viewport::applyScale(float scale, float minScale, float maxScale)
{
    float s = _viewport(0,0)*scale;
    if ((s-_initialScale)*(s-_initialScale) < 1.0e-6)
        s = _initialScale;

    if (s < minScale || s > maxScale)
        return;

    _viewport(0,0) = s;
    _viewport(1,1) = -s;
}

void Viewport::zoom(float scale, const Vec2f& pivot, float minScale, float maxScale)
{
    float s = _viewport(0,0)*scale; // new scaling factor

    // return to initial scale for numerical stability
    if ((s-_initialScale)*(s-_initialScale) < 1.0e-6)
        s = _initialScale;

    // scaling limits
    if (s < minScale || s > maxScale)
        return;

    // zoom transform
    Mat3f z;
    z <<    scale,  0.0f,   pivot(0)-scale*pivot(0),
            0.0f,   scale,  pivot(1)-scale*pivot(1),
            0.0f,   0.0f,   1.0f;

    _viewport = z * _viewport;

    // set scaling to the stabilized factor
    _viewport(0,0) = s;
    _viewport(1,1) = -s;
}

void Viewport::centerTo(const Vec2f& position)
{
    _viewport.block<2,1>(0,2) <<
        (0.5f * _windowWidth) - (position(0)*_viewport(0,0)),
        (0.5f * _windowHeight) - (position(1)*_viewport(1,1));
}

Vec2f Viewport::toWorld(const Vec2f& position)
{
    return (_viewport.inverse() * Vec3f(position(0), position(1), 1.0)).block<2,1>(0,0);
}

float Viewport::getWindowWidth() const
{
    return _windowWidth;
}

float Viewport::getWindowHeight() const
{
    return _windowHeight;
}

const Mat3f& Viewport::getViewport() const
{
    return _viewport;
}

Viewport::operator const Mat3f&() const
{
    return _viewport;
}
