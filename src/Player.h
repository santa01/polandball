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

#ifndef PLAYER_H
#define	PLAYER_H

#include "Entity.h"
#include "IMovable.h"
#include "INonCopyable.h"
#include "Signals.h"

#include <memory>

namespace PolandBall {

class Player: public IMovable, public INonCopyable {
public:
    enum {
        DEFAULT_SPEED = 2
    };

    using IMovable::setPosition;

    void setPosition(const Math::Vec3& position) {
        this->updatePosition(position - this->getPosition());  // Emit signal
        this->entity->setPosition(position);
    }
    
    Math::Vec3 getPosition() const {
        return this->entity->getPosition();
    }

    void setEntity(const std::shared_ptr<Entity>& entity) {
        this->entity = entity;
    }

    std::shared_ptr<Entity>& getEntity() {
        return this->entity;
    }

    Signals::Signal<Math::Vec3> updatePosition;

private:
    std::shared_ptr<Entity> entity;
};

}  // namespace PolandBall

#endif  // PLAYER_H
