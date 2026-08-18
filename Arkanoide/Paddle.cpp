#include "Paddle.h"

void Paddle::update(float dt)
{
	sprite.move(velocity * dt);
}

void Paddle::draw(sf::RenderWindow* window)
{
	window->draw(get_sprite());
}

void Paddle::clamp_position(sf::RenderWindow* window)
{
	float halfWidth = sprite.getGlobalBounds().width / 2.0f;

	float minXPos = halfWidth;
	float maxXPos = window->getSize().x - halfWidth;

	if (sprite.getPosition().x < minXPos)
	{
		sprite.setPosition(minXPos, sprite.getPosition().y);
	}

	if (sprite.getPosition().x > maxXPos)
	{
		sprite.setPosition(maxXPos, sprite.getPosition().y);
	}
}

void Paddle::reset()
{
	set_position(startPosition);
}