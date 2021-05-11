#pragma once
#include "SFML/Graphics.hpp"
#include "GameObject.h"

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

	virtual void update(const sf::Time& deltaTime);
	virtual void render(sf::RenderWindow& window);
protected:
	GameState(Game& game);

	Game* mpGame;
	std::vector<std::unique_ptr<GameObject>> mGameObjects;
};

