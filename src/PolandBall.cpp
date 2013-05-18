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

#include "PolandBall.h"
#include "Logger.h"
#include "Vec3.h"
#include "ResourceManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <algorithm>

namespace PolandBall {

PolandBall::PolandBall() {
    this->window = nullptr;
    this->context = nullptr;

    this->running = true;
    this->width = 800;
    this->height = 600;
    this->frameTime = 0.0f;
    this->frameStep = 0.001f;

    this->gravityAcceleration = Math::Vec3(0.0f, -35.0f, 0.0f);
    this->camera.setProjectionType(Game::Camera::TYPE_ORTHODRAPHIC);
    this->camera.setAspectRatio(this->width / (this->height / 1.0f));
    this->camera.setNearPlane(-5.0f);
    this->camera.setFarPlane(5.0f);
}

int PolandBall::exec() {
    if (!this->initialize()) {
        this->shutdown();
        return ERROR_SETUP;
    }

    SDL_Event event;
    Math::Vec3 cursorPosition;

    while (this->running) {
        unsigned int beginFrame = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    this->running = false;
                    break;

                case SDL_MOUSEMOTION:
                    cursorPosition = this->screenToWorld(Math::Vec3(event.motion.x, event.motion.y, 0.0f));
                    this->cursor->setOrigin(cursorPosition);
                    this->player->aimAt(cursorPosition);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == 0 && event.button.state == SDL_PRESSED) {
                        this->player->shoot();
                    }
                    break;
            }
        }

        this->purgeDestroyed();
        this->updatePlayer();
        this->updateScene();

        this->animate();
        this->render();

        this->frameTime = (SDL_GetTicks() - beginFrame) / 1000.0f;
        if (this->frameTime < 0.01f) {
            SDL_Delay((0.01f - this->frameTime) * 1000);
            this->frameTime = 0.01f;
        }
    }

    this->shutdown();
    return ERROR_OK;
}

bool PolandBall::initialize() {
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Initializing...");

    if (!this->initSDL() || !this->initOpenGL()) {
        return false;
    }

    this->initTestScene();
    return true;
}

void PolandBall::shutdown() {
    this->camera.positionChanged.disconnectAll();
    this->player->positionChanged.disconnectAll();

    this->player.reset();
    this->cursor.reset();
    this->entites.clear();

    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Cleaning caches...");
    Utils::ResourceManager::getInstance().purgeCaches();

    if (this->context) {
        SDL_GL_DeleteContext(this->context);
    }

    if (this->window) {
        SDL_DestroyWindow(this->window);
    }

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Shutting down...");
}

bool PolandBall::initSDL() {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_ERROR, "SDL_Init() failed: %s", SDL_GetError());
        return false;
    }

    if (TTF_Init()) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_ERROR, "TTF_Init() failed: %s", TTF_GetError());
        return false;
    }

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_ERROR, "IMG_Init() failed: %s", IMG_GetError());
        return false;
    }

    SDL_version sdlVersion;
    SDL_GetVersion(&sdlVersion);
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "SDL version: %d.%d.%d",
            sdlVersion.major, sdlVersion.minor, sdlVersion.patch);

    const SDL_version *sdlTtfVersion = TTF_Linked_Version();
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "SDL_ttf version: %d.%d.%d",
            sdlTtfVersion->major, sdlTtfVersion->minor, sdlTtfVersion->patch);

    const SDL_version *sdlImageVersion = IMG_Linked_Version();
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "SDL_image version: %d.%d.%d",
            sdlImageVersion->major, sdlImageVersion->minor, sdlImageVersion->patch);

    SDL_ShowCursor(0);
    return true;
}

