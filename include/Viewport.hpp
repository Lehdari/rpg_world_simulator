//
// Project: rpg_world_simulator
// File: Viewport.hpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#pragma once


#include <gut_utils/MathTypes.hpp>


class Viewport {
public:
    Viewport(float windowWidth, float windowHeight, const Vec2f& position, float scale);

    /** @brief  Multiply current scale
     *  @param  scale   Scaling factor by which to modify the current scale
     *  @param  minScale    Minimum allowed scale level
     *  @param  maxScale    Maximum allowed scale level
     */
    void applyScale(float scale, float minScale=0.5f, float maxScale=300.0f);

    /** @brief  Zoom the viewport with respect to a pivot point
     *  @param  zoom    Scaling factor, similar to scale in applyScale
     *  @param  pivot   Pivot point used as a centroid for zooming, in pixel coordinates
     *  @param  minScale    Minimum allowed scale level
     *  @param  maxScale    Maximum allowed scale level
     */
    void zoom(float scale, const Vec2f& pivot, float minScale=0.5f, float maxScale=300.0f);

    /** @brief  Center the viewport on world location
     *  @param  position    Position to center te viewport onto, in world coordinates
     */
    void centerTo(const Vec2f& position);

    Vec2f toWorld(const Vec2f& position);

    float getWindowWidth() const;
    float getWindowHeight() const;
    const Mat3f& getViewport() const;

    operator const Mat3f&() const;

private:
    float   _windowWidth;
    float   _windowHeight;
    float   _initialScale;
    Mat3f   _viewport;
};
