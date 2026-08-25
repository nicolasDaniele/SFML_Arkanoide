#include "PowerUp.h"

PowerUp::PowerUp(const sf::Texture& texture, sf::Vector2f _startPosition,
	PowerUpType _type, sf::Vector2f _scale) 
	: Entity(texture, _startPosition), type(_type)
{
	set_scale(_scale.x, _scale.y);
}

void PowerUp::update(float dt)
{
	sprite.move({ 0.0f, fallSpeed * dt });
}

PowerUpType PowerUp::get_type() const
{
	return type;
}
