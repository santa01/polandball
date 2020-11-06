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

#include <PolandBall.h>
#include <Sprite.h>
#include <EntityFactory.h>
#include <Vec3.h>
#include <Vec4.h>
#include <Mat4.h>
#include <sstream>

namespace PolandBall {

void PolandBall::onMouseMotion(int x, int y) {
    Math::Mat4 ndc;
    ndc.set(0, 0, 2.0f / (this->width / 1.0f));
    ndc.set(0, 3, -1.0f);
    ndc.set(1, 1, -2.0f / (this->height / 1.0f));
    ndc.set(1, 3, 1.0f);

    Math::Mat4 world(this->scene->getCamera().getProjection());
    world.invert();

    Math::Vec4 cursorPosition = (world * ndc) * Math::Vec4(x, y, 0.0f, 1.0f);
    this->cursor->setOrigin(cursorPosition.extractVec3());
    this->player->aimAt(cursorPosition.extractVec3());
}

void PolandBall::onMouseButton(Graphene::MouseButton button, bool state) {
    if (button == Graphene::MouseButton::BUTTON_LEFT && state) {
        this->player->shoot();
    }
}

void PolandBall::onSetup() {
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(0.5f);

    this->setupScene();
    this->setupUI();
}

void PolandBall::onIdle() {
    this->updateScene();
    this->updateUI();
}

bool PolandBall::setupScene() {
    this->scene = std::shared_ptr<Game::Scene>(new Game::Scene());

    Game::Camera& camera = this->scene->getCamera();
    camera.setProjectionType(Game::Camera::TYPE_ORTHOGRAPHIC);
    camera.setAspectRatio(this->width / (this->height / 1.0f));
    camera.setNearPlane(-2.5f);
    camera.setFarPlane(2.5f);

    //-----------------
    auto backgroundEntity = Game::EntityFactory::getInstance().createBlock("assets/backgrounds/sunny.asset");
    if (backgroundEntity == nullptr) {
        return false;
    }

    backgroundEntity->scale(10.0f);
    backgroundEntity->scaleX(camera.getAspectRatio());  // Scale for aspect ratio
    this->scene->addEntity(backgroundEntity);

    //-----------------
    auto bricksEntity = Game::EntityFactory::getInstance().createBlock("assets/blocks/kazakhstan.asset");
    if (bricksEntity == nullptr) {
        return false;
    }

    bricksEntity->setPosition(0.0f, -2.0f, 0.0f);
    bricksEntity->scaleX(20.0f * 1.5f);  // Scale for aspect ratio
    bricksEntity->getSprite()->replicateX(20.0f);
    this->scene->addEntity(bricksEntity);

    //-----------------
    bricksEntity = Game::EntityFactory::getInstance().createBlock("assets/blocks/kazakhstan.asset");
    if (bricksEntity == nullptr) {
        return false;
    }

    bricksEntity->setPosition(4.0f, 1.0f, 0.0f);
    bricksEntity->scaleX(1.5f);  // Scale for aspect ratio
    this->scene->addEntity(bricksEntity);

    //-----------------
    this->player = Game::EntityFactory::getInstance().createPlayer("assets/players/turkey.asset");
    if (this->player == nullptr) {
        return false;
    }

    this->scene->addEntity(this->player);

    //-----------------
    auto pack_primary_ammo = Game::EntityFactory::getInstance().createPack("assets/items/pack_primary_ammo.asset");
    if (pack_primary_ammo == nullptr) {
        return false;
    }

    pack_primary_ammo->setPosition(-8.0f, 0.0f, 0.0f);
    this->scene->addEntity(pack_primary_ammo);

    //-----------------
    auto pack_secondary_ammo = Game::EntityFactory::getInstance().createPack("assets/items/pack_secondary_ammo.asset");
    if (pack_secondary_ammo == nullptr) {
        return false;
    }

    pack_secondary_ammo->setPosition(-6.0f, 0.0f, 0.0f);
    this->scene->addEntity(pack_secondary_ammo);

    //-----------------
    auto m4a1 = Game::EntityFactory::getInstance().createWeapon("assets/weapons/m4a1.asset");
    if (m4a1 == nullptr) {
        return false;
    }

    m4a1->setPosition(-4.0f, 0.0f, 0.0f);
    this->scene->addEntity(m4a1);

    //-----------------
    auto ak74 = Game::EntityFactory::getInstance().createWeapon("assets/weapons/ak74.asset");
    if (ak74 == nullptr) {
        return false;
    }

    ak74->setPosition(-2.0f, 0.0f, 0.0f);
    this->scene->addEntity(ak74);

    //-----------------
    auto m1911 = Game::EntityFactory::getInstance().createWeapon("assets/weapons/m1911.asset");
    if (m1911 == nullptr) {
        return false;
    }

    m1911->setPosition(2.0f, 0.0f, 0.0f);
    this->scene->addEntity(m1911);

    //-----------------
    auto beretta92 = Game::EntityFactory::getInstance().createWeapon("assets/weapons/beretta92.asset");
    if (beretta92 == nullptr) {
        return false;
    }

    beretta92->setPosition(4.0f, 0.0f, 0.0f);
    this->scene->addEntity(beretta92);

    //-----------------
    auto wrench = Game::EntityFactory::getInstance().createWeapon("assets/weapons/wrench.asset");
    if (wrench == nullptr) {
        return false;
    }

    wrench->setPosition(6.0f, 0.0f, 0.0f);
    this->scene->addEntity(wrench);

    //-----------------
    auto knife = Game::EntityFactory::getInstance().createWeapon("assets/weapons/knife.asset");
    if (knife == nullptr) {
        return false;
    }

    knife->setPosition(8.0f, 0.0f, 0.0f);
    this->scene->addEntity(knife);

    //-----------------
    auto pack_health = Game::EntityFactory::getInstance().createPack("assets/items/pack_health.asset");
    if (pack_health == nullptr) {
        return false;
    }

    pack_health->setPosition(10.0f, 0.0f, 0.0f);
    this->scene->addEntity(pack_health);

    //-----------------
    auto pack_armor = Game::EntityFactory::getInstance().createPack("assets/items/pack_armor.asset");
    if (pack_armor == nullptr) {
        return false;
    }

    pack_armor->setPosition(12.0f, 0.0f, 0.0f);
    this->scene->addEntity(pack_armor);

    //-----------------
    this->player->positionChanged.connect(
            std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
            backgroundEntity, std::placeholders::_1));
    this->player->positionChanged.connect(
            std::bind(static_cast<void(Game::Camera::*)(const Math::Vec3&)>(&Game::Camera::setPosition),
            &camera, std::placeholders::_1));

