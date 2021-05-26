#pragma once
#include "GameStateBase.h"
#include "GameObject.h"
#include "TextGO.h"

class PreConnectionState : public GameStateBase
{
public:
	PreConnectionState(Game&);

	void enter() override;
	void render(sf::RenderWindow& window) override;
	void update(const sf::Time& deltaTime) override;
private:
	TextGO mIpText; // <- change this to pointer, add to game object instead
};

