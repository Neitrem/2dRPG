#include "libs.h"
#ifndef GAME_H
#define GAME_H


#include "Character.h"
#include "level.h"
#include "MoveVector.h"



struct Game
{
public:
	sf::RenderWindow* getWindow();
	Level getLevel();

	void loadLevel(std::string name);
	
	void startGame();

	void initialiseWindow();
	
	Game();
	

private:
	template <typename T>
	void keyInput(sf::Vector2<T>& movementVec, std::map<sf::String, bool>* key_map, sf::Event evt, int inputDirection);

	void pollWindowIvent();

	void startRender();

	void checkColision();

	void startCollisionProcessing();

	sf::Vector2i screenSize;

	sf::RenderWindow renderWindow;

	sf::View view;

	Level level;

	Character player;
};

#endif