#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Entity.h"

class Block : public Entity
{

public:
	Block(string textureFilePath, sf::Vector2f startPos) :
		Entity(textureFilePath, startPos) { }
	virtual ~Block() { }
};