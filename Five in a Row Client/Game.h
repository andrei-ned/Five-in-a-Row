#pragma once
#include "SFML/Graphics.hpp"
#include <string>
#include "ButtonGO.h"
#include "GameObject.h"
#include "GameStateBase.h"
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <mutex>
#include <iostream>
#include "Connection.h"

#include <winsock2.h>
#include <ws2tcpip.h>

class Game
{
public:
	Game(sf::RenderWindow&);
	~Game();

	/// <summary> Game logic happens in here, should be called every frame </summary>
	/// <param name="deltaTime"> Time since last frame </param>
	void update(const sf::Time& deltaTime);
	// Draw things on screen, should be called every frame
	void render();

	void textEvent(const unsigned int unicode);

	const sf::Font& getFont() const;

	void startConnection();
	void closeConnection();

	void resizeWindow(const sf::Vector2u&);

	// Change current state to T, create it if it doesn't exist
	template <class T>
	void changeState();

	std::unique_ptr<Connection> mConnection;
private:
	sf::RenderWindow* mpWindow;

	// Buttons
	ButtonGO mTestButton;

	// Assets
	sf::Font mFont;

	// Game States
	std::unordered_map<std::type_index, std::unique_ptr<GameStateBase>> mStates;
	GameStateBase* mpCurrentState;

	std::mutex mStateMutex;

	std::unique_ptr<std::thread> mCreateConnectionThread;
	//std::atomic<bool> mConnectionFlag;

	SOCKET mConnectSocket;

	void failedConnection();
};

template <class T>
void Game::changeState()
{
	std::lock_guard<std::mutex> lockGuard(mStateMutex);

	GameStateBase* desiredState;

	auto search = mStates.find(typeid(T));
	if (search != mStates.end())
	{
		desiredState = (*search).second.get();
	}
	else
	{
		// Create instance
		auto upT = std::make_unique<T>(*this);
		T* pT = upT.get();
		mStates.insert({ typeid(T), std::move(upT) });

		desiredState = pT;
	}

	if (desiredState != mpCurrentState)
	{
		if (mpCurrentState)
			mpCurrentState->exit();
		mpCurrentState = desiredState;
		mpCurrentState->enter();
	}
}

