#include "level.h"
#include "libs.h"



// Tile implementation
Tile::Tile()
{
	isTexture = false;
	sprite = nullptr;
	Rect = nullptr;
}

// Object imlementation
Object::Object()
{
	this->name = "";
	this->type = "";
	this->sprite = sf::Sprite();
	this->rect = sf::Rect(0 ,0, 0, 0);
}




//Level implementation

Tileset Level::getTileset()
{
	return this->tileImages[0];
}


sf::Sprite* Level::getCurrentSprite(int tileGID)
{
	Tileset tempTileSet;
	int subRectToUse = -1, firstTileID = 0;
	
	for (Tileset tile_set : this->tileImages)
	{
		if (tile_set.firstTileID <= tileGID && tile_set.firstTileID > firstTileID)
		{
			firstTileID = tile_set.firstTileID;
			tempTileSet = tile_set;
		}
	}

	subRectToUse = tileGID - tempTileSet.firstTileID;
	
	if (subRectToUse >= 0)
	{
		sf::Sprite* sprite;
		sprite = new sf::Sprite();
		sprite->setTexture(*tempTileSet.tileImage);
		sprite->setTextureRect(tempTileSet.subRects[subRectToUse]);


		return sprite;
		
	}
	else {
		std::cout << "Smthg wrong for tile with GID: " << tileGID << std::endl;
		return nullptr;
	}
	
}
/// <summary>
/// Loads static elements of level like walls, doors and etc
/// </summary>
/// <returns>bool</returns>
bool Level::loadMap(rapidxml::xml_node<char>* map)
{
	std::cout << "Loading map..." << std::endl;

	rapidxml::xml_node<char>* layerElement;
	layerElement = map->first_node("layer");

	if (layerElement == NULL) return false;

	while (layerElement)
	{
		Layer layer;

		layer.opacity = 255;
		layer.name = layerElement->first_attribute("name")->value();

		rapidxml::xml_node<char>* layerDataElement;
		layerDataElement = layerElement->first_node("data");

		std::stringstream dataLine;
		std::string cell;

		dataLine << layerDataElement->value();

		int x = 0;
		int y = 0;

		while (std::getline(dataLine, cell, ','))
		{
			int spriteGID = atoi(cell.c_str());
			Tile tile;

			if (spriteGID > 0)
			{

				sf::Sprite* sprite = getCurrentSprite(spriteGID);
				if (&sprite != NULL)
				{

					sprite->setPosition(x * tileWidth, y * tileHeight);
					sprite->setColor(sf::Color(255, 255, 255, layer.opacity));

					sf::Rect<int>* tileRect = new sf::Rect<int>;
					tileRect->left = x * tileWidth;
					tileRect->top = y * tileHeight;
					tileRect->height = tileHeight;
					tileRect->width = tileWidth;


					tile.isTexture = true;
					tile.Rect = tileRect;
					tile.sprite = sprite;
				}
			}
			layer.tiles.push_back(tile);
			x++;
			if (x >= width)
			{
				x = 0;
				y++;
				if (y >= height)
					y = 0;
			}

		}
		this->layers.push_back(layer);

		layerElement = layerElement->next_sibling("layer");
	}

	return true;
}

