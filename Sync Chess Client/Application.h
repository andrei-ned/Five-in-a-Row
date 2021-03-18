#pragma once
#include "SFML/Graphics.hpp"
#include "Game.h"

class Application
{
public:
	Application(unsigned int, unsigned int);
	~Application();
	void run();
private:
	Game mGame;
	sf::RenderWindow mWindow;
};

