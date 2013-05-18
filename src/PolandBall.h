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
#define POLANDBALL_H

#include "Entity.h"
#include "Player.h"
#include "Camera.h"
#include "Vec3.h"
#include "NonCopyable.h"

#include <memory>
#include <vector>
#include <SDL2/SDL_video.h>

namespace PolandBall {

class PolandBall: public Common::NonCopyable {
public:
    enum {
        ERROR_OK = 0,
        ERROR_SETUP = 1
    };

    PolandBall();
    int exec();

private:
    bool initialize();
    void shutdown();

    bool initSDL();
    bool initOpenGL();
    void initTestScene();

    void purgeDestroyed();
    void updatePlayer();
    void updateScene();

    void animate();
    void render();

    // Orthographic projection only
    Math::Vec3 screenToWorld(const Math::Vec3 vector) const;

    SDL_Window* window;
    SDL_GLContext context;

    std::vector<std::shared_ptr<Game::Entity>> entites;
    std::shared_ptr<Game::Player> player;
    std::shared_ptr<Game::Entity> cursor;

    Math::Vec3 gravityAcceleration;
    Game::Camera camera;

    bool running;
    int width, height;
    float frameTime;
    float frameStep;
};

}  // namespace PolandBall

#endif  // POLANDBALL_H
