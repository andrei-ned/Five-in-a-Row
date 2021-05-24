#pragma once
#include "GameStateBase.h"
#include "ButtonGO.h"
#include "GameConstants.h"

class PlayState : public GameStateBase
{
public:
	PlayState(Game&);

	void update(const sf::Time& deltaTime) override;
	void enter() override;
	void exit() override;
private:
	ButtonGO* mBoard[GameConstants::boardWidth][GameConstants::boardHeight];
};

