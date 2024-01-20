//
// Project: rpg_world_simulator
// File: SpriteRenderer.hpp
//
// Copyright (c) 2021 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#pragma once

#include "SpriteSheet.hpp"

#include <gut_utils/TypeUtils.hpp>
#include <gut_opengl/Shader.hpp>


class SpriteRenderer {
public:
    SpriteRenderer();

    SpriteRenderer(const SpriteRenderer&) = delete;
    SpriteRenderer(SpriteRenderer&&) = delete;
    SpriteRenderer& operator=(const SpriteRenderer&) = delete;
    SpriteRenderer& operator=(SpriteRenderer&&) = delete;

    ~SpriteRenderer();

    void init();

    SpriteSheetId addSpriteSheetFromFile(
        const std::string& fileName,
        int spriteWidth,
        int spriteHeight);

    void setWindowSize(int windowWidth, int windowHeight);

    void render(const Mat3f& viewport = Mat3f::Identity());

    // Clear sprite memory without rendering;
    void clear();

    friend class Sprite;

private:
    std::vector<SpriteSheet>    _spriteSheets;
    gut::Shader                 _shader;

    int                         _windowWidth;
    int                         _windowHeight;

    GLuint                      _vertexArrayObjectId;
    GLuint                      _positionBufferId;
    GLuint                      _texCoordBufferId;
    GLuint                      _colorBufferId;

    Vector<Vector<Vec2f>>       _spriteVertexPositions;
    Vector<Vector<Vec2f>>       _spriteVertexTexCoords;
    Vector<Vector<Vec3f>>       _spriteVertexColors;
};

