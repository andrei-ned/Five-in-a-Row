#include "Match.h"

Match::Match(std::unique_ptr<Connection> c1, std::unique_ptr<Connection> c2) : 
	p1Connection(std::move(c1)),
	p2Connection(std::move(c2))
{
	p1Connection->sendMessage(MessageType::MatchStart);
	p2Connection->sendMessage(MessageType::MatchStart);

	p1Connection->mOnConnectionLost = [=]() { p2Connection->sendMessage(MessageType::OpponentDisconnect); };
	p2Connection->mOnConnectionLost = [=]() { p1Connection->sendMessage(MessageType::OpponentDisconnect); };
}

bool Match::isActive() const
{
	return p1Connection->isActive() && p2Connection->isActive();
}