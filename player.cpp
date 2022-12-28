#include "player.h"

Player::Player()
{
	this->obj = Object();
	this->speed = 1;
}


void Player::setObject(Object obj)
{
	this->obj = obj;
}

void Player::setSpeed(int speed)
{
	this->speed = speed;
}

Object* Player::getObject()
{
	return &(this->obj);
}

int Player::getSpeed()
{
	return this->speed;
}


void Player::move(b2Vec2 velosity)
{
	this->rigidBody->SetLinearVelocity(b2Vec2(velosity.x * this->speed * 0.1, velosity.y * this->speed * 0.1));

}