#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Paddle.h"
#include "Ball.h"
#include "Block.h"
#include "ScoreManager.h"
#include "LevelManager.h"
#include "SoundManager.h"
#include "Label.h"

sf::RenderWindow* window;
Paddle* paddle;
Ball* ball;

const int maxLives = 5;
const int subSteps = 4;
int lives;
float currentTime;
float prevTime;
int currentLevel = 0;

Label* scoreLabel;
Label* livesLabel;
Label* gameOverLabel;
Label* countdownLabel;
Label* resetLabel;
Label* gameCompleteLabel;

std::optional<sf::Font> font;

// Paddle movement variables
bool movingLeft = false;
bool movingRight = false;

// Ball speed incrementation variables
float speedIncreaseTimer = 0.0f;
const float speedIncreaseInterval = 5.0f;
const float speedMultiplier = 1.1f;

enum class GameState
{
    COUNTDOWN,
    PLAYING,
    GAME_OVER,
    GAME_COMPLETE
};

GameState state;

void init();
void handle_inputs(const sf::Event& ev);
void update(float dt);
bool check_collision(const sf::FloatRect& rect1, const sf::FloatRect& rect2);
void draw();
void reset();
void finish_game();


int main()
{
    window = new sf::RenderWindow( sf::VideoMode({ 450, 600 }), "Arkanoide");

    window->setFramerateLimit(60);

    sf::Clock clock;

    lives = maxLives;
    currentTime = 0.0f;
    prevTime = 0.0f;

    state = GameState::COUNTDOWN;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    init();

    while (window->isOpen())
    {
        while (const std::optional event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window->close();
            }

            handle_inputs(*event);

            // Update Paddle velocity
            if (movingLeft && !movingRight)
            {
                paddle->set_velocity(-paddle->get_current_speed(), 0);
            }
            else if (movingRight && !movingLeft)
            {
                paddle->set_velocity(paddle->get_current_speed(), 0);
            }
            else
            {
                paddle->set_velocity(0, 0);
            }
        }

        sf::Time dt = clock.restart();

        if (state != GameState::GAME_OVER)
        {
            update(dt.asSeconds());
        }

        window->clear();
        draw();
        window->display();
    }

    finish_game();

    return 0;
}


void init()
{
    float centerX = static_cast<float>(window->getSize().x) / 2.0f;
    float centerY = static_cast<float>(window->getSize().y) / 2.0f;

    // Paddle initialization
    std::string paddleTexturePath = "Assets/Sprites/paddle.png";
    sf::Vector2f paddlePos(centerX, 550.0f);
    paddle = new Paddle(paddleTexturePath, paddlePos, 250.0f);

    // Ball initialization
    std::string ballTexturePath = "Assets/Sprites/ball.png";
    sf::Vector2f ballPos(centerX, 300.0f);
    ball = new Ball(ballTexturePath, ballPos, 200.0f);
    ball->set_velocity(sf::Vector2f(0.0f, ball->get_current_speed()));
    ball->set_max_speed(1800.0f);
    ball->set_scale(0.25f, 0.25f);

    // Font initialization
    try
    {
        font.emplace("Assets/Fonts/ARCADE_I.TTF");
    }
    catch (const sf::Exception& e)
    {
        std::cerr << "Could not load font: " << e.what() << "\n";
        return;
    }

    // ScoreLabel
    scoreLabel = new Label(*font, 15, sf::Vector2f(10.0f, 10.0f), 
                    sf::Color::White, "Score:0", false);

    // LivesLabel
    livesLabel = new Label(*font, 15, sf::Vector2f(320.0f, 10.0f),
                    sf::Color::White, "Lives:" + std::to_string(lives), false);

    // GameOverLabel
    gameOverLabel = new Label(*font, 35, sf::Vector2f(centerX, centerY - 60.0f),
                        sf::Color::White, "GAME OVER", true);

    // CountdownLabel
    countdownLabel = new Label(*font, 40, sf::Vector2f(centerX, centerY),
                        sf::Color::White, "READY", true);

    // ResetLabel
    resetLabel = new Label(*font, 16, sf::Vector2f(centerX, centerY + 80.0f),
                    sf::Color::White, "", true);

    // GameCompleteLabel
    gameCompleteLabel = new Label(*font, 26, sf::Vector2f(centerX, centerY - 150.0f),
                        sf::Color::White, " GAME COMPLETE!\nCONGRATULATIONS!", true);

    // Init Managers
    SoundManager::get_instance().init();
    ScoreManager::get_instance().init();
    LevelManager::get_instance().init();

    LevelManager::get_instance().load_level(0);
}


