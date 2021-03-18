#include "Application.h"

Application::Application(unsigned int windowWidth, unsigned int windowHeight)
{
	mWindow.create(sf::VideoMode(windowWidth, windowHeight), "Sync Chess", sf::Style::Titlebar | sf::Style::Close);
}


Application::~Application()
{
}

void Application::run() {
	// Start the game loop 
	sf::Clock clock;
	while (mWindow.isOpen())
	{
		// Process events
		sf::Event event;
		while (mWindow.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				mWindow.close();
		}

		// Clear screen
		mWindow.clear();

		mGame.update(clock.restart());
		mGame.render(mWindow);

		// Update the window
		mWindow.display();
	}
}
