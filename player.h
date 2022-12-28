#ifndef PLAYER_H
#define PLAYER_H

#include "level.h"
#include "libs.h"

class Player
{
public:
	b2Body* rigidBody;
	Object obj;
	
	void setSpeed(int speed);
	void setObject(Object obj);
	Object* getObject();
	int getSpeed();

	void move(b2Vec2 velosity);
	
	
	Player();
	


protected:
	int speed;
	
};

#endif