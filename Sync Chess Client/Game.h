#pragma once
#include "SFML/Graphics.hpp"
#include <string>
#include "Button.h"
#include "GameObject.h"
#include "GameState.h"
#include <unordered_map>
#include <typeindex>
#include <memory>

class Game
{
public:
	Game();
	~Game();

	/// <summary> Game logic happens in here, should be called every frame </summary>
	/// <param name="deltaTime"> Time since last frame </param>
	void update(const sf::Time& deltaTime);
	// Draw things on screen, should be called every frame
	void render(sf::RenderWindow& window);

	// Change current state to T, create it if it doesn't exist
	template <class T>
	void changeState();
private:
	// Buttons
	Button mTestButton;

	// Assets
	sf::Texture mBoardTexture;
	sf::Font mFont;

	// Game States
	std::unordered_map<std::type_index, std::unique_ptr<GameState>> mStates;
	GameState* mpCurrentState;
	GameState* mpDesiredState;

	// Sprites
	sf::Sprite mBoardSprite;

	std::vector<sf::Drawable*> mDrawables;

	std::vector<GameObject*> mGameObjects;
};

template <class T>
void Game::changeState()
{
	auto search = mStates.find(typeid(T));
	if (search != mStates.end())
	{
		mpDesiredState = (*search).second.get();
	}
	else
	{
		auto upT = std::make_unique<T>(*this);
		T* pT = upT.get();
		mStates.insert({ typeid(T), std::move(upT) });

		mpDesiredState = pT;
	}
}

