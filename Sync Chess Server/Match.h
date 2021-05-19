#pragma once
#include "Connection.h"

class Match
{
public:
	Match(std::unique_ptr<Connection>, std::unique_ptr<Connection>);
private:
	std::unique_ptr<Connection> p1Connection;
	std::unique_ptr<Connection> p2Connection;
};

