#include "PlayState.h"
#include "GameConstants.h"
#include "Game.h"
#include <iostream>
#include "PlayerMove.h"
#include "OpponentDisconnectState.h"
#include "Constants.h"

PlayState::PlayState(Game& game) : GameStateBase(game)
{
	using namespace GameConstants;

	sf::Vector2 btnSize(33.0f, 33.0f);
	float border = 1.0f;

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
			btnBoard->setSize(btnSize);
			btnBoard->setTextFont(mpGame->getFont());
			btnBoard->setTextSize(60);
			btnBoard->mTextOffset = { -4.0f , -28.0f };
			btnBoard->setOutlineWidth(border);
			btnBoard->setPosition({ (btnSize.x + border * 2.0f) * (x + 1), (btnSize.y + border * 2.0f) * (y + 1) });
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