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
#include "Weapon.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>

namespace PolandBall {

PolandBall::PolandBall() {
    this->window = nullptr;
    this->context = nullptr;

    this->running = true;
    this->width = 800;
    this->height = 600;
    this->maxFps = 100.0f;

    this->frameTime = 0.0f;
    this->frameStep = 0.1f / this->maxFps;
}

int PolandBall::exec() {
    if (!this->initialize()) {
        this->shutdown();
        return ERROR_SETUP;
    }

    SDL_Event event;
    while (this->running) {
        unsigned int beginFrame = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    this->running = false;
                    break;

                case SDL_MOUSEMOTION:
                    this->onMouseMotion(event.motion);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    this->onMouseButton(event.button);
                    break;
            }
        }

        this->onIdle();

        this->frameTime = (SDL_GetTicks() - beginFrame) / 1000.0f;
        float maxFrameTime = 1.0f / this->maxFps;

        if (this->frameTime < maxFrameTime) {
            SDL_Delay((maxFrameTime - this->frameTime) * 1000);
            this->frameTime = maxFrameTime;
        }

        SDL_GL_SwapWindow(this->window);
    }

    this->shutdown();
    return ERROR_OK;
}

bool PolandBall::initialize() {
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Initializing...");

    if (!this->initSDL() || !this->initOpenGL()) {
        return false;
    }

    if (!this->initScene() || !this->initUi()) {
        return false;
    }

    return true;
}

void PolandBall::shutdown() {
    this->scene->getCamera().positionChanged.disconnectAll();
    this->player->positionChanged.disconnectAll();

    this->player.reset();
    this->cursor.reset();
    this->scene.reset();

    for (auto& weapon: this->weapons) {
        weapon.first.reset();
        weapon.second.reset();
    }

    this->emptySlot.reset();
    this->armor.reset();
    this->health.reset();

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

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_ERROR, "glewInit() failed: %s",
                glewGetErrorString(glewError));
        return false;
    }

    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "GLEW version: %s", glewGetString(GLEW_VERSION));

    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

bool PolandBall::initScene() {
    this->scene = std::shared_ptr<Game::Scene>(new Game::Scene());

    Game::Camera& camera = this->scene->getCamera();
    camera.setProjectionType(Game::Camera::TYPE_ORTHOGRAPHIC);
    camera.setAspectRatio(this->width / (this->height / 1.0f));
    camera.setNearPlane(-5.0f);
    camera.setFarPlane(5.0f);

    //-----------------
    auto backgroundEntity = Utils::ResourceManager::getInstance().makeEntity("assets/backgrounds/sunny.asset");
    if (backgroundEntity == nullptr) {
        return false;
    }

    backgroundEntity->scaleY(camera.getFarPlane() - camera.getNearPlane());
    backgroundEntity->scaleX((camera.getFarPlane() - camera.getNearPlane()) * camera.getAspectRatio());
    this->scene->addEntity(backgroundEntity);

    //-----------------
    auto bricksEntity = Utils::ResourceManager::getInstance().makeEntity("assets/blocks/kazakhstan.asset");
    if (bricksEntity == nullptr) {
        return false;
    }

    bricksEntity->setPosition(0.0f, -4.0f, 0.0f);
    bricksEntity->scaleX(20.0f * 1.5f);  // Scale for aspect ratio
    bricksEntity->replicateX(20.0f);
    this->scene->addEntity(bricksEntity);

    //-----------------
    bricksEntity = Utils::ResourceManager::getInstance().makeEntity("assets/blocks/kazakhstan.asset");
    if (bricksEntity == nullptr) {
        return false;
    }

    bricksEntity->setPosition(4.0f, 1.0f, 0.0f);
    bricksEntity->scaleX(1.5f);  // Scale for aspect ratio
    this->scene->addEntity(bricksEntity);

    //-----------------
    this->player = std::dynamic_pointer_cast<Game::Player>(
            Utils::ResourceManager::getInstance().makeEntity("assets/players/turkey.asset"));
    if (this->player == nullptr) {
        return false;
    }

    this->scene->addEntity(this->player);

    //-----------------
    auto m4a1 = Utils::ResourceManager::getInstance().makeEntity("assets/weapons/m4a1.asset");
    if (m4a1 == nullptr) {
        return false;
    }

    m4a1->setPosition(-6.0f, 0.0f, 0.0f);
    this->scene->addEntity(m4a1);

    //-----------------
    auto ak74 = Utils::ResourceManager::getInstance().makeEntity("assets/weapons/ak74.asset");
    if (ak74 == nullptr) {
        return false;
    }

    ak74->setPosition(-3.0f, 0.0f, 0.0f);
    this->scene->addEntity(ak74);

    //-----------------
    auto m1911 = Utils::ResourceManager::getInstance().makeEntity("assets/weapons/m1911.asset");
    if (m1911 == nullptr) {
        return false;
    }

    m1911->setPosition(3.0f, 0.0f, 0.0f);
    this->scene->addEntity(m1911);

    //-----------------
    auto beretta92 = Utils::ResourceManager::getInstance().makeEntity("assets/weapons/beretta92.asset");
    if (beretta92 == nullptr) {
        return false;
    }

    beretta92->setPosition(6.0f, 0.0f, 0.0f);
    this->scene->addEntity(beretta92);

    //-----------------
    this->player->positionChanged.connect(
            std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
            backgroundEntity, std::placeholders::_1));
    this->player->positionChanged.connect(
            std::bind(static_cast<void(Game::Camera::*)(const Math::Vec3&)>(&Game::Camera::setPosition),
            &camera, std::placeholders::_1));

    return true;
}

