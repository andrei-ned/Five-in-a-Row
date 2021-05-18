#include "Game.h"
#include <cassert>
#include "Constants.h"
#include <iostream>
#include "MenuState.h"
#include <winsock2.h>
#include <ws2tcpip.h>

Game::Game() /*: testThread(&Game::testFunc, this)*/ {
	// Load assets
	if (!mBoardTexture.loadFromFile(Constants::assetPath + "Board.png"))
	{
		assert(false);
	}
	if (!mFont.loadFromFile(Constants::assetPath + "RobotoMono-VariableFont_wght.ttf"))
	{
		assert(false);
	}

	changeState<MenuState>();
}

Game::~Game() {
	//testThread.join();
	closeConnection();
}

const sf::Font& Game::getFont() const
{
	return mFont;
}

void Game::update(const sf::Time& deltaTime) 
{
	//for (auto go : mGameObjects)
	//{
	//	go->update(deltaTime);
	//}

	if (mpCurrentState)
	{
		mpCurrentState->update(deltaTime);
	}
}

void Game::render(sf::RenderWindow& window) 
{
	//for (auto drawable : mDrawables)
	//{
	//	window.draw(*drawable);
	//}

	//for (auto go : mGameObjects)
	//{
	//	go->render(window);
	//}
	////window.draw(mBoardSprite);

	if (mpCurrentState)
	{
		mpCurrentState->render(window);
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
	if (!mServerThread)
	{
		mConnectionFlag = true;
		mServerThread = std::make_unique<std::thread>([=]() {
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
			result = getaddrinfo("localhost", "8888", &hints, &info);
			if (result != 0)
			{
				printf("getaddrinfo() failed: %d\n", result);
				WSACleanup();
				return -1;
			}

			// Create socket
			connect_socket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
			if (connect_socket == INVALID_SOCKET)
			{
				printf("Unable to create server socket: %d\n", WSAGetLastError());
				freeaddrinfo(info);
				WSACleanup();
				return -1;
			}

			//// Set socket to non-blocking mode
			//u_long iMode = 1;
			//result = ioctlsocket(connect_socket, FIONBIO, &iMode);

			result = connect(connect_socket, info->ai_addr, (int)info->ai_addrlen);
			freeaddrinfo(info);
			if (connect_socket == SOCKET_ERROR)
			{
				printf("Unable to connect to server.\n");
				closesocket(connect_socket);
				WSACleanup();
				return -1;
			}

			mConnection = std::make_unique<Connection>(connect_socket);
			return 0;
		});
	}
}

void Game::closeConnection()
{
	if (mServerThread)
	{
		closesocket(connect_socket);
		mConnectionFlag = false;
		mServerThread->join();
		mServerThread.reset();
	}
	if (mConnection)
	{
		mConnection.reset();
	}
}
