#include "GameStateBase.h"

GameStateBase::GameStateBase(Game& game) : mpGame(&game) {}

void GameStateBase::update(const sf::Time& deltaTime)
{
	for (unsigned int i = 0; i < mGameObjects.size(); i++)
	{
		mGameObjects.at(i)->update(deltaTime);
	}
}

void GameStateBase::render(sf::RenderWindow& window)
{
	for (unsigned int i = 0; i < mGameObjects.size(); i++)
	{
		mGameObjects.at(i)->render(window);
	}
}