/// <summary>
/// Loads all sprites for this map
/// </summary>
/// <returns>bool</returns>
bool Level::loadTilesets(rapidxml::xml_node<char>* map)
{
	rapidxml::xml_node<char>* tilesetElement;

	int firstTileID, columns, rows;
	std::string imagepath;

	std::cout << "Loading tilesets..." << std::endl;

	tilesetElement = map->first_node("tileset");

	while (tilesetElement)
	{
		imagepath = tilesetElement->first_attribute("source")->value();
		firstTileID = atoi(tilesetElement->first_attribute("firstgid")->value());

		rsize_t sp;
		std::string sub_str = ".tsx";


		sp = imagepath.find(sub_str);
		imagepath.erase(sp, sub_str.size());

		imagepath += ".png";

		sf::Image img;
		if (!img.loadFromFile("map/" + imagepath))
		{
			std::cout << "Failed to load tile sheet. (" << imagepath << ")" << std::endl;
			return false;
		}
		img.createMaskFromColor(sf::Color(109, 159, 185));

		sf::Texture* tempTexture;
		tempTexture = new sf::Texture;
		tempTexture->loadFromImage(img);
		tempTexture->setSmooth(false);

		columns = tempTexture->getSize().x / tileWidth;
		rows = tempTexture->getSize().y / tileHeight;

		std::vector<sf::Rect<int>> subRects;



		for (int y = 0; y < rows; y++)
			for (int x = 0; x < columns; x++)
			{
				sf::Rect<int> rect;

				rect.top = y * tileHeight;
				rect.height = tileHeight;
				rect.left = x * tileWidth;
				rect.width = tileWidth;

				subRects.push_back(rect);
			}



		Tileset tileset;

		tileset.firstTileID = firstTileID;
		tileset.tileImage = tempTexture;
		tileset.name = imagepath;
		tileset.rows = rows;
		tileset.columns = columns;
		tileset.subRects = subRects;


		this->tileImages.push_back(tileset);

		/*
		std::cout << tileset.name << std::endl;
		std::cout << tileset.rows << std::endl;
		std::cout << tileset.columns << std::endl;
		*/
		map->remove_first_node();

		tilesetElement = map->first_node("tileset");
	}
	return true;
}

bool Level::loadObjects(rapidxml::xml_node<char>* map)
{
	std::cout << "Loading objects..." << std::endl;

	rapidxml::xml_node<char>* objectGroupElement;
	objectGroupElement = map->first_node("objectgroup");

	if (objectGroupElement != NULL)
	{
		while (objectGroupElement)
		{
			rapidxml::xml_node<char>* objectElement;
			objectElement = objectGroupElement->first_node("object");

			while (objectElement)
			{
				std::string objectType;
				if (objectElement->first_attribute("type") != NULL)
				{
					objectType = objectElement->first_attribute("type")->value();
					std::transform(objectType.begin(), objectType.end(), objectType.begin(), tolower);
				}

				std::string objectName;
				if (objectElement->first_attribute("name") != NULL)
				{
					objectName = objectElement->first_attribute("name")->value();
					std::transform(objectName.begin(), objectName.end(), objectName.begin(), tolower);
				}

				int objPos_x = atoi(objectElement->first_attribute("x")->value());
				int objPos_y = atoi(objectElement->first_attribute("y")->value());

				int objWidth, objHeight;

				objWidth = atoi(objectElement->first_attribute("width")->value());
				objHeight = atoi(objectElement->first_attribute("height")->value());

				int spriteGID = atoi(objectElement->first_attribute("gid")->value());

				sf::Sprite* sprite = getCurrentSprite(spriteGID);
				if (&sprite != nullptr)
				{
					sprite->setPosition(objPos_x, objPos_y);
				}

				Object object;
				object.name = objectName;
				object.type = objectType;
				object.sprite = *sprite;

				sf::Rect<int> objectRect;
				objectRect.top = objPos_x;
				objectRect.left = objPos_y;
				objectRect.width = objWidth;
				objectRect.height = objHeight;

				object.rect = objectRect;


				objects.push_back(object);
				objectElement = objectElement->next_sibling("object");
			}

			objectGroupElement = objectGroupElement->next_sibling("objectgroup");
		}
	}
	else {
		std::cout << "No object layers found..." << std::endl;
		return false;
	}

	return true;
}

bool Level::loadFromFile(std::string filename)
{
	std::cout << "LEVEL LOADING:" << std::endl;
	std::ifstream theFile(filename.c_str());
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	rapidxml::xml_document<> doc;

	doc.parse<0>(&buffer[0]);

	rapidxml::xml_node<char>* map;
	map = doc.first_node("map");

	// If file is correct and acceseble
	if (map == NULL)
	{
		std::cout << "Loading file \ " << filename << " \ failed" << std::endl;
		return false;
	}

	// Map properties
	this->width = atoi(map->first_attribute("width")->value());
	this->height = atoi(map->first_attribute("height")->value());
	this->tileWidth = atoi(map->first_attribute("tilewidth")->value());
	this->tileHeight = atoi(map->first_attribute("tileheight")->value());

	
	this->loadTilesets(map);
	
	this->loadMap(map);
	
	this->loadObjects(map);
	//Some staff for objects

	

}

