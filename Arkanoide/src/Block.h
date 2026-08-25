#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Entity.h"

class Block : public Entity
{
public:
	Block(const sf::Texture& texture, const sf::Vector2f _startPosition,
		const sf::Color color, const sf::Vector2f startScale = { 1.0f, 1.0f },
		bool _isBreakable = true, bool _dropsPowerUp = false);
	~Block() = default;

	void update(float dt) override {}
	bool is_breakable() const;
	bool drops_power_up() const;

private:
	bool isBreakable;
	bool dropsPowerUp;
};