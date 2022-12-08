#include "Entity.h"

Entity::Entity(string textureFilePath, sf::Vector2f _startPosition)
{
    if (!texture.loadFromFile(textureFilePath))
    {
        cout << "ERROR: paddle_texture could not be loaded. " << endl;
    }

    sprite.setTexture(texture);
    startPosition = _startPosition;
    sprite.setPosition(_startPosition);
}

void Entity::set_scale(const float xScale, const float yScale)
{
    sprite.setScale(xScale, yScale);
}

void Entity::set_position(const sf::Vector2f newPosition)
{
    sprite.setPosition(newPosition);
}
void Entity::set_position(const float xPosition, const float yPosition)
{
    sprite.setPosition(xPosition, yPosition);
}

sf::Sprite Entity::get_sprite() const
{
    return sprite;
}

sf::Texture Entity::get_texture() const
{
    return  texture;
}

sf::Vector2f Entity::get_start_position() const
{
    return startPosition;
}