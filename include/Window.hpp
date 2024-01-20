//
// Project: rpg_world_simulator
// File: Window.hpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#pragma once


#include <Viewport.hpp>
#include <SpriteRenderer.hpp>
#include <World.hpp>
#include <string>
#include <SDL.h>
#include <glad/glad.h>


class Window {
public:
    // Settings for the window
    struct WindowSettings {
        std::string name;
        int64_t width;
        int64_t height;
        int64_t framerateLimit;

        explicit WindowSettings(
                const std::string& name = "",
                int64_t width = 1280,
                int64_t height = 720,
                int64_t framerateLimit = 60) :
                name(name),
                width(width),
                height(height),
                framerateLimit(framerateLimit)
        {}
    };

    struct GLSettings {
        int32_t contextMajor;
        int32_t contextMinor;
        SDL_GLcontextFlag contextFlags;
        SDL_GLprofile profileMask;
        bool doubleBuffer;

        explicit GLSettings(
                int32_t contextMajor = 4,
                int32_t contextMinor = 5,
                SDL_GLcontextFlag contextFlags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG,
                SDL_GLprofile profileMask = SDL_GL_CONTEXT_PROFILE_CORE,
                bool doubleBuffer = true) :
                contextMajor(contextMajor),
                contextMinor(contextMinor),
                contextFlags(contextFlags),
                profileMask(profileMask),
                doubleBuffer(doubleBuffer)
        {}
    };

    // Struct for Window context to be passed to pipeline functions
    struct Context {
        WindowSettings* windowSettings;
        GLSettings*     glSettings;

        SDL_Window*     window;
        SDL_GLContext*  glContext;

        bool*           quit; // set to true to exit the loop

        Viewport*       viewport;

        Context(Window& window) :
                windowSettings  (&window._settings.window),
                glSettings      (&window._settings.gl),
                window          (window._window),
                glContext       (&window._glCtx),
                quit            (&window._quit),
                viewport        (&window._viewport)
        {}
    };

    struct Settings {
        WindowSettings window;
        GLSettings gl;

        explicit Settings(
                const WindowSettings& window    = WindowSettings(),
                const GLSettings& gl            = GLSettings()
        ) :
                window          (window),
                gl              (gl)
        {}
    };

    explicit Window(const Settings& settings = Settings());

    ~Window();

    void init(void);
    void loop(void);
    void handleEvent(SDL_Event& event);
    void updateGUI();

private:
    Settings            _settings;
    SDL_Window*         _window;
    SDL_GLContext       _glCtx;

    bool                _quit; // flag for quitting the application
    bool                _paused; // flag for pausing the simulation
    Viewport            _viewport;
    Vec2f               _cursorPosition;
    int                 _renderMode;

    uint32_t            _lastTicks;
    uint32_t            _frameTicks;

    Window::Context     _windowContext;

    SpriteRenderer      _spriteRenderer;
    World               _world;

    // Resources
    SpriteSheetId       _spriteSheetId;
};
