#include "Block.h"

Block::Block(const std::string& textureFilePath, const sf::Vector2f _startPosition,
	const sf::Color color, const sf::Vector2f startScale, bool _isBreakable) :
	Entity(textureFilePath, _startPosition, startScale)
{
	startPosition = _startPosition;
	isBreakable = _isBreakable;

	sprite.setColor(color);
}

bool Block::is_breakable() const
{
	return isBreakable;
}