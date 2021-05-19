#include "ConnectingState.h"
#include "PreConnectionState.h"
#include "Game.h"
#include "Constants.h"
#include "MenuState.h"
#include <cmath>
#include "PlayState.h"

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
	txt->mText.setFont(mpGame->getFont());
	mpText = txt.get();
	mGameObjects.push_back(std::move(txt));

	mUseDots = true;
	mElapsed = 0.0f;
}

void ConnectingState::updateText(std::string s, bool useDots)
{
	mTextMtx.lock();
	mTextString = s;
	mpText->mText.setString(s);
	float txtX = (Constants::windowWidth - mpText->mText.getGlobalBounds().width) * 0.5f;
	mpText->mText.setPosition({ txtX, 100.0f });

	mUseDots = useDots;
	mElapsed = 0.0f;
	mTextMtx.unlock();
}

void ConnectingState::enter()
{
	mpGame->startConnection();
	updateText("Connecting to server");
}

void ConnectingState::update(const sf::Time& deltaTime)
{
	if (mpGame->mConnection)
	{
		while (mpGame->mConnection->hasRecvMessages())
		{
			Message m = mpGame->mConnection->popRecvQueue();
			if (m.getMessageType() == MessageType::MatchStart)
			{
				mpGame->changeState<PlayState>();
				break;
			}
		}
	}

	if (mUseDots)
	{
		mTextMtx.lock();
		mElapsed += deltaTime.asSeconds();
		int dots = static_cast<int>(mElapsed) % 3 + 1;
		std::string dotsString = "";
		for (int i = 0; i < dots; i++)
			dotsString += ".";
		mpText->mText.setString(mTextString + dotsString);
		mTextMtx.unlock();
	}
}

void ConnectingState::render(sf::RenderWindow& window)
{
	GameStateBase::render(window);
}
