#include "level.h"
#include "game.h"
#include "libs.h"


int main()
{
	Game game;
	game.loadLevel("map/test_shape.xml");
	game.initialiseWindow();

	

	game.startGame();
	
	//game.startMoving();


	return 0;

}


//int main()
//{
//	sf::RenderWindow window(sf::VideoMode(256, 128), "Example");
//
//	HitboxSprite* sprite = new Character();
//	sprite->setPosition({ 64.f, 64.f });
//
//	sf::Vector2f v;
//
//
//	// create sprite texture and apply to sprites
//	sf::Texture square_texture;
//	square_texture.loadFromFile("map/32x32_wardrobe.png");
//	sprite->setTexture(square_texture);
//
//
//	v.x = 2;
//	v.y = 3;
//	v *= float(4);
//	std::cout << v.x << std::endl;
//	//Converting HitboxSprite into Character
//	
//	
//	//std::cout << "fddsd" << std::endl;
//		//Dealing with Character class obj
//}


//#include <SFML/Graphics.hpp>
//
///// custom sprite class with hitbox
//class HitboxSprite : public sf::Sprite {
//public:
//    /// sets the hitbox
//    void setHitbox(const sf::FloatRect& hitbox) {
//        m_hitbox = hitbox;
//    }
//    /// gets the hitbox (use this instead of getGlobalBounds())
//    sf::FloatRect getGlobalHitbox() const {
//        return getTransform().transformRect(m_hitbox);
//    }
//private:
//    sf::FloatRect m_hitbox;
//};
//
//int main() {
//    sf::RenderWindow window(sf::VideoMode(256, 128), "Example");
//
//    // create two sprites, player and enemy
//    HitboxSprite player;
//    player.setPosition({ 64.f, 64.f });
//    HitboxSprite enemy;
//    enemy.setPosition({ 128.f, 64.f });
//    enemy.setColor(sf::Color::Red);
//    // create sprite texture and apply to sprites
//    sf::Texture square_texture;
//    square_texture.loadFromFile("map/32x32_wardrobe.png");
//    player.setTexture(square_texture);
//    enemy.setTexture(square_texture);
//    // set custom hitboxes
//    // (this one starts (8, 8) pixels from the top left and has a size of (16, 16)
//    // (this means the hitbox will be 1/2 of the square in the middle)
//    player.setHitbox({ 8.f, 8.f, 16.f, 16.f });
//    enemy.setHitbox({ 8.f, 8.f, 16.f, 16.f });
//
//    sf::Clock clock;
//    while (window.isOpen()) {
//        // process events
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        const float dt = clock.restart().asSeconds();
//        constexpr float player_speed = 128.f;
//        // move player with arrow keys
//        player.move({
//            player_speed * dt * (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) - sf::Keyboard::isKeyPressed(sf::Keyboard::Left)),
//            player_speed * dt * (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) - sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
//            });
//
//        // check for collision
//        const bool colliding = player.getGlobalHitbox().intersects(enemy.getGlobalHitbox());
//
//        // set background color based on collision
//        window.clear(colliding ? sf::Color::Green : sf::Color::Blue);
//        // draw sprites
//        window.draw(enemy);
//        window.draw(player);
//        // display
//        window.display();
//    }
//
//    return 0;
//}