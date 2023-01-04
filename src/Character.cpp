#include "Character.h"

Character::Character()
{
	this->speed = 1;
}


sf::RectangleShape Character::getSensorBox()
{
	return this->sensorBox;
}

void Character::setSensorBox(sf::RectangleShape &source)
{
	this->sensorBox = source;
}

sf::RectangleShape Character::getMoveBox()
{
	return this->moveBox;
}

void Character::setMoveBox(sf::RectangleShape &source)
{
	this->moveBox = source;
}

void Character::setSpeed(int speed)
{
	this->speed = speed;
}


int Character::getSpeed()
{
	return this->speed;
}

void Character::moveCharacter(MoveVector moveVector)
{
	this->characterSprite.moveSprite(moveVector, this->speed);
	this->moveBox.setPosition(this->characterSprite.getPosition().x + 6, this->characterSprite.getPosition().y + 22);

	this->sensorBox.setPosition(this->characterSprite.getPosition().x - 16, this->characterSprite.getPosition().y - 16);
}