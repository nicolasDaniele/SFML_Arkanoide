#include <cstdlib>
#include <iostream>
#include "PowerUpManager.h"
#include "Paddle.h"

void PowerUpManager::init()
{
	load_texture(PowerUpType::PaddleSpeed, "Assets/Sprites/powerup_speed.png");
	load_texture(PowerUpType::PaddleWidth, "Assets/Sprites/powerup_width.png");
	load_texture(PowerUpType::ExtraLife, "Assets/Sprites/powerup_life.png");
	load_texture(PowerUpType::MultiBall, "Assets/Sprites/powerup_multiball.png");
}

void PowerUpManager::load_texture(PowerUpType type, const std::string& path)
{
	if (!powerUpTextures[static_cast<size_t>(type)].loadFromFile(path))
	{
		std::cout << "PowerUpManager: could not load power-up texture: " << path << "\n";
	}
}

void PowerUpManager::spawn(sf::Vector2f position)
{
	PowerUpType type = choose_random_type();

	PowerUp* newPowerUp = new PowerUp(get_texture_for_type(type), position, type, { 0.5f, 0.5f });

	fallingPowerUps.push_back(newPowerUp);
}

PowerUpType PowerUpManager::choose_random_type() const
{
	float totalWeight = 0.0f;

	for (float weight : spawnWeights)
	{
		totalWeight += weight;
	}

	float roll = (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * totalWeight;

	float cumulative = 0.0f;

	for (int i = 0; i < powerUpTypeCount; i++)
	{
		cumulative += spawnWeights[i];

		if (roll <= cumulative)
		{
			return static_cast<PowerUpType>(i);
		}
	}

	// Fallback in case of floating point rounding error.
	return static_cast<PowerUpType>(powerUpTypeCount - 1);
}

std::optional<PowerUpType> PowerUpManager::update(float dt,
	sf::RenderWindow* window, Paddle* paddle)
{
	std::optional<PowerUpType> collectedType = std::nullopt;

	for (auto it = fallingPowerUps.begin(); it != fallingPowerUps.end(); )
	{
		PowerUp* currentPowerUp = *it;
		currentPowerUp->update(dt);

		bool shouldRemove = false;

		if (currentPowerUp->get_sprite().getGlobalBounds().findIntersection(
			paddle->get_sprite().getGlobalBounds()).has_value())
		{
			collectedType = currentPowerUp->get_type();
			apply_effect(collectedType.value(), paddle);

			shouldRemove = true;
		}
		else if (currentPowerUp->get_sprite().getPosition().y >
			static_cast<float>(window->getSize().y))
		{
			shouldRemove = true;
		}

		if (shouldRemove)
		{
			delete currentPowerUp;
			it = fallingPowerUps.erase(it);
		}
		else
		{
			++it;
		}
	}

	return collectedType;
}

void PowerUpManager::draw(sf::RenderWindow* window)
{
	for (PowerUp* currentPowerUp : fallingPowerUps)
	{
		currentPowerUp->draw(window);
	}
}

void PowerUpManager::apply_effect(PowerUpType type, Paddle* paddle)
{
	switch (type)
	{
	case PowerUpType::PaddleSpeed:

		if (speedBoostStacks == 0)
		{
			basePaddleSpeed = paddle->get_current_speed();
		}

		if (speedBoostStacks < maxSpeedBoostStacks)
		{
			paddle->set_current_speed(paddle->get_current_speed() * speedBoostMultiplier);
			speedBoostStacks++;
		}

		break;

	case PowerUpType::PaddleWidth:

		if (widthBoostStacks == 0)
		{
			sf::Vector2f baseScale = paddle->get_scale();

			baseScaleX = baseScale.x;
			baseScaleY = baseScale.y;
		}

		if (widthBoostStacks < maxWidthBoostStacks)
		{
			sf::Vector2f currentScale = paddle->get_scale();

			paddle->set_scale(currentScale.x * widthBoostMultiplier, currentScale.y);
			widthBoostStacks++;
		}

		break;

	case PowerUpType::ExtraLife:
	case PowerUpType::MultiBall:

		// Resolved by Game.cpp using update()'s return value.
		break;
	}
}

void PowerUpManager::on_life_lost(Paddle* paddle)
{
	if (speedBoostStacks > 0)
	{
		paddle->set_current_speed(basePaddleSpeed);
		speedBoostStacks = 0;
	}

	if (widthBoostStacks > 0)
	{
		paddle->set_scale(baseScaleX, baseScaleY);
		widthBoostStacks = 0;
	}
}

void PowerUpManager::clear_falling_power_ups()
{
	for (PowerUp* currentPowerUp : fallingPowerUps)
	{
		delete currentPowerUp;
	}

	fallingPowerUps.clear();
}

void PowerUpManager::clear()
{
	clear_falling_power_ups();

	speedBoostStacks = 0;
	widthBoostStacks = 0;
}

const sf::Texture& PowerUpManager::get_texture_for_type(PowerUpType type) const
{
	return powerUpTextures[static_cast<size_t>(type)];
}

PowerUpManager::~PowerUpManager()
{
	for (PowerUp* currentPowerUp : fallingPowerUps)
	{
		delete currentPowerUp;
	}
}
