#include "TriggerRectangleShape.h"


sf::FloatRect TriggerRectangleShape::getGlobalTriggerArea() const {
	return getTransform().transformRect(this->triggerArea);
}

void TriggerRectangleShape::setTriggerArea(const sf::FloatRect& movebox) {
	this->triggerArea = movebox;
}

sf::FloatRect TriggerRectangleShape::getTriggerArea() const
{
	return this->triggerArea;
}