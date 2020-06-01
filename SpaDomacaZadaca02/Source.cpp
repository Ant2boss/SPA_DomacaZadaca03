#include <SFML/Graphics.hpp>
#include "Pozadina.h"

#include<iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1320, 700), "SPA Domaca Zadaca 3");
	window.setFramerateLimit(60);

	Pozadina MojaPozadina(&window, 23, 13, 50, 50);
	MojaPozadina.SetOrigin(25, 25);

	sf::RectangleShape GUI;
	GUI.setSize(sf::Vector2f(120, 700));
	
	sf::Texture Image;
	if (!Image.loadFromFile("Halper.png")) {
		std::cout << "Error 404!" << std::endl;
		return 404;
	}

	GUI.setTexture(&Image);
	GUI.setPosition(1200, 0);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
		}

		window.clear(sf::Color::Color(102, 102, 153, 255));

		MojaPozadina.Append();
		window.draw(GUI);

		window.display();
	}

	return 0;
}