/*
 * Copyright (c) 2013 Pavlo Lavrenenko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Game.h"
#include "Logger.h"
#include "Vec3.h"
#include "ResourceManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL/SDL.h>

namespace PolandBall {

Game::Game() {
    this->initialize();
    this->width = 800;
    this->height = 600;
}

Game::Game(int width, int height) {
    this->initialize();
    this->width = width;
    this->height = height;
}

int Game::exec() {
    if (!this->setUp()) {
        this->tearDown();
        return ERROR_SETUP;
    }

    std::chrono::system_clock::time_point startTime, endTime;

    SDL_Event event;
    while (this->running) {
        startTime = std::chrono::system_clock::now();  // TODO: steady_clock

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    this->running = false;
                    break;
            }
        }

        this->updateWorld();
        this->renderWorld();

        endTime = std::chrono::system_clock::now();
        this->frameTime = (std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() /
                1000000.0f);
    }

    this->tearDown();
    return ERROR_OK;
}

bool Game::setUp() {
    Logger::getInstance().log(Logger::LOG_INFO, "Setting up...");

    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) {
        Logger::getInstance().log(Logger::LOG_ERROR, "SDL_Init() failed: %s", SDL_GetError());
        return false;
    }

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) {
        Logger::getInstance().log(Logger::LOG_ERROR, "IMG_Init() failed: %s", SDL_GetError());
        return false;
    }

    SDL_version sdlVersion;
    SDL_GetVersion(&sdlVersion);
    Logger::getInstance().log(Logger::LOG_INFO, "SDL version: %d.%d.%d",
            sdlVersion.major, sdlVersion.minor, sdlVersion.patch);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    Logger::getInstance().log(Logger::LOG_INFO, "Initializing %d x %d viewport", this->width, this->height);
    this->window = SDL_CreateWindow("Polandball The Gaem", 0, 0, this->width, this->height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!this->window) {
        Logger::getInstance().log(Logger::LOG_ERROR, "SDL_CreateWindow() failed: %s", SDL_GetError());
        return false;
    }

    this->context = SDL_GL_CreateContext(this->window);
    if (!this->context) {
        Logger::getInstance().log(Logger::LOG_ERROR, "SDL_GL_CreateContext() failed: %s", SDL_GetError());
        return false;
    }

    SDL_ShowCursor(SDL_DISABLE);

    Logger::getInstance().log(Logger::LOG_INFO, "OpenGL vendor: %s", glGetString(GL_VENDOR));
    Logger::getInstance().log(Logger::LOG_INFO, "OpenGL version: %s", glGetString(GL_VERSION));

    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // GL_DEPTH_TEST is OFF! Manually arrange sprites, farthest renders first!
    auto sprite = std::shared_ptr<Sprite>(new Sprite());
    sprite->setTexture(ResourceManager::getInstance().makeTexture("textures/day_sky_3x4.png"));
    sprite->setPosition(0.0f, 0.0f, SCENE_DEPTH);
    sprite->scaleX(this->width / (this->height / 1.0f));  // Scale for aspect ratio
    sprite->scale(1.5f);  // Fit the screen

    auto entity = std::shared_ptr<Entity>(new Entity());
    entity->setSprite(sprite);
    this->entites.push_back(entity);

    sprite = std::shared_ptr<Sprite>(new Sprite());
    sprite->setTexture(ResourceManager::getInstance().makeTexture("textures/applejack.png"));
    sprite->setPosition(0.0f, -2.0f, SCENE_DEPTH);
    sprite->scale(0.25f);

    entity = std::shared_ptr<Entity>(new Entity());
    entity->setSprite(sprite);
    this->entites.push_back(entity);

    this->player.setEntity(entity);

    return true;
}

void Game::tearDown() {
    Logger::getInstance().log(Logger::LOG_INFO, "Tearing down...");

    if (this->context) {
        SDL_GL_DeleteContext(this->context);
    }

    if (this->window) {
        SDL_DestroyWindow(this->window);
    }

    IMG_Quit();
    SDL_Quit();
}

void Game::updateWorld() {
    Uint8 *keyStates = SDL_GetKeyboardState(nullptr);
    if (keyStates[SDL_SCANCODE_ESCAPE]) {
        this->running = false;
    }

    Math::Vec3 playerPosition = this->player.getPosition();
    if (keyStates[SDL_SCANCODE_RIGHT]) {
        playerPosition += Math::Vec3::UNIT_X * this->frameTime * DEFAULT_SPEED;
    }

    if (keyStates[SDL_SCANCODE_LEFT]) {
        playerPosition -= Math::Vec3::UNIT_X * this->frameTime * DEFAULT_SPEED;
    }

    this->player.setPosition(playerPosition);
}

void Game::renderWorld() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Math::Mat4 mvp(this->camera.getProjectionMatrix() *
                   this->camera.getRotationMatrix() *
                   this->camera.getTranslationMatrix());

    for (auto& entity: this->entites) {
        entity->getSprite()->getEffect()->setMVP(mvp);
        entity->getSprite()->render();
    }

    SDL_GL_SwapWindow(this->window);
}

void Game::initialize() {
    this->running = true;
    this->window = nullptr;
    this->context = nullptr;
    this->frameTime = 0.0f;
}

}  // namespace PolandBall
