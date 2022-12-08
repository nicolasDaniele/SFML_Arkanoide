#include "Label.h"

Label::Label(sf::Font _font, int size, sf::Vector2f position,
	sf::Color color, std::string content, bool _centered)
{
	font = _font;
	text.setCharacterSize(size);
	text.setPosition(position.x, position.y);
	text.setFillColor(color);
	text.setString(content);
	centered = _centered;
}

void Label::set_string(std::string newString)
{
	text.setString(newString);
}

void Label::draw(sf::RenderWindow* window)
{
	if (centered)
	{
		text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
	}

	text.setFont(font);
	window->draw(text);
}