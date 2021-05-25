#include "PlayState.h"
#include "GameConstants.h"
#include "Game.h"
#include <iostream>
#include "PlayerMove.h"
#include "OpponentDisconnectState.h"
#include "Constants.h"
#include "MenuState.h"
#include "TextGO.h"

PlayState::PlayState(Game& game) : GameStateBase(game), boardCellSize(33.0f, 33.0f), border(1.0f)
{
	using namespace GameConstants;
	using namespace Constants;

	for (unsigned int y = 0; y < boardHeight; y++)
	{
		for (unsigned int x = 0; x < boardWidth; x++)
		{
			auto btnBoard = std::make_unique<ButtonGO>();
			mBoard[x][y] = btnBoard.get();
			btnBoard->setColors({
				sf::Color::Red,
				sf::Color::Green,
				sf::Color::Blue,
				sf::Color::White
			});
			btnBoard->setSize(boardCellSize);
			btnBoard->setTextFont(mpGame->getFont());
			btnBoard->setTextSize(58);
			btnBoard->mTextOffset = { -4.0f , -28.0f };
			btnBoard->setOutlineWidth(border);
			btnBoard->setPosition({ (boardCellSize.x + border * 2.0f) * (x + 1), (boardCellSize.y + border * 2.0f) * (y + 1) });
			btnBoard->mOnClick = [=]() { 
				std::cout << "clicked " << x << ", " << y << "\n"; 
				PlayerMove move(x, y);
				mpGame->mConnection->sendMessage(Message(MessageType::PlayerMove, move.getContent()));
				mBoard[x][y]->setTextString("x");
			};
			//btnBoard->setTextString(((x + y) % 2 == 0) ? "x" : "o");
			mGameObjects.push_back(std::move(btnBoard));
		}
	}

	//float boardSizeX = (boardCellSize.x + border * 2.0f) * (boardWidth + 1);
	//float btnX = boardSizeX + (playWindowWidth - boardSizeX - btnSize.x) * 0.5f;
	float btnX = getUIElementX(btnSize.x);

	// Leave Button
	auto btnLeave = std::make_unique<ButtonGO>();
	btnLeave->setColors(btnColors);
	btnLeave->setSize(btnSize);
	btnLeave->mTextOffset = { 0.0f, -10.0f };
	btnLeave->setTextFont(mpGame->getFont());
	btnLeave->setPosition({ btnX, playWindowHeight - 35.0f - btnSize.y });
	btnLeave->setTextString("Leave");
	btnLeave->mOnClick = [=]() { mpGame->changeState<MenuState>(); };
	mGameObjects.push_back(std::move(btnLeave));

	// Rematch Button
	auto btnRematch = std::make_unique<ButtonGO>();
	mpRematchBtn = btnRematch.get();
	btnRematch->setColors(btnColors);
	btnRematch->setSize(btnSize);
	btnRematch->mTextOffset = { 0.0f, -10.0f };
	btnRematch->setTextFont(mpGame->getFont());
	btnRematch->setPosition({ btnX, playWindowHeight - 35.0f - btnSize.y * 3 });
	btnRematch->setTextString("Rematch");
	btnRematch->mOnClick = [=]() { std::exit(EXIT_SUCCESS); };
	mGameObjects.push_back(std::move(btnRematch));

	// Status Text
	auto statusTxt = std::make_unique<TextGO>();
	mpStatusTxt = statusTxt.get();
	mGameObjects.push_back(std::move(statusTxt));

	// Score Text
	auto scoreText = std::make_unique<TextGO>();
	mpScoreText = scoreText.get();
	mGameObjects.push_back(std::move(scoreText));
}

void PlayState::update(const sf::Time& deltaTime)
{
	if (mpGame->mConnection)
	{
		while (mpGame->mConnection->hasRecvMessages())
		{
			Message m = mpGame->mConnection->popRecvQueue();
			switch (m.getMessageType())
			{
			case MessageType::PlayerMove:
			{
				PlayerMove move(m);
				mBoard[move.getX()][move.getY()]->setTextString("o");
				break;
			}
			case MessageType::OpponentDisconnect:
				mpGame->changeState<OpponentDisconnectState>();
				break;
			}
		}
	}
}

void PlayState::enter()
{
	using namespace Constants;
	mpGame->resizeWindow({ playWindowWidth, playWindowHeight });
}

void PlayState::exit()
{
	using namespace Constants;
	mpGame->resizeWindow({ windowWidth, windowHeight });
}

void PlayState::enableBoard(bool enabled)
{
	using namespace GameConstants;

	for (unsigned int y = 0; y < boardHeight; y++)
	{
		for (unsigned int x = 0; x < boardWidth; x++)
		{
			if (mBoard[x][y]->hasText())
				mBoard[x][y]->enable(enabled);
		}
	}
}

void PlayState::resetBoard()
{
	using namespace GameConstants;

	for (unsigned int y = 0; y < boardHeight; y++)
	{
		for (unsigned int x = 0; x < boardWidth; x++)
		{
			mBoard[x][y]->setTextString("");
		}
	}
}

float PlayState::getUIElementX(float elementWidth)
{
	using namespace Constants;
	using namespace GameConstants;

	float boardSizeX = (boardCellSize.x + border * 2.0f) * (boardWidth + 1);
	float x = boardSizeX + (playWindowWidth - boardSizeX - elementWidth) * 0.5f;

	return x;
}