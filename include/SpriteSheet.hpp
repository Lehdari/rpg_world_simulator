//
// Project: rpg_world_simulator
// File: SpriteSheet.hpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#pragma once


#include <gut_opengl/Texture.hpp>
#include <vector>


class SpriteSheet {
public:
    SpriteSheet();

    SpriteSheet(
        const std::string& fileName,
        int spriteWidth,
        int spriteHeight);

    void loadFromFile(
        const std::string& fileName,
        int spriteWidth,
        int spriteHeight);

    void getDimensions(
        int spriteId,
        int& spriteWidth,
        int& spriteHeight) const;

    void getUVCoordinates(
        int spriteId,
        float& left,
        float& right,
        float& top,
        float& bottom) const;

    friend class SpriteRenderer;

private:
    gut::Texture    _texture;
    int             _spriteWidth;
    int             _spriteHeight;
    int             _nSpritesX;
    int             _nSpritesY;
};


using SpriteSheetId = std::vector<SpriteSheet>::size_type;
