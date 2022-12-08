#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "MovableObject.h"

class Ball : public MovableObject
{
public:
	Ball(string textureFilePath, sf::Vector2f _startPosition) :
		MovableObject(textureFilePath, _startPosition) { }
	virtual ~Ball() { }
	void update(float dt) override;
	void clamp_position(sf::RenderWindow* window) override;
};