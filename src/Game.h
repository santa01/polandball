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
#include "Camera.h"
#include "Player.h"

#include <SDL2/SDL_video.h>
#include <memory>
#include <chrono>
#include <vector>

namespace PolandBall {

class Game {
public:
    enum {
        ERROR_OK = 0,
        ERROR_SETUP = 1
    };

    enum {
        SCENE_DEPTH = 3,
        DEFAULT_SPEED = 2
    };

    Game();
    Game(int width, int height);

    int exec();

private:
    Game(const Game&) = delete;
    Game& operator =(const Game&) = delete;

    bool setUp();
    void tearDown();

    void updateWorld();
    void renderWorld();

    void initialize();

    SDL_Window *window;
    SDL_GLContext context;

    std::vector<std::shared_ptr<Entity>> entites;
    Camera camera;
    Player player;

    bool running;
    int width, height;
    float frameTime;
};

}  // namespace PolandBall

#endif  // POLANDBALL_H
