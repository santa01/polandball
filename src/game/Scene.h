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

#ifndef SCENE_H
#define	SCENE_H

#include "NonCopyable.h"
#include "Camera.h"
#include "Entity.h"
#include "RenderEffect.h"
#include "Logger.h"
#include "Vec3.h"
#include "Label.h"

#include <memory>
#include <map>
#include <unordered_set>
#include <utility>
#include <algorithm>

namespace PolandBall {

namespace Game {

class Scene: public Common::NonCopyable {
public:
    Scene():
            gravityAcceleration(0.0f, -35.0f, 0.0f) {
    }

    void setGravityAcceleration(const Math::Vec3& gravityAcceleration) {
        this->gravityAcceleration = gravityAcceleration;
    }

    const Math::Vec3& getGravityAcceleration() const {
        return this->gravityAcceleration;
    }

    void setCamera(const Camera& camera) {
        this->camera = camera;

        for (auto& entity: this->entities) {
            if (entity.first == Entity::EntityType::TYPE_UI) {
                auto label = std::dynamic_pointer_cast<Label>(entity.second);
                if (label != nullptr) {
                    label->setProjection(this->camera.getProjection());
                }
            }
        }
    }

    Camera& getCamera() {
        return this->camera;
    }

    void addEntity(const std::shared_ptr<Entity>& entity);

    void render();
    void update(float frameTime, float frameStep);

private:
    std::multimap<Entity::EntityType, std::shared_ptr<Entity>> entities;
    std::unordered_set<std::shared_ptr<Opengl::RenderEffect>> effects;

    Math::Vec3 gravityAcceleration;
    Camera camera;
};

}  // namespace Game

}  // namespace PolandBall

#endif  // SCENE_H
