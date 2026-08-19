#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Label
{
public:
	Label(const sf::Font& font, int size, sf::Vector2f position,
		sf::Color color, const std::string& content, bool _centered);
	~Label() = default;

	void set_string(const std::string& newString);
	void draw(sf::RenderWindow* window);

private:
	sf::Text text;
	bool centered;
};