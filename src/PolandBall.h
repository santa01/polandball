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
#include "TextLabel.h"
#include "Player.h"
#include "Scene.h"
#include "NonCopyable.h"
#include "ArgumentParser.h"

#include <SDL2/SDL_events.h>
#include <memory>
#include <utility>

namespace PolandBall {

class PolandBall: public Common::NonCopyable {
public:
    enum {
        ERROR_OK,
        ERROR_SETUP
    };

    PolandBall(int argc, char** argv);
    int exec();

private:
    bool initialize();
    void shutdown();

    bool parseCLI();
    bool initSDL();
    bool initOpenGL();
    bool initScene();
    bool initUi();

    void onMouseMotion(SDL_MouseMotionEvent& event);
    void onMouseButton(SDL_MouseButtonEvent& event);
    void onIdle();

    // Orthographic projection only
    Math::Vec3 screenToWorld(const Math::Vec3 vector) const;

    SDL_Window* window;
    SDL_GLContext context;
    Utils::ArgumentParser arguments;

    std::shared_ptr<Game::Scene> scene;
    std::shared_ptr<Game::Player> player;
    std::shared_ptr<Game::Entity> cursor;

    // Requires sync if Weapon::WeaponSlot is updated
    std::pair<std::shared_ptr<Game::Entity>, std::shared_ptr<Game::TextLabel>> weapons[3];
    std::shared_ptr<Game::Entity> emptySlot;
    std::shared_ptr<Game::TextLabel> health;
    std::shared_ptr<Game::TextLabel> armor;

    int argc;
    char** argv;

    int width;
    int height;
    float maxFps;
    bool vsync;

    bool running;
    float frameTime;
    float frameStep;
};

}  // namespace PolandBall

#endif  // POLANDBALL_H
