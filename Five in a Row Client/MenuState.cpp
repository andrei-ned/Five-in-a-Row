#include "Constants.h"
#include "Game.h"
#include "MenuState.h"
#include "ButtonGO.h"
#include "TextGO.h"
#include "PreConnectionState.h"

MenuState::MenuState(Game& game) : GameStateBase(game)
{
	using namespace Constants;

	// Title text
	auto txt = std::make_unique<TextGO>();
	txt->mText.setFont(mpGame->getFont());
	txt->mText.setString("Five in a Row");
	txt->mText.setCharacterSize(52);
	auto txtBounds = txt->mText.getGlobalBounds();
	txt->mText.setPosition({ (Constants::windowWidth - txtBounds.width) * 0.5f, 50.0f });

	mGameObjects.push_back(std::move(txt));

	// Button settings
	float btnX = (Constants::windowWidth - btnSize.x) * 0.5f;

	// Play Button
	auto btnPlay = std::make_unique<ButtonGO>();
	btnPlay->setColors(btnColors);		   
	btnPlay->setSize(btnSize);
	btnPlay->setTextFont(mpGame->getFont());
	btnPlay->setPosition({ btnX,200.0f });
	btnPlay->setTextString("Play");
	btnPlay->mOnClick = [=]() { mpGame->changeState<PreConnectionState>(); };
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