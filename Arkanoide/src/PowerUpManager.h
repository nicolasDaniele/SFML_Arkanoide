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

	// currentBallCount: how many balls are in play right now, needed to gate
	// MultiBall (and to pick its adaptive multiplier - see get_multiball_multiplier).
	void spawn(sf::Vector2f position, int currentBallCount);

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

	// Adaptive MultiBall multiplier for the given current ball count: tries x3 first,
	// falls back to x2 if x3 would overflow maxBallsOnScreen, or 0 if not even x2 fits
	// (meaning MultiBall shouldn't spawn/apply at all right now). Game.cpp calls this
	// both to know how many clones to create on collection, and PowerUpManager calls
	// it internally (via is_type_capped) to decide spawn eligibility.
	int get_multiball_multiplier(int currentBallCount) const;

private:
	PowerUpManager() = default;
	~PowerUpManager();

	void apply_effect(PowerUpType type, Paddle* paddle);
	void load_texture(PowerUpType type, const std::string& path);
	const sf::Texture& get_texture_for_type(PowerUpType type) const;
	std::optional<PowerUpType> choose_random_type(int currentBallCount) const;
	bool is_type_capped(PowerUpType type, int currentBallCount) const;

	static constexpr int powerUpTypeCount = 4;

	std::vector<PowerUp*> fallingPowerUps;
	std::array<sf::Texture, powerUpTypeCount> powerUpTextures;

	// Relative weight of each type when choosing which power-up to spawn (they don't
	// need to add up to 1, they get normalized automatically). Order = PowerUpType enum
	// order: PaddleSpeed, PaddleWidth, ExtraLife, MultiBall. Lower ExtraLife's to keep it rare.
	// NOTE: these proportions stay fixed even when a type is capped (see
	// choose_random_type/is_type_capped) - a roll that lands on a capped type is
	// simply discarded (no power-up spawns) instead of being redistributed among
	// the others, so ExtraLife/MultiBall's relative share doesn't change over time.
	std::array<float, powerUpTypeCount> spawnWeights = { 0.40f, 0.40f, 0.05f, 0.15f };

	// PaddleSpeed / PaddleWidth: stackable boosts, each one multiplies on top of the
	// previous one up to a cap, and only reset when a life is lost.
	int speedBoostStacks = 0;
	int widthBoostStacks = 0;

	static constexpr int maxSpeedBoostStacks = 4;
	static constexpr int maxWidthBoostStacks = 3;

	static constexpr float speedBoostMultiplier = 1.35f;
	static constexpr float widthBoostMultiplier = 1.25f;

	float basePaddleSpeed = 0.0f;
	float baseScaleX = 0.0f;
	float baseScaleY = 0.0f;

	// MultiBall: hard cap on total balls on screen, plus the two multipliers it tries
	// in order (see get_multiball_multiplier).
	static constexpr int maxBallsOnScreen = 36;
	static constexpr int multiballPrimaryMultiplier = 3;
	static constexpr int multiballFallbackMultiplier = 2;
};
