#include "Paddle.h"

void Paddle::update(float dt)
{
	sprite.move(velocity * dt);
}

void Paddle::clamp_position(sf::RenderWindow* window)
{
	float minXPos = 0;
	float maxXPos = (float)(window->getSize().x -texture.getSize().x);

	if (sprite.getPosition().x < minXPos)
	{
		sprite.setPosition(minXPos, sprite.getPosition().y);
	}

	if (sprite.getPosition().x > maxXPos)
	{
		sprite.setPosition(maxXPos, sprite.getPosition().y);
	}
}