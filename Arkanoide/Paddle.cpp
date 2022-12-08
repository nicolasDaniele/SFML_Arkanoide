#include "Paddle.h"

void Paddle::add_move_vector(sf::Vector2f vec)
{
    sprite.setPosition(sprite.getPosition() + vec);
}

sf::Sprite Paddle::get_sprite()
{
    return sprite;
}