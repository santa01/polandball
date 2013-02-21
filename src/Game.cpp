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
#include <cmath>

namespace PolandBall {

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

        this->updatePlayer();
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
    auto backgroundSprite = std::shared_ptr<Sprite>(new Sprite());
    backgroundSprite->setTexture(ResourceManager::getInstance().makeTexture("textures/day_sky_3x4.png"));

    auto backgroundEntity = std::shared_ptr<Entity>(new Entity());
    backgroundEntity->setSprite(backgroundSprite);
    backgroundEntity->setCollidable(false);
    backgroundEntity->scale(1.5f);  // Fit the screen
    backgroundEntity->scaleX(this->width / (this->height / 1.0f));  // Scale for aspect ratio
    this->entites.push_back(backgroundEntity);

    //-----------------
    auto bricksSprite = std::shared_ptr<Sprite>(new Sprite());
    bricksSprite->setTexture(ResourceManager::getInstance().makeTexture("textures/bricks.png"));

    auto bricksEntity = std::shared_ptr<Entity>(new Entity());
    bricksEntity->setSprite(bricksSprite);
    bricksEntity->setPosition(0.0f, -2.7f, 0.0f);
    bricksEntity->scale(0.15f);
    bricksEntity->scaleX(15.0f);
    this->entites.push_back(bricksEntity);

    //-----------------
    bricksSprite = std::shared_ptr<Sprite>(new Sprite());
    bricksSprite->setTexture(ResourceManager::getInstance().makeTexture("textures/bricks.png"));

    bricksEntity = std::shared_ptr<Entity>(new Entity());
    bricksEntity->setSprite(bricksSprite);
    bricksEntity->setPosition(2.0f, 0.0f, 0.0f);
    bricksEntity->scale(0.15f);
    bricksEntity->scaleY(15.0f);
    this->entites.push_back(bricksEntity);

    //-----------------
    auto playerSprite = std::shared_ptr<Sprite>(new Sprite());
    playerSprite->setTexture(ResourceManager::getInstance().makeTexture("textures/turkey_ball.png"));

    this->player = std::shared_ptr<Player>(new Player());
    this->player->setSprite(playerSprite);
    this->player->scale(0.15f);
    this->player->setPosition(0.0f, -1.0f, 0.0f);
    this->entites.push_back(this->player);

    //-----------------
    this->player->updatePosition.connect(std::bind(&Entity::onPositionUpdate, backgroundEntity, std::placeholders::_1));
    this->player->updatePosition.connect(std::bind(&Camera::onPositionUpdate, &this->camera, std::placeholders::_1));

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
    for (auto& entity: this->entites) {
        if (entity->getType() == Entity::TYPE_DYNAMIC) {
            if (!entity->isCollidable()) {
                continue;
            }

            entity->setSpeed(entity->getSpeed() + this->gravityAcceleration * this->frameTime);

            for (auto& another: this->entites) {
                if (!another->isCollidable()) {
                    continue;
                }

                Collider::CollideSide collide = entity->getCollider()->collides(another->getCollider());
                Math::Vec3 speed = entity->getSpeed();

                switch (collide) {
                    case Collider::SIDE_BOTTOM:
                        if (speed.get(Math::Vec3::Y) < 0.0f) {
                            speed.set(Math::Vec3::Y, 0.0f);
                        }
                        break;

                    case Collider::SIDE_TOP:
                        if (speed.get(Math::Vec3::Y) > 0.0f) {
                            speed.set(Math::Vec3::Y, 0.0f);
                        }
                        break;

                    case Collider::SIDE_LEFT:
                        if (speed.get(Math::Vec3::X) < 0.0f) {
                            speed.set(Math::Vec3::X, 0.0f);
                        }
                        break;

                    case Collider::SIDE_RIGHT:
                        if (speed.get(Math::Vec3::X) > 0.0f) {
                            speed.set(Math::Vec3::X, 0.0f);
                        }
                        break;

                    default:
                        break;
                }

                entity->setSpeed(speed);
                entity->collideSide(collide);
            }
        }

        entity->setPosition(entity->getPosition() + entity->getSpeed() * this->frameTime);
    }
}

