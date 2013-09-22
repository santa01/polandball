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
#include "Player.h"

#include <GL/glew.h>

namespace PolandBall {

namespace Game {

void Scene::addEntity(const std::shared_ptr<Entity>& entity) {
    if (entity != nullptr) {
        this->entities.insert(std::make_pair(entity->getType(), entity));

        auto effect = entity->getPrimitive()->getEffect();
        if (effect != nullptr) {
            this->effects.insert(effect);
        }

        if (entity->getType() == Entity::EntityType::TYPE_WIDGET) {
            auto label = std::dynamic_pointer_cast<Label>(entity);
            if (label != nullptr) {
                label->setProjection(this->camera.getProjection());
            }
        }
    }
}

void Scene::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    Math::Mat4 mvp(this->camera.getProjection() *
                   this->camera.getRotation() *
                   this->camera.getTranslation());

    for (auto& effect: this->effects) {
        effect->setUniform("mvp", mvp);
    }

    auto playerEntity = this->entities.find(Entity::EntityType::TYPE_PLAYER);
    for (auto& entity: this->entities) {
        if (!entity.second->isVisible()) {
            continue;
        }

        /* Do not render any picked and non-active weapon */
        if (entity.first == Entity::EntityType::TYPE_WEAPON) {
            auto weapon = std::dynamic_pointer_cast<Weapon>(entity.second);
            if (weapon->getState() == Weapon::WeaponState::STATE_PICKED) {
                auto player = std::dynamic_pointer_cast<Player>(playerEntity->second);
                bool skipWeapon = false;

                for (int slot = 0; slot < 3; slot++) {
                    auto& ownWeapon = player->getWeapon(static_cast<Game::Weapon::WeaponSlot>(slot));
                    if (weapon == ownWeapon && slot != player->activeSlot) {
                        skipWeapon = true;
                        break;
                    }
                }

                if (skipWeapon) {
                    continue;
                }
            }
        }

        entity.second->getPrimitive()->render();
    }
}

void Scene::update(float frameTime, float frameStep) {
    for (auto entity = this->entities.begin(); entity != this->entities.end(); ++entity) {
        if (entity->second->destroyed || !entity->second->isCollidable() ||
                (entity->second->isCollidable() && entity->second->isPassive())) {
            continue;
        }

        Entity::EntityType type = entity->second->getType();
        if (type == Entity::EntityType::TYPE_WEAPON) {
            auto weapon = std::dynamic_pointer_cast<Weapon>(entity->second);
            if (weapon->getState() == Weapon::WeaponState::STATE_PICKED) {
                continue;
            }
        }

        for (float step = frameStep, totalTime = step; totalTime < frameTime; totalTime += step) {
            entity->second->setSpeed(entity->second->getSpeed() + this->gravityAcceleration * step);

            for (auto another = this->entities.begin(); another != this->entities.end(); ++another) {
                if (another->second->destroyed || !another->second->isCollidable()) {
                    continue;
                }

                Entity::EntityType anotherType = another->second->getType();
                if ((anotherType == Entity::EntityType::TYPE_WEAPON || anotherType == Entity::EntityType::TYPE_PACK) &&
                        (type == Entity::EntityType::TYPE_WEAPON || type == Entity::EntityType::TYPE_PACK)) {
                    continue;
                }

                if (anotherType == Entity::EntityType::TYPE_WEAPON) {
                    auto weapon = std::dynamic_pointer_cast<Weapon>(another->second);
                    if (weapon->getState() == Weapon::WeaponState::STATE_PICKED) {
                        continue;
                    }
                }

                Collider::CollideSide side = entity->second->getCollider()->collides(another->second->getCollider());
                if (side != Collider::CollideSide::SIDE_NONE) {
                    entity->second->onCollision(another->second, side);
                }

                if (((anotherType == Entity::EntityType::TYPE_WEAPON || anotherType == Entity::EntityType::TYPE_PACK) &&
                        type == Entity::EntityType::TYPE_PLAYER) ||
                        ((type == Entity::EntityType::TYPE_WEAPON || type == Entity::EntityType::TYPE_PACK) &&
                        anotherType == Entity::EntityType::TYPE_PLAYER)) {
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
    }

    for (auto entity = this->entities.begin(); entity != this->entities.end(); ) {
        if (entity->second->destroyed) {
            this->entities.erase(entity++);
        } else {
            entity->second->animate(frameTime);
            ++entity;
        }
    }
}

}  // namespace Game

}  // namespace PolandBall
