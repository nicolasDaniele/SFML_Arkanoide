#include <iostream>
#include <SFML/Graphics.hpp>
#include "Paddle.h"

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 600), "Arkanoide");
    
    string paddleTexturePath = "Assets/Sprites/paddle.png";
    sf::Vector2f paddlePos(225, 500);
    Paddle* paddle = new Paddle(paddleTexturePath, paddlePos);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::A)
                {
                    sf::Vector2f vec(-6, 0);
                    paddle->add_move_vector(vec);
                }

                if (event.key.code == sf::Keyboard::D)
                {
                    sf::Vector2f vec(6, 0);
                    paddle->add_move_vector(vec);
                }
            }
        }

        window.clear();
        window.draw(paddle->get_sprite());
        window.display();
    }
}