Object Level::GetObject(std::string name)
{
	for (Object obj : objects)
	{
		if (obj.name == name)
			return obj;
	}
}

std::vector<Object> Level::GetObjects(std::string type)
{
	std::vector<Object> objArr;
	for (Object obj : objects)
	{
		if (obj.type == type)
			objArr.push_back(obj);
	}

	return objArr;
}

sf::Vector2i Level::GetTileSize()
{
	return sf::Vector2i(tileWidth, tileHeight);
}

void Level::Draw(sf::RenderWindow* window) 
{
	int i = 0;
	for (Layer layer : this->layers)
	{
		for (Tile tile : layer.tiles)
		{
			
			if (tile.isTexture)
			{
				//std::cout << i << "  " << tile.sprite << std::endl;
				window->draw(*(tile.sprite) );
				
			}
			
			
			/*
			if (layer.name == "wals")
			{
				sf::Vector2i tileSize = this->GetTileSize();
				sf::RectangleShape sh;
				sh.setSize(sf::Vector2f(32, 32));
				sh.setFillColor(sf::Color(200, 75, 20, 100));
				sh.setPosition(tile.Rect.left, tile.Rect.top);
				window.draw(sh);
			}
			*/
			i++;
		}
		
	}
}

std::optional<Layer> Level::GetLayer(std::string layerName)
{
	for (Layer layer : layers)
	{
		if (layer.name == layerName)
		{
			return layer;
		}
	}
	
	return {};
}

