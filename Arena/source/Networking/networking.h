#ifndef _NETWORKING_H
#define _NETWORKING_H

#include "globals.h"
#include "player.h"

void startupListener();
void startupBroadcaster();

void listenForName();
void broadcastName();

int startupServerForListening(unsigned short port,char *myName);
void shutdownServer(int socket);
int startupClient(unsigned short port, const char* serverName);
void shutdownClient(int clientSocket);
void sendPacket(CPlayer *player);
void recievePacket(CPlayer *player2);

void recievePlayerData(CPlayer *player);
void sendPlayerData(CPlayer *player);

void sendLobbyData(int message);
int recieveLobbyData();

void sendExit(bool leave);
int recieveExit();


class CNetworkPlay{

public:

	CNetworkPlay();
	~CNetworkPlay();



private:

	int m_error;
	WSAData wsaData;



};


#endif