#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Entity.h"

class Block : public Entity
{
public:
	Block(const std::string& textureFilePath, const sf::Vector2f _startPosition,
		const sf::Color color, const sf::Vector2f scale = { 1.f, 1.f }, bool _isBreakable = true);
	~Block() = default;

	void update(float dt) override {}
	bool is_breakable() const;

private:
	bool isBreakable;
};