bool PolandBall::initOpenGL() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Initializing %d x %d viewport", this->width, this->height);
    this->window = SDL_CreateWindow("Polandball The Gaem", 0, 0, this->width, this->height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (this->window == nullptr) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_ERROR, "SDL_CreateWindow() failed: %s", SDL_GetError());
        return false;
    }

    this->context = SDL_GL_CreateContext(this->window);
    if (this->context == nullptr) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_ERROR, "SDL_GL_CreateContext() failed: %s", SDL_GetError());
        return false;
    }

    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "OpenGL vendor: %s", glGetString(GL_VENDOR));
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "OpenGL version: %s", glGetString(GL_VERSION));

    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void PolandBall::initTestScene() {
    // GL_DEPTH_TEST is OFF! Manually arrange sprites, farthest renders first!

    auto backgroundEntity = Utils::ResourceManager::getInstance().makeEntity("assets/backgrounds/sunny.asset");
    backgroundEntity->scaleY(this->camera.getFarPlane() - this->camera.getNearPlane());
    backgroundEntity->scaleX((this->camera.getFarPlane() - this->camera.getNearPlane()) * this->camera.getAspectRatio());
    this->entites.push_back(backgroundEntity);

    //-----------------
    auto bricksEntity = Utils::ResourceManager::getInstance().makeEntity("assets/blocks/kazakhstan.asset");
    bricksEntity->setPosition(0.0f, -4.0f, 0.0f);
    bricksEntity->scaleX(20.0f * 1.5f);  // Scale for aspect ratio
    bricksEntity->replicateX(20.0f);
    this->entites.push_back(bricksEntity);

    //-----------------
    bricksEntity = Utils::ResourceManager::getInstance().makeEntity("assets/blocks/kazakhstan.asset");
    bricksEntity->setPosition(4.0f, 1.0f, 0.0f);
    bricksEntity->scaleX(1.5f);  // Scale for aspect ratio
    this->entites.push_back(bricksEntity);

    //-----------------
    this->player = std::dynamic_pointer_cast<Game::Player>(
            Utils::ResourceManager::getInstance().makeEntity("assets/players/turkey.asset"));
    this->entites.push_back(this->player);

    //-----------------
    auto m4a1 = Utils::ResourceManager::getInstance().makeEntity("assets/weapons/m4a1.asset");
    m4a1->setPosition(-4.0f, 0.0f, 0.0f);
    this->entites.push_back(m4a1);

    //-----------------
    this->cursor = Utils::ResourceManager::getInstance().makeEntity("assets/cursors/aim.asset");
    this->entites.push_back(this->cursor);

    //-----------------
    this->player->positionChanged.connect(
            std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
            backgroundEntity, std::placeholders::_1));
    this->player->positionChanged.connect(
            std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
            this->cursor, std::placeholders::_1));
    this->player->positionChanged.connect(
            std::bind(static_cast<void(Game::Camera::*)(const Math::Vec3&)>(&Game::Camera::setPosition),
            &this->camera, std::placeholders::_1));
}

void PolandBall::purgeDestroyed() {
    this->entites.erase(
            std::remove_if(this->entites.begin(), this->entites.end(),
                [](const std::shared_ptr<Game::Entity>& entity) -> bool {
                    return entity->isDestroyed();
                }),
            this->entites.end());
}

void PolandBall::updatePlayer() {
    Uint8 *keyStates = SDL_GetKeyboardState(nullptr);

    if (keyStates[SDL_SCANCODE_ESCAPE] > 0) {
        this->running = false;
    }
    if (keyStates[SDL_SCANCODE_RIGHT] > 0) {
        this->player->setState(Game::Player::PlayerState::STATE_RIGHT_STEP);
    }
    if (keyStates[SDL_SCANCODE_LEFT] > 0) {
        this->player->setState(Game::Player::PlayerState::STATE_LEFT_STEP);
    }
    if (keyStates[SDL_SCANCODE_UP] > 0) {
        this->player->setState(Game::Player::PlayerState::STATE_JUMP);
    }
    if (keyStates[SDL_SCANCODE_RETURN] > 0) {
        this->player->dropWeapon();
    }
    if (keyStates[SDL_SCANCODE_1] > 0) {
        this->player->activateSlot(Game::Weapon::WeaponSlot::SLOT_PRIMARY);
    }
    if (keyStates[SDL_SCANCODE_2] > 0) {
        this->player->activateSlot(Game::Weapon::WeaponSlot::SLOT_SECONDARY);
    }
    if (keyStates[SDL_SCANCODE_3] > 0) {
        this->player->activateSlot(Game::Weapon::WeaponSlot::SLOT_MEELE);
    }
}

