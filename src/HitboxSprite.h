#include "libs.h"
#ifndef HITBOXSPITE_H
#define HITBOXSPITE_H


/// <summary>
/// Bases on sf::Sprite class. For any sprite with collider.
/// </summary>
class HitboxSprite : public sf::Sprite
{
public:
    bool isCollided;
    bool isUpper;

    /// sets the hitbox
    void setHitbox(const sf::FloatRect& hitbox);

    /// gets the hitbox (use this instead of getGlobalBounds())
    sf::FloatRect getGlobalHitbox() const;

    sf::FloatRect getHitbox() const;

    void setType(std::string type);
    std::string getType();

    HitboxSprite();

    /// Return true if current object Y position lesser or equal to given object Y position else return false
    bool isHigher(HitboxSprite oppositeSprite);

protected:

    /// Type of this object
    std::string m_type;
    
    /// Rectangle that is using in collision calculation
    sf::FloatRect m_hitbox;

};

#endif