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
#include "Config.h"
#include "Logger.h"

#include <sstream>

namespace PolandBall {

namespace Game {

std::shared_ptr<Pack> EntityFactory::createPack(const std::string& name) const {
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Building new Pack from `%s'", name.c_str());

    auto asset = this->resourceCache->loadAsset(name);
    if (asset == nullptr) {
        return nullptr;
    }

    std::shared_ptr<Game::Pack> pack(new Game::Pack());
    this->loadBase(pack, asset);

    json_object* payload = json_object_object_get(asset.get(), "payload");
    if (payload == nullptr || json_object_get_type(payload) != json_type_string) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `payload' parameter");
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
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `value' parameter");
    } else {
        pack->setValue(json_object_get_int(value));
    }

    return pack;
}

std::shared_ptr<Player> EntityFactory::createPlayer(const std::string& name) const {
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Building new Player from `%s'", name.c_str());

    auto asset = this->resourceCache->loadAsset(name);
    if (asset == nullptr) {
        return nullptr;
    }

    std::shared_ptr<Game::Player> player(new Game::Player());
    this->loadBase(player, asset);

    json_object* maxMoveSpeed = json_object_object_get(asset.get(), "max_move_speed");
    if (maxMoveSpeed == nullptr || json_object_get_type(maxMoveSpeed) != json_type_double) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `max_move_speed' parameter");
    } else {
        player->setMaxMoveSpeed(json_object_get_double(maxMoveSpeed));
    }

    json_object* maxJumpSpeed = json_object_object_get(asset.get(), "max_jump_speed");
    if (maxJumpSpeed == nullptr || json_object_get_type(maxJumpSpeed) != json_type_double) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `max_jump_speed' parameter");
    } else {
        player->setMaxJumpSpeed(json_object_get_double(maxJumpSpeed));
    }

    json_object* maxJumpTime = json_object_object_get(asset.get(), "max_jump_time");
    if (maxJumpTime == nullptr || json_object_get_type(maxJumpTime) != json_type_double) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `max_jump_time' parameter");
    } else {
        player->setMaxJumpTime(json_object_get_double(maxJumpTime));
    }

    json_object* maxHealth = json_object_object_get(asset.get(), "max_health");
    if (maxHealth == nullptr || json_object_get_type(maxHealth) != json_type_int) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `max_health' parameter");
    } else {
        player->setMaxHealth(json_object_get_int(maxHealth));
    }

    json_object* maxArmor = json_object_object_get(asset.get(), "max_armor");
    if (maxArmor == nullptr || json_object_get_type(maxArmor) != json_type_int) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `max_armor' parameter");
    } else {
        player->setMaxArmor(json_object_get_int(maxArmor));
    }

    json_object* health = json_object_object_get(asset.get(), "health");
    if (health == nullptr || json_object_get_type(health) != json_type_int) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `health' parameter");
    } else {
        player->setHealth(json_object_get_int(health));
    }

    json_object* armor = json_object_object_get(asset.get(), "armor");
    if (armor == nullptr || json_object_get_type(armor) != json_type_int) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `armor' parameter");
    } else {
        player->setArmor(json_object_get_int(armor));
    }

    return player;
}

std::shared_ptr<Weapon> EntityFactory::createWeapon(const std::string& name) const {
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Building new Weapon from `%s'", name.c_str());

    auto asset = this->resourceCache->loadAsset(name);
    if (asset == nullptr) {
        return nullptr;
    }

    std::shared_ptr<Game::Weapon> weapon(new Game::Weapon());
    this->loadBase(weapon, asset);

    json_object* slot = json_object_object_get(asset.get(), "target_slot");
    if (slot == nullptr || json_object_get_type(slot) != json_type_string) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `target_slot' parameter");
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
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `max_ammo' parameter");
    } else {
        weapon->setMaxAmmo(json_object_get_int(maxAmmo));
    }

    json_object* ammo = json_object_object_get(asset.get(), "ammo");
    if (ammo == nullptr || json_object_get_type(ammo) != json_type_int) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `ammo' parameter");
    } else {
        weapon->setAmmo(json_object_get_int(ammo));
    }

    json_object* groupingAngle = json_object_object_get(asset.get(), "grouping_angle");
    if (groupingAngle == nullptr || json_object_get_type(groupingAngle) != json_type_double) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `grouping_angle' parameter");
    } else {
        weapon->setGroupingAngle(json_object_get_double(groupingAngle));
    }

    json_object* firingSpeed = json_object_object_get(asset.get(), "firing_speed");
    if (firingSpeed == nullptr || json_object_get_type(firingSpeed) != json_type_double) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `firing_speed' parameter");
    } else {
        weapon->setFiringSpeed(json_object_get_double(firingSpeed));
    }

    return weapon;
}

std::shared_ptr<Game::Widget> EntityFactory::createWidget(const std::string& name) const {
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Building new Widget from `%s'", name.c_str());

    auto asset = this->resourceCache->loadAsset(name);
    if (asset == nullptr) {
        return nullptr;
    }

    std::shared_ptr<Game::Widget> widget(new Game::Widget());
    this->loadBase(widget, asset);

    widget->setCollidable(false);
    return widget;
}

