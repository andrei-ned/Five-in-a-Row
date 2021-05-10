#include "MenuState.h"
#include "Button.h"
#include <iostream>
#include "Game.h"

MenuState::MenuState(Game& game) : GameState(game)
{
	auto btn = std::make_unique<Button>();
	btn->mColorDefault = sf::Color::Green;
	btn->mColorHovered = sf::Color::Red;
	btn->mColorPressed = sf::Color::Blue;
			   
	btn->setPosition({ 10.0f,10.0f });
	btn->setSize({ 500.0f, 100.0f });
			 
	btn->setTextFont(mpGame->getFont());
	btn->setTextString("Hello world");
	btn->enable(true);
	
	btn->mOnClick = [=]() { std::cout << "clickity\n"; };

	mGameObjects.push_back(std::move(btn));
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