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

#ifndef POLANDBALL_H
#define	POLANDBALL_H

#include "Entity.h"
#include "Player.h"
#include "Camera.h"
#include "Vec3.h"
#include "INonCopyable.h"

#include <SDL2/SDL_video.h>
#include <memory>
#include <chrono>
#include <vector>

namespace PolandBall {

class Game: public INonCopyable {
public:
    enum {
        ERROR_OK = 0,
        ERROR_SETUP = 1
    };

    Game() {
        this->width = 800;
        this->height = 600;
        this->initialize();
    }

    Game(int width, int height) {
        this->width = width;
        this->height = height;
        this->initialize();
    }

    int exec();

private:
    bool setUp();
    void tearDown();

    void updateWorld();
    void updatePlayer();
    void renderWorld();

    void initialize() {
        this->running = true;
        this->window = nullptr;
        this->context = nullptr;
        this->frameTime = 0.0f;

        this->cameraOffset = 3.0f;
        this->maxJumpTime = 0.25f;
        this->maxJumpSpeed = 4.0f;
        this->maxMoveSpeed = 3.0f;

        this->defaultAcceleration = Math::Vec3(20.0f, 0.0f, 0.0f);
        this->gravityAcceleration = Math::Vec3(0.0f, -10.0f, 0.0f);

        this->camera.setPosition(0.0f, 0.0f, -this->cameraOffset);
    }

    SDL_Window *window;
    SDL_GLContext context;

    std::vector<std::shared_ptr<Entity>> entites;
    std::shared_ptr<Player> player;

    Math::Vec3 defaultAcceleration;
    Math::Vec3 gravityAcceleration;
    Camera camera;

    bool running;
    int width, height;
    float frameTime;

    float cameraOffset;
    float maxJumpTime;
    float maxJumpSpeed;
    float maxMoveSpeed;
};

}  // namespace PolandBall

#endif  // POLANDBALL_H
