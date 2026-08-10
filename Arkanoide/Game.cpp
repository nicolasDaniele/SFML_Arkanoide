#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "Block.h"
#include "ScoreManager.h"
#include "Label.h"

using namespace std;

sf::RenderWindow* window; 
Paddle* paddle;
Ball* ball;
const int blocksRows = 9;
const int blocksColumns = 15;
vector<Block*> blocks;

const int maxLives = 3;
int lives;
float currentTime;
float prevTime;

Label* scoreLabel;
Label* livesLabel;
Label* gameOverLabel;
Label* countdownLabel;
Label* resetLabel;
sf::Font font;

ScoreManager* ScoreManager::instance = nullptr;

// Ball speed incrementation variables
float speedIncreaseTimer = 0.0f;
const float speedIncreaseInterval = 5.0f;
const float speedMultiplier = 1.1f;

enum GameState
{
    COUNTDOWN,
    PLAYING,
    GAMEOVER
};

GameState state;

void init();
void handle_inputs(sf::Event ev);
void update(float dt);
sf::Color from_hsv(float hue, float saturation, float value);
bool check_collision(sf::FloatRect rect1, sf::FloatRect rect2);
void draw();
void reset();

int main()
{
    window = new sf::RenderWindow(sf::VideoMode(500, 600), "Arkanoide");
    window->setFramerateLimit(60);
 
    sf::Clock clock;

    lives = maxLives;
    currentTime = 0.0f;
    prevTime = 0.0f;

    state = GameState::COUNTDOWN;

    srand(time(NULL));
    init();
    
    while (window->isOpen())
    {
        sf::Event ev;
        while (window->pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window->close();
            }

            handle_inputs(ev);
        }

        sf::Time dt = clock.restart();
        if (state != GameState::GAMEOVER)
        {
            update(dt.asSeconds());
        }

        window->clear();
        draw();
        window->display();
    }
}

void init()
{

    // Blocks initialization
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distrib_s(0.8f, 1.0f);
    std::uniform_real_distribution<float> distrib_v(0.4f, 1.0f);

    for (int i = 0; i < blocksRows; i++)
    {
        float hue = (360.0f / blocksRows) * i;

        float s = distrib_s(gen);
        float v = distrib_v(gen);
        sf::Color color = from_hsv(hue, s, v);

        for (int j = 0; j < blocksColumns; j++)
        {
            sf::Vector2f offsetPos(3, 50);
            Block* block = new Block(color, { 0, 0 }, { 30, 15 });
            float blockPosX = (block->get_size().x) * 1.1f * j;
            float blockPosY = (block->get_size().y) * 1.1f * i;
            block->set_position(offsetPos + sf::Vector2f(blockPosX, blockPosY));
            blocks.push_back(block);
        }
    }


    // Paddle initialization
    string paddleTexturePath = "Assets/Sprites/paddle.png";
    sf::Vector2f paddlePos(225, 500);
    paddle = new Paddle(paddleTexturePath, paddlePos, 200.0f);

    // Ball initialization
    string ballTexturePath = "Assets/Sprites/ball.png";
    sf::Vector2f ballPos(225, 300);
    ball = new Ball(ballTexturePath, ballPos, 200.0f);
    ball->set_velocity(sf::Vector2f(0, ball->get_current_speed()));
    ball->set_max_speed(1800.0f);
    ball->set_scale(2, 2);

    // Labels initialization
    if (!font.loadFromFile("Assets/Fonts/Pixellari.ttf"))
    {
        cout << "Could not load font" << endl;
    }
    // ScoreLabel
    scoreLabel = new Label(font, 20, sf::Vector2f(10, 10),
        sf::Color::White, "Score: 0", false);
    // LivesLabel
    livesLabel = new Label(font, 20, sf::Vector2f(400, 10),
        sf::Color::White, "Lives: 3", false);
    // GameOverLabel
    float centerPosX = (float)(window->getSize().x / 2);
    float centerPosY = (float)(window->getSize().y / 2);
    gameOverLabel = new Label(font, 40, sf::Vector2f(centerPosX, centerPosY),
        sf::Color::White, "GAME OVER", true);
    // CountdownLabel
    countdownLabel = new Label(font, 40, sf::Vector2f(centerPosX, centerPosY),
        sf::Color::White, "READY", true);
    // ResetLabel
    resetLabel = new Label(font, 25, sf::Vector2f(centerPosX, centerPosY + 80),
        sf::Color::White, "", true);
}

