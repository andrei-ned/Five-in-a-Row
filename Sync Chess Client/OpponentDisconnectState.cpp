#include "OpponentDisconnectState.h"
#include "Game.h"
#include "Constants.h"
#include "TextGO.h"
#include "PreConnectionState.h"

OpponentDisconnectState::OpponentDisconnectState(Game& game) : GameStateBase(game) 
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
	txt->mText.setFont(mpGame->getFont());
	txt->mText.setString("Opponent disconnected.");
	float txtX = (Constants::windowWidth - txt->mText.getGlobalBounds().width) * 0.5f;
	txt->mText.setPosition(txtX, 100.0f);
	mGameObjects.push_back(std::move(txt));
}