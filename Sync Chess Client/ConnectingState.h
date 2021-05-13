#pragma once
#include "GameStateBase.h"
#include "GameObject.h"
#include "TextGO.h"

class ConnectingState : public GameStateBase
{
public:
	ConnectingState(Game&);

	void enter() override;
	void render(sf::RenderWindow& window) override;
private:

};

