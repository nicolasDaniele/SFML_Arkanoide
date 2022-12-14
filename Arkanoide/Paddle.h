#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "MovableObject.h"

using namespace std;

class Paddle : public MovableObject
{

public:
	Paddle(string textureFilePath, sf::Vector2f _startPosition) :
		MovableObject(textureFilePath, _startPosition) { }
	virtual ~Paddle() { }
	void update(float dt) override;
	void clamp_position(sf::RenderWindow* window) override;
};