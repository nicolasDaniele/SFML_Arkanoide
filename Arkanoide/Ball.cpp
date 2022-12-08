#include "Ball.h"

void Ball::update(float dt)
{
	sprite.move(velocity * dt);
}

void Ball::clamp_position(sf::RenderWindow* window)
{
	float minXPos = 0;
	float maxXPos = (float)(window->getSize().x - texture.getSize().x);

	if (sprite.getPosition().x < minXPos || sprite.getPosition().x > maxXPos)
	{
		sf::Vector2f newBallVelocity(-velocity.x, velocity.y);
		velocity = newBallVelocity;
	}

	if (sprite.getPosition().y < 0)
	{
		sf::Vector2f newBallVelocity(velocity.x, -velocity.y);
		velocity = newBallVelocity;
	}
}