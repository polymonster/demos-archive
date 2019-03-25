#include "networking.h"
#pragma comment(lib, "ws2_32.lib")

sendStruct thatPlayer;
sendStruct thisPlayer;

#define MYPORT 4950    // the port users will be connecting to
#define MAXBUFLEN 100

CNetworkPlay::CNetworkPlay(){

	m_error = WSAStartup(MAKEWORD(2, 2), &wsaData);

	// So lets check for an error.
	if (m_error == SOCKET_ERROR) {
		cout << "Could Not Start Up Winsock!\n";
	}

	cout << "WinSocket started\n";

}

CNetworkPlay::~CNetworkPlay(){

}

void startupListener()
{

}

void startupBroadcaster()
{

}

void listenForName()
{

}

void broadcastName()
{

}

int startupServerForListening(unsigned short port, char *myName) {

	// the winsock data structure
	WSAData wsaData;

	// startup winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR) {
		printf("Could Not Start Up Winsock!\n");
		return -1;
	}

	// create my socket
	int mySocket = socket(AF_INET, SOCK_STREAM, 0);

	// make sure nothing bad happened
	if (mySocket == SOCKET_ERROR) {
		printf("Error Opening Socket!\n");
		return -1;
	}

	gethostname(myName,256);

	// the address structure
	struct sockaddr_in server;

	// fill the address structure with appropriate data
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	// and now bind my socket
	if (bind(mySocket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
		printf("Bind Failed!\n");
		closesocket(mySocket);
		return -1;
	}

	// mark my socket for listening
	if (listen(mySocket, 5) == SOCKET_ERROR) {
		printf("Listen Failed!\n");
		closesocket(mySocket);
		return -1;
	}

	printf("Server Started\n");

	return mySocket;
}

// -----------------------------------------------------------------------------------
// shutdownServer() - a function to shutdown a socket and clean up winsock

void shutdownServer(int socket) {
	// close our socket
	closesocket(socket);

	// shut down winsock
	WSACleanup();

	printf("Server Shutdown\n");
}

int startupClient(unsigned short port, const char* serverName) {
	// an error code we will use to get more information about our errors
	int error;

	// the winsock data structure
	WSAData wsaData;

	// startup winsock
	if ((error = WSAStartup(MAKEWORD(2, 2), &wsaData)) == SOCKET_ERROR) {
		printf("Could Not Start Up Winsock!\n");
		return -1;
	}

	// create my socket
	int mySocket = socket(AF_INET, SOCK_STREAM, 0);

	// make sure nothing bad happened
	if (mySocket == SOCKET_ERROR) {
		printf("Error Opening Socket!\n");
		return -1;
	}

	struct hostent *host_entry;

	// setup the host entry
	if ((host_entry = gethostbyname(serverName)) == NULL) {
		printf("Could not find host!\n");
		return -1;
	}

	struct sockaddr_in server;

	// fill in the server socket info
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = *(unsigned long*) host_entry->h_addr;

	// connect to the server
	if (connect(mySocket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
		printf("Error connecting to server!\n");
		return -1;
	}

	printf("Client Started\n");

	return mySocket;
}

// -----------------------------------------------------------------------------------
// shutdownClient() - a function to shutdown a socket and clean up winsock

void shutdownClient(int clientSocket) {
	// close our socket
	closesocket(clientSocket);

	// shut down winsock
	WSACleanup();

	printf("Client Shutdown\n");
}

//THIS  STUFF
void sendPacket(CPlayer *player){

		char sendBuffer[1024];
		int nBytes;
	
		///SEND PACKET

		thisPlayer = player->getPositionData();

		char sendReader[8];

		//pos
		sprintf(sendReader, "%.4g", thisPlayer.pos.x);
		strcpy(sendBuffer,sendReader);
		strcat(sendBuffer," ");
		
		sprintf(sendReader, "%.4g", thisPlayer.pos.y);
		strcat(sendBuffer,sendReader);
		strcat(sendBuffer," ");
		sprintf(sendReader, "%.4g", thisPlayer.pos.z);
		strcat(sendBuffer,sendReader);
		strcat(sendBuffer," ");
		//rot y
		sprintf(sendReader, "%.4g", thisPlayer.yrot);
		strcat(sendBuffer,sendReader);
		strcat(sendBuffer," ");
		//rot x
		sprintf(sendReader, "%.4g", thisPlayer.xrot);
		strcat(sendBuffer,sendReader);
		strcat(sendBuffer," ");
		//state
		sprintf(sendReader, "%d", thisPlayer.state);
		strcat(sendBuffer,sendReader);
		strcat(sendBuffer," ");
		//bool shoot
		sprintf(sendReader, "%d", thisPlayer.shotLock);
		strcat(sendBuffer,sendReader);
		strcat(sendBuffer," ");
		//shotline x1
		sprintf(sendReader, "%.4g", thisPlayer.lineAim.x);
		strcat(sendBuffer,sendReader);
		strcat(sendBuffer," ");
		//y1
		sprintf(sendReader, "%.4g", thisPlayer.lineAim.y);
		strcat(sendBuffer,sendReader);
		strcat(sendBuffer," ");
		//z1
		sprintf(sendReader, "%.4g", thisPlayer.lineAim.z);
		strcat(sendBuffer,sendReader);
		strcat(sendBuffer," ");
		//health
		sprintf(sendReader, "%d", thisPlayer.health);
		strcat(sendBuffer,sendReader);
		strcat(sendBuffer," ");
		//gun
		sprintf(sendReader, "%d", thisPlayer.gun);
		strcat(sendBuffer,sendReader);
		strcat(sendBuffer," ");
		//recieved shots
		sprintf(sendReader, "%d", thisPlayer.shotsRecieved);
		strcat(sendBuffer,sendReader);
		strcat(sendBuffer," ");
		//lills
		sprintf(sendReader, "%d", thisPlayer.kills);
		strcat(sendBuffer,sendReader);
		strcat(sendBuffer," ");
		//deaths
		sprintf(sendReader, "%d", thisPlayer.deaths);
		strcat(sendBuffer,sendReader);
		strcat(sendBuffer," ");
		//deaths
		sprintf(sendReader, "%d", thisPlayer.connected);
		strcat(sendBuffer,sendReader);
		strcat(sendBuffer," ");
		//ammo
		sprintf(sendReader, "%d", thisPlayer.magazines);
		strcat(sendBuffer,sendReader);
		strcat(sendBuffer," ");
		sprintf(sendReader, "%d", thisPlayer.ammo);
		strcat(sendBuffer,sendReader);
		strcat(sendBuffer," ");

		if(thisPlayer.shotLock) player->unlockShot();
		
		nBytes = send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
		//nBytes = send(clientSocket, thisPlayer, sizeof(thisPlayer), 0);
}

void sendPlayerData(CPlayer *player)
{
		char sendBuffer[1024];
		int nBytes;
		char sendReader[128];

		sprintf(sendReader, "%d", player->m_activeChar);
		strcpy(sendBuffer,sendReader);
		strcat(sendBuffer," ");
	
		strcat(sendBuffer,player->m_playerName);
		strcat(sendBuffer," ");

		nBytes = send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);

}

