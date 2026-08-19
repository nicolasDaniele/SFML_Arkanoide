#include "Label.h"

Label::Label(const sf::Font& font, int size, sf::Vector2f position,
	sf::Color color, const std::string& content, bool _centered)
	: text(font), centered(_centered)
{
	text.setCharacterSize(size);
	text.setPosition(position);
	text.setFillColor(color);
	text.setString(content);
}

void Label::set_string(const std::string& newString)
{
	text.setString(newString);
}

void Label::draw(sf::RenderWindow* window)
{
	if (centered)
	{
		sf::FloatRect bounds = text.getGlobalBounds();

		text.setOrigin(sf::Vector2f(bounds.size.x / 2.f,
			bounds.size.y / 2.f));
	}

	window->draw(text);
}