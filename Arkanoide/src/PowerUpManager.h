#pragma once

#include <array>
#include <vector>
#include <optional>
#include <SFML/Graphics.hpp>
#include "PowerUp.h"

class Paddle;

class PowerUpManager
{
public:
	static PowerUpManager& get_instance()
	{
		static PowerUpManager instance;
		return instance;
	}

	void init();
	void spawn(sf::Vector2f position);

	// Moves the falling power-ups, resolves collision with the paddle,
	// and applies PaddleSpeed/PaddleWidth directly. Returns the type collected
	// this frame (if any) so Game.cpp can resolve ExtraLife/MultiBall.
	std::optional<PowerUpType> update(float dt, sf::RenderWindow* window, Paddle* paddle);

	void draw(sf::RenderWindow* window);

	// Reverts the paddle boosts that last "until a life is lost".
	void on_life_lost(Paddle* paddle);

	// Clears only the power-ups currently falling (level transition / life lost).
	// Paddle boosts are NOT touched here: they should persist across a completed
	// level, and on life lost they're already reverted by on_life_lost().
	void clear_falling_power_ups();

	// Full game reset: falling power-ups + boost counters.
	void clear();

private:
	PowerUpManager() = default;
	~PowerUpManager();

	void apply_effect(PowerUpType type, Paddle* paddle);
	void load_texture(PowerUpType type, const std::string& path);
	const sf::Texture& get_texture_for_type(PowerUpType type) const;
	PowerUpType choose_random_type() const;

	static constexpr int powerUpTypeCount = 4;

	std::vector<PowerUp*> fallingPowerUps;
	std::array<sf::Texture, powerUpTypeCount> powerUpTextures;

	// Relative weight of each type when choosing which power-up to spawn (they don't
	// need to add up to 1, they get normalized automatically). Order = PowerUpType enum
	// order: PaddleSpeed, PaddleWidth, ExtraLife, MultiBall. Lower ExtraLife's to keep it rare.
	//std::array<float, powerUpTypeCount> spawnWeights = { 0.35f, 0.35f, 0.10f, 0.20f };
	std::array<float, powerUpTypeCount> spawnWeights = { 0.25f, 0.25f, 0.25f, 0.25f };

	// PaddleSpeed / PaddleWidth: stackable boosts, each one multiplies on top of the
	// previous one up to a cap, and only reset when a life is lost.
	int speedBoostStacks = 0;
	int widthBoostStacks = 0;

	static constexpr int maxSpeedBoostStacks = 3;
	static constexpr int maxWidthBoostStacks = 3;

	static constexpr float speedBoostMultiplier = 1.2f;
	static constexpr float widthBoostMultiplier = 1.2f;

	float basePaddleSpeed = 0.0f;
	float baseScaleX = 0.0f;
	float baseScaleY = 0.0f;
};
