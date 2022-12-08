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
	Entity(string textureFilePath, sf::Vector2f _startPosition);
	virtual ~Entity() { }
	virtual void update(float dt) = 0;
	void set_position(const sf::Vector2f newPosition);
	void set_position(const float xPosition, const float yPosition);
	void set_scale(const float xScale, const float yScale);
	sf::Sprite get_sprite() const;
	sf::Texture get_texture() const;
	sf::Vector2f get_start_position() const;
};