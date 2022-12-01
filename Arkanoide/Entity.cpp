#include "Entity.h"

Entity::Entity(string textureFilePath, sf::Vector2f startPos)
{
    if (!texture.loadFromFile(textureFilePath))
    {
        cout << "ERROR: paddle_texture could not be loaded. " << endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(startPos);
}