void PolandBall::updateScene() {
    for (auto& entity: this->entites) {
        Game::Entity::EntityType type = entity->getType();
        if (type == Game::Entity::EntityType::TYPE_PASSABLE ||
                type == Game::Entity::EntityType::TYPE_CLIP ||
                type == Game::Entity::EntityType::TYPE_SOLID) {
            continue;
        }

        if (type == Game::Entity::EntityType::TYPE_WEAPON) {
            std::shared_ptr<Game::Weapon> weapon = std::dynamic_pointer_cast<Game::Weapon>(entity);
            if (weapon->getState() == Game::Weapon::WeaponState::STATE_PICKED) {
                continue;
            }
        }

        for (float step = this->frameStep, totalTime = step; totalTime < this->frameTime; totalTime += step) {
            entity->setSpeed(entity->getSpeed() + this->gravityAcceleration * step);

            for (auto& another: this->entites) {
                Game::Entity::EntityType anotherType = another->getType();
                if (anotherType == Game::Entity::EntityType::TYPE_PASSABLE) {
                    continue;
                }

                if (anotherType == Game::Entity::EntityType::TYPE_WEAPON) {
                    std::shared_ptr<Game::Weapon> weapon = std::dynamic_pointer_cast<Game::Weapon>(another);
                    if (weapon->getState() == Game::Weapon::WeaponState::STATE_PICKED) {
                        continue;
                    }
                }

                Game::Collider::CollideSide side = entity->collides(another);
                if (side != Game::Collider::CollideSide::SIDE_NONE) {
                    entity->onCollision(another, side);
                }

                if ((type == Game::Entity::EntityType::TYPE_WEAPON &&
                        anotherType == Game::Entity::EntityType::TYPE_PLAYER) ||
                        (type == Game::Entity::EntityType::TYPE_PLAYER &&
                        anotherType == Game::Entity::EntityType::TYPE_WEAPON)) {
                    continue;
                }

                Math::Vec3 speed = entity->getSpeed();
                switch (side) {
                    case Game::Collider::CollideSide::SIDE_BOTTOM:
                        if (speed.get(Math::Vec3::Y) < 0.0f) {
                            speed.set(Math::Vec3::Y, 0.0f);
                        }
                        break;

                    case Game::Collider::CollideSide::SIDE_TOP:
                        if (speed.get(Math::Vec3::Y) > 0.0f) {
                            speed.set(Math::Vec3::Y, 0.0f);
                        }
                        break;

                    case Game::Collider::CollideSide::SIDE_LEFT:
                        if (speed.get(Math::Vec3::X) < 0.0f) {
                            speed.set(Math::Vec3::X, 0.0f);
                        }
                        break;

                    case Game::Collider::CollideSide::SIDE_RIGHT:
                        if (speed.get(Math::Vec3::X) > 0.0f) {
                            speed.set(Math::Vec3::X, 0.0f);
                        }
                        break;

                    default:
                        break;
                }
                entity->setSpeed(speed);
            }

            entity->setPosition(entity->getPosition() + entity->getSpeed() * step);
            step = (this->frameTime - totalTime > this->frameStep) ? this->frameStep : this->frameTime - totalTime;
        }
    }
}

void PolandBall::animate() {
    for (auto& entity: this->entites) {
        if (entity->getType() != Game::Entity::EntityType::TYPE_CLIP) {
            entity->animate(this->frameTime);
        }
    }
}

void PolandBall::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    Math::Mat4 mvp(this->camera.getProjection() *
                   this->camera.getRotation() *
                   this->camera.getTranslation());

    for (auto& entity: this->entites) {
        if (entity->getType() != Game::Entity::EntityType::TYPE_CLIP && entity->getEffect() != nullptr) {
            entity->getEffect()->setUniform("mvp", mvp);
            entity->render();
        }
    }

    SDL_GL_SwapWindow(this->window);
}

Math::Vec3 PolandBall::screenToWorld(const Math::Vec3 vector) const {
    Math::Mat4 translation;
    translation.set(0, 0, (this->camera.getFarPlane() - this->camera.getNearPlane()) *
                          this->camera.getAspectRatio() / (this->width / 2.0f));
    translation.set(1, 1, (this->camera.getNearPlane() - this->camera.getFarPlane()) /
                          (this->height / 2.0f));
    translation.set(0, 3, (this->camera.getNearPlane() - this->camera.getFarPlane()) *
                          this->camera.getAspectRatio());
    translation.set(1, 3, (this->camera.getFarPlane() - this->camera.getNearPlane()));

    Math::Vec4 worldPosition = translation * Math::Vec4(vector, 1.0f);
    return Math::Vec3(worldPosition.get(Math::Vec3::X), worldPosition.get(Math::Vec3::Y), 0.0f);
}

}  // namespace PolandBall