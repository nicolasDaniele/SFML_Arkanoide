#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "Block.h"
#include "ScoreManager.h"
#include "LevelManager.h"
#include "Label.h"

sf::RenderWindow* window; 
Paddle* paddle;
Ball* ball;

const int maxLives = 3;
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
sf::Font font;

// Paddle movement variables
bool movingLeft = false;
bool movingRight = false;

// Ball speed incrementation variables
float speedIncreaseTimer = 0.0f;
const float speedIncreaseInterval = 5.0f;
const float speedMultiplier = 1.1f;

ScoreManager* ScoreManager::instance = nullptr;
LevelManager* LevelManager::instance = nullptr;

enum GameState
{
    COUNTDOWN,
    PLAYING,
    GAME_OVER,
    GAME_COMPLETE
};

GameState state;

void init();
void handle_inputs(sf::Event ev);
void update(float dt);
bool check_collision(sf::FloatRect rect1, sf::FloatRect rect2);
void draw();
void reset();
void finish_game();

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
}

void init()
{
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
    gameOverLabel = new Label(font, 40, sf::Vector2f(centerPosX, centerPosY - 80),
        sf::Color::White, "GAME OVER", true);
    // CountdownLabel
    countdownLabel = new Label(font, 40, sf::Vector2f(centerPosX, centerPosY),
        sf::Color::White, "READY", true);
    // ResetLabel
    resetLabel = new Label(font, 25, sf::Vector2f(centerPosX, centerPosY + 80),
        sf::Color::White, "", true);
    //GameCompleteLabel
    gameCompleteLabel = new Label(font, 45, sf::Vector2f(centerPosX, centerPosY - 150),
        sf::Color::White, "   GAME COMPLETE!\nCONGRATULATIONS!", true);

    LevelManager::get_instance()->load_level(0);
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
                movingLeft = true;
            }

            if (ev.key.code == sf::Keyboard::D ||
                ev.key.code == sf::Keyboard::Right)
            {
                movingRight = true;
            }
        }
        

        if (ev.key.code == sf::Keyboard::Space && 
            (state == GameState::GAME_OVER || state == GameState::GAME_COMPLETE))
        {
            reset();
        }
    }

    if (ev.type == sf::Event::KeyReleased)
    {
        if (ev.key.code == sf::Keyboard::A ||
            ev.key.code == sf::Keyboard::Left)
        {
            movingLeft = false;
        }

        if (ev.key.code == sf::Keyboard::D ||
            ev.key.code == sf::Keyboard::Right)
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
        ball->ricochet(paddle);
    }


    // Block-Ball collision
    auto blockBounds = LevelManager::get_instance()->check_block_collision(ball->get_sprite().getGlobalBounds());
    if (blockBounds.has_value())
    {
        ball->bounce_from(blockBounds.value());

        ScoreManager::get_instance()->add_to_score(10);
        scoreLabel->set_string("Score: " + to_string(ScoreManager::get_instance()->get_score()));
    }

    // Check for level complete
    if (LevelManager::get_instance()->is_level_complete())
    {
        currentLevel++;

        if (currentLevel >= LevelManager::get_instance()->get_num_levels())
        {
            state = GameState::GAME_COMPLETE;
            return;
        }
        else
        {
            LevelManager::get_instance()->load_level(currentLevel);

            ball->reset();
            paddle->set_position(paddle->get_start_position());

            state = GameState::COUNTDOWN;
            currentTime = 0.0f;
            prevTime = 0.0f;
            speedIncreaseTimer = 0.0f;
        }
    }

    // Loose life
    if (ball->get_sprite().getPosition().y > window->getSize().y)
    {
        lives--;
        livesLabel->set_string("Lives: " + to_string(lives));

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

bool check_collision(sf::FloatRect rect1, sf::FloatRect rect2)
{
    return rect1.intersects(rect2);
}

void draw()
{
    LevelManager::get_instance()->draw(window);

    if (state == GameState::GAME_OVER)
    {
        resetLabel->set_string("\t\tYour Score: " + to_string(ScoreManager::get_instance()->get_score()) +
            "\n\nPress SpaceBar to Reset");

        gameOverLabel->draw(window);
        resetLabel->draw(window);

        return;
    }
    else if (state == GameState::GAME_COMPLETE)
    {
        resetLabel->set_string("\t\tYour Score: " + to_string(ScoreManager::get_instance()->get_score()) +
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
    ScoreManager::get_instance()->reset_score();

    livesLabel->set_string("Lives: " + to_string(lives));
    scoreLabel->set_string("Score: " + to_string(ScoreManager::get_instance()->get_score()));

    LevelManager::get_instance()->load_level(0);

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