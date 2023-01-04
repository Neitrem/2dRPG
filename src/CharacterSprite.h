#ifndef CHARACTERSPRITE_H
#define CHARACTERSPRITE_H

#include "HitboxSprite.h"
#include "MoveVector.h"

/// <summary>
/// Class based on HitboxSprite is used for objects that able to move
/// </summary>
class CharacterSprite : public HitboxSprite
{
public:
	///Change sprite position based on MoveVector class object and it's speed
	void moveSprite(MoveVector movementVec, int speed);
	
	///create new object based on HitboxSprite class object
	void fromHitboxSprite(HitboxSprite hs);
	
	//CharacterSprite();
protected:

};

#endif