void Game::updatePlayer() {
    Uint8 *keyStates = SDL_GetKeyboardState(nullptr);

    if (keyStates[SDL_SCANCODE_ESCAPE]) {
        this->running = false;
    }

    float playerMoveSpeed = this->player->getSpeed().get(Math::Vec3::X);
    float playerUpSpeed = this->player->getSpeed().get(Math::Vec3::Y);

    Math::Vec3 moveAcceleration;
    float moveSpeedDelta = this->maxMoveSpeed * this->frameTime * 10.0f;

    // Descelerate only on foot
    if (!keyStates[SDL_SCANCODE_LEFT] && !keyStates[SDL_SCANCODE_RIGHT] && playerUpSpeed == 0.0f) {
        if ((playerMoveSpeed > 0.0f && playerMoveSpeed - moveSpeedDelta < 0.0f) ||
                (playerMoveSpeed < 0.0f && playerMoveSpeed + moveSpeedDelta > 0.0f)) {
            moveAcceleration = Math::Vec3::UNIT_X * fabs(playerMoveSpeed);
        } else {
            moveAcceleration = Math::Vec3::UNIT_X * moveSpeedDelta;
        }

        if (playerMoveSpeed < 0.0f) {
            this->player->accelerateBy(moveAcceleration);
        } else if (playerMoveSpeed > 0.0f) {
            this->player->accelerateBy(-moveAcceleration);
        }
    } else {
        if (keyStates[SDL_SCANCODE_RIGHT] && !keyStates[SDL_SCANCODE_LEFT]) {
            // We press RIGHT and we already moving RIGHT
            if (playerMoveSpeed > 0.0f && fabs(playerMoveSpeed) + moveSpeedDelta > this->maxMoveSpeed) {
                moveAcceleration = Math::Vec3::UNIT_X * (this->maxMoveSpeed - fabs(playerMoveSpeed));
            } else {
                moveAcceleration = Math::Vec3::UNIT_X * moveSpeedDelta;
            }
        } else if (keyStates[SDL_SCANCODE_LEFT] && !keyStates[SDL_SCANCODE_RIGHT]) {
            // We press LEFT and we already moving LEFT
            if (playerMoveSpeed < 0.0f && fabs(playerMoveSpeed) + moveSpeedDelta > this->maxMoveSpeed) {
                moveAcceleration = -Math::Vec3::UNIT_X * (this->maxMoveSpeed - fabs(playerMoveSpeed));
            } else {
                moveAcceleration = -Math::Vec3::UNIT_X * moveSpeedDelta;
            }
        }

        this->player->accelerateBy(moveAcceleration);
    }

    if (keyStates[SDL_SCANCODE_UP]) {
        if (this->player->getJumpTime() < this->maxJumpTime) {
            if (playerUpSpeed < this->maxJumpSpeed) {
                Math::Vec3 jumpAcceleration(Math::Vec3::UNIT_Y * (this->maxJumpSpeed - playerUpSpeed));
                this->player->accelerateBy(jumpAcceleration);
            }

            this->player->updateJumpTime(this->frameTime);
        }
    }
}

void Game::renderWorld() {
    glClear(GL_COLOR_BUFFER_BIT);

    Math::Mat4 mvp(this->camera.getProjectionMatrix() *
                   this->camera.getRotationMatrix() *
                   this->camera.getTranslationMatrix());

    for (auto& entity: this->entites) {
        if (entity->isRenderable()) {
            entity->getSprite()->getEffect()->setMVP(mvp);
            entity->getSprite()->render();
        }
    }

    SDL_GL_SwapWindow(this->window);
}

}  // namespace PolandBall
