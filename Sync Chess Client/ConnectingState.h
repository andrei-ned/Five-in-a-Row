#pragma once
#include "GameStateBase.h"
#include "TextGO.h"
#include <mutex>

class ConnectingState : public GameStateBase
{
public:
	ConnectingState(Game&);

	void updateText(std::string, bool = true);

	void enter() override;
	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderWindow& window) override;
private:
	TextGO* mpText;
	std::string mTextString;
	float mElapsed;
	bool mUseDots;
	std::mutex mTextMtx;
};

