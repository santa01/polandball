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

#include "EntityFactory.h"
#include "Logger.h"

#include <sstream>

namespace PolandBall {

namespace Game {

std::shared_ptr<Pack> EntityFactory::createPack(const std::string& name) const {
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Building Pack from `%s'", name.c_str());

    auto asset = this->resourceCache->loadAsset(name);
    if (asset == nullptr) {
        return nullptr;
    }

    std::shared_ptr<Game::Pack> pack(new Game::Pack());
    this->loadBase(pack, asset);

    json_object* payload = json_object_object_get(asset.get(), "payload");
    if (payload == nullptr || json_object_get_type(payload) != json_type_string) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Parameter `payload' is not set");
    } else {
        std::string payloadValue(json_object_get_string(payload));
        Game::Pack::PayloadType payloadType = pack->getPayloadType();

        if (payloadValue == "health") {
            payloadType = Game::Pack::PayloadType::TYPE_HEALTH;
        } else if (payloadValue == "armor") {
            payloadType = Game::Pack::PayloadType::TYPE_ARMOR;
        } else if (payloadValue == "primary_ammo") {
            payloadType = Game::Pack::PayloadType::TYPE_PRIMARY_AMMO;
        } else if (payloadValue == "secondary_ammo") {
            payloadType = Game::Pack::PayloadType::TYPE_SECONDARY_AMMO;
        } else {
            Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING,
                    "Got unknown `payload' value `%s'", payloadValue.c_str());
        }

        pack->setPayloadType(payloadType);
    }

    json_object* value = json_object_object_get(asset.get(), "value");
    if (value == nullptr || json_object_get_type(value) != json_type_int) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Parameter `value' is not set");
    } else {
        pack->setValue(json_object_get_int(value));
    }

    return pack;
}

std::shared_ptr<Player> EntityFactory::createPlayer(const std::string& name) const {
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Building Player from `%s'", name.c_str());

    auto asset = this->resourceCache->loadAsset(name);
    if (asset == nullptr) {
        return nullptr;
    }

    std::shared_ptr<Game::Player> player(new Game::Player());
    this->loadBase(player, asset);

    json_object* maxMoveSpeed = json_object_object_get(asset.get(), "max_move_speed");
    if (maxMoveSpeed == nullptr || json_object_get_type(maxMoveSpeed) != json_type_double) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Parameter `max_move_speed' is not set");
    } else {
        player->setMaxMoveSpeed(json_object_get_double(maxMoveSpeed));
    }

    json_object* maxJumpSpeed = json_object_object_get(asset.get(), "max_jump_speed");
    if (maxJumpSpeed == nullptr || json_object_get_type(maxJumpSpeed) != json_type_double) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Parameter `max_jump_speed' is not set");
    } else {
        player->setMaxJumpSpeed(json_object_get_double(maxJumpSpeed));
    }

    json_object* maxJumpTime = json_object_object_get(asset.get(), "max_jump_time");
    if (maxJumpTime == nullptr || json_object_get_type(maxJumpTime) != json_type_double) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Parameter `max_jump_time' is not set");
    } else {
        player->setMaxJumpTime(json_object_get_double(maxJumpTime));
    }

    json_object* maxHealth = json_object_object_get(asset.get(), "max_health");
    if (maxHealth == nullptr || json_object_get_type(maxHealth) != json_type_int) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Parameter `max_health' is not set");
    } else {
        player->setMaxHealth(json_object_get_int(maxHealth));
    }

    json_object* maxArmor = json_object_object_get(asset.get(), "max_armor");
    if (maxArmor == nullptr || json_object_get_type(maxArmor) != json_type_int) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Parameter `max_armor' is not set");
    } else {
        player->setMaxArmor(json_object_get_int(maxArmor));
    }

    json_object* health = json_object_object_get(asset.get(), "health");
    if (health == nullptr || json_object_get_type(health) != json_type_int) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Parameter `health' is not set");
    } else {
        player->setHealth(json_object_get_int(health));
    }

    json_object* armor = json_object_object_get(asset.get(), "armor");
    if (armor == nullptr || json_object_get_type(armor) != json_type_int) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Parameter `armor' is not set");
    } else {
        player->setArmor(json_object_get_int(armor));
    }

    return player;
}

std::shared_ptr<Weapon> EntityFactory::createWeapon(const std::string& name) const {
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Building Weapon from `%s'", name.c_str());

    auto asset = this->resourceCache->loadAsset(name);
    if (asset == nullptr) {
        return nullptr;
    }

    std::shared_ptr<Game::Weapon> weapon(new Game::Weapon());
    this->loadBase(weapon, asset);

    json_object* slot = json_object_object_get(asset.get(), "target_slot");
    if (slot == nullptr || json_object_get_type(slot) != json_type_string) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Parameter `target_slot' is not set");
    } else {
        std::string slotValue(json_object_get_string(slot));
        Game::Weapon::WeaponSlot targetSlot = weapon->getTargetSlot();

        if (slotValue == "primary") {
            targetSlot = Game::Weapon::WeaponSlot::SLOT_PRIMARY;
        } else if (slotValue == "secondary") {
            targetSlot = Game::Weapon::WeaponSlot::SLOT_SECONDARY;
        } else if (slotValue == "meele") {
            targetSlot = Game::Weapon::WeaponSlot::SLOT_MEELE;
        } else {
            Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING,
                    "Got unknown `target_slot' value `%s'", slotValue.c_str());
        }

        weapon->setTargetSlot(targetSlot);
    }

    json_object* maxAmmo = json_object_object_get(asset.get(), "max_ammo");
    if (maxAmmo == nullptr || json_object_get_type(maxAmmo) != json_type_int) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Parameter `max_ammo' is not set");
    } else {
        weapon->setMaxAmmo(json_object_get_int(maxAmmo));
    }

    json_object* ammo = json_object_object_get(asset.get(), "ammo");
    if (ammo == nullptr || json_object_get_type(ammo) != json_type_int) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Parameter `ammo' is not set");
    } else {
        weapon->setAmmo(json_object_get_int(ammo));
    }

    json_object* groupingAngle = json_object_object_get(asset.get(), "grouping_angle");
    if (groupingAngle == nullptr || json_object_get_type(groupingAngle) != json_type_double) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Parameter `grouping_angle' is not set");
    } else {
        weapon->setGroupingAngle(json_object_get_double(groupingAngle));
    }

    json_object* firingSpeed = json_object_object_get(asset.get(), "firing_speed");
    if (firingSpeed == nullptr || json_object_get_type(firingSpeed) != json_type_double) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Parameter `firing_speed' is not set");
    } else {
        weapon->setFiringSpeed(json_object_get_double(firingSpeed));
    }

    return weapon;
}

