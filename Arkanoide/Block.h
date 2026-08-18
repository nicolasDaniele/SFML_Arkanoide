#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"

class Block : public Entity
{
private:
	sf::RectangleShape rectangle;
	bool isBreakable;

public:
	Block(string textureFilePath, sf::Vector2f _startPosition) :
		Entity(textureFilePath, _startPosition) { }
	Block(sf::Color color, sf::Vector2f _startPosition, 
		sf::Vector2f size, bool _isBreakable = true);
	virtual ~Block() { }
	void set_position(const sf::Vector2f newPosition) override;
	void update(float dt) override { }
	void draw(sf::RenderWindow* window) override;
	sf::Vector2f get_size() const;
	sf::RectangleShape get_rectangle() const;
	bool is_breakable() const;
};