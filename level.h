#ifndef LEVEL_H
#define LEVEL_H

#include "libs.h"

struct Object
{
	std::string name;
	std::string type;
	sf::Rect<int> rect;
	sf::Sprite sprite;
	Object();
};

struct Tile
{
	bool isTexture;
	sf::Sprite* sprite;
	sf::Rect<int>* Rect;

	Tile();
};

struct Layer
{
	int opacity;
	std::string name;
	std::vector<Tile> tiles;
};

struct Tileset
{
	std::string name;
	int firstTileID, columns, rows;
	std::vector<sf::Rect<int>> subRects;
	sf::Texture *tileImage;
};

class Level
{
public:
	Tileset getTileset();

	bool loadFromFile(std::string filename);
	Object GetObject(std::string name);
	std::vector<Object> GetObjects(std::string name);
	void Draw(sf::RenderWindow* window);
	sf::Vector2i GetTileSize();
	sf::Sprite* getCurrentSprite(int tileGID);
	std::optional<Layer> GetLayer(std::string layerName);
	void loadPhysics(b2World *world);
	void InsertPLayerSprite();

	
private:
	int width, height, tileWidth, tileHeight;

	std::vector<Layer> layers;
	std::vector<b2Body*> EnemiesBOXes;
	std::vector<b2Body*> objectsBOXes;
	sf::Rect<float> drawingBounds;
	std::vector<Tileset> tileImages;
	std::vector<Object> objects;

	bool loadTilesets(rapidxml::xml_node<char>* map);
	bool loadMap(rapidxml::xml_node<char>* map);
	bool loadObjects(rapidxml::xml_node<char>* map);
	
};


#endif