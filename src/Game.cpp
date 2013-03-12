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

#include <sstream>
#include <iomanip>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <fontconfig/fontconfig.h>

namespace PolandBall {

Game::Game() {
    this->window = nullptr;
    this->context = nullptr;
    this->defaultFont = nullptr;

    this->running = true;
    this->width = 800;
    this->height = 600;
    this->frameTime = 0.0f;
    this->frameStep = 0.001f;

    this->gravityAcceleration = Math::Vec3(0.0f, -35.0f, 0.0f);
    this->camera.setAspectRatio(this->width / (this->height / 1.0f));
}

int Game::exec() {
    if (!this->setUp()) {
        this->tearDown();
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
                    Math::Vec3 cursorPosition(this->toWorld(Math::Vec3(event.motion.x, event.motion.y, 0.0f)));
                    this->cursor->setPosition(cursorPosition + this->player->getPosition());
                    this->player->aimAt(cursorPosition);
                    break;
            }
        }

        this->updatePlayer();
        this->updateWorld();
        this->updateFPS();
        this->renderWorld();

        this->frameTime = (SDL_GetTicks() - beginFrame) / 1000.0f;
    }

    this->tearDown();
    return ERROR_OK;
}

bool Game::setUp() {
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Setting up...");

    if (!this->initSDL() || !this->initOpenGL() || !this->initFontConfig()) {
        return false;
    }

    this->initTestScene();
    return true;
}

