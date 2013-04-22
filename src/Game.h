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
#include "INonCopyable.h"

#include <memory>
#include <vector>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_ttf.h>

namespace PolandBall {

class Game: public INonCopyable {
public:
    enum {
        ERROR_OK = 0,
        ERROR_SETUP = 1
    };

    Game();
    int exec();

private:
    bool setUp();
    void tearDown();

    bool initSDL();
    bool initOpenGL();
    bool initFontConfig();
    void initTestScene();

    void updateWorld();
    void updatePlayer();
    void updateFPS();
    void renderWorld();

    Math::Vec3 toWorld(const Math::Vec3 vector) const;

    void updateMousePosition() const {
        SDL_Event event = { SDL_MOUSEMOTION };
        SDL_GetMouseState(&event.motion.x, &event.motion.y);
        SDL_PushEvent(&event);
    }

    SDL_Window* window;
    SDL_GLContext context;
    TTF_Font* defaultFont;

    std::vector<std::shared_ptr<Entity>> entites;
    std::shared_ptr<Player> player;
    std::shared_ptr<Entity> cursor;
    std::shared_ptr<Entity> fpsCounter;

    Math::Vec3 gravityAcceleration;
    Camera camera;

    bool running;
    int width, height;
    float frameTime;
    float frameStep;
};

}  // namespace PolandBall

#endif  // POLANDBALL_H
