#include "Paddle.h"

void Paddle::update(float dt)
{
    sprite.move(velocity * dt);
}

void Paddle::clamp_position(float width, float height)
{
    const float halfWidth = sprite.getGlobalBounds().size.x / 2.0f;
    const float minXPos = halfWidth;
    const float maxXPos = width - halfWidth;

    const sf::Vector2f position = sprite.getPosition();

    if (position.x < minXPos)
    {
        sprite.setPosition({ minXPos, position.y });
    }

    if (position.x > maxXPos)
    {
        sprite.setPosition({ maxXPos, position.y });
    }
}

void Paddle::reset()
{
    set_position(startPosition);
}