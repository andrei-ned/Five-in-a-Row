#pragma once
#include "Message.h"


class PlayerMove
{
public:
	PlayerMove(Message message);
	PlayerMove(char, char);
	PlayerMove(unsigned int, unsigned int);

	char getX() const;
	char getY() const;

	void setX(char);
	void setX(unsigned int);
	void setY(char);
	void setY(unsigned int);

	const char* getContent() const;
private:
	char mContent[3];
};
