#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "MovableObject.h"

using namespace std;

class Paddle : public MovableObject
{

public:
	Paddle(string textureFilePath, sf::Vector2f _startPosition, float _initialSpeed) :
		MovableObject(textureFilePath, _startPosition, _initialSpeed) {	}
	virtual ~Paddle() { }

	void update(float dt) override;
	void draw(sf::RenderWindow* window) override;
	void clamp_position(sf::RenderWindow* window) override;
};