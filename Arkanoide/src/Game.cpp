#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include "Paddle.h"
#include "Ball.h"
#include "Block.h"
#include "ScoreManager.h"
#include "LevelManager.h"
#include "SoundManager.h"
#include "LivesManager.h"
#include "PowerUpManager.h"
#include "Label.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

sf::RenderWindow* window;
Paddle* paddle;
std::vector<Ball*> balls;

const int GAME_WIDTH = 450;
const int GAME_HEIGHT = 600;

sf::Clock gameClock;

// Ball spawn parameters (used to create new balls, e.g. when a life is lost)
const std::string ballTexturePath = "Assets/Sprites/ball.png";
const float ballInitialSpeed = 200.0f;
const float ballMaxSpeed = 1800.0f;
sf::Vector2f ballStartPosition;

const int subSteps = 4;
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
void game_loop();
void update_game(float dt);
bool check_collision(const sf::FloatRect& rect1, const sf::FloatRect& rect2);
void draw();
void reset();
void finish_game();
Ball* create_ball(sf::Vector2f position);
Ball* clone_ball(Ball* source, float angleOffsetDegrees);
void clear_balls();
void handle_collected_power_up(PowerUpType type);


int main()
{
    window = new sf::RenderWindow( sf::VideoMode({ GAME_WIDTH, GAME_HEIGHT }), "Arkanoide");

#ifndef __EMSCRIPTEN__
    window->setFramerateLimit(60);
#endif

    currentTime = 0.0f;
    prevTime = 0.0f;

    state = GameState::COUNTDOWN;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    init();

    gameClock.restart();


#ifdef __EMSCRIPTEN__
// Emscripten calls game_loop() once per browser frame.
// fps = 0 lets Emscripten use requestAnimationFrame.
// simulate_infinite_loop = 1 tells Emscripten that main
// does not return.
    emscripten_set_main_loop(game_loop, 0, 1);
#else
    while (window->isOpen())
    {
        game_loop();
    }

    finish_game();
#endif

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
    ballStartPosition = sf::Vector2f(centerX, 300.0f);
    balls.push_back(create_ball(ballStartPosition));

    LivesManager::get_instance().init();

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
                    sf::Color::White, "Lives:" + std::to_string(LivesManager::get_instance().get_lives()), false);

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
    PowerUpManager::get_instance().init();

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


