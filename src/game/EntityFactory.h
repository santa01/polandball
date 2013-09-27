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

#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "Pack.h"
#include "Player.h"
#include "Weapon.h"
#include "Widget.h"
#include "Label.h"
#include "ShotTrace.h"
#include "SpriteEntity.h"
#include "ResourceCache.h"
#include "NonCopyable.h"

#include <json-c/json.h>
#include <Vec3.h>
#include <string>
#include <memory>

namespace PolandBall {

namespace Game {

class EntityFactory: public Common::NonCopyable {
public:
    static EntityFactory& getInstance() {
        static EntityFactory instance;
        return instance;
    }

    std::shared_ptr<Utils::ResourceCache>& getResourceCache() {
        return this->resourceCache;
    }

    void setResourceCache(const std::shared_ptr<Utils::ResourceCache>& resourceCache) {
        this->resourceCache = resourceCache;
    }

    std::shared_ptr<Game::Pack> createPack(const std::string& name) const;
    std::shared_ptr<Game::Player> createPlayer(const std::string& name) const;
    std::shared_ptr<Game::Weapon> createWeapon(const std::string& name) const;
    std::shared_ptr<Game::Widget> createWidget(const std::string& name) const;
    std::shared_ptr<Game::ShotTrace> createTrace(const Math::Vec3& from, const Math::Vec3& to) const;
    std::shared_ptr<Game::Label> createLabel(const std::string& fontName, unsigned int size) const;
    std::shared_ptr<Game::SpriteEntity> createBlock(const std::string& name) const;

private:
    EntityFactory():
            resourceCache(new Utils::ResourceCache()) {
    }

    void loadBase(std::shared_ptr<Game::SpriteEntity> entity, const std::shared_ptr<json_object> asset) const;

    std::shared_ptr<Utils::ResourceCache> resourceCache;
};

}  // namespace Game

}  // namespace PolandBall

#endif  // ENTITYFACTORY_H