    return true;
}

bool PolandBall::setupUI() {
    Math::Mat4 ndc;
    ndc.set(0, 0, 2.0f / (this->width / 1.0f));
    ndc.set(0, 3, -1.0f);
    ndc.set(1, 1, -2.0f / (this->height / 1.0f));
    ndc.set(1, 3, 1.0f);

    Math::Mat4 world(this->scene->getCamera().getProjection());
    world.invert();

    //-----------------
    this->emptySlot = Game::EntityFactory::getInstance().createWidget("assets/ui/slot_empty.asset");
    if (this->emptySlot == nullptr) {
        return false;
    }

    //-----------------
    auto& primaryWeapon = this->weapons[Game::Weapon::WeaponSlot::SLOT_PRIMARY];

    primaryWeapon.first = Game::EntityFactory::getInstance().createWidget("assets/ui/slot_empty.asset");
    Math::Vec4 primaryWeaponOrigin = (world * ndc) * Math::Vec4(40.0f, 40.0f, 0.0f, 1.0f);
    primaryWeapon.first->setOrigin(primaryWeaponOrigin.extractVec3());
    this->scene->addEntity(primaryWeapon.first);

    auto primaryWeaponBorder = Game::EntityFactory::getInstance().createWidget("assets/ui/border_weapon.asset");
    if (primaryWeaponBorder == nullptr) {
        return false;
    }

    primaryWeaponBorder->setOrigin(primaryWeaponOrigin.extractVec3());
    this->scene->addEntity(primaryWeaponBorder);

    primaryWeapon.second = Game::EntityFactory::getInstance().createLabel("dejavu-sans", 14);
    Math::Vec4 primaryLabelOrigin = (world * ndc) * Math::Vec4(40.0f, 80.0f, 0.0f, 1.0f);
    primaryWeapon.second->setOrigin(primaryLabelOrigin.extractVec3());
    this->scene->addEntity(primaryWeapon.second);

    //-----------------
    auto& secondaryWeapon = this->weapons[Game::Weapon::WeaponSlot::SLOT_SECONDARY];

    secondaryWeapon.first = Game::EntityFactory::getInstance().createWidget("assets/ui/slot_empty.asset");
    Math::Vec4 secondaryWeaponOrigin = (world * ndc) * Math::Vec4(110.0f, 40.0f, 0.0f, 1.0f);
    secondaryWeapon.first->setOrigin(secondaryWeaponOrigin.extractVec3());
    this->scene->addEntity(secondaryWeapon.first);

    auto secondaryWeaponBorder = Game::EntityFactory::getInstance().createWidget("assets/ui/border_weapon.asset");
    if (secondaryWeaponBorder == nullptr) {
        return false;
    }

    secondaryWeaponBorder->setOrigin(secondaryWeaponOrigin.extractVec3());
    this->scene->addEntity(secondaryWeaponBorder);

    secondaryWeapon.second = Game::EntityFactory::getInstance().createLabel("dejavu-sans", 14);
    Math::Vec4 secondaryLabelOrigin = (world * ndc) * Math::Vec4(110.0f, 80.0f, 0.0f, 1.0f);
    secondaryWeapon.second->setOrigin(secondaryLabelOrigin.extractVec3());
    this->scene->addEntity(secondaryWeapon.second);

    //-----------------
    auto& meeleWeapon = this->weapons[Game::Weapon::WeaponSlot::SLOT_MEELE];

    meeleWeapon.first = Game::EntityFactory::getInstance().createWidget("assets/ui/slot_empty.asset");
    Math::Vec4 meeleWeaponOrigin = (world * ndc) * Math::Vec4(180.0f, 40.0f, 0.0f, 1.0f);
    meeleWeapon.first->setOrigin(meeleWeaponOrigin.extractVec3());
    this->scene->addEntity(meeleWeapon.first);

    auto meeleWeaponBorder = Game::EntityFactory::getInstance().createWidget("assets/ui/border_weapon.asset");
    if (meeleWeaponBorder == nullptr) {
        return false;
    }

    meeleWeaponBorder->setOrigin(meeleWeaponOrigin.extractVec3());
    this->scene->addEntity(meeleWeaponBorder);

    meeleWeapon.second = Game::EntityFactory::getInstance().createLabel("dejavu-sans", 14);
    Math::Vec4 meeleLabelOrigin = (world * ndc) * Math::Vec4(180.0f, 80.0f, 0.0f, 1.0f);
    meeleWeapon.second->setOrigin(meeleLabelOrigin.extractVec3());
    this->scene->addEntity(meeleWeapon.second);

    //-----------------
    auto armorShield = Game::EntityFactory::getInstance().createWidget("assets/ui/shield_armor.asset");
    if (armorShield == nullptr) {
        return false;
    }

    Math::Vec4 armorShieldOrigin = (world * ndc) * Math::Vec4(Math::Vec3(this->width - 40.0f, 40.0f, 0.0f), 1.0f);
    armorShield->setOrigin(armorShieldOrigin.extractVec3());
    this->scene->addEntity(armorShield);

    this->armor = Game::EntityFactory::getInstance().createLabel("dejavu-sans", 14);
    Math::Vec4 armorLabelOrigin = (world * ndc) * Math::Vec4(Math::Vec3(this->width - 40.0f, 80.0f, 0.0f), 1.0f);
    this->armor->setOrigin(armorLabelOrigin.extractVec3());
    this->scene->addEntity(this->armor);

    //-----------------
    auto healthShield = Game::EntityFactory::getInstance().createWidget("assets/ui/shield_health.asset");
    if (healthShield == nullptr) {
        return false;
    }

    Math::Vec4 healthShieldOrigin = (world * ndc) * Math::Vec4(Math::Vec3(this->width - 100.0f, 40.0f, 0.0f), 1.0f);
    healthShield->setOrigin(healthShieldOrigin.extractVec3());
    this->scene->addEntity(healthShield);

    this->health = Game::EntityFactory::getInstance().createLabel("dejavu-sans", 14);
    Math::Vec4 healthLabelOrigin = (world * ndc) * Math::Vec4(Math::Vec3(this->width - 100.0f, 80.0f, 0.0f), 1.0f);
    this->health->setOrigin(healthLabelOrigin.extractVec3());
    this->scene->addEntity(this->health);

    //-----------------
    Game::Camera& camera = this->scene->getCamera();

    for (auto& weapon: this->weapons) {
        camera.positionChanged.connect(
            std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
            weapon.first, std::placeholders::_1));
        camera.positionChanged.connect(
            std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
            weapon.second, std::placeholders::_1));
    }

    camera.positionChanged.connect(
        std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
        primaryWeaponBorder, std::placeholders::_1));
    camera.positionChanged.connect(
        std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
        secondaryWeaponBorder, std::placeholders::_1));
    camera.positionChanged.connect(
        std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
        meeleWeaponBorder, std::placeholders::_1));

    //-----------------
    camera.positionChanged.connect(
        std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
        healthShield, std::placeholders::_1));
    camera.positionChanged.connect(
        std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
        armorShield, std::placeholders::_1));

    camera.positionChanged.connect(
        std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
        this->health, std::placeholders::_1));
    camera.positionChanged.connect(
        std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
        this->armor, std::placeholders::_1));

    //-----------------
    this->cursor = Game::EntityFactory::getInstance().createWidget("assets/ui/cursor_aim.asset");
    if (this->cursor == nullptr) {
        return false;
    }

    this->scene->addEntity(this->cursor);

    //-----------------
    this->player->positionChanged.connect(
        std::bind(static_cast<void(Game::Entity::*)(const Math::Vec3&)>(&Game::Entity::setPosition),
        this->cursor, std::placeholders::_1));

    return true;
}

