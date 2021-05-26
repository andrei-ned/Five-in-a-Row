#include "Constants.h"
#include "Game.h"
#include "SFML/Graphics.hpp"

int main()
{
	sf::RenderWindow window;
	window.create(sf::VideoMode(Constants::windowWidth, Constants::windowHeight), "Five in a Row", sf::Style::Titlebar | sf::Style::Close);

	Game game(window);

	// Start the game loop 
	sf::Clock clock;
	while (window.isOpen())
	{
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		window.clear(Constants::bgColor);

		game.update(clock.restart());
		game.render();

		// Update the window
		window.display();
	}

	return 0;
}