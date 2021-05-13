#pragma once
#include "SFML/Graphics.hpp"
#include "GameObject.h"

class Game;

// Base state that all other states inherit from
class GameStateBase
{
public:
	virtual ~GameStateBase() {};

	// Called when entering the state
	virtual void enter() {}
	// Called when exiting the state
	virtual void exit() {}

	virtual void update(const sf::Time& deltaTime);
	virtual void render(sf::RenderWindow& window);
	virtual void textEvent(const unsigned int unicode) {}
protected:
	GameStateBase(Game& game);

	Game* mpGame;
	std::vector<std::unique_ptr<GameObject>> mGameObjects;
};

