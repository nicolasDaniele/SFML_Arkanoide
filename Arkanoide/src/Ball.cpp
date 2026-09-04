#include <algorithm>
#include <cmath>
#include <cstdlib>
#include "Ball.h"

void Ball::update(float dt)
{
	sprite.move(velocity * dt);
}

void Ball::clamp_position(sf::RenderWindow* window)
{
	sf::FloatRect bounds = sprite.getGlobalBounds();

	// Left Screen Border
	if (bounds.position.x < 0)
	{
		sprite.move({ -bounds.position.x, 0 });

		if (velocity.x < 0)
			velocity.x = -velocity.x;
	}

	// Right Screen Border
	else if (bounds.position.x + bounds.size.x > window->getSize().x)
	{
		float overflow = bounds.position.x + 
			bounds.size.x - window->getSize().x;

		sprite.move({ -overflow, 0 });

		if (velocity.x > 0)
			velocity.x = -velocity.x;
	}

	// Upper Screen border
	if (bounds.position.y < 0)
	{
		sprite.move({ 0, -bounds.position.y });

		if (velocity.y < 0)
			velocity.y = -velocity.y;

	}
	
	prevent_horizontal_trajectory();
}

void Ball::reset()
{
	set_position(startPosition);
	velocity = sf::Vector2f(0.0f, initialSpeed);
	currentSpeed = initialSpeed;
}

void Ball::ricochet(Entity* other)
{
	sf::FloatRect paddleBounds = other->get_sprite().getGlobalBounds();

	sf::FloatRect ballBounds = sprite.getGlobalBounds();

	float paddleCenter = paddleBounds.position.x + 
		paddleBounds.size.x / 2.0f;

	float ballCenter = ballBounds.position.x + 
		ballBounds.size.x / 2.0f;

	// -1 = hit paddle left winger
	//  0 = hit paddle center
	// +1 = hit paddle right winger
	float hitPosition = (ballCenter - paddleCenter) /
		(paddleBounds.size.x / 2.0f);

	hitPosition = std::clamp(hitPosition, -1.0f, 1.0f);

	constexpr float PI = 3.14159265f;

	const float minAngle = 20.0f * PI / 180.0f;
	const float maxAngle = 60.0f * PI / 180.0f;

	float angle = hitPosition * maxAngle;

	// Avoid too vertical/horizontal bounces
	if (std::abs(angle) < minAngle)
	{
		if (angle < 0.0f)
			angle = -minAngle;
		else
			angle = minAngle;
	}

	sf::Vector2f newVelocity(
		std::sin(angle) * currentSpeed,
		-std::cos(angle) * currentSpeed
	);

	set_velocity(newVelocity);
}

void Ball::bounce_from(sf::FloatRect otherBounds)
{
	sf::FloatRect ballBounds = sprite.getGlobalBounds();

	float ballCenterX = ballBounds.position.x + ballBounds.size.x / 2.0f;
	float ballCenterY = ballBounds.position.y + ballBounds.size.y / 2.0f;

	float otherCenterX = otherBounds.position.x + otherBounds.size.x / 2.0f;
	float otherCenterY = otherBounds.position.y + otherBounds.size.y / 2.0f;

	float deltaX = ballCenterX - otherCenterX;
	float deltaY = ballCenterY - otherCenterY;

	float overlapX = (ballBounds.size.x + otherBounds.size.x) / 
		2.0f - std::abs(deltaX);

	float overlapY = (ballBounds.size.y + otherBounds.size.y) / 
		2.0f - std::abs(deltaY);

	if (overlapX < overlapY)
	{
		velocity.x = -velocity.x;

		if (deltaX > 0)
			sprite.move({ overlapX, 0 });
		else
			sprite.move({ -overlapX, 0 });
	}
	else
	{
		velocity.y = -velocity.y;

		if (deltaY > 0)
			sprite.move({ 0, overlapY });
		else
			sprite.move({ 0, -overlapY });
	}

	float angle = (std::rand() % 21 - 10) * 3.14159f / 180.0f;

	float newX = velocity.x * std::cos(angle) -
		velocity.y * std::sin(angle);

	float newY = velocity.x * std::sin(angle) +
		velocity.y * std::cos(angle);

	set_velocity(newX, newY);

	prevent_horizontal_trajectory();
}

void Ball::prevent_horizontal_trajectory()
{
	const float minAngle = 20.0f * 3.14159f / 180.0f;

	float speed = std::sqrt(
		velocity.x * velocity.x +
		velocity.y * velocity.y
	);

	float angle = std::atan2(
		std::abs(velocity.y),
		std::abs(velocity.x)
	);

	if (angle < minAngle)
	{
		float directionX = velocity.x < 0.0f ? -1.0f : 1.0f;
		float directionY = velocity.y < 0.0f ? -1.0f : 1.0f;

		velocity.x = directionX * std::cos(minAngle) * speed;
		velocity.y = directionY * std::sin(minAngle) * speed;

		set_velocity(velocity);
	}
}