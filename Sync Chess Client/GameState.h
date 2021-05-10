#pragma once
#include "SFML/Graphics.hpp"

class Game;

// Base state that all other states inherit from
class GameState
{
public:
	virtual ~GameState() {};

	// Called when entering the state
	virtual void enter() {}
	// Called when exiting the state
	virtual void exit() {}

	virtual void update(const sf::Time& deltaTime) {}
	virtual void render(sf::RenderWindow& window) {}
protected:
	Game* mpGame;
	GameState(Game& game);
};

