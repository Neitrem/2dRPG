#ifndef LEVEL_H
#define LEVEL_H

#include "libs.h"

#include "Character.h"

struct Layer
{
	int opacity;
	std::string name;
	std::vector<HitboxSprite> tiles;
};
	
struct Tileset
{
	std::string name;
	int firstTileID, columns, rows;
	std::vector<sf::Rect<int>> subRects;
	sf::Texture* tileImage;
};

class Level
{
public:
	Tileset getTileset();

	bool loadFromFile(std::string filename);

	void drawUncollidedTiles(sf::RenderWindow* window);
	void drawUpperLayer(sf::RenderWindow* window);
	sf::Vector2i GetTileSize();

	HitboxSprite getCurrentSprite(int tileGID);
	//std::pair<sf::Image, sf::Rect<int>> getTexture(int tileGID);

	//sf::Texture* getTexture(int tileGID);


	std::optional<Layer> GetLayer(std::string layerName);
	std::pair<std::vector<Layer>::iterator, std::vector<Layer>::iterator> GetAllLayers();

	void pushToCollidedHitboxes(HitboxSprite& collidedHitbox);
	//void popFromCollidedHitboxes(std::vector <HitboxSprite>);

	void pushToUpperLayer(HitboxSprite& collidedHitBox);

	Character getPlayer();

	
private:
	int width, height, tileWidth, tileHeight;

	std::vector<Layer> layers;

	std::vector<Tileset> tilesets;

	std::vector<Character> dynamicSprites;

	std::vector<HitboxSprite*> collidedHitboxes;

	std::vector<HitboxSprite*> upperLayer;

	bool loadTilesets(rapidxml::xml_node<char>* map);
	bool loadMap(rapidxml::xml_node<char>* map);
	bool loadObjects(rapidxml::xml_node<char>* map);
	
};


#endif