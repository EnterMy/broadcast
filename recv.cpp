// broadcast.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinSock2.h>
#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT 1111
#define MAX_SIZE 1024

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
	{
		printf("startup failed\n");
	}

	SOCKET sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == sfd){
		perror("socket failed");
		exit(-1);
	}
	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEFAULT_PORT);
	addr.sin_addr.s_addr = INADDR_ANY;
	bind(sfd, (struct sockaddr *)&addr, sizeof(sockaddr_in));

	//set socket opt is SO_BROADCAST
	char brocadcast = 1;
	setsockopt(sfd, SOL_SOCKET, SO_BROADCAST, &brocadcast, sizeof(brocadcast));

	char buffer[MAX_SIZE];
	int sockaddr_in_len, num;

	struct sockaddr_in addrClient;

	cout << "waitting recv..."<<endl;

	while (TRUE){
		memset(buffer, 0, sizeof(buffer));

		sockaddr_in_len = sizeof(struct sockaddr_in);
		num = recvfrom(sfd, buffer, sizeof(buffer), 0, (sockaddr *)&addrClient, &sockaddr_in_len);

		if (num > 0){
			cout << "recv ip:" << inet_ntoa(addrClient.sin_addr);
			cout << "-->" << buffer << endl;
		}
		else{
			perror("recvfrom:");
		}
	}

	closesocket(sfd);
	WSACleanup();

	return 0;
}
