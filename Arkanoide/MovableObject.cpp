#include "MovableObject.h"

void MovableObject::add_move_vector(/*const sf::Vector2f vec*/)
{
    sprite.setPosition(sprite.getPosition() + velocity);
}

void MovableObject::set_velocity(const sf::Vector2f newVelocity)
{
    velocity = newVelocity;

    clamp_velocity();
}

void MovableObject::set_velocity(const float xVelocity, const float yVelocity)
{
    velocity.x = xVelocity;
    velocity.y = yVelocity;

    clamp_velocity();
}

void MovableObject::set_current_speed(const float newSpeed)
{
    currentSpeed = newSpeed;
}

float MovableObject::get_current_speed() const
{
    return currentSpeed;
}

float MovableObject::get_initial_speed() const
{
    return initialSpeed;
}

bool MovableObject::is_at_max_speed() const
{
    return currentSpeed >= maxSpeed;
}

void MovableObject::set_max_speed(const float newMaxSpeed)
{
    maxSpeed = newMaxSpeed;
}

void MovableObject::clamp_velocity()
{
    if (velocity.x > maxSpeed)
        velocity.x = maxSpeed;

    if (velocity.y > maxSpeed)
        velocity.y = maxSpeed;
}

sf::Vector2f MovableObject::get_velocity() const
{
    return velocity;
}