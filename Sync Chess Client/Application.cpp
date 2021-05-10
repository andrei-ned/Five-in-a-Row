#include "Application.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cassert>
#include <cstdio>
#include <thread>
#include <functional>

Application::Application(unsigned int windowWidth, unsigned int windowHeight)
{
	mWindow.create(sf::VideoMode(windowWidth, windowHeight), "Sync Chess", sf::Style::Titlebar | sf::Style::Close);
}


Application::~Application()
{
}

void Application::run() {
	auto callback = std::bind(&Application::handleServerConnection, this);
	std::thread serverThread(callback);

	// Start the game loop 
	sf::Clock clock;
	while (mWindow.isOpen())
	{
		// Process events
		sf::Event event;
		while (mWindow.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				mWindow.close();
		}

		// Clear screen
		mWindow.clear();

		mGame.update(clock.restart());
		mGame.render(mWindow);

		// Update the window
		mWindow.display();
	}

	serverThread.join();
}

int Application::handleServerConnection() {
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
	SOCKET connect_socket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (connect_socket == INVALID_SOCKET)
	{
		printf("Unable to create server socket: %d\n", WSAGetLastError());
		freeaddrinfo(info);
		WSACleanup();
		return -1;
	}

	result = connect(connect_socket, info->ai_addr, (int)info->ai_addrlen);
	freeaddrinfo(info);
	if (connect_socket == SOCKET_ERROR)
	{
		printf("Unable to connect to server.\n");
		closesocket(connect_socket);
		WSACleanup();
		return -1;
	}

	const int buffer_size = 512;
	char buffer[buffer_size];
	strcpy_s(buffer, buffer_size, "Hello server");

	do
	{
		// Send to server
		result = send(connect_socket, buffer, (int)strlen(buffer) + 1, 0);
		if (result == SOCKET_ERROR)
		{
			printf("send() failed with error: %d\n", WSAGetLastError());
		}

		// Receive from server
		result = recv(connect_socket, buffer, buffer_size, 0);
		if (result > 0)
		{
			//printf("Received %d bytes: %s\n", (int)strlen(buffer), buffer);
		}
		else if (result == 0)
		{
			printf("Connection to server closing...\n");
		}
		else
		{
			printf("recv() failed with error: %d\n", WSAGetLastError());
		}
	} while (result > 0);

	closesocket(connect_socket);
	WSACleanup();

	return 0;
}
