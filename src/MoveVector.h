#ifndef MOVEVECTOR_H
#define MOVEVECTOR_H
#include "libs.h"

class MoveVector : public sf::Vector2<float>
{
public:
	MoveVector normalize();
	float length();
};

#endif