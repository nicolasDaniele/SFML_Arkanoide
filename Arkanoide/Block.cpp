#include "Block.h"

Block::Block(sf::Color color, sf::Vector2f _startPosition, sf::Vector2f size)
{
	rectangle = sf::RectangleShape(size);
	rectangle.setFillColor(color);

	startPosition = _startPosition;
}

void Block::set_position(const sf::Vector2f newPosition)
{
	rectangle.setPosition(newPosition);
}

void Block::draw(sf::RenderWindow* window)
{
	window->draw(rectangle);
}

sf::Vector2f Block::get_size() const
{
	return rectangle.getSize();
}

sf::RectangleShape Block::get_rectangle() const
{
	return rectangle;
}
