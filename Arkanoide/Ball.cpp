#include <algorithm>
#include "Ball.h"

void Ball::update(float dt)
{
	sprite.move(velocity * dt);
}

void Ball::draw(sf::RenderWindow* window)
{
	window->draw(get_sprite());
}

void Ball::clamp_position(sf::RenderWindow* window)
{
	sf::FloatRect bounds = sprite.getGlobalBounds();

	// Left Screen Border
	if (bounds.left < 0)
	{
		sprite.move(-bounds.left, 0);

		if (velocity.x < 0)
			velocity.x = -velocity.x;
	}

	// Right Screen Border
	else if (bounds.left + bounds.width > window->getSize().x)
	{
		float overflow =
			bounds.left + bounds.width - window->getSize().x;

		sprite.move(-overflow, 0);

		if (velocity.x > 0)
			velocity.x = -velocity.x;
	}

	// Upper Screen border
	if (bounds.top < 0)
	{
		sprite.move(0, -bounds.top);

		if (velocity.y < 0)
			velocity.y = -velocity.y;
	}
}

void Ball::reset()
{
	set_position(startPosition);
	velocity = sf::Vector2f(0.0f, initialSpeed);
	currentSpeed = initialSpeed;
}

void Ball::ricochet(Entity* other)
{
	float otherCenter = other->get_sprite().getPosition().x
		+ other->get_sprite().getGlobalBounds().width / 2.0f;

	float ballCenter = sprite.getPosition().x
		+ sprite.getGlobalBounds().width / 2.0f;

	float hitPosition = (ballCenter - otherCenter)
		/ (other->get_sprite().getGlobalBounds().width / 2.0f);
	hitPosition = std::clamp(hitPosition, -1.0f, 1.0f);

	float maxAngle = 60.0f * 3.14159f / 180.0f;
	float angle = hitPosition * maxAngle;

	sf::Vector2f newBallVelocity(std::sin(angle) * currentSpeed,
								-std::cos(angle) * currentSpeed);

	set_velocity(newBallVelocity);
}

void Ball::bounce_from(sf::FloatRect otherBounds)
{
	sf::FloatRect ballBounds = sprite.getGlobalBounds();

	float ballCenterX = ballBounds.left + ballBounds.width / 2.0f;
	float ballCenterY = ballBounds.top + ballBounds.height / 2.0f;

	float otherCenterX = otherBounds.left + otherBounds.width / 2.0f;
	float otherCenterY = otherBounds.top + otherBounds.height / 2.0f;

	float deltaX = ballCenterX - otherCenterX;
	float deltaY = ballCenterY - otherCenterY;

	float overlapX =
		(ballBounds.width + otherBounds.width) / 2.0f
		- std::abs(deltaX);

	float overlapY =
		(ballBounds.height + otherBounds.height) / 2.0f
		- std::abs(deltaY);

	if (overlapX < overlapY)
	{
		velocity.x = -velocity.x;

		if (deltaX > 0)
			sprite.move(overlapX, 0);
		else
			sprite.move(-overlapX, 0);
	}
	else
	{
		velocity.y = -velocity.y;

		if (deltaY > 0)
			sprite.move(0, overlapY);
		else
			sprite.move(0, -overlapY);
	}

	float angle = (rand() % 21 - 10) * 3.14159f / 180.0f;

	float newX = velocity.x * std::cos(angle)
		- velocity.y * std::sin(angle);

	float newY = velocity.x * std::sin(angle)
		+ velocity.y * std::cos(angle);

	set_velocity(newX, newY);
}