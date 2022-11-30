#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 600), "Arkanoide");


    sf::Texture paddle_texture;
    if (!paddle_texture.loadFromFile("Assets/Sprites/paddle.png"))
    {
        cout << "ERROR: paddle_texture could not be loaded. " << endl;
    }
    sf::Sprite paddle_sprite;
    paddle_sprite.setTexture(paddle_texture);
    paddle_sprite.setPosition(225, 500);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
                
            if(event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Right)
                {
                    paddle_sprite.setPosition(paddle_sprite.getPosition().x + 5, paddle_sprite.getPosition().y);
                }

                if (event.key.code == sf::Keyboard::Left)
                {
                    paddle_sprite.move(-2, 0);
                }
            }
        }

        window.clear();
        window.draw(paddle_sprite);
        window.display();
    }
}
