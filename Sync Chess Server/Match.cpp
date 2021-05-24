#include "Match.h"
#include "PlayerMove.h"

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
	while (player->hasRecvMessages())
	{
		Message m = player->popRecvQueue();
		switch (m.getMessageType())
		{
		case MessageType::PlayerMove:
		{
			PlayerMove move(m);
			mGameBoard[move.getX()][move.getY()] = 'O';
			otherPlayer->sendMessage(Message(MessageType::PlayerMove, move.getContent()));
			break;
		}
		case MessageType::OpponentDisconnect:
			otherPlayer->sendMessage(Message(MessageType::OpponentDisconnect));
			break;
		}
	}
}