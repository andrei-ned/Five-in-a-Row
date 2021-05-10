#include "Game.h"
#include <cassert>
#include "Constants.h"
#include <iostream>
#include "MenuState.h"

Game::Game() {
	// Load assets
	if (!mBoardTexture.loadFromFile(Constants::assetPath + "Board.png"))
	{
		assert(false);
	}
	if (!mFont.loadFromFile(Constants::assetPath + "RobotoMono-VariableFont_wght.ttf"))
	{
		assert(false);
	}

	changeState<MenuState>();
}

Game::~Game() {
}

const sf::Font& Game::getFont() const
{
	return mFont;
}

void Game::update(const sf::Time& deltaTime) {
	//for (auto go : mGameObjects)
	//{
	//	go->update(deltaTime);
	//}

	if (mpCurrentState)
	{
		mpCurrentState->update(deltaTime);
	}
}

void Game::render(sf::RenderWindow& window) {
	//for (auto drawable : mDrawables)
	//{
	//	window.draw(*drawable);
	//}

	//for (auto go : mGameObjects)
	//{
	//	go->render(window);
	//}
	////window.draw(mBoardSprite);

	if (mpCurrentState)
	{
		mpCurrentState->render(window);
	}
}
