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

struct BlockCollision
{
    sf::FloatRect blockBounds;
    bool isBreakable;
};

enum class BlockType
{
    Empty,
    Normal,
    Unbreakable
};

class LevelManager
{
private:
    LevelManager() = default;
    ~LevelManager();

    BlockType get_block_type(char symbol);
    Block* create_block(BlockType type, sf::Color color, sf::Vector2f position);
    sf::Color from_hsv(float hue, float saturation, float value);

    std::vector<Level> levels;
    std::vector<Block*> blocks;

public:
    static LevelManager& get_instance()
    {
        static LevelManager instance;
        return instance;
    }

    void init();
    void load_level(int levelIndex);
    void draw(sf::RenderWindow* window);
    std::optional<BlockCollision> check_block_collision(sf::FloatRect bounds);
    bool is_level_complete() const;
    int get_num_levels() const;
};