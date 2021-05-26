#include "PreConnectionState.h"
#include "Game.h"
#include "Constants.h"
#include "MenuState.h"
#include "ConnectingState.h"
#include "GameConstants.h"

PreConnectionState::PreConnectionState(Game& game) : GameStateBase(game)
{
	using namespace Constants;

	// Button settings
	float btnX = (Constants::windowWidth - btnSize.x) * 0.5f;

	mIpText.mText.setFont(mpGame->getFont());
	mIpText.mText.setString("localhost:" + GameConstants::defaultPort);
	float txtX = (Constants::windowWidth - mIpText.mText.getGlobalBounds().width) * 0.5f;
	mIpText.mText.setPosition({ txtX, 100.0f });

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

	// Tooltip text
	auto tipTxt = std::make_unique<TextGO>();
	tipTxt->mText.setFont(mpGame->getFont());
	tipTxt->mText.setCharacterSize(20);
	tipTxt->mText.setString("(paste from clipboard to change ip)");
	txtX = (Constants::windowWidth - tipTxt->mText.getGlobalBounds().width) * 0.5f;
	tipTxt->mText.setPosition({ txtX, 150.0f });
	mGameObjects.push_back(std::move(tipTxt));
}

void PreConnectionState::enter()
{
	mpGame->closeConnection();
}

void PreConnectionState::render(sf::RenderWindow& window)
{
	GameStateBase::render(window);
	mIpText.render(window);
}

void PreConnectionState::update(const sf::Time& deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::V))
	{
		auto s = sf::Clipboard::getString();
		mIpText.mText.setString(s);
		auto pos = s.find(":");
		mpGame->mIp = s.substring(0, pos);
		mpGame->mPort = pos == std::string::npos ? GameConstants::defaultPort : static_cast<std::string>(s.substring(pos+1));
		float txtX = (Constants::windowWidth - mIpText.mText.getGlobalBounds().width) * 0.5f;
		mIpText.mText.setPosition({ txtX, mIpText.mText.getPosition().y });
	}
}