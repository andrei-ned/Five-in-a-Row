#pragma once
#include "GameStateBase.h"
#include "ButtonGO.h"
#include "GameConstants.h"
#include "PlayerMove.h"
#include "TextGO.h"

class PlayState : public GameStateBase
{
public:
	PlayState(Game&);

	void update(const sf::Time& deltaTime) override;
	void enter() override;
	void exit() override;
private:
	const sf::Vector2f boardCellSize;
	const float border;

	TextGO* mpStatusTxt;
	TextGO* mpScoreText;
	ButtonGO* mpRematchBtn;
	ButtonGO* mBoard[GameConstants::boardWidth][GameConstants::boardHeight];

	unsigned int mMyScore;
	unsigned int mOpponentScore;

	bool mOpponentReadyForRematch;

	void enableBoard(bool);
	void resetBoard();
	void placeOnBoard(unsigned int x, unsigned int y, bool isOpponent);
	void updateScoreText();
	void updateStatusText(std::string);
	void initRound();

	float getUIElementX(float elementWidth);
};

