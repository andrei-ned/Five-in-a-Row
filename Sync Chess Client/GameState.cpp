#include "GameState.h"

GameState::GameState(Game& game) : mpGame(&game) {}

void GameState::update(const sf::Time& deltaTime)
{
	for (unsigned int i = 0; i < mGameObjects.size(); i++)
	{
		mGameObjects.at(i)->update(deltaTime);
	}
}

void GameState::render(sf::RenderWindow& window)
{
	for (unsigned int i = 0; i < mGameObjects.size(); i++)
	{
		mGameObjects.at(i)->render(window);
	}
}

