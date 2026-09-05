#pragma once

#include <SFML/Graphics.hpp>
#include "MovableObject.h"

class Paddle : public MovableObject
{
public:
    Paddle(const std::string& textureFilePath, sf::Vector2f startPosition,
        float initialSpeed) : MovableObject(textureFilePath, startPosition, initialSpeed) { }
    ~Paddle() override = default;

    void update(float dt) override;
    void clamp_position(float width, float height) override;
    void reset() override;
};