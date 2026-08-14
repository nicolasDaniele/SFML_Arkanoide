#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "MovableObject.h"

class Ball : public MovableObject
{
public:
	Ball(string textureFilePath, sf::Vector2f _startPosition, float _initialSpeed) :
		MovableObject(textureFilePath, _startPosition, _initialSpeed) { }
	virtual ~Ball() { }

	void update(float dt) override;
	void draw(sf::RenderWindow* window) override;
	void clamp_position(sf::RenderWindow* window) override;
	void reset() override;

	// Functions called when colliding with other entities
	void ricochet(Entity* other);
	void bounce_from(sf::FloatRect otherBounds);
};