#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Entity
{
public:
	Entity(const std::string& _textureFilePath, const sf::Vector2f _startPosition,
		const sf::Vector2f startScale = { 1.f, 1.f });
	Entity(const sf::Texture& texture, const sf::Vector2f _startPosition);
	virtual ~Entity() = default;

	virtual void update(float dt) = 0;
	virtual void draw(sf::RenderWindow* window);
	virtual void set_position(const sf::Vector2f newPosition);
	void set_position(const float xPosition, const float yPosition);
	void set_scale(const float xScale, const float yScale);
	const sf::Sprite& get_sprite() const;
	const sf::Texture& get_texture() const;
	sf::Vector2f get_scale() const;
	sf::Vector2i get_texture_rect() const;
	sf::Vector2f get_start_position() const;

protected:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f startPosition;
	std::string textureFilePath;
};