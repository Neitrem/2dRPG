#include "libs.h"
#ifndef GAME_H
#define GAME_H


#include "player.h"



struct Game
{
public:
	

	sf::RenderWindow* getWindow();
	Level getLevel();

	void loadLevel(std::string name);
	void startRender();

	void startGame();
	void initialiseWindow();
	void pollWindowIvent();
	Game();
	

private:
	b2World world = *(new b2World(b2Vec2(0.0f, 0.0f)));
	sf::Vector2i screenSize;
	sf::RenderWindow renderWindow;
	sf::View view;
	Level level;
	Player player;
	
	
	void createPlayer(Object& player);
};

#endif