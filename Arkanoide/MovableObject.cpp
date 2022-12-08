#include "MovableObject.h"

void MovableObject::add_move_vector(const sf::Vector2f vec)
{
    sprite.setPosition(sprite.getPosition() + velocity);
}

void MovableObject::set_velocity(const sf::Vector2f newVelocity)
{
    velocity = newVelocity;
}

void MovableObject::set_velocity(const float xVelocity, const float yVelocity)
{
    velocity.x = xVelocity;
    velocity.y = yVelocity;
}

sf::Vector2f MovableObject::get_velocity() const
{
    return velocity;
}