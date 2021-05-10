#include "Game.h"
#include <cassert>
#include "Constants.h"
#include <iostream>

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

	// Set up
	mBoardSprite.setTexture(mBoardTexture);
	mBoardSprite.setPosition(20.0f, 20.0f);
	mBoardSprite.setScale(0.5f, 0.5f);

	mDrawables.push_back(&mBoardSprite);

	//mTestButton.mRectShape.setPosition({ 10.0f, 10.0f });
	//mTestButton.mRectShape.setSize({ 200.0f, 50.0f });
	mTestButton.mColorDefault = sf::Color::Green;
	mTestButton.mColorHovered = sf::Color::Red;
	mTestButton.mColorPressed = sf::Color::Blue;

	mTestButton.setPosition({ 10.0f,10.0f });
	mTestButton.setSize({ 500.0f, 100.0f });

	mTestButton.setTextFont(mFont);
	mTestButton.setTextString("Hello world");

	mTestButton.mOnClick = [=]() { std::cout << "clickity\n"; };

	mGameObjects.push_back(&mTestButton);
}

Game::~Game() {
}

void Game::update(const sf::Time& deltaTime) {
	for (auto go : mGameObjects)
	{
		go->update(deltaTime);
	}

	if (!mpCurrentState)
		return;
	if (mpCurrentState != mpDesiredState && mpDesiredState)
	{
		if (mpCurrentState)
			mpCurrentState->exit();
		mpCurrentState = mpDesiredState;
		mpCurrentState->enter();
	}
	mpCurrentState->update(deltaTime);
}

void Game::render(sf::RenderWindow& window) {
	for (auto drawable : mDrawables)
	{
		window.draw(*drawable);
	}

	for (auto go : mGameObjects)
	{
		go->render(window);
	}
	//window.draw(mBoardSprite);

	if (mpCurrentState)
	{
		mpCurrentState->render(window);
	}
}
