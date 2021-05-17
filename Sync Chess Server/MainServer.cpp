#include <winsock2.h>
#include <ws2tcpip.h>
#include <cassert>
#include <cstdio>
#include <thread>
#include <vector>

int handleClientThread(SOCKET client_socket)
{
	int result;

	const int buffer_size = 512;
	char buffer[buffer_size];

	do
	{
		// Wait for client to send some data
		result = recv(client_socket, buffer, buffer_size, 0);
		if (result > 0)
		{
			printf("Received %d bytes: %s\n", (int)strlen(buffer), buffer);

			// Send back to client
			result = send(client_socket, buffer, result, 0);
			if (result == SOCKET_ERROR)
			{
				printf("send() failed with error: %d\n", WSAGetLastError());
			}
		}
		else if (result == 0)
		{
			printf("Connection to client closing...\n");
		}
		else
		{
			printf("recv() failed with error: %d\n", WSAGetLastError());
		}
	} while (result > 0);

	//closesocket(listen_socket);
	//WSACleanup();
	return 0;
}

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

	std::vector<std::thread> threads;

	// Accept a client socket
	SOCKET client_socket;
	while (client_socket = accept(listen_socket, nullptr, nullptr))
	{
		if (client_socket == INVALID_SOCKET)
		{
			printf("accept() failed with error: %d\n", WSAGetLastError());
			//closesocket(listen_socket);
			//WSACleanup();
			continue;
		}
		printf("uwu\n");
		threads.push_back(std::thread(handleClientThread, client_socket));
	}

	if (client_socket == INVALID_SOCKET)
	{
		printf("accept() failed with errror: %d\n", WSAGetLastError());
		closesocket(listen_socket);
		WSACleanup();
		return -1;
	}

	const int buffer_size = 512;
	char buffer[buffer_size];

	do
	{
		// Wait for client to send some data
		result = recv(client_socket, buffer, buffer_size, 0);
		if (result > 0)
		{
			printf("Received %d bytes: %s\n", (int)strlen(buffer), buffer);

			// Send back to client
			result = send(client_socket, buffer, result, 0);
			if (result == SOCKET_ERROR)
			{
				printf("send() failed with error: %d\n", WSAGetLastError());
			}
		}
		else if (result == 0)
		{
			printf("Connection to client closing...\n");
		}
		else
		{
			printf("recv() failed with error: %d\n", WSAGetLastError());
		}
	} while (result > 0);

	closesocket(listen_socket);
	WSACleanup();
	return 0;
}