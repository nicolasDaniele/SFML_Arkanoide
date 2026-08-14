#include <random>
#include "LevelManager.h"
#include "Block.h"

LevelManager::LevelManager()
{
    levels =
    {
        {
            {
                "###############",
                "###############",
                "###############",
                "###############",
                "###############",
                "###############",
                "###############",
                "###############",
                "###############"
            }
            /*{
                "...............",
                "...............",
                "...............",
                "...............",
                "...............",
                "...............",
                "...............",
                "...............",
                "........#......"
            }*/
        },

        {
            {
                "......####.....",
                ".....######....",
                "....########...",
                "...##########..",
                "..############.",
                "...##########..",
                "....########...",
                ".....######....",
                "......####....."
            }
        }
    };
}

void LevelManager::load_level(int levelIndex)
{
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

    sf::Vector2f offsetPos(3, 50);

    for (int i = 0; i < level.layout.size(); i++)
    {
        float hue = (360.0f / level.layout.size()) * i;

        float s = distrib_s(gen);
        float v = distrib_v(gen);

        sf::Color color = from_hsv(hue, s, v);

        for (int j = 0; j < level.layout[i].size(); j++)
        {
            if (level.layout[i][j] != '#')
                continue;

            Block* block = new Block(
                color,
                { 0, 0 },
                { 30, 15 }
            );

            float blockPosX = block->get_size().x * 1.1f * j;
            float blockPosY = block->get_size().y * 1.1f * i;

            block->set_position(
                offsetPos + sf::Vector2f(blockPosX, blockPosY)
            );

            blocks.push_back(block);
        }
    }
}

void LevelManager::draw(sf::RenderWindow* window)
{
    for (Block* block : blocks)
    {
        block->draw(window);
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

std::optional<sf::FloatRect> LevelManager::check_block_collision(sf::FloatRect bounds)
{
    for (int i = 0; i < blocks.size(); i++)
    {
        if (blocks[i]->get_rectangle().getGlobalBounds().intersects(bounds))
        {
            sf::FloatRect blockBounds = blocks[i]->get_rectangle().getGlobalBounds();

            delete blocks[i];
            blocks.erase(blocks.begin() + i);

            return blockBounds;
        }
    }

    return std::nullopt;
}

bool LevelManager::is_level_complete() const
{
    return blocks.empty();
}

int LevelManager::get_num_levels() const
{
    return levels.size();
}