#include <iostream>
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
const int blocksRows = 8;
const int blocksColumns = 13;
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
bool check_collision(sf::Sprite sprite1, sf::Sprite sprite2);
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
    for (int i = 0; i < blocksRows; i++)
    {
        for (int j = 0; j < blocksColumns; j++)
        {
            string blolckTexturePath = "Assets/Sprites/Blocks/block_" + to_string(i) + ".png";
            sf::Vector2f offsetPos(3, 50);
            Block* block = new Block(blolckTexturePath, offsetPos);
            block->set_scale(2, 2);
            float blockPosX = (block->get_texture().getSize().x) * 2 * j;
            float blockPosY = (block->get_texture().getSize().y) * 2 * i;
            block->set_position(offsetPos + sf::Vector2f(blockPosX, blockPosY));
            blocks.push_back(block);
        }
    }

    // Paddle initialization
    string paddleTexturePath = "Assets/Sprites/paddle.png";
    sf::Vector2f paddlePos(225, 500);
    paddle = new Paddle(paddleTexturePath, paddlePos);

    // Ball initialization
    string ballTexturePath = "Assets/Sprites/ball.png";
    sf::Vector2f ballPos(225, 300);
    ball = new Ball(ballTexturePath, ballPos);
    ball->set_velocity(sf::Vector2f(0, 200));
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
    resetLabel = new Label(font, 25, sf::Vector2f(centerPosX, centerPosY + 50),
        sf::Color::White, "Press SpaceBar to Reset", true);
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
                paddle->set_velocity(-200, 0);
            }

            if (ev.key.code == sf::Keyboard::D ||
                ev.key.code == sf::Keyboard::Right)
            {
                paddle->set_velocity(200, 0);
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

    // Paddle-Ball collision
    if (check_collision(paddle->get_sprite(), ball->get_sprite()))
    {
        float newBallXVelocity = rand() % 200;
        sf::Vector2f newBallVelocity(newBallXVelocity, -ball->get_velocity().y);
        ball->set_velocity(newBallVelocity);
    }

    // Block-Ball collision
    for (int i = 0; i < blocks.size(); i++)
    {
        if (check_collision(blocks[i]->get_sprite(), ball->get_sprite()))
        {
            float newBallXVelocity = rand() % 200;
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
            paddle->set_position(paddle->get_start_position());
            state = GameState::COUNTDOWN;

            currentTime = 0.0f;
            prevTime = 0.0f;
        }
    }
}

bool check_collision(sf::Sprite sprite1, sf::Sprite sprite2)
{
    sf::FloatRect shape1 = sprite1.getGlobalBounds();
    sf::FloatRect shape2 = sprite2.getGlobalBounds();

    return shape1.intersects(shape2);
}

void draw()
{
    for (Block* block : blocks)
    {
        if (block)
        {
            window->draw(block->get_sprite());
        }
    }

    if (state == GameState::GAMEOVER)
    {
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
        window->draw(ball->get_sprite());
    }

    window->draw(paddle->get_sprite());
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