std::shared_ptr<Game::Widget> EntityFactory::createWidget(const std::string& name) const {
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Building Widget from `%s'", name.c_str());

    auto asset = this->resourceCache->loadAsset(name);
    if (asset == nullptr) {
        return nullptr;
    }

    std::shared_ptr<Game::Widget> widget(new Game::Widget());
    this->loadBase(widget, asset);

    widget->setCollidable(false);
    return widget;
}

std::shared_ptr<Game::ShotTrace> EntityFactory::createTrace(const Math::Vec3& from, const Math::Vec3& to) const {
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Building ShotTrace from (%.3f; %.3f) to (%.3f; %.3f)",
            from.get(Math::Vec3::X), from.get(Math::Vec3::Y), to.get(Math::Vec3::X), to.get(Math::Vec3::Y));

    auto effect = this->resourceCache->loadEffect("shaders/trace.shader");

    std::shared_ptr<Game::ShotTrace> trace(new Game::ShotTrace(from, to));
    trace->getLine()->setEffect(effect);

    return trace;
}

std::shared_ptr<Game::Label> EntityFactory::createLabel(const std::string& fontName, unsigned int size) const {
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO,
            "Building Label with `%s' (%dpt) font", fontName.c_str(), size);

    std::stringstream fontPath;
    fontPath << "fonts/" << fontName << ".ttf";
    auto font = this->resourceCache->loadFont(fontPath.str(), size);
    auto effect = this->resourceCache->loadEffect("shaders/default.shader");

    std::shared_ptr<Game::Label> label(new Game::Label());
    label->setFont(font);
    label->getSprite()->setEffect(effect);

    return label;
}

std::shared_ptr<SpriteEntity> EntityFactory::createBlock(const std::string& name) const {
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Building Block from `%s'", name.c_str());

    auto asset = this->resourceCache->loadAsset(name);
    if (asset == nullptr) {
        return nullptr;
    }

    std::shared_ptr<Game::SpriteEntity> entity(new Game::SpriteEntity());
    this->loadBase(entity, asset);

    return entity;
}

void EntityFactory::loadBase(std::shared_ptr<Game::SpriteEntity> entity,
        const std::shared_ptr<json_object> asset) const {
    json_object* texture = json_object_object_get(asset.get(), "texture");
    if (texture == nullptr || json_object_get_type(texture) != json_type_string) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Parameter `texture' is not set");
    } else {
        entity->getSprite()->setTexture(this->resourceCache->loadTexture(json_object_get_string(texture)));
    }

    json_object* effect = json_object_object_get(asset.get(), "effect");
    if (effect == nullptr || json_object_get_type(effect) != json_type_string) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Parameter `effect' is not set");
    } else {
        entity->getSprite()->setEffect(this->resourceCache->loadEffect(json_object_get_string(effect)));
    }

    json_object* visible = json_object_object_get(asset.get(), "visible");
    if (visible != nullptr && json_object_get_type(visible) == json_type_boolean) {
        entity->setVisible(json_object_get_boolean(visible));
    }

    json_object* passive = json_object_object_get(asset.get(), "passive");
    if (passive != nullptr && json_object_get_type(passive) == json_type_boolean) {
        entity->setPassive(json_object_get_boolean(passive));
    }

    json_object* collidable = json_object_object_get(asset.get(), "collidable");
    if (collidable != nullptr && json_object_get_type(collidable) == json_type_boolean) {
        entity->setCollidable(json_object_get_boolean(collidable));
    }
}

}  // namespace Game

}  // namespace PolandBall
