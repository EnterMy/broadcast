#include "stdafx.h"
#include <WinSock2.h>

#include <iostream>
using namespace std;

#define DEFAULT_PORT 1111
#define MAX_SIZE 1024

#pragma comment(lib, "ws2_32.lib")


int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
	{
		printf("startup failed\n");
	}

	SOCKET sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sfd){
		perror("socket failed");
		exit(-1);
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEFAULT_PORT);
	addr.sin_addr.s_addr = INADDR_BROADCAST;  //broadcast address

	bind(sfd, (struct sockaddr *)&addr, sizeof(sockaddr_in));

	//set socket opt is SO_BROADCAST
	char brocadcast = true;
	setsockopt(sfd, SOL_SOCKET, SO_BROADCAST, &brocadcast, sizeof(brocadcast));

	char buffer[MAX_SIZE];
	int num;

	while (TRUE){
		cout << "please input data, (bye) quit" << endl;
		cin >> buffer;

		if (!strcmp(buffer, "bye"))
		{
			cout << "bye" << endl;
			return 0;
		}

		num = sendto(sfd, buffer, strlen(buffer), 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
		if (num > 0){
			cout << "send data length:" << num;
			cout << " --> " << buffer << endl;
		}
		else{
			perror("sendto fail");
		}
	}
	
	WSACleanup();
	closesocket(sfd);

	return 0;
}
