#include "Constants.h"
#include "Game.h"
#include "MenuState.h"
#include "ButtonGO.h"
#include "TextGO.h"
#include <iostream>


MenuState::MenuState(Game& game) : GameState(game)
{
	// Title text
	auto txt = std::make_unique<TextGO>();

	txt->mText.setFont(mpGame->getFont());
	txt->mText.setString("Title");
	txt->mText.setCharacterSize(52);
	auto txtBounds = txt->mText.getGlobalBounds();
	txt->mText.setPosition({ (Constants::windowWidth - txtBounds.width) * 0.5f, 50.0f });

	mGameObjects.push_back(std::move(txt));

	// Button settings
	ButtonGO::ButtonColors btnColors = { sf::Color::Green, sf::Color::Red, sf::Color::Blue };
	sf::Vector2f btnSize(250.0f, 50.0f);
	float btnX = (Constants::windowWidth - btnSize.x) * 0.5f;

	// Play Button
	auto btnPlay = std::make_unique<ButtonGO>();
	btnPlay->setColors(btnColors);		   
	btnPlay->setSize(btnSize);
	btnPlay->setTextFont(mpGame->getFont());
	btnPlay->setPosition({ btnX,200.0f });
	btnPlay->setTextString("Play");
	btnPlay->mOnClick = [=]() { std::cout << "clickity\n"; };
	mGameObjects.push_back(std::move(btnPlay));

	// Quit Button
	auto btnQuit = std::make_unique<ButtonGO>();
	btnQuit->setColors(btnColors);
	btnQuit->setSize(btnSize);
	btnQuit->setTextFont(mpGame->getFont());
	btnQuit->setPosition({ btnX,300.0f });
	btnQuit->setTextString("Quit");
	btnQuit->mOnClick = [=]() { std::exit(EXIT_SUCCESS); };
	mGameObjects.push_back(std::move(btnQuit));
}

void MenuState::update(const sf::Time& deltaTime)
{
	for (unsigned int i = 0; i < mGameObjects.size(); i++)
	{
		mGameObjects.at(i)->update(deltaTime);
	}
}

void MenuState::render(sf::RenderWindow& window)
{
	for (unsigned int i = 0; i < mGameObjects.size(); i++)
	{
		mGameObjects.at(i)->render(window);
	}
}