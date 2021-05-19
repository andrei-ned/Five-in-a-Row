#include "PlayState.h"
#include "GameConstants.h"
#include "ButtonGO.h"
#include "Game.h"
#include <iostream>

PlayState::PlayState(Game& game) : GameStateBase(game)
{
	using namespace GameConstants;

	sf::Vector2 btnSize(33.0f, 33.0f);
	float border = 1.0f;

	for (int y = 0; y < boardHeight; y++)
	{
		for (int x = 0; x < boardWidth; x++)
		{
			auto btnBoard = std::make_unique<ButtonGO>();
			btnBoard->setColors({
				sf::Color::Red,
				sf::Color::Green,
				sf::Color::Blue,
				sf::Color::White
			});
			btnBoard->setSize(btnSize);
			btnBoard->setTextFont(mpGame->getFont());
			btnBoard->setOutlineWidth(border);
			btnBoard->setPosition({ (btnSize.x + border * 2.0f) * (x + 1), (btnSize.y + border * 2.0f) * (y + 1) });
			btnBoard->mOnClick = [=]() { std::cout << x << ", " << y << "\n"; };
			btnBoard->setTextString(((x + y) % 2 == 0) ? "x" : "o");
			mGameObjects.push_back(std::move(btnBoard));
		}
	}
}