#include "stdafx.h"
#include <SFML/Graphics.hpp>


int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Window Title");
	sf::CircleShape shape;
	shape.setRadius(20);
	shape.setFillColor(sf::Color::Green);
	shape.setPosition(100, 0);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		window.clear();
		window.draw(shape);
		window.display();
	}

    return 0;
}

