#include "Game.h"
#include "textureSingleton.h"
#include "timerSingleton.h"
#include "windowsThings.h"
#include "player.h"
#include "ModelManager.h"
#include "map.h"
#include "networking.h"
#include "soundSingleton.h"
#include "game.h"
#include "shader manager/shaderManager.h"
#include "HCI.h"

bool CGame::leaveGame()
{
	bool leave = false;

	//if you want to leave
	if(player[USER_PLAYER]->getPositionData().connected == -1)
	{
		leave = true;
	}

	//send a message
	sendExit(leave);

	//check for confirmation
	if(recieveExit() == 1)
	{
		return true;
	}

	return false;

}

bool CGame::checkLeave()
{
	bool confirmLeave = false;
	//if the player wants to leave
	if(recieveExit() == 1) confirmLeave = true;
	sendExit(confirmLeave);

	return confirmLeave;
}

void CGame::closeSockets()
{
	if(startNetwork == 0) 
	{
		shutdownClient(clientSocket);
		shutdownServer(serverSocket);

	}
	else if(startNetwork == 1) 
	{
		shutdownClient(clientSocket);
	}

	if(startNetwork != -1)
	{
		menu->networkConnected = false;
		char message[128];
		strcpy(message, player[OPPONENT_PLAYER]->m_playerName);
		strcat(message," has left the game");
		player[USER_PLAYER]->addMessage(message);
	}

	startNetwork = -1;
}

void CGame::networkChoice()
{
	while(1)
	{
		if(startNetwork == 0) 
		{
			hostThread();
		}
		else if(startNetwork == 1) 
		{
			clientThread();
		}
	}
}

void CGame::clientThread()
{
	bool connected = false;
	bool cantconnect = false;

	while((!connected) && (!cantconnect))
	{	
		clientSocket = startupClient(7654,menu->computerName);

		if(clientSocket == -1) 
		{
			// shutdown socket
			menu->addMessage("cannot connect to host",menu->computerName);
			menu->networkConnected = false;
			shutdownClient(clientSocket);
			cantconnect = true;
			startNetwork = -1;
			return;
		}
		else
		{	
			connected = true;
		}
	}

	if(connected)
	{
		sendPlayerData(player[USER_PLAYER]);
		recievePlayerData(player[OPPONENT_PLAYER]);
		menu->addToNetworkPlayerList(player[OPPONENT_PLAYER]);

		//while in the lobby loop
		bool inLobby = true;
		while(inLobby)
		{
			sendLobbyData(menu->inNetworkLobby);
			if(recieveLobbyData() == 1) inLobby = false;

			if(menu->inNetworkLobby != 1) 
			{
				closeSockets();
				connected = false;
				inLobby = false;
				return;
			}
		}

		//initialise the game
		player[USER_PLAYER]->m_health = 100;
		player[USER_PLAYER]->setPositionData();

		player[USER_PLAYER]->stayConnected();

		recievePacket(player[OPPONENT_PLAYER]);
		sendPacket(player[USER_PLAYER]);

		m_loopMode = GAME_LOOP;

		menu->clearPlayerList();
	}

	menu->setInLobby(false);

	while((connected) && (startNetwork != -1))
	{
		//check for exits
		bool leave = false;

		//check if the player wants to quit via the map
		//just incase the original disconnect message is missed
		//if(map->disconnectWish()) player[USER_PLAYER]->disconnect();

		//if i want to disconnet, disconnet me
		if(leaveGame())
		{
			leave = true;
		}
		//otherwise remain connected
		else player[USER_PLAYER]->stayConnected();

		//if the opponent wants to leave, disconnect
		if(checkLeave()) 
		{
			leave = true;
		}

		//if were not leaving send messages
		if(!leave)
		{
			sendPacket(player[USER_PLAYER]);
			recievePacket(player[OPPONENT_PLAYER]);
		}
		//otherwise clean up
		else
		{
			//tell the map that is ok now to quit
			map->disconnectCompleted();
			closeSockets();
		}

		Sleep(1);
	}

}

void CGame::hostThread()
{
	bool connected = false;

	while(!connected)
	{
		// startup our server utilities with my handy function
		char myName[256];

		serverSocket = startupServerForListening(7654,myName);

		if (serverSocket == -1) {
			printf("Network Startup Failed!\nProgram Terminating\n");
			exit(0);
		}

		menu->setNetworkHostName(myName);

		// accept a client
		clientSocket = accept(serverSocket, 0, 0);

		// check for errors
		if (clientSocket == SOCKET_ERROR) 
		{
			menu->networkConnected = false;
			printf("Accept Failed!\n");
			startNetwork = -1;
			return;
		}

		menu->networkConnected = true;
		connected = true;
	}

	if(connected)
	{
		recievePlayerData(player[OPPONENT_PLAYER]);
		sendPlayerData(player[USER_PLAYER]);

		menu->addToNetworkPlayerList(player[OPPONENT_PLAYER]);

		bool inLobby = true;
		while(inLobby)
		{
			//wait until the user has clicked start game
			int ready = 0;
			if(m_loopMode == NETWORK_GAME_START) ready = 1;

			//if the client quits during this time
			if(recieveLobbyData() != 1)
			{
				inLobby = false;
				connected = false;
				closeSockets();
				menu->addMessage(player[OPPONENT_PLAYER]->m_playerName,"Has left the lobby");
				menu->clearPlayerList();
				return;
			}

			//send the ready message
			sendLobbyData(ready);

			if(ready == 1)
			{
				inLobby = false;
			}
		}

		menu->clearPlayerList();
		menu->setInLobby(false);

		while(m_loopMode != NETWORK_GAME_START)
		{
			//stall
		}

		player[USER_PLAYER]->m_health = 100;
		player[USER_PLAYER]->setPositionData();

		player[USER_PLAYER]->stayConnected();

		sendPacket(player[USER_PLAYER]);
		recievePacket(player[OPPONENT_PLAYER]);


		m_loopMode = GAME_LOOP;
	}
	
	while((connected) && (startNetwork != -1))
	{
		//check for exits
		bool leave = false;

		//check if the player wants to quit via the map
		//just incase the original disconnect message is missed
		//if(map->disconnectWish()) player[USER_PLAYER]->disconnect();

		//if the opponent wants to leave, disconnect
		if(checkLeave()) 
		{
			leave = true;
		}
		//if i want to disconnet, disconnet me
		if(leaveGame())
		{
			leave = true;
		}
		//otherwise remain connected
		else player[USER_PLAYER]->stayConnected();

		//if were not leaving send messages
		if(!leave)
		{
			recievePacket(player[OPPONENT_PLAYER]);
			sendPacket(player[USER_PLAYER]);
		}
		//otherwise clean up
		else
		{
			//tell the map that is ok now to quit
			map->disconnectCompleted();
			closeSockets();
		}

		Sleep(1);
	}
}