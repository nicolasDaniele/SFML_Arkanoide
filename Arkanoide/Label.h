#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

class Label
{
private:
	sf::Text text;
	sf::Font font;
	bool centered;
public:
	Label(sf::Font font, int size, sf::Vector2f position, 
		sf::Color color, std::string content, bool _centered);
	virtual ~Label() { }
	void set_string(std::string newString);
	void draw(sf::RenderWindow* window);
};