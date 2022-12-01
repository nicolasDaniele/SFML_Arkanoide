#pragma once
#include <iostream>
#include "Entity.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Paddle : public Entity
{

public:
	Paddle(string textureFilePath, sf::Vector2f startPos) :
		Entity(textureFilePath, startPos) { } 
	virtual ~Paddle() { }
	void add_move_vector(sf::Vector2f vec);
	void add_move_vector(sf::Vector2f vec);
	sf::Sprite get_sprite();
};