#ifndef CHARACTER_H
#define CHARACTER_H

#include "libs.h"
#include "CharacterSprite.h"

class Character
{
public:
	CharacterSprite characterSprite;

	void moveCharacter(MoveVector movementVec);

	int getSpeed();
	void setSpeed(int speed);


	sf::RectangleShape getSensorBox();
	void setSensorBox(sf::RectangleShape &source);

	sf::RectangleShape getMoveBox();
	void setMoveBox(sf::RectangleShape &source);

	Character();

private:
	sf::RectangleShape sensorBox;

	sf::RectangleShape moveBox;

	int speed;
};

#endif