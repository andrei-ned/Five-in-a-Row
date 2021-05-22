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
	mMatchThread = std::thread([=]() {
		while (mMatchActive)
		{
			while (mP1Connection->hasRecvMessages())
			{
				Message m = mP1Connection->popRecvQueue();
				switch (m.getMessageType())
				{
				case MessageType::PlayerMove:
				{
					PlayerMove move(m);
					mGameBoard[move.getX()][move.getY()] = 'O';
					mP2Connection->sendMessage(Message(MessageType::PlayerMove, move.getContent()));
					break;
				}
				case MessageType::OpponentDisconnect:
					mP2Connection->sendMessage(Message(MessageType::OpponentDisconnect));
					break;
				}
			}
		}
	});
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