void handle_inputs(sf::Event ev)
{
    if (ev.type == sf::Event::KeyPressed)
    {
        if (state == GameState::PLAYING)
        {
            if (ev.key.code == sf::Keyboard::A ||
                ev.key.code == sf::Keyboard::Left)
            {
                paddle->set_velocity(-paddle->get_current_speed(), 0);
            }

            if (ev.key.code == sf::Keyboard::D ||
                ev.key.code == sf::Keyboard::Right)
            {
                paddle->set_velocity(paddle->get_current_speed(), 0);
            }
        }
        

        if (ev.key.code == sf::Keyboard::Space && state == GameState::GAMEOVER)
        {
            reset();
        }
    }
    if (ev.type == sf::Event::KeyReleased)
    {
        paddle->set_velocity(0, 0);
    }
}

void update(float dt)
{
    currentTime += dt;
    if (state == GameState::COUNTDOWN)
    {
        countdownLabel->set_string("READY");
        if (currentTime >= prevTime + 1.0f)
        {
            countdownLabel->set_string("GO!");
            if (currentTime >= prevTime + 2.0f)
            {
                state = GameState::PLAYING;
                prevTime = currentTime;
            }
        }

        return;
    }

    paddle->update(dt);
    paddle->clamp_position(window);
    ball->update(dt);
    ball->clamp_position(window);


    // Ball speed incrementation
    speedIncreaseTimer += dt;
    if (speedIncreaseTimer >= speedIncreaseInterval)
    {
        speedIncreaseTimer -= speedIncreaseInterval;

        if (!ball->is_at_max_speed())
        {
            float newSpeed = ball->get_current_speed() * speedMultiplier;
            ball->set_current_speed(newSpeed);
        }
    }


    // Paddle-Ball collision
    if (check_collision(paddle->get_sprite().getGlobalBounds(), 
        ball->get_sprite().getGlobalBounds()))
    {
        float newBallXVelocity = rand() % (int)ball->get_current_speed();
        sf::Vector2f newBallVelocity(newBallXVelocity, -ball->get_velocity().y);
        ball->set_velocity(newBallVelocity);
    }

    // Block-Ball collision
    for (int i = 0; i < blocks.size(); i++)
    {
        if (check_collision(blocks[i]->get_rectangle().getGlobalBounds(), 
            ball->get_sprite().getGlobalBounds()))
        {
            float newBallXVelocity = rand() % (int)ball->get_current_speed();
            sf::Vector2f newBallVelocity(newBallXVelocity, -ball->get_velocity().y);
            ball->set_velocity(newBallVelocity);

            delete(blocks[i]);
            blocks.erase(blocks.begin() + i);

            ScoreManager::get_instance()->add_to_score(10);
            scoreLabel->set_string("Score: " + to_string(ScoreManager::get_instance()->get_score()));
        }
    }

    // Loose life
    if (ball->get_sprite().getPosition().y > window->getSize().y)
    {
        lives--;
        livesLabel->set_string("Lives: " + to_string(lives));

        if (lives < 1)
        {
            state = GameState::GAMEOVER;
        }
        else
        {
            ball->set_position(ball->get_start_position());
            ball->set_current_speed(ball->get_initial_speed());
            paddle->set_position(paddle->get_start_position());
            state = GameState::COUNTDOWN;

            currentTime = 0.0f;
            prevTime = 0.0f;
        }
    }
}

sf::Color from_hsv(float hue, float saturation, float value)
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


bool check_collision(sf::FloatRect rect1, sf::FloatRect rect2)
{
    return rect1.intersects(rect2);
}

void draw()
{
    for (Block* block : blocks)
    {
        if (block)
        {
            block->draw(window);
        }
    }

    if (state == GameState::GAMEOVER)
    {
        resetLabel->set_string("\t\tYour Score: " + to_string(ScoreManager::get_instance()->get_score()) +
            "\n\nPress SpaceBar to Reset");

        gameOverLabel->draw(window);
        resetLabel->draw(window);
    }
    if (state == GameState::COUNTDOWN)
    {
        countdownLabel->draw(window);
    }

    scoreLabel->draw(window);
    livesLabel->draw(window);

    if (state == GameState::PLAYING)
    {
        ball->draw(window);
    }

    paddle->draw(window);
}

void reset()
{
    paddle->set_position(paddle->get_start_position());
    ball->set_position(ball->get_start_position());

    currentTime = 0.0f;
    prevTime = 0.0f;
    lives = maxLives;
    ScoreManager::get_instance()->reset_score();

    livesLabel->set_string("Lives: " + to_string(lives));
    scoreLabel->set_string("Score: " + to_string(ScoreManager::get_instance()->get_score()));

    state = GameState::COUNTDOWN;
}