void PolandBall::updateScene() {
    const Graphene::KeyboardState& keyboardState = this->getWindow()->getKeyboardState();

    if (keyboardState[Graphene::KeyboardKey::KEY_ESCAPE]) {
        this->exit(0);
    }
    if (keyboardState[Graphene::KeyboardKey::KEY_RIGHT]) {
        this->player->setState(Game::Player::PlayerState::STATE_RIGHT_STEP);
    }
    if (keyboardState[Graphene::KeyboardKey::KEY_LEFT]) {
        this->player->setState(Game::Player::PlayerState::STATE_LEFT_STEP);
    }
    if (keyboardState[Graphene::KeyboardKey::KEY_UP]) {
        this->player->setState(Game::Player::PlayerState::STATE_JUMP);
    }
    if (keyboardState[Graphene::KeyboardKey::KEY_ENTER]) {
        this->player->setState(Game::Player::PlayerState::STATE_DROP_WEAPON);
    }
    if (keyboardState[Graphene::KeyboardKey::KEY_1]) {
        this->player->activateSlot(Game::Weapon::WeaponSlot::SLOT_PRIMARY);
    }
    if (keyboardState[Graphene::KeyboardKey::KEY_2]) {
        this->player->activateSlot(Game::Weapon::WeaponSlot::SLOT_SECONDARY);
    }
    if (keyboardState[Graphene::KeyboardKey::KEY_3]) {
        this->player->activateSlot(Game::Weapon::WeaponSlot::SLOT_MEELE);
    }

    const Graphene::MouseState& mouseState = this->getWindow()->getMouseState();

    if (mouseState[Graphene::MouseButton::BUTTON_LEFT]) {
        this->player->shoot();
    }
}

