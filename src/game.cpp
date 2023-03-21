#include "game.h"
#include "libs.h"

Game::Game()
{
	this->screenSize = sf::Vector2i(800, 600);
}

void Game::checkColision()
{
	std::pair<std::vector<Layer>::iterator, std::vector<Layer>::iterator> layersPtr = this->level.GetAllLayers();

	HitboxSprite* playerCollider = &(this->player.characterSprite);
	sf::FloatRect characterGlobalHitbox = playerCollider->getGlobalHitbox();

	for (auto layer_ptr = layersPtr.first; layer_ptr != layersPtr.second; ++layer_ptr)
	{
		for (auto tile_ptr = std::begin(layer_ptr->tiles); tile_ptr != std::end(layer_ptr->tiles); ++tile_ptr)
		{

			HitboxSprite tileHitboxSprite = tile_ptr->getVisionHitboxSprite();

			sf::FloatRect tileGlobalHitbox = tileHitboxSprite.getGlobalHitbox();

			const bool colliding = characterGlobalHitbox.intersects(tileGlobalHitbox);

			if (colliding)
			{
				if (!tileHitboxSprite.isCollided)
				{
					//std::cout << tile_ptr->getType() << std::endl;
					tileHitboxSprite.isCollided = true;

					tileHitboxSprite.setColor(sf::Color::Red);

					if (tileHitboxSprite.getType() != "floor")
					{
						//this->level.pushToCollidedHitboxes(*tile_ptr);

						if (playerCollider->isHigher(tileHitboxSprite))
						{
							std::cout << "upper" << std::endl;
							tileHitboxSprite.isUpper = true;
							this->level.pushToUpperLayer(*tile_ptr);
						}
					}
				}
				else
				{
					if (tileHitboxSprite.getType() != "floor")
					{
						if (!playerCollider->isHigher(tileHitboxSprite))
						{
							tileHitboxSprite.isUpper = false;
						}
					}
				}
			}
			else if(tileHitboxSprite.isCollided)
			{
				tileHitboxSprite.isUpper = false;
				tileHitboxSprite.isCollided = false;
				tileHitboxSprite.setColor(sf::Color(255, 255, 255));
			}

			tile_ptr->setVisionHitboxSprite(tileHitboxSprite);
		}
	}
}

void Game::initialiseWindow()
{
	this->renderWindow.create(sf::VideoMode(this->screenSize.x, this->screenSize.y), "Game");
	this->renderWindow.setActive(false);
	this->renderWindow.setFramerateLimit(60);

	this->view.reset(sf::FloatRect(0.0f, 0.0f, float(this->screenSize.x), float(this->screenSize.y)));
	this->view.setViewport(sf::FloatRect(0.0f, 0.0f, 2.0f, 2.0f));
	
}

sf::RenderWindow* Game::getWindow()
{
	return &(this->renderWindow);
}

Level Game::getLevel()
{
	return this->level;
}

template <typename T>
void Game::keyInput(sf::Vector2<T> &movementVec,  std::map<sf::String, bool>* key_map, sf::Event evt, int inputDirection)
{
	if (evt.key.code == sf::Keyboard::W) {
		if (!(bool(true + inputDirection) && ((*key_map)["key_w"])))
		{
			//*movementVec += MoveVector<T>(0, -1 * inputDirection);
			movementVec.y += T( - 1 * inputDirection);
		}
		(*key_map)["key_w"] = bool(true + inputDirection);
	}


	if (evt.key.code == sf::Keyboard::D) {
		if (!(bool(true + inputDirection) && ((*key_map)["key_d"])))
		{
			//*movementVec += MoveVector<T>(1 * inputDirection, 0);
			movementVec.x += T(1 * inputDirection);
		}
		(*key_map)["key_d"] = bool(true + inputDirection);
	}

	if (evt.key.code == sf::Keyboard::A) {
		if (!(bool(true + inputDirection) && ((*key_map)["key_a"])))
		{
			/**movementVec += MoveVector<T>(-1 * inputDirection, 0);*/
			movementVec.x += T(- 1 * inputDirection);
		}
		(*key_map)["key_a"] = bool(true + inputDirection);
	}

	else if (evt.key.code == sf::Keyboard::S) {
		if (!(bool(true + inputDirection) && ((*key_map)["key_s"])))
		{
			movementVec.y += T(1 * inputDirection);
		}
		(*key_map)["key_s"] = bool(true + inputDirection);
	}
}




void Game::startGame()
{
	
	//std::thread tA(&Game::startMoving, this);
	sf::Thread thread(&Game::startRender, this);
	thread.launch();

	sf::Thread threadA(&Game::startCollisionProcessing, this);
	threadA.launch();
	
	//std::thread tB(&Game::startRender, this);
	//tA.join();
	//tB.join();
	this->pollWindowIvent();
	//this->startRender();

	
}

void Game::pollWindowIvent()
{
	MoveVector movementVec;

	std::map<sf::String, bool> key_map = {
		{ "key_w", false},
		{ "key_d", false},
		{ "key_a", false},
		{ "key_s", false},
	};

	
	while (this->renderWindow.isOpen())
	{
		sf::Event evt;
		//std::cout << "evt" << std::endl;
		while (this->renderWindow.pollEvent(evt))
		{
			switch (evt.type)
			{
			case sf::Event::Closed:
				this->renderWindow.close();
				break;

			case sf::Event::KeyPressed:
				keyInput(movementVec, &key_map, evt, 1);
				break;

			case sf::Event::KeyReleased:
				keyInput(movementVec, &key_map, evt, -1);
				break;

			}
		}

		this->player.moveCharacter(movementVec);
	}
}

void Game::startCollisionProcessing()
{
	while (this->renderWindow.isOpen())
	{
		this->checkColision();
	}
}


void Game::startRender()
{
	this->renderWindow.setActive();

	while (this->renderWindow.isOpen())
	{
		//view.setCenter(this->player.rigidBody->GetPosition().x + this->screenSize.x / 4, this->player.rigidBody->GetPosition().y + this->screenSize.y / 4);
		this->renderWindow.setView(view);

		//this->player.obj.sprite.setPosition(this->player.rigidBody->GetPosition().x, this->player.rigidBody->GetPosition().y);

		this->renderWindow.clear();


		this->level.drawUncollidedTiles(&(this->renderWindow));


		this->renderWindow.draw(this->player.characterSprite);
		


		this->level.drawUpperLayer(&(this->renderWindow));

		this->level.drawPhysicsBounds(&(this->renderWindow));
		this->renderWindow.draw(this->player.getMoveBox());
		this->renderWindow.draw(this->player.getSensorBox());

		this->renderWindow.display();

		
	}
	
}



void Game::loadLevel(std::string name)
{
	Level lvl;

	lvl.loadFromFile(name);

	Character ch = lvl.getPlayer();

	this->player = ch;

	this->level = lvl;

}