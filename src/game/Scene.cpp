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

#include "Scene.h"
#include "Weapon.h"

#include <GL/glew.h>

namespace PolandBall {

namespace Game {

void Scene::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    Math::Mat4 mvp(this->camera.getProjection() *
                   this->camera.getRotation() *
                   this->camera.getTranslation());

    for (auto& effect: this->effects) {
        effect->setUniform("mvp", mvp);
    }

    for (auto& entity: this->entities) {
        if (entity.first != Entity::EntityType::TYPE_CLIP) {
            entity.second->render();
        }
    }
}

void Scene::update(float frameTime, float frameStep) {
    for (auto entity = this->entities.begin(); entity != this->entities.end(); ) {
        if (entity->second->isDestroyed()) {
            this->entities.erase(entity++);
            continue;
        }

        Entity::EntityType type = entity->second->getType();
        if (type == Entity::EntityType::TYPE_PASSABLE || type == Entity::EntityType::TYPE_UI ||
                type == Entity::EntityType::TYPE_CLIP || type == Entity::EntityType::TYPE_SOLID) {
            entity->second->animate(frameTime);
            ++entity;
            continue;
        }

        if (type == Entity::EntityType::TYPE_WEAPON) {
            std::shared_ptr<Weapon> weapon = std::dynamic_pointer_cast<Weapon>(entity->second);
            if (weapon->getState() == Weapon::WeaponState::STATE_PICKED) {
                entity->second->animate(frameTime);
                ++entity;
                continue;
            }
        }

        for (float step = frameStep, totalTime = step; totalTime < frameTime; totalTime += step) {
            entity->second->setSpeed(entity->second->getSpeed() + this->gravityAcceleration * step);

            for (auto another = this->entities.begin(); another != this->entities.end(); ++another) {
                Entity::EntityType anotherType = another->second->getType();
                if (anotherType == Entity::EntityType::TYPE_PASSABLE || anotherType == Entity::EntityType::TYPE_UI) {
                    continue;
                }

                if (anotherType == Entity::EntityType::TYPE_WEAPON) {
                    std::shared_ptr<Weapon> weapon = std::dynamic_pointer_cast<Weapon>(another->second);
                    if (weapon->getState() == Weapon::WeaponState::STATE_PICKED) {
                        continue;
                    }
                }

                Collider::CollideSide side = entity->second->collides(another->second);
                if (side != Collider::CollideSide::SIDE_NONE) {
                    entity->second->onCollision(another->second, side);
                }

                if ((type == Entity::EntityType::TYPE_WEAPON && anotherType == Entity::EntityType::TYPE_PLAYER) ||
                        (type == Entity::EntityType::TYPE_PLAYER && anotherType == Entity::EntityType::TYPE_WEAPON)) {
                    continue;
                }

                Math::Vec3 speed = entity->second->getSpeed();

                if (side == Collider::CollideSide::SIDE_BOTTOM && speed.get(Math::Vec3::Y) < 0.0f) {
                    speed.set(Math::Vec3::Y, 0.0f);
                } else if (side == Collider::CollideSide::SIDE_TOP && speed.get(Math::Vec3::Y) > 0.0f) {
                    speed.set(Math::Vec3::Y, 0.0f);
                } else if (side == Collider::CollideSide::SIDE_LEFT && speed.get(Math::Vec3::X) < 0.0f) {
                    speed.set(Math::Vec3::X, 0.0f);
                } else if (side == Collider::CollideSide::SIDE_RIGHT && speed.get(Math::Vec3::X) > 0.0f) {
                    speed.set(Math::Vec3::X, 0.0f);
                }

                entity->second->setSpeed(speed);
            }

            entity->second->setPosition(entity->second->getPosition() + entity->second->getSpeed() * step);
            step = (frameTime - totalTime > frameStep) ? frameStep : frameTime - totalTime;
        }

        entity->second->animate(frameTime);
        ++entity;
    }
}

}  // namespace Game

}  // namespace PolandBall