void game_loop()
{
    if (!window->isOpen())
    {
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif

        finish_game();
        return;
    }

    // Process events
    while (const std::optional event = window->pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window->close();

#ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
            finish_game();
            return;
#endif
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

    // Calculate delta time
    sf::Time dt = gameClock.restart();

    if (state != GameState::GAME_OVER)
    {
        update_game(dt.asSeconds());
    }

    // Render
    window->clear();
    draw();
    window->display();
}


void update_game(float dt)
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
        paddle->clamp_position(static_cast<float>(GAME_WIDTH), static_cast<float>(GAME_HEIGHT));

        for (Ball* currentBall : balls)
        {
            currentBall->update(subDt);
            currentBall->clamp_position(static_cast<float>(GAME_WIDTH), static_cast<float>(GAME_HEIGHT));

            // Paddle-Ball collision
            if (check_collision(
                paddle->get_sprite().getGlobalBounds(),
                currentBall->get_sprite().getGlobalBounds()))
            {
                currentBall->ricochet(paddle);

                SoundManager::get_instance().play_boop();
            }

            // Block-Ball collision
            auto blockCollision =
                LevelManager::get_instance().check_block_collision(
                    currentBall->get_sprite().getGlobalBounds());

            if (blockCollision.has_value())
            {
                currentBall->bounce_from(blockCollision.value().blockBounds);

                if (blockCollision.value().isBreakable)
                {
                    ScoreManager::get_instance().add_to_score(10);

                    scoreLabel->set_string("Score:" +
                        std::to_string(ScoreManager::get_instance().get_score()));

                    SoundManager::get_instance().play_beep();

                    if (blockCollision.value().shouldSpawnPowerUp)
                    {
                        sf::FloatRect blockBounds = blockCollision.value().blockBounds;

                        sf::Vector2f spawnPosition(
                            blockBounds.position.x + blockBounds.size.x / 2.0f,
                            blockBounds.position.y + blockBounds.size.y / 2.0f);

                        PowerUpManager::get_instance().spawn(spawnPosition,
                            static_cast<int>(balls.size()));
                    }
                }
                else
                {
                    SoundManager::get_instance().play_cling();
                }
            }
        }
    }

    // PowerUps: falling, paddle collision, and PaddleSpeed/PaddleWidth application
    std::optional<PowerUpType> collectedPowerUp =
        PowerUpManager::get_instance().update(dt, window, paddle);

    if (collectedPowerUp.has_value())
    {
        handle_collected_power_up(collectedPowerUp.value());
    }

    // Ball speed incrementation
    speedIncreaseTimer += dt;

    if (speedIncreaseTimer >= speedIncreaseInterval)
    {
        speedIncreaseTimer -= speedIncreaseInterval;

        for (Ball* currentBall : balls)
        {
            if (!currentBall->is_at_max_speed())
            {
                float newSpeed = currentBall->get_current_speed() * speedMultiplier;

                currentBall->set_current_speed(newSpeed);
            }
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

            clear_balls();
            balls.push_back(create_ball(ballStartPosition));

            paddle->set_position(paddle->get_start_position());

            PowerUpManager::get_instance().clear_falling_power_ups();

            state = GameState::COUNTDOWN;

            currentTime = 0.0f;
            prevTime = 0.0f;
            speedIncreaseTimer = 0.0f;
        }
    }

    // Remove balls that fell below the screen
    for (auto it = balls.begin(); it != balls.end(); )
    {
        if ((*it)->get_sprite().getPosition().y >
            static_cast<float>(window->getSize().y))
        {
            delete *it;
            it = balls.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Lose life
    if (balls.empty())
    {
        SoundManager::get_instance().play_lose();

        LivesManager::get_instance().lose_life();
        livesLabel->set_string("Lives:" + std::to_string(LivesManager::get_instance().get_lives()));

        PowerUpManager::get_instance().on_life_lost(paddle);

        if (LivesManager::get_instance().is_game_over())
        {
            state = GameState::GAME_OVER;
            return;
        }
        else
        {
            paddle->reset();
            balls.push_back(create_ball(ballStartPosition));

            PowerUpManager::get_instance().clear_falling_power_ups();

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
        for (Ball* currentBall : balls)
        {
            currentBall->draw(window);
        }

        PowerUpManager::get_instance().draw(window);
    }

    paddle->draw(window);
}


void reset()
{
    paddle->reset();

    clear_balls();
    balls.push_back(create_ball(ballStartPosition));

    PowerUpManager::get_instance().clear();

    currentTime = 0.0f;
    prevTime = 0.0f;

    LivesManager::get_instance().reset_lives();

    ScoreManager::get_instance().reset_score();

    livesLabel->set_string("Lives:" + std::to_string(LivesManager::get_instance().get_lives()));

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

    clear_balls();

    delete scoreLabel;
    delete livesLabel;
    delete gameOverLabel;
    delete countdownLabel;
    delete resetLabel;
    delete gameCompleteLabel;
}


Ball* create_ball(sf::Vector2f position)
{
    Ball* newBall = new Ball(ballTexturePath, position, ballInitialSpeed);
    newBall->set_velocity(sf::Vector2f(0.0f, newBall->get_current_speed()));
    newBall->set_max_speed(ballMaxSpeed);
    newBall->set_scale(0.25f, 0.25f);

    return newBall;
}


void clear_balls()
{
    for (Ball* currentBall : balls)
    {
        delete currentBall;
    }

    balls.clear();
}


Ball* clone_ball(Ball* source, float angleOffsetDegrees)
{
    Ball* newBall = create_ball(source->get_sprite().getPosition());

    sf::Vector2f sourceVelocity = source->get_velocity();
    float angleOffset = angleOffsetDegrees * 3.14159f / 180.0f;

    float newX = sourceVelocity.x * std::cos(angleOffset) -
        sourceVelocity.y * std::sin(angleOffset);

    float newY = sourceVelocity.x * std::sin(angleOffset) +
        sourceVelocity.y * std::cos(angleOffset);

    newBall->set_current_speed(source->get_current_speed());
    newBall->set_velocity(newX, newY);

    return newBall;
}


void handle_collected_power_up(PowerUpType type)
{
    SoundManager::get_instance().play_powerup();

    switch (type)
    {
    case PowerUpType::ExtraLife:

        LivesManager::get_instance().add_life();
        livesLabel->set_string("Lives:" + std::to_string(LivesManager::get_instance().get_lives()));

        break;

    case PowerUpType::MultiBall:
        {
            // Re-check the multiplier at collection time (not spawn time): the ball
            // count may have changed while this power-up was falling.
            int multiplier = PowerUpManager::get_instance().get_multiball_multiplier(
                static_cast<int>(balls.size()));

            int clonesPerBall = multiplier - 1;

            if (clonesPerBall > 0)
            {
                std::vector<Ball*> newBalls;

                for (Ball* existingBall : balls)
                {
                    // clonesPerBall is 1 (x2) or 2 (x3); this covers both with the
                    // same +25/-25 degree offsets used before.
                    for (int i = 0; i < clonesPerBall; i++)
                    {
                        float angleOffset = (i == 0) ? 25.0f : -25.0f;

                        newBalls.push_back(clone_ball(existingBall, angleOffset));
                    }
                }

                for (Ball* newBall : newBalls)
                {
                    balls.push_back(newBall);
                }
            }
        }
        break;

    case PowerUpType::PaddleSpeed:
    case PowerUpType::PaddleWidth:

        // Already handled inside PowerUpManager::update().
        break;
    }
}