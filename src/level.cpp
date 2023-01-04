#include "level.h"
#include "libs.h"
#include <memory>






//Level implementation

Tileset Level::getTileset()
{
	return this->tilesets[0];
}

void Level::pushToCollidedHitboxes(HitboxSprite& collidedHitbox)
{
	this->collidedHitboxes.push_back(&collidedHitbox);
}

void Level::pushToUpperLayer(HitboxSprite& collidedHitbox)
{
	this->upperLayer.push_back(&collidedHitbox);
}


//sf::Texture* Level::getTexture(int tileGID)
//{
//	Tileset tempTileSet;
//	int subRectToUse = -1, firstTileID = 0;
//	
//	for (Tileset tile_set : this->tilesets)
//	{
//		if (tile_set.firstTileID <= tileGID && tile_set.firstTileID > firstTileID)
//		{
//			firstTileID = tile_set.firstTileID;
//			tempTileSet = tile_set;
//		}
//	}
//
//	subRectToUse = tileGID - tempTileSet.firstTileID;
//	sf::Texture* newTexture;
//	newTexture = new sf::Texture();
//	if (subRectToUse >= 0)
//	{
//		newTexture->loadFromImage(*(tempTileSet.tileImage), tempTileSet.subRects[subRectToUse]);
//
//		return newTexture;
//
//		//return std::make_pair(tempTileSet.tileImage, tempTileSet.subRects[subRectToUse]);
//	}
//	else {
//		std::cout << "Smthg wrong for tile with GID: " << tileGID << std::endl;
//		//throw __std_exception_data();
//		return newTexture;
//
//		//return std::make_pair(sf::Image(), sf::Rect<int>());
//	}
//	
//}

HitboxSprite Level::getCurrentSprite(int tileGID)
{
	Tileset tempTileSet;
	int subRectToUse = -1, firstTileID = 0;

	for (Tileset tile_set : this->tilesets)
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
		HitboxSprite sprite;
		//sprite = new sf::Sprite();
		sprite.setTexture(*tempTileSet.tileImage);
		sprite.setTextureRect(tempTileSet.subRects[subRectToUse]);


		return sprite;

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

			if (spriteGID > 0)
			{

				//sf::Texture texture = getTexture(spriteGID);

				HitboxSprite sprite = getCurrentSprite(spriteGID);
				//sf::Texture* texture;

				//texture = getTexture(spriteGID);
				//std::cout<<texture.loadFromImage(getTexture(spriteGID).first, getTexture(spriteGID).second)<<std::endl;
				if (sprite.getTexture()->getSize().x > 0)
				{
					

					//sprite->setTexture(*texture);

					sprite.setType(layer.name);

					sprite.setPosition(x * tileWidth, y * tileHeight);

					sprite.setColor(sf::Color(255, 255, 255, layer.opacity));

					sprite.setHitbox({ 0, 0, 32, 32 });

					//std::cout << sprite.getType() << std::endl;

					layer.tiles.push_back(sprite);
				}
			}
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
	std::cout << "Map loaded" << std::endl;
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

		/*columns = img.getSize().x / tileWidth;
		rows = img.getSize().y / tileHeight;*/

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


		this->tilesets.push_back(tileset);

		/*
		std::cout << tileset.name << std::endl;
		std::cout << tileset.rows << std::endl;
		std::cout << tileset.columns << std::endl;
		*/
		map->remove_first_node();

		tilesetElement = map->first_node("tileset");
	}

	std::cout << "Tilesets loaded" << std::endl;
	return true;
}


Character Level::getPlayer()
{
	for (Character ch : this->dynamicSprites)
	{
		if (ch.characterSprite.getType() == "player")
		{
			return ch;
		}
	}
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


				//sf::Texture texture;

				//texture = getTexture(spriteGID);
				//std::cout << texture.loadFromImage(getTexture(spriteGID).first, getTexture(spriteGID).second) << std::endl;

				CharacterSprite newCharacterSprite;
				newCharacterSprite.fromHitboxSprite((getCurrentSprite(spriteGID)));

				if (newCharacterSprite.getTexture()->getSize().x > 0)
				{
					

					newCharacterSprite.setType(objectName);

					newCharacterSprite.setPosition(objPos_x, objPos_y);

					newCharacterSprite.setColor(sf::Color(255, 255, 255, 255));

					newCharacterSprite.setHitbox({ 0, 0, 32, 32 });


					sf::RectangleShape sensorArea;

					sensorArea.setPosition(newCharacterSprite.getPosition().x - 16, newCharacterSprite.getPosition().y - 16);

					sensorArea.setOutlineColor(sf::Color(200, 200, 200, 100));

					sensorArea.setFillColor(sf::Color(0, 0, 255, 100));

					sensorArea.setSize(sf::Vector2f(64, 64));


					//sensorArea.setHitbox({ -8, -8, 40, 40 });

					sf::RectangleShape moveArea;

					moveArea.setPosition(newCharacterSprite.getPosition().x + 6, newCharacterSprite.getPosition().y + 22);

					moveArea.setFillColor(sf::Color(255, 165, 0, 200));

					moveArea.setSize(sf::Vector2f( 20, 10));


					//HitboxSprite moveArea;


					//moveArea.setHitbox({ 0, 20, 32, 12 });

					Character newCharacter;

					newCharacter.characterSprite = newCharacterSprite;

					newCharacter.setSensorBox(sensorArea);

					newCharacter.setMoveBox(moveArea);

					this->dynamicSprites.push_back(newCharacter);
				}

				objectElement = objectElement->next_sibling("object");
			}

			objectGroupElement = objectGroupElement->next_sibling("objectgroup");
		}
	}
	else {
		std::cout << "No object layers found..." << std::endl;
		return false;
	}

	std::cout << "Objects loaded" << std::endl;

	return true;
}

bool Level::loadFromFile(std::string filename)
{
	std::cout << "LEVEL LOADING:" << std::endl;
	std::ifstream theFile(filename.c_str());
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	
	buffer.push_back('\0');

	rapidxml::xml_document<>* doc;
	doc = new rapidxml::xml_document<>();

	doc->parse<0>(&buffer[0]);

	
	rapidxml::xml_node<char>* map;
	map = doc->first_node("map");

	std::cout << sizeof(doc) << std::endl;

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


sf::Vector2i Level::GetTileSize()
{
	return sf::Vector2i(tileWidth, tileHeight);
}

void Level::drawUncollidedTiles(sf::RenderWindow* window)
{
	
	for (Layer layer : this->layers)
	{
		//std::cout << "Displayed" << std::endl;
		for (auto tile_ptr = std::begin(layer.tiles); tile_ptr != std::end(layer.tiles); ++tile_ptr)
		{
			
			if((!tile_ptr->isUpper) || tile_ptr->getType() == "floor")
				
				window->draw(*tile_ptr);
		}
	}
}

void Level::drawUpperLayer(sf::RenderWindow* window)
{
	for (std::vector<HitboxSprite*>::iterator tile_ptr = std::begin(this->upperLayer); tile_ptr != std::end(this->upperLayer); )
	{	
		//std::cout << "draw upper" << std::endl;
		if ((*tile_ptr)->isCollided && (*tile_ptr)->isUpper)
		{
			window->draw(*(*tile_ptr));
			++tile_ptr;
		}
		else
			tile_ptr = this->upperLayer.erase(tile_ptr);
		
	}
	//this->flushCollidedHitboxes();
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

std::pair<std::vector<Layer>::iterator, std::vector<Layer>::iterator> Level::GetAllLayers()
{
	return std::make_pair(this->layers.begin(), this->layers.end());
}

