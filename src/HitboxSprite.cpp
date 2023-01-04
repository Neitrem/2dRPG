#include "HitboxSprite.h"

HitboxSprite::HitboxSprite()
{
    this->isCollided = false;
    this->isUpper = false;

}

sf::FloatRect HitboxSprite::getGlobalHitbox() const {
    return this->getTransform().transformRect(m_hitbox);
}

void HitboxSprite::setHitbox(const sf::FloatRect& hitbox) {
    m_hitbox = hitbox;
}

sf::FloatRect HitboxSprite::getHitbox() const
{
    return this->m_hitbox;
}

void HitboxSprite::setType(std::string type)
{
    this->m_type = type;
}

std::string HitboxSprite::getType()
{
    return this->m_type;
}

bool HitboxSprite::isHigher(HitboxSprite oppositeSprite)
{
    return this->getPosition().y <= oppositeSprite.getPosition().y;
}