void Level::loadPhysics(b2World* world)
{
	std::cout << "Loading physics..." << std::endl;

	std::vector<Tile> walsTiles = this->GetLayer("wals").value().tiles;
	std::vector<Tile> gatesTiles = this->GetLayer("gates").value().tiles;

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;

	b2Body* walsBody = world->CreateBody(&bodyDef);

	std::string* point = new std::string;
	*point = "wals";
	walsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(point);

	b2FixtureDef WalsFixtureDef;

	b2EdgeShape edgeShape;
	WalsFixtureDef.shape = &edgeShape;
	WalsFixtureDef.friction = 0;
	WalsFixtureDef.restitution = 0;
	for (int i = 0; i < walsTiles.size(); ++i) {
		Tile tile = walsTiles[i];
		if (tile.isTexture)
		{
			float left = walsTiles[i].Rect->left - 16;
			float right = left + 32;
			//top
			if (i >= width)
			{
				if (!walsTiles[i - width].isTexture)
				{
					edgeShape.SetTwoSided(b2Vec2(left, tile.Rect->top - 16), b2Vec2(right, tile.Rect->top - 16));
					walsBody->CreateFixture(&WalsFixtureDef);
				}
			}
			else {
				edgeShape.SetTwoSided(b2Vec2(left, tile.Rect->top - 16), b2Vec2(right, tile.Rect->top - 16));
				walsBody->CreateFixture(&WalsFixtureDef);
			}
			//bottom
			if (i + width < walsTiles.size())
			{
				if (!walsTiles[i + width].isTexture)
				{
					edgeShape.SetTwoSided(b2Vec2(left, tile.Rect->top + 16), b2Vec2(right, tile.Rect->top + 16));
					walsBody->CreateFixture(&WalsFixtureDef);
				}
			}
			else {
				edgeShape.SetTwoSided(b2Vec2(left, tile.Rect->top + 16), b2Vec2(right, tile.Rect->top + 16));
				walsBody->CreateFixture(&WalsFixtureDef);
			}
			if (i > 0 && i < walsTiles.size() - 1)
			{
				//left
				if (!walsTiles[i - 1].isTexture)
				{
					edgeShape.SetTwoSided(b2Vec2(left, tile.Rect->top - 16), b2Vec2(left, tile.Rect->top + 16));
					walsBody->CreateFixture(&WalsFixtureDef);
				}
				//right
				if (!walsTiles[i + 1].isTexture)
				{
					edgeShape.SetTwoSided(b2Vec2(right, tile.Rect->top - 16), b2Vec2(right, tile.Rect->top + 16));
					walsBody->CreateFixture(&WalsFixtureDef);
				}
			}
			else {
				if (i == 0)
				{
					edgeShape.SetTwoSided(b2Vec2(left, tile.Rect->top - 16), b2Vec2(left, tile.Rect->top + 16));
					walsBody->CreateFixture(&WalsFixtureDef);
				}
				else {
					edgeShape.SetTwoSided(b2Vec2(right, tile.Rect->top - 16), b2Vec2(right, tile.Rect->top + 16));
					walsBody->CreateFixture(&WalsFixtureDef);
				}
			}
		}


	}


	b2FixtureDef gatesFixtureDef;
	gatesFixtureDef.shape = &edgeShape;
	gatesFixtureDef.friction = 0;
	gatesFixtureDef.restitution = 0;
	//bodyDef.userData = b2BodyUserData

	for (int i = 0; i < gatesTiles.size(); ++i)
	{
		Tile tile = gatesTiles[i];


		if (tile.isTexture)
		{
			float left = tile.Rect->left - 16;
			float right = left + 32;
			b2Body* gatesBody = world->CreateBody(&bodyDef);
			std::string* point = new std::string;
			*point = "wals";	
			gatesBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(point);
			if (i + width < gatesTiles.size() || i - width >= 0)
			{

				if (walsTiles[i + width].isTexture || walsTiles[i - width].isTexture)
				{
					std::cout << "up & down collider" << std::endl;
					edgeShape.SetTwoSided(b2Vec2(left + 7, tile.Rect->top - 16), b2Vec2(left + 7, tile.Rect->top + 16));
					gatesBody->CreateFixture(&gatesFixtureDef);

					edgeShape.SetTwoSided(b2Vec2(left + 25, tile.Rect->top - 16), b2Vec2(left + 25, tile.Rect->top + 16));
					gatesBody->CreateFixture(&gatesFixtureDef);

					if (walsTiles[i + width].isTexture)
					{
						tile = walsTiles[i + width];
					}
					else
					{
						tile = walsTiles[i - width];
					}

					float left = tile.Rect->left - 16;
					float right = left + 32;

					edgeShape.SetTwoSided(b2Vec2(left + 7, tile.Rect->top - 16), b2Vec2(left + 7, tile.Rect->top + 16));
					gatesBody->CreateFixture(&gatesFixtureDef);

					edgeShape.SetTwoSided(b2Vec2(left + 25, tile.Rect->top - 16), b2Vec2(left + 25, tile.Rect->top + 16));
					gatesBody->CreateFixture(&gatesFixtureDef);
				}
			}
			if (i + 1 < gatesTiles.size() || i - 1 >= 0)
			{
				if (walsTiles[i + 1].isTexture || walsTiles[i - 1].isTexture)
				{
					std::cout << "right & left collider" << std::endl;
					edgeShape.SetTwoSided(b2Vec2(left, tile.Rect->top + 8), b2Vec2(right, tile.Rect->top + 8));
					gatesBody->CreateFixture(&gatesFixtureDef);

					edgeShape.SetTwoSided(b2Vec2(left, tile.Rect->top - 8), b2Vec2(right, tile.Rect->top - 8));
					gatesBody->CreateFixture(&gatesFixtureDef);

					if (walsTiles[i + 1].isTexture)
					{
						tile = walsTiles[i + 1];
					}
					else
					{
						tile = walsTiles[i - 1];
					}

					float left = tile.Rect->left - 16;
					float right = left + 32;

					edgeShape.SetTwoSided(b2Vec2(left, tile.Rect->top + 8), b2Vec2(right, tile.Rect->top + 8));
					gatesBody->CreateFixture(&gatesFixtureDef);

					edgeShape.SetTwoSided(b2Vec2(left, tile.Rect->top - 8), b2Vec2(right, tile.Rect->top - 8));
					gatesBody->CreateFixture(&gatesFixtureDef);
				}
			}
		}
	}
}
