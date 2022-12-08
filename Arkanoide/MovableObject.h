#pragma once

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Entity.h"

using namespace std;

class MovableObject : public Entity
{
protected:
	sf::Vector2f velocity;

public:
	MovableObject(string textureFilePath, sf::Vector2f _startPosition) :
		Entity(textureFilePath, _startPosition) { }
	virtual ~MovableObject() { }
	sf::Vector2f get_velocity() const;
	void add_move_vector(const sf::Vector2f vec);
	void set_velocity(const sf::Vector2f newVelocity);
	void set_velocity(const float xVelocity, const float yVelocity);
	virtual void clamp_position(sf::RenderWindow* window) = 0;
};