void PolandBall::updateUI() {
    std::stringstream text;
    int weaponAmmo = 0;

    for (int slot = 0; slot < 3; slot++) {
        auto& weapon = this->player->getWeapon(static_cast<Game::Weapon::WeaponSlot>(slot));
        auto weaponSprite = std::dynamic_pointer_cast<Opengl::Sprite>(this->weapons[slot].first->getPrimitive());

        if (weapon != nullptr) {
            weaponAmmo = weapon->getAmmo();
            text.str("");

            if (weaponAmmo >= 999) {
                text << "--/--";  // Infinite
            } else {
                text << weaponAmmo;
            }

            auto playerWeaponSprite = std::dynamic_pointer_cast<Opengl::Sprite>(weapon->getPrimitive());
            weaponSprite->setTexture(playerWeaponSprite->getTexture());
            weaponSprite->shearX(0.0f, 2);
            this->weapons[slot].first->roll(45.0f);
            this->weapons[slot].second->setText(text.str());
        } else {
            auto emptySprite = std::dynamic_pointer_cast<Opengl::Sprite>(this->emptySlot->getPrimitive());
            weaponSprite->setTexture(emptySprite->getTexture());
            this->weapons[slot].second->clear();
        }
    }

    text.str("");
    text << this->player->getHealth();
    this->health->setText(text.str());

    text.str("");
    text << this->player->getArmor();
    this->armor->setText(text.str());

    this->scene->update(this->frameTime, this->frameStep);
    this->scene->render();
}

}  // namespace PolandBall
