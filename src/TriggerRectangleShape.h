#include "libs.h"
#ifndef TRIGGERRECTANGLESHAPE_H
#define TRIGGERRECTANGLESHAPE_H

class TriggerRectangleShape : public sf::RectangleShape
{
public:
	sf::FloatRect getTriggerArea() const;

	sf::FloatRect getGlobalTriggerArea() const;

	void setTriggerArea(const sf::FloatRect& movebox);
private:
	sf::FloatRect triggerArea;
};

#endif