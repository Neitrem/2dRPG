#ifndef MAPELEMENT_H
#define MAPELEMENT_H

#include "HitboxSprite.h"

class MapElement
{
public:

	

	/*std::map<sf::String, sf::RectangleShape> shapes_map = {
		{ "upper_shape", sf::RectangleShape()},
		{ "right_shape", sf::RectangleShape()},
		{ "bottom_shape", sf::RectangleShape()},
		{ "left_shape", sf::RectangleShape()},
	};*/

	void setVisionHitboxSprite(HitboxSprite visionHitboxSprite);
	HitboxSprite getVisionHitboxSprite();

	void setPhysicalShape(sf::RectangleShape physicalShape);
	sf::RectangleShape getPhysicalShape();

private:
	

	/// <summary>
	/// Contains HitboxSprite which is used for drawing staff
	/// </summary>
	HitboxSprite visionHitboxSprite;

	/// <summary>
	/// Contains Shape which is using for moving staff
	/// </summary>
	sf::RectangleShape physicalShape;
};

#endif
