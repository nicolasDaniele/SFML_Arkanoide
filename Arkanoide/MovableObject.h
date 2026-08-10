#pragma once

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Entity.h"

using namespace std;

class MovableObject : public Entity
{
protected:
	sf::Vector2f velocity;
	float initialSpeed;
	float currentSpeed = 200.0f;
	float maxSpeed = 500.0f;

public:
	MovableObject(string textureFilePath, sf::Vector2f _startPosition
		, float _initialSpeed) :
		Entity(textureFilePath, _startPosition) {
		initialSpeed = currentSpeed = _initialSpeed;
	}
	virtual ~MovableObject() { }

	sf::Vector2f get_velocity() const;
	void add_move_vector(const sf::Vector2f vec);
	void set_velocity(const sf::Vector2f newVelocity);
	void set_velocity(const float xVelocity, const float yVelocity);
	void set_current_speed(const float newSpeed);
	float get_current_speed() const;
	float get_initial_speed() const;
	bool is_at_max_speed() const;
	void set_max_speed(const float newMaxSpeed);
	void clamp_velocity();
	virtual void clamp_position(sf::RenderWindow* window) = 0;
};