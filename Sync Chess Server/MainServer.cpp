#include <winsock2.h>
#include <ws2tcpip.h>
#include <cassert>
#include <cstdio>
#include <thread>
#include <vector>
#include "Connection.h"
#include "Match.h"

int main()
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	assert(result == 0);

	const addrinfo hints = {
		.ai_flags = AI_PASSIVE,
		.ai_family = AF_INET,
		.ai_socktype = SOCK_STREAM,
		.ai_protocol = IPPROTO_TCP
	};

	// Resolve server local address and port
	addrinfo* info;
	result = getaddrinfo(nullptr, "8888", &hints, &info);
	if (result != 0)
	{
		printf("getaddrinfo() failed: %d\n", result);
		WSACleanup();
		return -1;
	}

	// Create server socket
	SOCKET listen_socket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (listen_socket == INVALID_SOCKET)
	{
		printf("Unable to create server socket: %d\n", WSAGetLastError());
		freeaddrinfo(info);
		WSACleanup();
		return -1;
	}

	// Setup TCP listening socket
	result = bind(listen_socket, info->ai_addr, (int)info->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		printf("bind() failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(info);
		closesocket(listen_socket);
		WSACleanup();
		return -1;
	}

	// Listen for connection
	result = listen(listen_socket, 1024);
	if (result == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listen_socket);
		WSACleanup();
		return -1;
	}

	std::vector<std::unique_ptr<Connection>> connections;

	std::unique_ptr<Connection> connectionWaitingForMatch;
	std::list<std::unique_ptr<Match>> matches;

	// Accept a client socket
	SOCKET client_socket;
	while (client_socket = accept(listen_socket, nullptr, nullptr))
	{
		if (client_socket == INVALID_SOCKET)
		{
			printf("accept() failed with error: %d\n", WSAGetLastError());
			continue;
		}
		printf("Started new connection\n");

		// Clear waiting connection if inactive
		if (connectionWaitingForMatch)
		{
			if (!connectionWaitingForMatch->isActive())
			{
				connectionWaitingForMatch.reset();
			}
		}

		matches.remove_if([](const std::unique_ptr<Match>& m) {
			return !m->isActive();
		});

		if (connectionWaitingForMatch)
		{
			matches.push_back(std::make_unique<Match>(std::move(connectionWaitingForMatch), std::make_unique<Connection>(client_socket)));
		}
		else
		{
			connectionWaitingForMatch = std::make_unique<Connection>(client_socket);
		}
	}

	return 0;
}