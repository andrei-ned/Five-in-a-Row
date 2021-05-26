#include "PlayState.h"
#include "GameConstants.h"
#include "Game.h"
#include <iostream>
#include "PlayerMove.h"
#include "OpponentDisconnectState.h"
#include "Constants.h"
#include "MenuState.h"
#include "TextGO.h"
#include <algorithm>

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
				boardColor,
				btnColors.hovered,
				btnColors.pressed,
				sf::Color::White
			});
			btnBoard->setSize(boardCellSize);
			btnBoard->setTextFont(mpGame->getFont());
			btnBoard->setTextSize(56);
			btnBoard->mTextOffset = { -3.0f , -27.0f };
			btnBoard->setOutlineWidth(border);
			btnBoard->setPosition({ (boardCellSize.x + border * 2.0f) * (x + 1), (boardCellSize.y + border * 2.0f) * (y + 1) });
			btnBoard->mOnClick = [=]() { 
				std::cout << "clicked " << x << ", " << y << "\n"; 
				PlayerMove move(x, y);
				mpGame->mConnection->sendMessage(Message(MessageType::PlayerMove, move.getContent()));
				placeOnBoard(x, y, false);
				enableBoard(false);
				updateStatusText("Opponent's turn");
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
	btnRematch->mOnClick = [=]() { 
		mpGame->mConnection->sendMessage(Message(MessageType::Rematch)); 
		updateStatusText("Waiting for opponent");
		mpRematchBtn->enable(false);
		mReadyForRematch = true;
		if (mOpponentReadyForRematch)
			initRound();
	};
	btnRematch->enable(false);
	mGameObjects.push_back(std::move(btnRematch));

	// Status Text
	auto statusTxt = std::make_unique<TextGO>();
	mpStatusTxt = statusTxt.get();
	statusTxt->mText.setFont(mpGame->getFont());
	statusTxt->mText.setPosition({ 0.0f, 35.0f });
	mGameObjects.push_back(std::move(statusTxt));

	// Score Text
	auto scoreText = std::make_unique<TextGO>();
	mpScoreText = scoreText.get();
	scoreText->mText.setFont(mpGame->getFont());
	scoreText->mText.setPosition({ 0.0f, 100.0f });
	updateScoreText();
	mGameObjects.push_back(std::move(scoreText));

	initRound();
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
				placeOnBoard(move.getX(), move.getY(), true);
				enableBoard(true);
				updateStatusText("Your turn");
				break;
			}
			case MessageType::OpponentDisconnect:
				mpGame->changeState<OpponentDisconnectState>();
				return;
			case MessageType::OpponentWon:
				updateStatusText("Opponent won!");
				mOpponentScore++;
				endRound();
				highlightWinningLine("o");
				break;
			case MessageType::YouWon:
				updateStatusText("You won!");
				mMyScore++;
				endRound();
				highlightWinningLine("x");
				break;
			case MessageType::Draw:
				updateStatusText("Draw!");
				endRound();
				break;
			case MessageType::Rematch:
				updateStatusText("Opponent wants a rematch");
				mOpponentReadyForRematch = true;
				if (mReadyForRematch)
					initRound();
				break;
			case MessageType::GiveTurn:
				enableBoard(true);
				updateStatusText("Your turn");
				break;
			}
		}
	}
}

void PlayState::enter()
{
	using namespace Constants;
	mpGame->resizeWindow({ playWindowWidth, playWindowHeight });
	mMyScore = 0;
	mOpponentScore = 0;
	initRound();
}

void PlayState::exit()
{
	using namespace Constants;
	mpGame->resizeWindow({ windowWidth, windowHeight });
	mpGame->closeConnection();
}

void PlayState::enableBoard(bool enabled)
{
	using namespace GameConstants;

	for (unsigned int y = 0; y < boardHeight; y++)
	{
		for (unsigned int x = 0; x < boardWidth; x++)
		{
			if (!mBoard[x][y]->hasText())
				mBoard[x][y]->setInteractable(enabled);
		}
	}
}

void PlayState::resetBoard()
{
	using namespace GameConstants;
	using namespace Constants;

	for (unsigned int y = 0; y < boardHeight; y++)
	{
		for (unsigned int x = 0; x < boardWidth; x++)
		{
			mBoard[x][y]->setTextString("");
			mBoard[x][y]->setNormalColor(boardColor);
		}
	}
}

void PlayState::placeOnBoard(unsigned int x, unsigned int y, bool isOpponent)
{
	mBoard[x][y]->setTextString(isOpponent ? "o" : "x");
	mBoard[x][y]->setTextColor(isOpponent ? sf::Color(0x28, 0x92, 0xd7) : sf::Color(0xff, 0x81, 0x0a));
	mBoard[x][y]->setInteractable(false);
}

void PlayState::updateScoreText()
{
	std::string s = "You " + std::to_string(mMyScore) + " - " + std::to_string(mOpponentScore) + " Opponent";
	mpScoreText->mText.setString(s);
	float x = getUIElementX(mpScoreText->mText.getGlobalBounds().width);
	mpScoreText->mText.setPosition({ x,mpScoreText->mText.getPosition().y });
}

void PlayState::updateStatusText(std::string s)
{
	mpStatusTxt->mText.setString(s);
	float x = getUIElementX(mpStatusTxt->mText.getGlobalBounds().width);
	mpStatusTxt->mText.setPosition({ x,mpStatusTxt->mText.getPosition().y });
}

void PlayState::initRound()
{
	mOpponentReadyForRematch = false;
	mReadyForRematch = false;
	updateStatusText("Opponent's turn");

	resetBoard();
	enableBoard(false);
	mpRematchBtn->enable(false);
}

void PlayState::endRound()
{
	updateScoreText();
	mpRematchBtn->enable(true);
	enableBoard(false);
}

void PlayState::highlightWinningLine(std::string symbol)
{
	// Check for horizontal line
	highlightWinningLine(-1, 0, symbol);
	// Check for vertical line
	highlightWinningLine(0, -1, symbol);
	// Check for diagonal lines
	highlightWinningLine(-1, -1, symbol);
	highlightWinningLine(1, -1, symbol);
}

void PlayState::highlightWinningLine(int offsetX, int offsetY, std::string symbol)
{
	using namespace GameConstants;
	using namespace Constants;

	for (unsigned int y = 0; y < boardHeight; y++)
	{
		for (unsigned int x = 0; x < boardWidth; x++)
		{
			int score = 0;
			if (mBoard[x][y]->getText() == symbol)
			{
				score = 1;
				if (x != 0 && x < boardWidth - 1 && y != 0 && y < boardHeight - 1)
					score = std::max(score, mLineScores[x + offsetX][y + offsetY] + 1);
			}
			mLineScores[x][y] = score;
			if (score >= winningLength)
			{
				int X = x, Y = y;
				for (unsigned int i = 0; i < winningLength; i++)
				{
					mBoard[X][Y]->setNormalColor(btnColors.hovered);
					X += offsetX;
					Y += offsetY;
				}
			}
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