bool PolandBall::initUi() {
    auto defaultFont = Utils::ResourceManager::getInstance().makeFont("fonts/dejavu-sans.ttf");
    if (defaultFont == nullptr) {
        return false;
    }

    this->emptySlot = Utils::ResourceManager::getInstance().makeEntity("assets/ui/slot_empty.asset");
    if (this->emptySlot == nullptr) {
        return false;
    }

    //-----------------
    auto& primaryWeapon = this->weapons[Game::Weapon::WeaponSlot::SLOT_PRIMARY];

    primaryWeapon.first = Utils::ResourceManager::getInstance().makeEntity("assets/ui/slot_empty.asset");
    primaryWeapon.first->setOrigin(this->screenToWorld(Math::Vec3(40.0f, 40.0f, 0.0f)));
    this->scene->addEntity(primaryWeapon.first);

    auto primaryWeaponBorder = Utils::ResourceManager::getInstance().makeEntity("assets/ui/border_weapon.asset");
    if (primaryWeaponBorder == nullptr) {
        return false;
    }

    primaryWeaponBorder->setOrigin(this->screenToWorld(Math::Vec3(40.0f, 40.0f, 0.0f)));
    this->scene->addEntity(primaryWeaponBorder);

    primaryWeapon.second = std::shared_ptr<Game::TextLabel>(new Game::TextLabel());
    primaryWeapon.second->setOrigin(this->screenToWorld(Math::Vec3(40.0f, 80.0f, 0.0f)));
    primaryWeapon.second->setFont(defaultFont);
    primaryWeapon.second->scale(0.3f);
    this->scene->addEntity(primaryWeapon.second);

    //-----------------
    auto& secondaryWeapon = this->weapons[Game::Weapon::WeaponSlot::SLOT_SECONDARY];

    secondaryWeapon.first = Utils::ResourceManager::getInstance().makeEntity("assets/ui/slot_empty.asset");
    secondaryWeapon.first->setOrigin(this->screenToWorld(Math::Vec3(110.0f, 40.0f, 0.0f)));
    this->scene->addEntity(secondaryWeapon.first);

    auto secondaryWeaponBorder = Utils::ResourceManager::getInstance().makeEntity("assets/ui/border_weapon.asset");
    if (secondaryWeaponBorder == nullptr) {
        return false;
    }

    secondaryWeaponBorder->setOrigin(this->screenToWorld(Math::Vec3(110.0f, 40.0f, 0.0f)));
    this->scene->addEntity(secondaryWeaponBorder);

    secondaryWeapon.second = std::shared_ptr<Game::TextLabel>(new Game::TextLabel());
    secondaryWeapon.second->setOrigin(this->screenToWorld(Math::Vec3(110.0f, 80.0f, 0.0f)));
    secondaryWeapon.second->setFont(defaultFont);
    secondaryWeapon.second->scale(0.3f);
    this->scene->addEntity(secondaryWeapon.second);

    //-----------------
    auto& meeleWeapon = this->weapons[Game::Weapon::WeaponSlot::SLOT_MEELE];

    meeleWeapon.first = Utils::ResourceManager::getInstance().makeEntity("assets/ui/slot_empty.asset");
    meeleWeapon.first->setOrigin(this->screenToWorld(Math::Vec3(180.0f, 40.0f, 0.0f)));
    this->scene->addEntity(meeleWeapon.first);

    auto meeleWeaponBorder = Utils::ResourceManager::getInstance().makeEntity("assets/ui/border_weapon.asset");
    if (meeleWeaponBorder == nullptr) {
        return false;
    }

    meeleWeaponBorder->setOrigin(this->screenToWorld(Math::Vec3(180.0f, 40.0f, 0.0f)));
    this->scene->addEntity(meeleWeaponBorder);

    meeleWeapon.second = std::shared_ptr<Game::TextLabel>(new Game::TextLabel());
    meeleWeapon.second->setOrigin(this->screenToWorld(Math::Vec3(180.0f, 80.0f, 0.0f)));
    meeleWeapon.second->setFont(defaultFont);
    meeleWeapon.second->scale(0.3f);
    this->scene->addEntity(meeleWeapon.second);

    //-----------------
    auto armorShield = Utils::ResourceManager::getInstance().makeEntity("assets/ui/shield_armor.asset");
    if (armorShield == nullptr) {
        return false;
    }

    armorShield->setOrigin(this->screenToWorld(Math::Vec3(this->width - 40.0f, 40.0f, 0.0f)));
    this->scene->addEntity(armorShield);

    this->armor = std::shared_ptr<Game::TextLabel>(new Game::TextLabel());
    this->armor->setOrigin(this->screenToWorld(Math::Vec3(this->width - 40.0f, 80.0f, 0.0f)));
    this->armor->setFont(defaultFont);
    this->armor->scale(0.3f);
    this->scene->addEntity(this->armor);

    //-----------------
    auto healthShield = Utils::ResourceManager::getInstance().makeEntity("assets/ui/shield_health.asset");
    if (healthShield == nullptr) {
        return false;
    }

    healthShield->setOrigin(this->screenToWorld(Math::Vec3(this->width - 100.0f, 40.0f, 0.0f)));
    this->scene->addEntity(healthShield);

    this->health = std::shared_ptr<Game::TextLabel>(new Game::TextLabel());
    this->health->setOrigin(this->screenToWorld(Math::Vec3(this->width - 100.0f, 80.0f, 0.0f)));
    this->health->setFont(defaultFont);
    this->health->scale(0.3f);
    this->scene->addEntity(this->health);

    //-----------------
    Game::Camera& camera = this->scene->getCamera();

    for (auto& weapon: this->weapons) {
        camera.positionChanged.connect(
            std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
            weapon.first, std::placeholders::_1));
        camera.positionChanged.connect(
            std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
            weapon.second, std::placeholders::_1));
    }

    camera.positionChanged.connect(
        std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
        primaryWeaponBorder, std::placeholders::_1));
    camera.positionChanged.connect(
        std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
        secondaryWeaponBorder, std::placeholders::_1));
    camera.positionChanged.connect(
        std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
        meeleWeaponBorder, std::placeholders::_1));

    //-----------------
    camera.positionChanged.connect(
        std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
        healthShield, std::placeholders::_1));
    camera.positionChanged.connect(
        std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
        armorShield, std::placeholders::_1));

    camera.positionChanged.connect(
        std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
        this->health, std::placeholders::_1));
    camera.positionChanged.connect(
        std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
        this->armor, std::placeholders::_1));

    //-----------------
    this->cursor = Utils::ResourceManager::getInstance().makeEntity("assets/ui/cursor_aim.asset");
    if (this->cursor == nullptr) {
        return false;
    }

    this->scene->addEntity(this->cursor);

    //-----------------
    this->player->positionChanged.connect(
        std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
        this->cursor, std::placeholders::_1));

    return true;
}