std::shared_ptr<Game::Label> EntityFactory::createLabel(const std::string& fontName, unsigned int size) const {
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO,
            "Building new Label from `%s' (%dpt)", fontName.c_str(), size);

    std::stringstream fontPath;
    fontPath << POLANDBALL_DATADIR << "/fonts/" << fontName << ".ttf";
    auto font = this->resourceCache->loadFont(fontPath.str(), size);

    std::stringstream effectPath;
    effectPath << POLANDBALL_DATADIR << "/shaders/default.shader";
    auto effect = this->resourceCache->loadEffect(effectPath.str());

    std::shared_ptr<Game::Label> label(new Game::Label());
    label->setFont(font);
    label->getSprite()->setEffect(effect);

    return label;
}

std::shared_ptr<SpriteEntity> EntityFactory::createBlock(const std::string& name) const {
    Utils::Logger::getInstance().log(Utils::Logger::LOG_INFO, "Building new Block from `%s'", name.c_str());

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
    json_object* visible = json_object_object_get(asset.get(), "visible");
    if (visible == nullptr || json_object_get_type(visible) != json_type_boolean) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `visible' parameter");
    } else {
        entity->setVisible(json_object_get_boolean(visible));
    }

    json_object* passive = json_object_object_get(asset.get(), "passive");
    if (passive == nullptr || json_object_get_type(passive) != json_type_boolean) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `passive' parameter");
    } else {
        entity->setPassive(json_object_get_boolean(passive));
    }

    json_object* collidable = json_object_object_get(asset.get(), "collidable");
    if (collidable == nullptr || json_object_get_type(collidable) != json_type_boolean) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `collidable' parameter");
    } else {
        entity->setCollidable(json_object_get_boolean(collidable));
    }

    json_object* texture = json_object_object_get(asset.get(), "texture");
    if (texture == nullptr || json_object_get_type(texture) != json_type_string) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `texture' parameter");
    } else {
        std::stringstream texturePath;
        texturePath << POLANDBALL_DATADIR << "/" << json_object_get_string(texture);
        entity->getSprite()->setTexture(this->resourceCache->loadTexture(texturePath.str()));
    }

    json_object* effect = json_object_object_get(asset.get(), "effect");
    if (effect == nullptr || json_object_get_type(effect) != json_type_string) {
        Utils::Logger::getInstance().log(Utils::Logger::LOG_WARNING, "Failed to read `effect' parameter");
    } else {
        std::stringstream effectPath;
        effectPath << POLANDBALL_DATADIR << "/" << json_object_get_string(effect);
        entity->getSprite()->setEffect(this->resourceCache->loadEffect(effectPath.str()));
    }
}

}  // namespace Game

}  // namespace PolandBall
