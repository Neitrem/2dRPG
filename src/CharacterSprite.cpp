#include "CharacterSprite.h"	


void CharacterSprite::generateFromHitboxSprite(HitboxSprite hs)
{
	this->setTexture(*(hs.getTexture()));
	this->setTextureRect(hs.getTextureRect());

	this->setPosition(hs.getPosition().x, hs.getPosition().x);
	this->setHitbox(hs.getHitbox());

	this->setType(hs.getType());

}


void CharacterSprite::moveSprite(MoveVector movementVec, int speed)
{
	this->move(movementVec.normalize() * float(speed) * 0.0015f);
}