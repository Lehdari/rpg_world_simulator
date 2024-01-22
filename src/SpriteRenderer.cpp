//
// Project: rpg_world_simulator
// File: SpriteRenderer.cpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include "SpriteRenderer.hpp"
#include "Sprite.hpp"
#include "Orientation.hpp"
#include "FileUtils.hpp"

#include <gut_opengl/Texture.hpp>


SpriteRenderer::SpriteRenderer() :
    _windowWidth            (1280),
    _windowHeight           (720),
    _vertexArrayObjectId    (0),
    _positionBufferId       (0),
    _texCoordBufferId       (0),
    _colorBufferId          (0)
{}

SpriteRenderer::~SpriteRenderer()
{
    if (_vertexArrayObjectId != 0)
        glDeleteVertexArrays(1, &_vertexArrayObjectId);
    if (_positionBufferId != 0)
        glDeleteBuffers(1, &_positionBufferId);
    if (_texCoordBufferId != 0)
        glDeleteBuffers(1, &_texCoordBufferId);
    if (_colorBufferId != 0)
        glDeleteBuffers(1, &_colorBufferId);
}

void SpriteRenderer::init()
{
    _shader.load(shaderDir / "VS_Sprite.glsl", shaderDir / "FS_Sprite.glsl");

    //  create and bind the VAO
    glGenVertexArrays(1, &_vertexArrayObjectId);
    glBindVertexArray(_vertexArrayObjectId);

    //  upload the vertex data to GPU and set up the vertex attribute arrays
    glGenBuffers(1, &_positionBufferId);
    glGenBuffers(1, &_texCoordBufferId);
    glGenBuffers(1, &_colorBufferId);
}

SpriteSheetId SpriteRenderer::addSpriteSheetFromFile(
    const std::string& fileName, int spriteWidth, int spriteHeight)
{
    _spriteSheets.emplace_back(fileName, spriteWidth, spriteHeight);

    _spriteVertexPositions.resize(_spriteSheets.size());
    _spriteVertexTexCoords.resize(_spriteSheets.size());
    _spriteVertexColors.resize(_spriteSheets.size());

    return _spriteSheets.size()-1;
}

const SpriteSheet& SpriteRenderer::getSpriteSheet(SpriteSheetId id) const
{
    return _spriteSheets[id];
}

void SpriteRenderer::setWindowSize(int windowWidth, int windowHeight)
{
    _windowWidth = windowWidth;
    _windowHeight = windowHeight;
}

void SpriteRenderer::render(const Mat3f& viewport)
{
    glBindVertexArray(_vertexArrayObjectId);

    for (int i = 0; i < _spriteSheets.size(); ++i) {
        auto& vertexPositions = _spriteVertexPositions[i];
        auto& vertexTexCoords = _spriteVertexTexCoords[i];
        auto& vertexColors = _spriteVertexColors[i];

        glBindBuffer(GL_ARRAY_BUFFER, _positionBufferId);
        glBufferData(GL_ARRAY_BUFFER,
                     vertexPositions.size() * sizeof(Vec2f),
                     vertexPositions.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0);

        glBindBuffer(GL_ARRAY_BUFFER, _texCoordBufferId);
        glBufferData(GL_ARRAY_BUFFER,
                     vertexTexCoords.size() * sizeof(Vec2f),
                     vertexTexCoords.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0);

        glBindBuffer(GL_ARRAY_BUFFER, _colorBufferId);
        glBufferData(GL_ARRAY_BUFFER,
                     vertexColors.size() * sizeof(Vec3f),
                     vertexColors.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0);

        _shader.use();
        _shader.setUniform("windowWidth", _windowWidth);
        _shader.setUniform("windowHeight", _windowHeight);
        _shader.setUniform("viewport", viewport);
        _spriteSheets[i]._texture.bind();
        _shader.setUniform("tex", 0);
        glDrawArrays(GL_TRIANGLES, 0, vertexPositions.size());

        vertexPositions.clear();
        vertexTexCoords.clear();
        vertexColors.clear();
    }
}

void SpriteRenderer::operator()(Sprite& sprite, Orientation& orientation)
{
    sprite.update(*this);

    auto& vertexPositions = _spriteVertexPositions[sprite._spriteSheetId];
    Mat3f orientationMatrix = orientation.getOrientation();
    orientationMatrix(0,0) *= sprite._scale(0);
    orientationMatrix(1,0) *= sprite._scale(0);
    orientationMatrix(0,1) *= sprite._scale(1);
    orientationMatrix(1,1) *= sprite._scale(1);
    vertexPositions.emplace_back((orientationMatrix * sprite._positions[0]).block<2,1>(0,0));
    vertexPositions.emplace_back((orientationMatrix * sprite._positions[1]).block<2,1>(0,0));
    vertexPositions.emplace_back((orientationMatrix * sprite._positions[3]).block<2,1>(0,0));
    vertexPositions.emplace_back((orientationMatrix * sprite._positions[3]).block<2,1>(0,0));
    vertexPositions.emplace_back((orientationMatrix * sprite._positions[1]).block<2,1>(0,0));
    vertexPositions.emplace_back((orientationMatrix * sprite._positions[2]).block<2,1>(0,0));

    auto& vertexTexCoords = _spriteVertexTexCoords[sprite._spriteSheetId];
    vertexTexCoords.emplace_back(sprite._texCoords[0]);
    vertexTexCoords.emplace_back(sprite._texCoords[1]);
    vertexTexCoords.emplace_back(sprite._texCoords[3]);
    vertexTexCoords.emplace_back(sprite._texCoords[3]);
    vertexTexCoords.emplace_back(sprite._texCoords[1]);
    vertexTexCoords.emplace_back(sprite._texCoords[2]);

    auto& vertexColors = _spriteVertexColors[sprite._spriteSheetId];
    vertexColors.emplace_back(sprite._color);
    vertexColors.emplace_back(sprite._color);
    vertexColors.emplace_back(sprite._color);
    vertexColors.emplace_back(sprite._color);
    vertexColors.emplace_back(sprite._color);
    vertexColors.emplace_back(sprite._color);
}

void SpriteRenderer::clear()
{
    for (int i = 0; i < _spriteSheets.size(); ++i) {
        auto& vertexPositions = _spriteVertexPositions[i];
        auto& vertexTexCoords = _spriteVertexTexCoords[i];
        auto& vertexColors = _spriteVertexColors[i];

        vertexPositions.clear();
        vertexTexCoords.clear();
        vertexColors.clear();
    }
}
