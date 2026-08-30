#include <random>
#include <cmath>
#include <iostream>
#include "LevelManager.h"
#include "Block.h"

void LevelManager::init()
{
    if (!blockTexture.loadFromFile("Assets/Sprites/block.png"))
    {
        std::cout << "LevelManager: could not load block texture\n";
        return;
    }

    levels =
    {
        // LEVEL 1
        {
            {
                "###############",
                "###############",
                "###############",
                "###############",
                "###############",
                "###############",
                "###############",
                "###############"
            }
        },

        // LEVEL 2
        {
            {
                "......###......",
                ".....#####.....",
                "....#######....",
                "...#########...",
                "..###########..",
                ".#############.",
                "..###########..",
                "...#########...",
                "....#######....",
                ".....#####.....",
                "......###......"
            }
        },

        // LEVEL 3
        {
            {
                "UUUUUUUUUUUUUUU",
                "U#############U",
                "U##..#####..##U",
                "U##..#####..##U",
                "U#####UUU#####U",
                "U#####UUU#####U",
                "U##..#####..##U",
                "U##..#####..##U",
                "U#############U"
            }
        },

        // LEVEL 4
        {
            {
                "UUUUUUUUUUUUUUU",
                "U###UUUUUUU###U",
                "U####UUUUU####U",
                "U#####UUU#####U",
                "U######U######U",
                "U#############U",
                "U#############U",
                "UUUUU#####UUUUU"
            }
        },

        // LEVE 5
        {
            {
                "UUUUUUUUUUUUUUU",
                "U#############U",
                "UUUUUUUUUUUUU#U",
                "U#############U",
                "U#UUUUUUUUUUUUU",
                "U#############U",
                "UUUUUUUUUUUUU#U",
                "##############U",
                "#UUUUUUUUUUUUUU"
            }
        },

        // LEVEL 6
        {
            {
                "###############",
                "##UUU#####UUU##",
                "#UUUUUU#UUUUUU#",
                "#UUUUUUUUUUUUU#",
                "#UUUUUUUUUUUUU#",
                "###UUUUUUUUU###",
                "#####UUUUU#####",
                "######UUU######",
                "#######U#######",
                "###############"
            }
        },

        // LEVEL 7
        {
            {
                "###UUUUUUUUU###",
                "U###UUUUUUU###U",
                "UUU###UUU###UUU",
                "UUUUU#####UUUUU",
                "UUU###UUU###UUU",
                "U###UUUUUUU###U",
                "###UUUUUUUUU###"
            }
        }
    };
}

void LevelManager::load_level(int levelIndex)
{
    if (levelIndex < 0 || levelIndex >= static_cast<int>(levels.size()))
    {
        return;
    }

    for (Block* block : blocks)
    {
        delete block;
    }

    blocks.clear();

    const Level& level = levels[levelIndex];

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> distrib_s(0.8f, 1.0f);
    std::uniform_real_distribution<float> distrib_v(0.4f, 1.0f);
    std::uniform_real_distribution<float> distrib_powerup(0.0f, 1.0f);

    sf::Vector2f offsetPos(3, 50);

    for (int i = 0; i < static_cast<int>(level.layout.size()); i++)
    {
        float hue = (360.0f / level.layout.size()) * i;
        float s = distrib_s(gen);
        float v = distrib_v(gen);

        sf::Color rowColor = from_hsv(hue, s, v);

        for (int j = 0; j < static_cast<int>(level.layout[i].size()); j++)
        {
            BlockType blockType = get_block_type(level.layout[i][j]);
            if (blockType == BlockType::Empty)
                continue;

            bool dropsPowerUp = (blockType == BlockType::Normal) &&
                (distrib_powerup(gen) < powerUpChance);

            Block* block = create_block(blockType, rowColor, { 0.0f, 0.0f }, dropsPowerUp);
            if (block == nullptr)
                continue;

            float blockPosX = block->get_texture_rect().x * 1.1f * j;
            float blockPosY = block->get_texture_rect().y * 1.1f * i;

            block->set_position(offsetPos + sf::Vector2f(blockPosX, blockPosY));

            blocks.push_back(block);
        }
    }
}

Block* LevelManager::create_block(BlockType type, sf::Color color, sf::Vector2f position, bool dropsPowerUp)
{
    switch (type)
    {
    case BlockType::Normal:
        return new Block(blockTexture, position, 
            color, { 1.f, 1.f }, true, dropsPowerUp);

    case BlockType::Unbreakable:
        return new Block(blockTexture, position, 
            sf::Color::White, { 1.f, 1.f }, false, false);

    default:
        return nullptr;
    }
}

void LevelManager::draw(sf::RenderWindow* window)
{
    for (Block* block : blocks)
    {
        block->draw(window);
    }
}

BlockType LevelManager::get_block_type(char symbol)
{
    switch (symbol)
    {
    case '#':
        return BlockType::Normal;

    case 'U':
        return BlockType::Unbreakable;

    default:
        return BlockType::Empty;
    }
}

sf::Color LevelManager::from_hsv(float hue, float saturation, float value)
{
    float c = value * saturation;
    float x = c * (1.0f - std::abs(std::fmod(hue / 60.0f, 2.0f) - 1.0f));
    float m = value - c;
    float r = 0, g = 0, b = 0;

    if (hue >= 0 && hue < 60) { r = c; g = x; b = 0; }
    else if (hue >= 60 && hue < 120) { r = x; g = c; b = 0; }
    else if (hue >= 120 && hue < 180) { r = 0; g = c; b = x; }
    else if (hue >= 180 && hue < 240) { r = 0; g = x; b = c; }
    else if (hue >= 240 && hue < 300) { r = x; g = 0; b = c; }
    else { r = c; g = 0; b = x; }

    return sf::Color((r + m) * 255, (g + m) * 255, (b + m) * 255);
}

std::optional<BlockCollision> LevelManager::check_block_collision(sf::FloatRect bounds)
{
    for (int i = 0; i < static_cast<int>(blocks.size()); i++)
    {
        if (blocks[i]->get_sprite().getGlobalBounds().findIntersection(bounds).has_value())
        {
            BlockCollision collision;

            sf::FloatRect blockBounds = blocks[i]->get_sprite().getGlobalBounds();

            collision.blockBounds = blockBounds;
            collision.isBreakable = blocks[i]->is_breakable();
            collision.shouldSpawnPowerUp = blocks[i]->is_breakable() && blocks[i]->drops_power_up();

            if (blocks[i]->is_breakable())
            {
                delete blocks[i];
                blocks.erase(blocks.begin() + i);
            }

            return collision;
        }
    }

    return std::nullopt;
}

bool LevelManager::is_level_complete() const
{
    for (Block* block : blocks)
    {
        if (block->is_breakable())
            return false;
    }

    return true;
}

int LevelManager::get_num_levels() const
{
    return static_cast<int>(levels.size());
}

LevelManager::~LevelManager()
{
    for (Block* block : blocks)
    {
        delete block;
    }
}