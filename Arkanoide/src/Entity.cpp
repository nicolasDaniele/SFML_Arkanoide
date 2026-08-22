#include "Entity.h"

#include <iostream>

Entity::Entity(const std::string& _textureFilePath, const sf::Vector2f _startPosition,
	const sf::Vector2f startScale)	: textureFilePath(_textureFilePath), texture(), sprite(texture)
{
	texture = sf::Texture(textureFilePath);

	if (!texture.loadFromFile(textureFilePath))
	{
		std::cout << "ERROR: could not load texture file: "
			<< textureFilePath << "\n";
	}

	sprite.setTexture(texture);
	startPosition = _startPosition;

	sprite.setTextureRect(sf::IntRect({ 0, 0 },
			{
				static_cast<int>(texture.getSize().x),
				static_cast<int>(texture.getSize().y)
			}));

	sprite.setPosition(_startPosition);
	sprite.setScale(startScale);
}

Entity::Entity(const sf::Texture& texture, const sf::Vector2f _startPosition)
	: texture(texture), sprite(texture), startPosition(_startPosition)
{
	sprite.setTextureRect(sf::IntRect({ 0, 0 },
			{
				static_cast<int>(texture.getSize().x),
				static_cast<int>(texture.getSize().y)
			}));

	sprite.setPosition(_startPosition);
}

void Entity::draw(sf::RenderWindow* window)
{
	window->draw(sprite);
}

void Entity::set_scale(const float xScale, const float yScale)
{
	sprite.setScale(sf::Vector2f(xScale, yScale));
}

void Entity::set_position(const sf::Vector2f newPosition)
{
	sprite.setPosition(newPosition);
}

void Entity::set_position(const float xPosition, const float yPosition)
{
	sprite.setPosition(sf::Vector2f(xPosition, yPosition));
}

const sf::Sprite& Entity::get_sprite() const
{
	return sprite;
}

const sf::Texture& Entity::get_texture() const
{
	return texture;
}

sf::Vector2f Entity::get_scale() const
{
	return sprite.getScale();
}

sf::Vector2i Entity::get_texture_rect() const
{
	return sprite.getTextureRect().size;
}

sf::Vector2f Entity::get_start_position() const
{
	return startPosition;
}