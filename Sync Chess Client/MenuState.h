#pragma once
#include "GameState.h"
#include "GameObject.h"

class MenuState : public GameState
{
public:
	MenuState(Game&);

	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderWindow& window) override;
private:
	std::vector<std::unique_ptr<GameObject>> mGameObjects;
};

