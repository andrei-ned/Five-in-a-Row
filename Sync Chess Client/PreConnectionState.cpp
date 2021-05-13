#include "PreConnectionState.h"
#include "Game.h"
#include "Constants.h"
#include "MenuState.h"
#include "ConnectingState.h"

PreConnectionState::PreConnectionState(Game& game) : GameStateBase(game)
{
	using namespace Constants;

	// Button settings
	float btnX = (Constants::windowWidth - btnSize.x) * 0.5f;

	// ip input field (TODO)
	mIpText.mText.setFont(mpGame->getFont());
	mIpText.mText.setPosition(btnX, 100.0f);
	mIpText.mText.setString("localhost:8888");

	// Find match Button
	auto btnPlay = std::make_unique<ButtonGO>();
	btnPlay->setColors(btnColors);
	btnPlay->setSize(btnSize);
	btnPlay->setTextFont(mpGame->getFont());
	btnPlay->setPosition({ btnX,200.0f });
	btnPlay->setTextString("Find match");
	btnPlay->mOnClick = [=]() { mpGame->changeState<ConnectingState>(); };
	mGameObjects.push_back(std::move(btnPlay));

	// Back button
	auto btnBack = std::make_unique<ButtonGO>();
	btnBack->setColors(btnColors);
	btnBack->setSize(btnSize);
	btnBack->setTextFont(mpGame->getFont());
	btnBack->setPosition({ btnX,300.0f });
	btnBack->setTextString("Back");
	btnBack->mOnClick = [=]() { mpGame->changeState<MenuState>(); };
	mGameObjects.push_back(std::move(btnBack));
}

void PreConnectionState::enter()
{
	mpGame->endServerThread();
}

void PreConnectionState::textEvent(const unsigned int unicode)
{

}

void PreConnectionState::render(sf::RenderWindow& window)
{
	GameStateBase::render(window);
	mIpText.render(window);
}