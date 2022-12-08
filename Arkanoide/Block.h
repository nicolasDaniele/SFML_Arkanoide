#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"

class Block : public Entity
{

public:
	Block(string textureFilePath, sf::Vector2f _startPosition) :
		Entity(textureFilePath, _startPosition) { }
	virtual ~Block() { }
	void update(float dt) override { }
};