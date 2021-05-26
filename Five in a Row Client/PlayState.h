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
	int mLineScores[GameConstants::boardWidth][GameConstants::boardHeight];

	unsigned int mMyScore;
	unsigned int mOpponentScore;

	bool mOpponentReadyForRematch;
	bool mReadyForRematch;

	void enableBoard(bool);
	void resetBoard();
	void placeOnBoard(unsigned int x, unsigned int y, bool isOpponent);
	void updateScoreText();
	void updateStatusText(std::string);
	void initRound();
	void endRound();
	void highlightWinningLine(std::string symbol);
	void highlightWinningLine(int offsetX, int offsetY, std::string symbol);

	float getUIElementX(float elementWidth);
};

