#include "MapElement.h"

void MapElement::setVisionHitboxSprite(HitboxSprite visionHitboxSprite)
{
	this->visionHitboxSprite = visionHitboxSprite;
}

HitboxSprite MapElement::getVisionHitboxSprite()
{
	return this->visionHitboxSprite;
}

void MapElement::setPhysicalShape(sf::RectangleShape physicalShape)
{
	this->physicalShape = physicalShape;
}

sf::RectangleShape MapElement::getPhysicalShape()
{
	return this->physicalShape;
}