#include "Block.h"

Block::Block(const sf::Texture& texture, const sf::Vector2f _startPosition,
    const sf::Color color, const sf::Vector2f startScale, bool _isBreakable, bool _dropsPowerUp) :
	Entity(texture, _startPosition)
{
	isBreakable = _isBreakable;
	dropsPowerUp = _dropsPowerUp;

	sprite.setColor(color);
	sprite.setScale(startScale);
}

bool Block::is_breakable() const
{
	return isBreakable;
}

bool Block::drops_power_up() const
{
	return dropsPowerUp;
}