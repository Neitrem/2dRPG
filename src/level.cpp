#include "level.h"
#include "libs.h"
#include <memory>






//Level implementation

/// Returns this Level Tileset : <Tileset>
Tileset Level::getTileset()
{
	return this->tilesets[0];
}

/// <summary>
/// Push given link to CollidedHitbox 
/// </summary>
/// <param name="collidedHitbox">*MapElement</param>
void Level::pushToCollidedHitboxes(MapElement& collidedHitbox)
{
	this->collidedElements.push_back(&collidedHitbox);
}

/// <summary>
/// Push given link to UpperLayer
/// </summary>
/// <param name="collidedHitbox">*MapElement</param>
void Level::pushToUpperLayer(MapElement& collidedHitbox)
{
	this->upperElementsLayer.push_back(&collidedHitbox);
}


/// <summary>
/// Returns sf::Color based on type of Object
/// </summary>
/// <returns></returns>
sf::Color generateColor(std::string type)
{
	if (type == "wals")
	{
		return sf::Color::Magenta;
	}

	if (type == "gates")
	{
		return sf::Color::Green;
	}
}

/// <summary>
/// Creates phisical object for current sprite 
/// </summary>
/// <param name="tiles_map"></param>
/// <param name="mapElement"></param>
void Level::generatePhysicalShape(std::map<sf::String, int> tiles_map, MapElement &mapElement)
{
	const int OFFSET = 12;

	int offset = !tiles_map["upper_tile"] ? OFFSET : 0;


	sf::RectangleShape newShape;

	HitboxSprite hitboxSprite = mapElement.getVisionHitboxSprite();
	
	sf::FloatRect visionSpriteHitbox = hitboxSprite.getHitbox();
	sf::Vector2f visionSpritePosition = hitboxSprite.getPosition();


	int posX = visionSpritePosition.x;
	float width = visionSpriteHitbox.width;

	int posY = visionSpritePosition.y + offset;
	float height = visionSpriteHitbox.height - offset;


	newShape.setPosition(posX, posY);
	newShape.setSize({ width, height });


	sf::Color color = generateColor(hitboxSprite.getType());
	newShape.setFillColor(color);

	mapElement.setPhysicalShape(newShape);





	//if (!tiles_map["bottom_tile"])
	//{
	//	//std::cout << "Bottom line" << tiles_map["bottom_tile"] << std::endl;;
	//	sf::RectangleShape newShape;

	//	sf::Vector2f spritePosition = mapElement.hitboxSprite.getPosition();

	//	
	//	newShape.setPosition(spritePosition.x, spritePosition.y + this->tileHeight);
	//	newShape.setSize({ float(this->tileWidth), 1 });

	//	newShape.setFillColor(generateColor(mapElement.hitboxSprite.getType()));

	//	mapElement.shapes_map["bottom_shape"] = newShape;
	//}
	//if (!tiles_map["right_tile"])
	//{
	//	//std::cout << "Left line" << tiles_map["right_tile"] << std::endl;
	//	sf::RectangleShape newShape;

	//	sf::Vector2f spritePosition = mapElement.hitboxSprite.getPosition();

	//	newShape.setPosition(spritePosition.x + this->tileWidth, spritePosition.y);
	//	newShape.setSize({ 1, float(this->tileWidth) + 1 });
	//	if (!tiles_map["upper_tile"])
	//	{
	//		newShape.setPosition(spritePosition.x + this->tileWidth, spritePosition.y + offset);
	//		newShape.setSize({ 1, float(this->tileWidth) - offset + 1 });
	//	}
	//	if (tiles_map["bottom_tile"])
	//	{
	//		newShape.setSize({ 1, float(this->tileWidth) + offset + 1 });
	//	}
	//	//std::cout << this->tileWidth << std::endl;

	//	newShape.setFillColor(generateColor(mapElement.hitboxSprite.getType()));

	//	


	//	mapElement.shapes_map["right_shape"] = newShape;
	//}
	//if (!tiles_map["upper_tile"])
	//{
	//	//std::cout << "Left line" << tiles_map["right_tile"] << std::endl;
	//	sf::RectangleShape newShape;

	//	sf::Vector2f spritePosition = mapElement.hitboxSprite.getPosition();

	//	newShape.setPosition(spritePosition.x, spritePosition.y + offset);
	//	newShape.setSize({ float(this->tileWidth), 1 });

	//	//std::cout << this->tileWidth << std::endl;

	//	newShape.setFillColor(generateColor(mapElement.hitboxSprite.getType()));

	//	


	//	mapElement.shapes_map["upper_shape"] = newShape;
	//}
	//if (!tiles_map["left_tile"])
	//{
	//	//std::cout << "Left line" << tiles_map["right_tile"] << std::endl;
	//	sf::RectangleShape newShape;

	//	sf::Vector2f spritePosition = mapElement.hitboxSprite.getPosition();

	//	newShape.setPosition(spritePosition.x, spritePosition.y + offset);
	//	newShape.setSize({ 1, float(this->tileWidth) });
	//	if (!tiles_map["bottom_tile"])
	//	{
	//		newShape.setSize({ 1, float(this->tileWidth) - offset});
	//	}

	//	//std::cout << this->tileWidth << std::endl;

	//	newShape.setFillColor(generateColor(mapElement.hitboxSprite.getType()));

	//	


	//	mapElement.shapes_map["left_shape"] = newShape;
	//}

}

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

		std::vector<std::vector<int>> layerMatrics;

		std::vector<int> tempRowString;

		while (std::getline(dataLine, cell, ','))
		{
			int spriteGID = atoi(cell.c_str());

			tempRowString.push_back(spriteGID);

			x++;
			if (x >= width)
			{
				x = 0;

				for (auto iter = tempRowString.begin(); iter < tempRowString.end(); ++iter)
				{
					printf("%-4d", *iter);
				}

				//std::copy(tempRowString.begin(), tempRowString.end(), std::ostream_iterator<int>(std::cout, "  "));

				std::cout << std::endl;

				//std::cout << tempMapString << std::endl;

				layerMatrics.push_back(tempRowString);

				tempRowString.clear();

				y++;
					
			}
			
		}
		std::cout << std::endl << std::endl;

		x = 0;
		y = 0;

		for (auto string_iter = layerMatrics.begin(); string_iter < layerMatrics.end(); ++string_iter)
		{

			for (auto tile_iter = string_iter->begin(); tile_iter < string_iter->end(); ++tile_iter)
			{
				int spriteGID = *tile_iter;

				if (spriteGID > 0)
				{
					HitboxSprite sprite = getCurrentSprite(spriteGID);

					if (sprite.getTexture()->getSize().x > 0)
					{

						sprite.setType(layer.name);

						sprite.setPosition(x * tileWidth, y * tileHeight);

						sprite.setColor(sf::Color(255, 255, 255, layer.opacity));

						sprite.setHitbox({ 0, 0, 32, 32 });


						MapElement newMapElement;

						newMapElement.setVisionHitboxSprite(sprite);

						std::map<sf::String, int> tiles_map = {
							{ "upper_tile", y > 0 ? *(((string_iter - 1)->begin()) + x) : 0}, // Check if it isn't first string and if not then put current value in var
							{ "right_tile", x < this->width ? *(tile_iter + 1) : 0}, // Check if it isn't last tile in a row and if not then put current value in var
							{ "left_tile", x > 0 ? *(tile_iter - 1) : 0}, // Check if it isn't first tile in a row and if not then put current value in var
							{ "bottom_tile", y < this->height ? *(((string_iter + 1)->begin()) + x) : 0}, // Chaeck if it isn't last string an if not then put current value in var
						};

						if (layer.name != "floor")
						{
							this->generatePhysicalShape(tiles_map, newMapElement);
						}


						

						//std::cout << newMapElement.physicalShape.getSize().x << std::endl;

						layer.tiles.push_back(newMapElement);
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
				newCharacterSprite.generateFromHitboxSprite((getCurrentSprite(spriteGID)));

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
			HitboxSprite tileHitboxSprite = tile_ptr->getVisionHitboxSprite();

			if ((!tileHitboxSprite.isUpper) || tileHitboxSprite.getType() == "floor")
			{
				window->draw(tileHitboxSprite);
			}	
		}
	}
}

void Level::drawPhysicsBounds(sf::RenderWindow* window)
{
	for (Layer layer : this->layers)
	{
		//std::cout << "Displayed" << std::endl;
		for (auto tile_ptr = std::begin(layer.tiles); tile_ptr != std::end(layer.tiles); ++tile_ptr)
		{
			HitboxSprite tileVisionSprite = tile_ptr->getVisionHitboxSprite();

			if (tileVisionSprite.getType() != "floor")
			{
				window->draw(tile_ptr->getPhysicalShape());
			}
		}
	}
}

void Level::drawUpperLayer(sf::RenderWindow* window)
{
	for (std::vector<MapElement*>::iterator tile_ptr = std::begin(this->upperElementsLayer); tile_ptr != std::end(this->upperElementsLayer); )
	{
		HitboxSprite tileVisionSprite = (*tile_ptr)->getVisionHitboxSprite();

		//std::cout << "draw upper" << std::endl;
		if (tileVisionSprite.isCollided && tileVisionSprite.isUpper)
		{
			window->draw(tileVisionSprite);
			++tile_ptr;
		}
		else
			tile_ptr = this->upperElementsLayer.erase(tile_ptr);
		
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

