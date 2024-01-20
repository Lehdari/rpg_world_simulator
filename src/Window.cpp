//
// Project: rpg_world_simulator
// File: Window.cpp
//
// Copyright (c) 2024 Miika 'Lehdari' Lehtimäki
// You may use, distribute and modify this code under the terms
// of the licence specified in file LICENSE which is distributed
// with this source code package.
//

#include <Window.hpp>
#include <FileUtils.hpp>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include "backends/imgui_impl_sdl2.h"


Window::Window(
    const Window::Settings &settings
) :
    _settings               (settings),
    _window                 (nullptr),
    _quit                   (false),
    _paused                 (false),
    _viewport               (_settings.window.width, _settings.window.height,
                             Vec2f(_settings.window.width*0.5f, _settings.window.height*0.5f), 32.0f),
    _cursorPosition         (0.0f, 0.0f),
    _renderMode             (1), // TODO set to 0
    _lastTicks              (0),
    _frameTicks             (0),
    _windowContext          (*this),
    _spriteSheetId          (-1)
{
    int err;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error: Could not initialize SDL!\n");
        return;
    }

    _window = SDL_CreateWindow(
            _settings.window.name.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            (int)_settings.window.width,
            (int)_settings.window.height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (_window == nullptr) {
        printf("Error: SDL Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Initialize OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, _settings.gl.contextMajor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, _settings.gl.contextMinor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, _settings.gl.contextFlags);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, _settings.gl.profileMask);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, _settings.gl.doubleBuffer);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    _glCtx = SDL_GL_CreateContext(_window);

    if (_glCtx == nullptr) {
        printf("Error: SDL OpenGL context could not be created! SDL_Error: %s\n",
               SDL_GetError());
        return;
    }

    // Load OpenGL extensions
    if (!gladLoadGL()) {
        printf("Error: gladLoadGL failed\n");
        return;
    }

    // Initialize OpenGL
    glViewport(0, 0, _settings.window.width, _settings.window.height);
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize ImGui
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(_window, _glCtx);
    ImGui_ImplOpenGL3_Init("#version 460");
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Initialize world
    init();
}

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // Destroy window and quit SDL subsystems
    SDL_GL_DeleteContext(_glCtx);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void Window::init(void)
{
    _spriteRenderer.init();
    _spriteRenderer.setWindowSize((int)_settings.window.width, (int)_settings.window.height);
    _spriteSheetId = _spriteRenderer.addSpriteSheetFromFile(assetsDir / "sprites/sprites.png", 128, 128);

//    fug::SpriteComponent creatureSpriteComponent(_spriteSheetId, 0);
//    creatureSpriteComponent.setOrigin(Vec2f(ConfigSingleton::spriteRadius, ConfigSingleton::spriteRadius));
//    fug::SpriteComponent foodSpriteComponent(_spriteSheetId, 1);
//    foodSpriteComponent.setOrigin(Vec2f(ConfigSingleton::spriteRadius, ConfigSingleton::spriteRadius));
//    foodSpriteComponent.setColor(Vec3f(0.2f, 0.6f, 0.0f));
}

void Window::loop(void)
{
    uint32_t frameId = 0;

    // Application main loop
    while (!_quit) {
        // Event handling
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            handleEvent(event);
        }

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!_paused)
//            updateWorld();

//        _creatureSystem.setStage(CreatureSystem::Stage::PROCESS_INPUTS);
//        _ecs.runSystem(_creatureSystem);

        updateGUI();

        // Render world
//        map.render(_viewport, _renderMode);
        _spriteRenderer.render(_viewport);

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap draw and display buffers
        SDL_GL_SwapWindow(_window);

        if (!_paused) {
            ++frameId;
        }

        uint32_t curTicks = SDL_GetTicks();
        _frameTicks = curTicks - _lastTicks;
        _lastTicks = curTicks;
    }
}

void Window::handleEvent(SDL_Event& event)
{
    switch (event.type) {
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
                case SDL_WINDOWEVENT_CLOSE:
                    _quit = true;
                    break;
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    _quit = true;
                    break;
                case SDLK_PAUSE:
                    _paused = !_paused;
                    break;
                case SDLK_F12:
                    SDL_SetWindowFullscreen(_window, SDL_GetWindowFlags(_window) ^ SDL_WINDOW_FULLSCREEN);
                    break;
            }
        case SDL_MOUSEWHEEL:
            _viewport.zoom(std::pow(1.414213562373f, (float)event.wheel.y), _cursorPosition, 0.0625f);
            break;
        case SDL_MOUSEMOTION:
            _cursorPosition << (float)event.motion.x, (float)event.motion.y;
            break;
#if 0
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button) {
                case SDL_BUTTON_LEFT: {
                    Vec2f clickWorldPos = _viewport.toWorld(_cursorPosition);
                    static Vec2f maxRadiusVec(
                        ConfigSingleton::maxObjectRadius, ConfigSingleton::maxObjectRadius);
                    // Find the clicked creature (if any)
                    Vector<fug::EntityId> entities;
                    world.getEntities(entities, clickWorldPos-maxRadiusVec, clickWorldPos+maxRadiusVec);
                    for (auto& eId : entities) {
                        if (_ecs.getComponent<FoodComponent>(eId) != nullptr)
                            continue;
                        auto* oc = _ecs.getComponent<fug::Orientation2DComponent>(eId);
                        if ((oc->getPosition()-clickWorldPos).norm() <
                            oc->getScale()*ConfigSingleton::spriteRadius) {
                            _activeCreature = eId;
                            break;
                        }
                    }
                } break;
            }
#endif
    }
}

void Window::updateGUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(_window);
    ImGui::NewFrame();

    ImGui::Begin("Simulation Controls");
    ImGui::End();
}