void recievePlayerData(CPlayer *player)
{
		int nBytes;
		char sendBuffer[1024];

		//RECIEVEPACKET
		//convert them back
		char buf[32];
		char variable[64];
		float bun[32];
		int counter = 0;
		int currentVar = 0;

		nBytes = recv(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
		// And now receive the rest of the message.
		int messageSize = strlen(sendBuffer);

		if(nBytes != -1)
		{

			for(int i = 0; i < messageSize; i++)
			{
				if(sendBuffer[i] == ' ')
				{
					if(currentVar == 1) strcpy(player->m_playerName,variable);
					else bun[currentVar] = atof(variable);

					counter = 0;
					currentVar++;
				}

				sprintf(buf,"%c",sendBuffer[i]);
				
				if(counter == 0) strcpy(variable,buf);
				else strcat(variable,buf);

				counter++;
			}

			player->changeCharacter(bun[0]);

		}
}

void recievePacket(CPlayer *player){

		int nBytes;
		char sendBuffer[1024];

		//RECIEVEPACKET
		//convert them back
		char buf[32];
		char variable[64];
		float bun[32];
		int counter = 0;
		int currentVar = 0;

		nBytes = recv(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
		// And now receive the rest of the message.
		int messageSize = strlen(sendBuffer);

		if(nBytes != -1)
		{
			for(int i = 0; i < messageSize; i++)
			{
				if(sendBuffer[i] == ' ')
				{
					bun[currentVar] = atof(variable);

					counter = 0;
					currentVar++;
				}

				sprintf(buf,"%c",sendBuffer[i]);
				
				if(counter == 0) strcpy(variable,buf);
				else strcat(variable,buf);

				counter++;
			}

			if(counter != 18)
			{

				//set those back to the player :)
				thatPlayer.pos.x = bun[0];
				thatPlayer.pos.y = bun[1];
				thatPlayer.pos.z = bun[2];
				thatPlayer.yrot = bun[3];
				thatPlayer.xrot = bun[4];
				thatPlayer.state = bun[5];
				thatPlayer.shotLock = bun[6];
				thatPlayer.lineAim.x = bun[7];
				thatPlayer.lineAim.y = bun[8];
				thatPlayer.lineAim.z = bun[9];
				thatPlayer.health = bun[10];
				thatPlayer.gun = bun[11];
				thatPlayer.shotsRecieved = bun[12];
				thatPlayer.kills = bun[13];
				thatPlayer.deaths = bun[14];
				thatPlayer.connected = bun[15];
				thatPlayer.magazines = bun[16];
				thatPlayer.ammo = bun[17];

				if(thatPlayer.connected == 1) player->updateViaNetwork(thatPlayer);
			}
		}
}

void sendLobbyData(int message)
{
		char sendBuffer[10];

		itoa(message,sendBuffer,10);

		int nBytes = send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);

}

int recieveLobbyData()
{
	char sendBuffer[10];

	int nBytes = recv(clientSocket, sendBuffer, sizeof(sendBuffer), 0);

	if(nBytes == -1) return -1;

	return atoi(sendBuffer);
}

void sendExit(bool leave)
{
	char sendBuffer[10];

	itoa(leave,sendBuffer,10);

	int nBytes = send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
}

int recieveExit()
{
	char sendBuffer[10];

	int nBytes = recv(clientSocket, sendBuffer, sizeof(sendBuffer), 0);

	if(nBytes == -1) return 1;

	return atoi(sendBuffer);
}