void handle_inputs(const sf::Event& ev)
{
    if (const auto* keyPressed = ev.getIf<sf::Event::KeyPressed>())
    {
        if (state == GameState::PLAYING)
        {
            if (keyPressed->code == sf::Keyboard::Key::A ||
                keyPressed->code == sf::Keyboard::Key::Left)
            {
                movingLeft = true;
            }

            if (keyPressed->code == sf::Keyboard::Key::D ||
                keyPressed->code == sf::Keyboard::Key::Right)
            {
                movingRight = true;
            }
        }

        if (keyPressed->code == sf::Keyboard::Key::Space &&
            (state == GameState::GAME_OVER ||
                state == GameState::GAME_COMPLETE))
        {
            reset();
        }
    }

    if (const auto* keyReleased = ev.getIf<sf::Event::KeyReleased>())
    {
        if (keyReleased->code == sf::Keyboard::Key::A ||
            keyReleased->code == sf::Keyboard::Key::Left)
        {
            movingLeft = false;
        }

        if (keyReleased->code == sf::Keyboard::Key::D ||
            keyReleased->code == sf::Keyboard::Key::Right)
        {
            movingRight = false;
        }
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

    // Substeps to avoid tunneling
    float subDt = dt / static_cast<float>(subSteps);

    for (int i = 0; i < subSteps; ++i)
    {
        paddle->update(subDt);
        paddle->clamp_position(window);

        ball->update(subDt);
        ball->clamp_position(window);

        // Paddle-Ball collision
        if (check_collision(
            paddle->get_sprite().getGlobalBounds(),
            ball->get_sprite().getGlobalBounds()))
        {
            ball->ricochet(paddle);

             SoundManager::get_instance().play_boop();
        }

        // Block-Ball collision
        auto blockCollision =
            LevelManager::get_instance().check_block_collision(
                ball->get_sprite().getGlobalBounds());

        if (blockCollision.has_value())
        {
            ball->bounce_from(blockCollision.value().blockBounds);

            if (blockCollision.value().isBreakable)
            {
                ScoreManager::get_instance().add_to_score(10);

                scoreLabel->set_string("Score:" + 
                    std::to_string(ScoreManager::get_instance().get_score()));

                 SoundManager::get_instance().play_beep();
            }
            else
            {
                 SoundManager::get_instance().play_cling();
            }
        }
    }

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

    // Check for level complete
    if (LevelManager::get_instance().is_level_complete())
    {
        currentLevel++;

        if (currentLevel >= LevelManager::get_instance().get_num_levels())
        {
            state = GameState::GAME_COMPLETE;
            return;
        }
        else
        {
            LevelManager::get_instance().load_level(currentLevel);

            ball->reset();
            paddle->set_position(paddle->get_start_position());

            state = GameState::COUNTDOWN;

            currentTime = 0.0f;
            prevTime = 0.0f;
            speedIncreaseTimer = 0.0f;
        }
    }

    // Lose life
    if (ball->get_sprite().getPosition().y >
        static_cast<float>(window->getSize().y))
    {
        SoundManager::get_instance().play_lose();

        lives--;
        livesLabel->set_string("Lives:" + std::to_string(lives));

        if (lives < 1)
        {
            state = GameState::GAME_OVER;
            return;
        }
        else
        {
            paddle->reset();
            ball->reset();

            state = GameState::COUNTDOWN;

            currentTime = 0.0f;
            prevTime = 0.0f;
            speedIncreaseTimer = 0.0f;
        }
    }
}


bool check_collision(const sf::FloatRect& rect1, const sf::FloatRect& rect2)
{
    return rect1.findIntersection(rect2).has_value();
}


void draw()
{
    LevelManager::get_instance().draw(window);

    if (state == GameState::GAME_OVER)
    {
        resetLabel->set_string("\t Your Score:" +
            std::to_string(ScoreManager::get_instance().get_score()) +
            "\n\nPress SpaceBar to Reset");

        gameOverLabel->draw(window);
        resetLabel->draw(window);

        return;
    }
    else if (state == GameState::GAME_COMPLETE)
    {
        resetLabel->set_string("\t Your Score:" +
            std::to_string(ScoreManager::get_instance().get_score()) +
            "\n\nPress SpaceBar to Reset");

        resetLabel->draw(window);
        gameCompleteLabel->draw(window);

        return;
    }
    else if (state == GameState::COUNTDOWN)
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
    paddle->reset();
    ball->reset();

    currentTime = 0.0f;
    prevTime = 0.0f;
    lives = maxLives;

    ScoreManager::get_instance().reset_score();

    livesLabel->set_string("Lives:" + std::to_string(lives));

    scoreLabel->set_string("Score:" +
        std::to_string(ScoreManager::get_instance().get_score()));

    LevelManager::get_instance().load_level(0);

    currentLevel = 0;
    movingLeft = false;
    movingRight = false;
    speedIncreaseTimer = 0.0f;

    state = GameState::COUNTDOWN;
}


void finish_game()
{
    delete window;
    delete paddle;
    delete ball;

    delete scoreLabel;
    delete livesLabel;
    delete gameOverLabel;
    delete countdownLabel;
    delete resetLabel;
    delete gameCompleteLabel;
}