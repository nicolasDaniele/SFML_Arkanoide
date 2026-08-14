#pragma once

#include <vector>
#include <string>
#include <optional>
#include <SFML/Graphics.hpp>

class Block;

struct Level
{
    std::vector<std::string> layout;
};

class LevelManager
{
private:
	LevelManager();
    ~LevelManager() { }
    sf::Color from_hsv(float hue, float saturation, float value);

	static LevelManager* instance;
    std::vector<Level> levels;
    std::vector<Block*> blocks;

public:
    static LevelManager* get_instance()
    {
        if (instance == nullptr)
        {
            instance = new LevelManager();
        }
        return instance;
    }

    void load_level(int levelIndex);
    void draw(sf::RenderWindow* window);
    std::optional<sf::FloatRect> check_block_collision(sf::FloatRect bounds);
    bool is_level_complete() const;
    int get_num_levels() const;
};