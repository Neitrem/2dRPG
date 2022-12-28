#include "game.h"
#include "libs.h"

Game::Game()
{
	b2BodyDef bodyDef;
	this->screenSize = sf::Vector2i(800, 600);
	bodyDef.type = b2_dynamicBody;
	Player player;
	this->player = player;
}

void Game::initialiseWindow()
{
	this->renderWindow.create(sf::VideoMode(this->screenSize.x, this->screenSize.y), "Game");
	this->renderWindow.setActive(false);
	this->renderWindow.setFramerateLimit(60);

	this->view.reset(sf::FloatRect(0.0f, 0.0f, this->screenSize.x, this->screenSize.y));
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

void keyInput(b2Vec2* velosity, std::map<sf::String, bool>* key_map, sf::Event evt, int inputDirection)
{
	if (evt.key.code == sf::Keyboard::W) {
		if (!(bool(true + inputDirection) && ((*key_map)["key_w"])))
		{
			*velosity += b2Vec2(0, -1 * inputDirection);
		}
		(*key_map)["key_w"] = bool(true + inputDirection);
	}


	if (evt.key.code == sf::Keyboard::D) {
		if (!(bool(true + inputDirection) && ((*key_map)["key_d"])))
		{
			*velosity += b2Vec2(1 * inputDirection, 0);
		}
		(*key_map)["key_d"] = bool(true + inputDirection);
	}

	if (evt.key.code == sf::Keyboard::A) {
		if (!(bool(true + inputDirection) && ((*key_map)["key_a"])))
		{
			*velosity += b2Vec2(-1 * inputDirection, 0);
		}
		(*key_map)["key_a"] = bool(true + inputDirection);
	}

	else if (evt.key.code == sf::Keyboard::S) {
		if (!(bool(true + inputDirection) && ((*key_map)["key_s"])))
		{
			*velosity += b2Vec2(0, 1 * inputDirection);
		}
		(*key_map)["key_s"] = bool(true + inputDirection);
	}
}


void Game::startGame()
{
	
	//std::thread tA(&Game::startMoving, this);
	sf::Thread thread(&Game::startRender, this);
	thread.launch();
	//std::thread tB(&Game::startRender, this);
	//tA.join();
	//tB.join();
	this->pollWindowIvent();
	//this->startRender();
}

void Game::pollWindowIvent()
{
	b2Vec2 velosity;
	velosity.Set(0, 0);

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
				keyInput(&velosity, &key_map, evt, 1);
				break;

			case sf::Event::KeyReleased:
				keyInput(&velosity, &key_map, evt, -1);
				break;

			}
		}
		this->player.move(velosity);

		this->world.Step(1.0f / 60, 10, 10);

	}
}

void Game::startRender()
{
	this->renderWindow.setActive();

	while (this->renderWindow.isOpen())
	{
		view.setCenter(this->player.rigidBody->GetPosition().x + this->screenSize.x / 4, this->player.rigidBody->GetPosition().y + this->screenSize.y / 4);
		this->renderWindow.setView(view);

		this->player.obj.sprite.setPosition(this->player.rigidBody->GetPosition().x, this->player.rigidBody->GetPosition().y);

		this->renderWindow.clear();

		this->renderWindow.draw(this->player.obj.sprite);

		this->level.Draw(&(this->renderWindow));

		

		this->renderWindow.display();
	}
	
}

void Game::createPlayer(Object& obj)
{
	b2FixtureDef fixtureDef;
	b2FixtureDef sensorFixtureDef;
	b2CircleShape shape;
	b2CircleShape sensorShape;
	b2Body* playerBody;
	b2BodyDef bodyDef;

	//b2PolygonShape boxShape;
	//boxShape.SetAsBox(1, 1);

	//fixtureDef.shape = &boxShape;

	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(obj.rect.left, obj.rect.top);
	bodyDef.fixedRotation = true;
	playerBody = this->world.CreateBody(&bodyDef);

	std::string* point = new std::string; //variable that can contains any type of data
	*point = "player";
	playerBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(point); //GetUserData returns point whith we putted in, none by default

	fixtureDef.density = 1;
	fixtureDef.friction = 0.0f;
	shape.m_radius = 6;
	shape.m_p.Set(0, 10);
	fixtureDef.shape = &shape;
	playerBody->CreateFixture(&fixtureDef);
	//playerBody->ResetMassData();
	//playerBody->SetBullet(false);
	playerBody->SetFixedRotation(true);

	this->player.setObject(obj);
	this->player.rigidBody = playerBody;
}

void Game::loadLevel(std::string name)
{
	Level lvl;
	Object player;
	
	lvl.loadFromFile(name);

	player = lvl.GetObject("player");

	this->world = world;
	this->level = lvl;
	this->createPlayer(player);

	this->level.loadPhysics(&(this->world));

}