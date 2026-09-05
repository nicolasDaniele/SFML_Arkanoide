#include <cstdlib>
#include <iostream>
#include "PowerUpManager.h"
#include "Paddle.h"
#include "LivesManager.h"

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

void PowerUpManager::spawn(sf::Vector2f position, int currentBallCount)
{
	std::optional<PowerUpType> type = choose_random_type(currentBallCount);

	if (!type.has_value())
	{
		return;
	}

	PowerUp* newPowerUp = new PowerUp(get_texture_for_type(type.value()), position, type.value());

	fallingPowerUps.push_back(newPowerUp);
}

std::optional<PowerUpType> PowerUpManager::choose_random_type(int currentBallCount) const
{
	float totalWeight = 0.0f;

	for (float weight : spawnWeights)
	{
		totalWeight += weight;
	}

	if (totalWeight <= 0.0f)
	{
		return std::nullopt;
	}

	float roll = (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * totalWeight;

	float cumulative = 0.0f;
	PowerUpType rolledType = static_cast<PowerUpType>(powerUpTypeCount - 1);

	for (int i = 0; i < powerUpTypeCount; i++)
	{
		cumulative += spawnWeights[i];

		if (roll <= cumulative && spawnWeights[i] > 0.0f)
		{
			rolledType = static_cast<PowerUpType>(i);
			break;
		}
	}

	// The roll landed on a type that's already at its cap: skip this spawn
	// entirely instead of redistributing its share among the other types.
	if (is_type_capped(rolledType, currentBallCount))
	{
		return std::nullopt;
	}

	return rolledType;
}

bool PowerUpManager::is_type_capped(PowerUpType type, int currentBallCount) const
{
	switch (type)
	{
	case PowerUpType::PaddleSpeed:
		return speedBoostStacks >= maxSpeedBoostStacks;

	case PowerUpType::PaddleWidth:
		return widthBoostStacks >= maxWidthBoostStacks;

	case PowerUpType::ExtraLife:
		return LivesManager::get_instance().is_at_max_lives();

	case PowerUpType::MultiBall:
		return get_multiball_multiplier(currentBallCount) == 0;

	default:
		return false;
	}
}

int PowerUpManager::get_multiball_multiplier(int currentBallCount) const
{
	if (currentBallCount <= 0)
	{
		return 0;
	}

	if (currentBallCount * multiballPrimaryMultiplier <= maxBallsOnScreen)
	{
		return multiballPrimaryMultiplier;
	}

	if (currentBallCount * multiballFallbackMultiplier <= maxBallsOnScreen)
	{
		return multiballFallbackMultiplier;
	}

	return 0;
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