void Game::tearDown() {
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Tearing down...");

    if (this->defaultFont) {
        TTF_CloseFont(this->defaultFont);
    }

    if (this->context) {
        SDL_GL_DeleteContext(this->context);
    }

    if (this->window) {
        SDL_DestroyWindow(this->window);
    }

    FcFini();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

bool Game::initSDL() {
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

bool Game::initOpenGL() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Initializing %d x %d viewport", this->width, this->height);
    this->window = SDL_CreateWindow("Polandball The Gaem", 0, 0, this->width, this->height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!this->window) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_ERROR, "SDL_CreateWindow() failed: %s", SDL_GetError());
        return false;
    }

    this->context = SDL_GL_CreateContext(this->window);
    if (!this->context) {
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

bool Game::initFontConfig() {
    if (!FcInit()) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_ERROR, "FcInit() failed");
        return false;
    }

    int fcVersion = FcGetVersion();
    int fcMajor = fcVersion / 10000;
    int fcMinor = (fcVersion - fcMajor * 10000) / 100;
    int fcRevision = fcVersion - fcMajor * 10000 - fcMinor * 100;
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Fontconfig version: %d.%d.%d",
            fcMajor, fcMinor, fcRevision);

    FcPattern* pattern = FcNameParse((const FcChar8*)"monospace");
    FcConfigSubstitute(nullptr, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);

    FcResult result;
    FcPattern* match = FcFontMatch(nullptr, pattern, &result);
    FcPatternDestroy(pattern);

    if (!match) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_ERROR, "FcFontMatch() failed: no `sans-serif' font found");
        return false;
    }

    FcChar8* fontPath = FcPatternFormat(match, (const FcChar8*)"%{file}");
    FcChar8* fontName = FcPatternFormat(match, (const FcChar8*)"%{family}");

    this->defaultFont = TTF_OpenFont((const char*)fontPath, 12);
    if (!this->defaultFont) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_ERROR, "TTF_OpenFont failed: %s", TTF_GetError());
        return false;
    }

    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Using `%s' family font", fontName);

    FcStrFree(fontName);
    FcStrFree(fontPath);
    FcPatternDestroy(match);

    return true;
}

void Game::initTestScene() {
    // GL_DEPTH_TEST is OFF! Manually arrange sprites, farthest renders first!
    auto backgroundSprite = std::shared_ptr<Sprite>(new Sprite());
    backgroundSprite->setTexture(Utils::ResourceManager::getInstance().makeTexture("textures/background_static_800_600.png"));

    auto backgroundEntity = std::shared_ptr<Entity>(new Entity());
    backgroundEntity->setSprite(backgroundSprite);
    backgroundEntity->setCollidable(false);
    backgroundEntity->scaleY(10.0f);
    backgroundEntity->scaleX(10.0f * this->camera.getAspectRatio());
    this->entites.push_back(backgroundEntity);

    //-----------------
    auto bricksSprite = std::shared_ptr<Sprite>(new Sprite());
    bricksSprite->setTexture(Utils::ResourceManager::getInstance().makeTexture("textures/entity_static_kz.png"));

    auto bricksEntity = std::shared_ptr<Entity>(new Entity());
    bricksEntity->setSprite(bricksSprite);
    bricksEntity->setPosition(0.0f, -4.0f, 0.0f);
    bricksEntity->scaleX(20.0f * 1.5f);  // Scale for aspect ratio
    bricksEntity->replicateX(20.0f);
    this->entites.push_back(bricksEntity);

    //-----------------
    bricksSprite = std::shared_ptr<Sprite>(new Sprite());
    bricksSprite->setTexture(Utils::ResourceManager::getInstance().makeTexture("textures/entity_static_kz.png"));

    bricksEntity = std::shared_ptr<Entity>(new Entity());
    bricksEntity->setSprite(bricksSprite);
    bricksEntity->setPosition(4.0f, 1.0f, 0.0f);
    bricksEntity->scaleX(1.5f);  // Scale for aspect ratio
    this->entites.push_back(bricksEntity);

    //-----------------
    auto playerSprite = std::shared_ptr<Sprite>(new Sprite());
    playerSprite->setTexture(Utils::ResourceManager::getInstance().makeTexture("textures/player_tk.png"));

    this->player = std::shared_ptr<Player>(new Player());
    this->player->setSprite(playerSprite);
    this->player->shearX(0, 2);
    this->entites.push_back(this->player);

    //-----------------
    auto cursorSprite = std::shared_ptr<Sprite>(new Sprite());
    cursorSprite->setTexture(Utils::ResourceManager::getInstance().makeTexture("textures/cursor.png"));

    this->cursor = std::shared_ptr<Entity>(new Entity());
    this->cursor->setSprite(cursorSprite);
    this->cursor->setCollidable(false);
    this->cursor->scale(0.5f);
    this->entites.push_back(this->cursor);

    //-----------------
    this->fpsCounter = std::shared_ptr<Entity>(new Entity());
    this->fpsCounter->setCollidable(false);
    this->fpsCounter->scale(0.25f);
    this->fpsCounter->setPosition(-12.0f, 9.5f, 0.0f);
    this->entites.push_back(this->fpsCounter);

    //-----------------
    this->player->positionChanged.connect(
            std::bind(static_cast<void(Entity::*)(const Math::Vec3&)>(&Entity::setPosition),
            backgroundEntity, std::placeholders::_1));
    this->player->positionChanged.connect(
            std::bind(static_cast<void(Camera::*)(const Math::Vec3&)>(&Camera::setPosition),
            &this->camera, std::placeholders::_1));
    this->player->positionChanged.connect(
            std::bind(static_cast<void(Entity::*)(const Math::Vec3&)>(&Entity::setPosition),
            this->fpsCounter, std::placeholders::_1));
    this->player->positionChanged.connect(std::bind(&Game::updateMousePosition, this));
}

void Game::updateWorld() {
    for (auto& entity: this->entites) {
        if (entity->getType() != Entity::TYPE_DYNAMIC || !entity->isCollidable()) {
            continue;
        }

        float scaleFactor = (this->frameTime > this->frameTime) ? this->frameTime : this->frameStep;
        float totalTime = scaleFactor;

        while (totalTime <= this->frameTime && scaleFactor > 0.0f) {
            entity->setSpeed(entity->getSpeed() + this->gravityAcceleration * scaleFactor);

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
                entity->collideWith(another, collide);
            }

            entity->setPosition(entity->getPosition() + entity->getSpeed() * scaleFactor);

            scaleFactor = (this->frameTime - totalTime > this->frameStep) ? this->frameStep : this->frameTime - totalTime;
            totalTime += scaleFactor;
        }
    }
}

void Game::updatePlayer() {
    Uint8 *keyStates = SDL_GetKeyboardState(nullptr);
    if (keyStates[SDL_SCANCODE_ESCAPE]) {
        this->running = false;
    }

    // Both pressed or released
    if (keyStates[SDL_SCANCODE_LEFT] == keyStates[SDL_SCANCODE_RIGHT]) {
        this->player->slowDown(this->frameTime);
    } else if (keyStates[SDL_SCANCODE_RIGHT]) {
        this->player->moveRight(this->frameTime);
    } else if (keyStates[SDL_SCANCODE_LEFT]) {
        this->player->moveLeft(this->frameTime);
    }

    if (keyStates[SDL_SCANCODE_UP]) {
        this->player->jump(this->frameTime);
    } else {
        this->player->breakJump();
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

void Game::updateFPS() {
    static float updateTime = 1.0f;
    static float frames = 0.0f;
    static float textAspectRatio = 1.0f;

    if (updateTime >= 0.25f) {
        std::stringstream fps;
        fps << "fps: " << std::setw(5) << std::left << std::setprecision(0) << std::fixed << frames / updateTime;

        int textWidth, textHeight;
        TTF_SizeUTF8(this->defaultFont, fps.str().c_str(), &textWidth, &textHeight);
        this->fpsCounter->scaleX(1.0f / textAspectRatio);
        textAspectRatio = textWidth / (textHeight / 1.0f);
        this->fpsCounter->scaleX(textAspectRatio);

        SDL_Color color = {0, 0, 0, 0};
        SDL_Surface *textOverlay = TTF_RenderUTF8_Blended(this->defaultFont, fps.str().c_str(), color);
        this->fpsCounter->getSprite()->getTexture()->load(textOverlay);
        SDL_FreeSurface(textOverlay);

        updateTime = 0.0f;
        frames = 0.0f;
    }

    updateTime += this->frameTime;
    frames++;
}

Math::Vec3 Game::toWorld(const Math::Vec3 vector) const {
    Math::Vec4 worldPosition(vector, 1.0f);
    Math::Mat4 translation;

    translation.set(0, 0, this->camera.getPlaneDistance() * 2.0f / this->width);
    translation.set(0, 3, -this->camera.getPlaneDistance());
    translation.set(1, 1, -this->camera.getPlaneDistance() * 2.0f / this->height);
    translation.set(1, 3, this->camera.getPlaneDistance());

    worldPosition = translation * worldPosition;
    return Math::Vec3(worldPosition.get(Math::Vec3::X) * this->camera.getAspectRatio(),
                      worldPosition.get(Math::Vec3::Y),
                      worldPosition.get(Math::Vec3::Z));
}

}  // namespace PolandBall