void PolandBall::onMouseMotion(SDL_MouseMotionEvent& event) {
    Math::Vec3 cursorPosition = this->screenToWorld(Math::Vec3(event.x, event.y, 0.0f));
    this->cursor->setOrigin(cursorPosition);
    this->player->aimAt(cursorPosition);
}

void PolandBall::onMouseButton(SDL_MouseButtonEvent& event) {
    if (event.button == 0 && event.state == SDL_PRESSED) {
        this->player->shoot();
    }
}

void PolandBall::onIdle() {
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

    std::stringstream text;
    for (int slot = 0; slot < 3; slot++) {
        auto& weapon = this->player->getWeapon(static_cast<Game::Weapon::WeaponSlot>(slot));
        if (weapon != nullptr) {
            text << weapon->getAmmo();
            this->weapons[slot].first->setTexture(weapon->getTexture());
            this->weapons[slot].first->shearX(0.0f, 2);
            //this->weapons[slot].first->shearY(0.0f, 1);
            this->weapons[slot].first->roll(45.0f);
            this->weapons[slot].second->setText(text.str());
        } else {
            this->weapons[slot].first->setTexture(this->emptySlot->getTexture());
            this->weapons[slot].second->clear();
        }
    }

    text.str("");
    text << this->player->getHealth();
    this->health->setText(text.str());

    text.str("");
    text << this->player->getArmor();
    this->armor->setText(text.str());

    this->scene->update(this->frameTime, this->frameStep);
    this->scene->render();
}

Math::Vec3 PolandBall::screenToWorld(const Math::Vec3 vector) const {
    const Game::Camera& camera = this->scene->getCamera();
    Math::Mat4 translation;

    translation.set(0, 0, (camera.getFarPlane() - camera.getNearPlane()) * camera.getAspectRatio() /
                          (this->width / 2.0f));
    translation.set(1, 1, (camera.getNearPlane() - camera.getFarPlane()) / (this->height / 2.0f));
    translation.set(0, 3, (camera.getNearPlane() - camera.getFarPlane()) * camera.getAspectRatio());
    translation.set(1, 3, camera.getFarPlane() - camera.getNearPlane());

    Math::Vec4 worldPosition = translation * Math::Vec4(vector, 1.0f);
    return Math::Vec3(worldPosition.get(Math::Vec3::X), worldPosition.get(Math::Vec3::Y), vector.get(Math::Vec3::Z));
}

}  // namespace PolandBall
