#include "Game.h"
#include <cassert>
#include "Constants.h"

Game::Game() {
	if (!boardTexture.loadFromFile(Constants::assetPath + "Board.png"))
	{
		assert(false);
	}

	mBoardSprite.setTexture(boardTexture);
	mBoardSprite.setPosition(20.0f, 20.0f);
	mBoardSprite.setScale(0.5f, 0.5f);

	mDrawables.push_back(&mBoardSprite);
}

Game::~Game() {
}

void Game::update(const sf::Time& deltaTime) {

}

void Game::render(sf::RenderWindow& window) {
	for (auto drawable : mDrawables)
	{
		window.draw(*drawable);
	}
	//window.draw(mBoardSprite);
}
