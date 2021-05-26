#include "Match.h"
#include "PlayerMove.h"
#include "GameConstants.h"
#include <ctime>
#include <iostream>
#include <algorithm>

Match::Match(std::unique_ptr<Connection> c1, std::unique_ptr<Connection> c2) : 
	mP1Connection(std::move(c1)),
	mP2Connection(std::move(c2))
{
	mP1Connection->sendMessage(MessageType::MatchStart);
	mP2Connection->sendMessage(MessageType::MatchStart);

	mP1Connection->mOnConnectionLost = [=]() { 
		mP2Connection->sendMessage(MessageType::OpponentDisconnect); 
		mMatchActive = false;
	};
	mP2Connection->mOnConnectionLost = [=]() { 
		mP1Connection->sendMessage(MessageType::OpponentDisconnect); 
		mMatchActive = false;
	};

	// Determine starting player
	srand(static_cast<unsigned int>(time(NULL)));
	if (rand() % 2)
	{
		mpStartingPlayer = mP1Connection.get();
	}
	else
	{
		mpStartingPlayer = mP2Connection.get();
	}

	initRound();
	mMatchActive = true;
	mMatchThread = std::thread(&Match::runMatch, this);
}

Match::~Match()
{
	mMatchActive = false;
	mMatchThread.join();
}

bool Match::isActive() const
{
	return mP1Connection->isActive() && mP2Connection->isActive();
}

void Match::initRound()
{
	using namespace GameConstants;

	mRematch1 = false;
	mRematch2 = false;
	roundEnded = false;

	for (int y = 0; y < boardHeight; y++)
	{
		for (int x = 0; x < boardWidth; x++)
		{
			mGameBoard[x][y] = 0;
		}
	}

	if (mpStartingPlayer == mP1Connection.get())
	{
		mpStartingPlayer = mP2Connection.get();
	}
	else
	{
		mpStartingPlayer = mP1Connection.get();
	}

	mpStartingPlayer->sendMessage(Message(MessageType::GiveTurn));
	mpLastMovePlayer = nullptr;
}

void Match::runMatch()
{
	while (mMatchActive)
	{
		handlePlayerMessages(mP1Connection.get(), mP2Connection.get());
		handlePlayerMessages(mP2Connection.get(), mP1Connection.get());
	}
}

void Match::handlePlayerMessages(Connection* player, Connection* otherPlayer)
{
	using namespace GameConstants;

	while (player->hasRecvMessages())
	{
		Message m = player->popRecvQueue();
		switch (m.getMessageType())
		{
		case MessageType::PlayerMove:
		{
			if (roundEnded)
			{
				printf("Received move after round end, ignoring\n");
				break;
			}
			if (player == mpLastMovePlayer)
			{
				printf("Received move from the same player twice in a row, ignoring\n");
				break;
			}
			PlayerMove move(m);
			if (mGameBoard[move.getX()][move.getY()] != 0)
			{
				printf("Received invalid move, ignoring\n");
				break;
			}
			char symbol = player == mpStartingPlayer ? 'x' : 'o';
			mGameBoard[move.getX()][move.getY()] = symbol;
			otherPlayer->sendMessage(Message(MessageType::PlayerMove, move.getContent()));

			// Check if won
			bool won = false;
			int maxScore = -1;
			// Check for horizontal line
			maxScore = std::max(calculateLineScores(-1, 0, symbol), maxScore);
			// Check for vertical line
			maxScore = std::max(calculateLineScores(0, -1, symbol), maxScore);
			// Check for diagonal lines
			maxScore = std::max(calculateLineScores(-1, -1, symbol), maxScore);
			maxScore = std::max(calculateLineScores(1, -1, symbol), maxScore);

			if (maxScore >= winningLength)
				won = true;

			// Check if draw
			bool draw = true;
			for (unsigned int y = 0; y < boardHeight; y++)
			{
				for (unsigned int x = 0; x < boardWidth; x++)
				{
					if (mGameBoard[x][y] == 0)
						draw = false;
				}
			}

			// Send message
			if (won)
			{
				player->sendMessage(Message(MessageType::YouWon));
				otherPlayer->sendMessage(Message(MessageType::OpponentWon));
				roundEnded = true;
			}
			else if (draw)
			{
				player->sendMessage(Message(MessageType::Draw));
				otherPlayer->sendMessage(Message(MessageType::Draw));
				roundEnded = true;
			}
			break;
		}
		case MessageType::Rematch:
			otherPlayer->sendMessage(Message(MessageType::Rematch));
			if (player == mpStartingPlayer)
				mRematch1 = true;
			else
				mRematch2 = true;
			if (mRematch1 && mRematch2)
				initRound();
			break;
		}
	}
}

int Match::calculateLineScores(int offsetX, int offsetY, char symbol)
{
	using namespace GameConstants;

	int maxScore = -1;
	for (unsigned int y = 0; y < boardHeight; y++)
	{
		for (unsigned int x = 0; x < boardWidth; x++)
		{
			int score = 0;
			if (mGameBoard[x][y] == symbol)
			{
				score = 1;
				if (x != 0 && x < boardWidth - 1 && y != 0 && y < boardHeight -1)
					score = std::max(score, mLineScores[x + offsetX][y + offsetY] + 1);
			}
			mLineScores[x][y] = score;
			maxScore = std::max(maxScore, score);
		}
	}
	return maxScore;
}

void Match::printBoard() const
{
	using namespace GameConstants;

	for (unsigned int y = 0; y < boardHeight; y++)
	{
		for (unsigned int x = 0; x < boardWidth; x++)
		{
			std::cout << (mGameBoard[x][y] == 0 ? '_' : mGameBoard[x][y]);
		}
		std::cout << "\n";
	}
}