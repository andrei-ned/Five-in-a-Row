#pragma once
#include "GameStateBase.h"
#include "ButtonGO.h"
#include "GameConstants.h"

class PlayState : public GameStateBase
{
public:
	PlayState(Game&);

	void update(const sf::Time& deltaTime) override;
private:
	ButtonGO* mBoard[GameConstants::boardWidth][GameConstants::boardHeight];
};

