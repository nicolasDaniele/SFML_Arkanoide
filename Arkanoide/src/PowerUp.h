#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"

enum class PowerUpType
{
	PaddleSpeed,
	PaddleWidth,
	ExtraLife,
	MultiBall
};

class PowerUp : public Entity
{
public:
	PowerUp(const sf::Texture& texture, sf::Vector2f _startPosition,
		PowerUpType _type, sf::Vector2f _scale);
	~PowerUp() = default;

	void update(float dt) override;

	PowerUpType get_type() const;

private:
	PowerUpType type;

	static constexpr float fallSpeed = 150.0f;
};
