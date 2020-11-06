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

#include <Engine.h>
#include <Entity.h>
#include <Label.h>
#include <Player.h>
#include <Scene.h>
#include <memory>
#include <utility>

namespace PolandBall {

class PolandBall: public Graphene::Engine {
private:
    void onMouseMotion(int x, int y) override;
    void onMouseButton(Graphene::MouseButton button, bool state) override;
    void onSetup() override;
    void onIdle() override;

    bool setupScene();
    bool setupUI();
    void updateScene();
    void updateUI();

    std::shared_ptr<Game::Scene> scene;
    std::shared_ptr<Game::Player> player;
    std::shared_ptr<Game::Entity> cursor;

    // Requires sync if Weapon::WeaponSlot is updated
    std::pair<std::shared_ptr<Game::Entity>, std::shared_ptr<Game::Label>> weapons[3];
    std::shared_ptr<Game::Entity> emptySlot;
    std::shared_ptr<Game::Label> health;
    std::shared_ptr<Game::Label> armor;
};

}  // namespace PolandBall

#endif  // POLANDBALL_H
