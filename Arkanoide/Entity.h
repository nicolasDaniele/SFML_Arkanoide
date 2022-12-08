#pragma once

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

class Entity
{
protected:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f startPosition;
	string textureFilePath;

public:
	Entity(string textureFilePath, sf::Vector2f startPos);
	virtual ~Entity() { }
};