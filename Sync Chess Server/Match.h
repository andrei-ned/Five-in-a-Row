#pragma once
#include "Connection.h"
#include "GameConstants.h"

class Match
{
public:
	Match(std::unique_ptr<Connection>, std::unique_ptr<Connection>);
	~Match();

	bool isActive() const;
private:
	std::unique_ptr<Connection> mP1Connection;
	std::unique_ptr<Connection> mP2Connection;
	std::thread mMatchThread;
	std::atomic<bool> mMatchActive;
	char mGameBoard[GameConstants::boardWidth][GameConstants::boardHeight];

	void runMatch();
	void handlePlayerMessages(Connection* player, Connection* otherPlayer);
};

