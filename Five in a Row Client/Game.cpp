#include "Game.h"
#include <cassert>
#include "Constants.h"
#include <iostream>
#include "MenuState.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include "ConnectingState.h"
#include "ConnectionLostState.h"
#include "GameConstants.h"

Game::Game(sf::RenderWindow& window) : mpWindow(&window) {
	// Load assets
	if (!mFont.loadFromFile(Constants::assetPath + "RobotoMono-VariableFont_wght.ttf"))
	{
		assert(false);
	}

	changeState<MenuState>();

	mIp = "localhost";
	mPort = GameConstants::defaultPort;
}

Game::~Game() {
	closeConnection();
}

const sf::Font& Game::getFont() const
{
	return mFont;
}

void Game::update(const sf::Time& deltaTime) 
{
	if (mpCurrentState)
	{
		mpCurrentState->update(deltaTime);
	}
}

void Game::render() 
{
	if (mpCurrentState)
	{
		mpCurrentState->render(*mpWindow);
	}
}

void Game::textEvent(const unsigned int unicode) 
{
	if (mpCurrentState)
	{
		mpCurrentState->textEvent(unicode);
	}
}

void Game::startConnection()
{
	if (!mCreateConnectionThread)
	{
		mCreateConnectionThread = std::make_unique<std::thread>([=]() {
			WSADATA wsaData;
			int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
			assert(result == 0);

			const addrinfo hints = {
				.ai_family = AF_INET,
				.ai_socktype = SOCK_STREAM,
				.ai_protocol = IPPROTO_TCP
			};

			// Resolve server address and port
			addrinfo* info;
			result = getaddrinfo(mIp.c_str(), mPort.c_str(), &hints, &info);
			if (result != 0)
			{
				printf("getaddrinfo() failed: %d\n", result);
				WSACleanup();
				failedConnection();
				return -1;
			}

			// Create socket
			mConnectSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
			if (mConnectSocket == INVALID_SOCKET)
			{
				printf("Unable to create server socket: %d\n", WSAGetLastError());
				freeaddrinfo(info);
				WSACleanup();
				failedConnection();
				return -1;
			}

			result = connect(mConnectSocket, info->ai_addr, (int)info->ai_addrlen);
			freeaddrinfo(info);
			if (result == SOCKET_ERROR)
			{
				printf("Unable to connect to server.\n");
				closesocket(mConnectSocket);
				WSACleanup();
				failedConnection();
				return -1;
			}

			mConnection = std::make_unique<Connection>(mConnectSocket);
			mConnection->mOnConnectionLost = [=]() { changeState<ConnectionLostState>(); };
			auto connState = dynamic_cast<ConnectingState*>(mpCurrentState);
			connState->updateText("Waiting for opponent");
			return 0;
		});
	}
}

void Game::closeConnection()
{
	if (mCreateConnectionThread)
	{
		mCreateConnectionThread->join();
		mCreateConnectionThread.reset();
	}
	if (mConnection)
	{
		mConnection.reset();
		WSACleanup();
	}
}

void Game::resizeWindow(const sf::Vector2u& size)
{
	mpWindow->setSize(size);
	sf::FloatRect rect(0, 0, static_cast<float>(size.x), static_cast<float>(size.y));
	mpWindow->setView(sf::View(rect));
}

bool Game::isInFocus() const
{
	return mpWindow->hasFocus();
}

void Game::failedConnection()
{
	auto connState = dynamic_cast<ConnectingState*>(mpCurrentState);
	connState->updateText("Failed to connect", false);
}
