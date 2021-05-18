#include "ConnectingState.h"
#include "PreConnectionState.h"
#include "Game.h"
#include "Constants.h"
#include "MenuState.h"

ConnectingState::ConnectingState(Game& game) : GameStateBase(game)
{
	using namespace Constants;

	// Button settings
	float btnX = (Constants::windowWidth - btnSize.x) * 0.5f;


	// Back button
	auto btnBack = std::make_unique<ButtonGO>();
	btnBack->setColors(btnColors);
	btnBack->setSize(btnSize);
	btnBack->setTextFont(mpGame->getFont());
	btnBack->setPosition({ btnX,300.0f });
	btnBack->setTextString("Back");
	btnBack->mOnClick = [=]() { mpGame->changeState<PreConnectionState>(); };
	mGameObjects.push_back(std::move(btnBack));

	// Text
	auto txt = std::make_unique<TextGO>();
	txt->mText.setString("Connecting to server...");
	txt->mText.setPosition({ btnX, 100.0f });
	mGameObjects.push_back(std::move(txt));

	//mpGame->ptrTestThread = std::make_unique<std::thread>([]() {
	//	std::cout << "yoyoyo\n";
	//});
}

void ConnectingState::enter()
{
	mpGame->startConnection();
}

void ConnectingState::render(sf::RenderWindow& window)
{
	GameStateBase::render(window);
}
