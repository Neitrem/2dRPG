#include "level.h"
#include "game.h"
#include "libs.h"


int main()
{
	Game game;
	game.loadLevel("map/test_shape.xml");
	game.initialiseWindow();

	//sf::RenderWindow* win = game.getWindow();


	//int i = 0;
	//for (Layer layer : game.getLevel().layers)
	//{
	//	for (Tile tile : layer.tiles)
	//	{

	//		if (tile.isTexture)
	//		{
	//			std::cout << i << "  " << tile.sprite << std::endl;
	//			win->draw(*(tile.sprite));

	//		}


	//		/*
	//		if (layer.name == "wals")
	//		{
	//			sf::Vector2i tileSize = this->GetTileSize();
	//			sf::RectangleShape sh;
	//			sh.setSize(sf::Vector2f(32, 32));
	//			sh.setFillColor(sf::Color(200, 75, 20, 100));
	//			sh.setPosition(tile.Rect.left, tile.Rect.top);
	//			window.draw(sh);
	//		}
	//		*/
	//		i++;
	//	}
	//	std::cout << "qqqq" << std::endl;
	//}
	//while (win->isOpen())
	//{
	//	sf::Event evt;

	//	while (win->pollEvent(evt))
	//	{

	//	}
	//	game.world.Step(1.0f / 1.0f, 2, 2);
	//}

	game.startGame();
	
	//game.startMoving();


	return 0;

}

