using System;
using System.Net;
using System.Net.Sockets;
using System.IO;
using Microsoft.Xna.Framework.Net;
using System.Collections.Generic;
using System.Security.Principal;
using System.Threading;
using System.Diagnostics;

namespace Tanky
{
    public struct queuedPacket
    {
        public gameClient m_Client;
        public PacketHeader m_Header; 
        public byte[] m_Data;
    }

    public class gameServer
    {
        // Used to listen for the game clients
        private TcpListener m_Listener;

        // The port used for connections
        public static int m_ListenPort = 10;

        // The clients connected to the server
        public List<gameClient> m_Clients;

        // Max number of clients
        private const int m_MaxClients = 4;

        private float m_milliSecondsElapsed;

        private bool m_Listening;

        // The list of packets waiting to be processed
        private List<queuedPacket> m_PacketQueue;

        // Used to pump out UDP messages to let clients on a lan know we're here
        private UDPBroadcaster m_Broadcaster;
        Stopwatch m_BroadCastTimer;

        public gameServer()
        {
            // Listen on all network interfaces
            IPAddress allInterfaces = IPAddress.Parse("0.0.0.0");

            // Set up the listening socket
            m_Listener = new TcpListener(allInterfaces, m_ListenPort);

            // Open the port and listen for clients
            m_Listener.Start();

            // Create the container to hold the list of connected clients
            m_Clients = new List<gameClient>();

            // We are ready to accept connections
            m_Listening = true;

            m_PacketQueue = new List<queuedPacket>();

            m_Broadcaster = new UDPBroadcaster();
            m_BroadCastTimer = new Stopwatch();
            m_BroadCastTimer.Start();
        }

        public void StopListening()
        {
            if (m_Listening == true)
            {
                m_Listener.Stop();
                m_BroadCastTimer.Stop();
            }
            m_Listening = false;
        }

        public void Update()
        {
            // Handle any packets we've aquired
            ProcessPacketsInQueue();

            // Connect any new clients that are waiting
            CheckForNewClients();

            // Remove any clients that have disconnected
            RemoveDeadClients();

            // Check if 1 second has passed
            m_milliSecondsElapsed += g.game_time.ElapsedGameTime.Milliseconds;
            if ( m_milliSecondsElapsed >= 1000 )
            {
                // Reset
                m_milliSecondsElapsed = 0;

                // Ping the clients
                foreach (gameClient client in m_Clients)
                {
                    if ( client.m_Socket.Connected == true )
                    {
                        if (!client.m_PingReponsePending)
                        {
                            client.m_PingReponsePending = true;
                            client.m_PingMillisecondsElapsed = System.Environment.TickCount;
                            client.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerPingRequest, "");
                        }
                    }
                }
            }
        }

        private void RemoveDeadClients()
        {
            foreach (gameClient client in m_Clients)
            {
                if ( client.m_Socket.Connected == false )
                {
                    // If this player was fully connected
                    if (client.m_PlayerInfo != null)
                    {
                        // Then tell every other client that this one is gone
                        foreach (gameClient clientConnected in m_Clients)
                        {
                            clientConnected.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerAnnouce, client.m_PlayerInfo.m_Name + " has left the game");
                            clientConnected.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerDisconnected, client.m_PlayerInfo.m_Name);
                            g.GameRef.HandleAccouncement(client.m_PlayerInfo.m_Name + " has left the game");
                        }
                    }

                    m_Clients.Remove( client );
                    //Console.WriteLine("Removed dead client: " + client.m_Socket.RemoteEndPoint.ToString());
                    // TODO: Remove all dead clients at once, do this in reverse
                    break;
                }
            }
        }

        public void SendToAllClients(PacketFams aFam, PacketSubTypes aSubType, byte[] aData )
        {
            // Server to clients message
            foreach (gameClient client in m_Clients)
            {
                client.SendMessage(aFam, aSubType, aData);
            }
        }

        public void SendToAllClients(PacketFams aFam, PacketSubTypes aSubType, string aString)
        {
            // Server to clients message
            foreach (gameClient client in m_Clients)
            {
                client.SendMessage(aFam, aSubType, aString);
            }
        }

        public bool GetUniqueName(ref string aName)
        {
            // Make sure the name in not already taken
            bool nameUsed = false;
            foreach (gameClient client in m_Clients)
            {
                if (client.m_PlayerInfo != null)
                {
                    if (client.m_PlayerInfo.m_Name == aName)
                    {
                        // The name is used
                        nameUsed = true;

                        // Append a random number to the name
                        Random rand = new Random();
                        int number = rand.Next(0, 9999);
                        aName = aName + number.ToString();

                        // Exit the loop
                        break;
                    }
                }
            }

            // Finally check the it dosent clash with the servers name
            if (!nameUsed)
            {
                if (g.playerInfo.m_Name == aName)
                {
                    nameUsed = true;

                    // Append a random number to the name
                    Random rand = new Random();
                    int number = rand.Next(0, 9999);
                    aName = aName + number.ToString();

                }
            }

            return nameUsed;
        }

        public void SpawnPlayer(string aPlayerName)
        {
            // If its the local server player that died then we just respawn
            if (aPlayerName == g.playerInfo.m_Name)
            {
                //g.playerInfo.CreateCorpse();
                g.playerInfo.Spawn();
            }
            else
            {
                // Find out which client it is
                foreach (gameClient client in m_Clients)
                {
                    if (client.m_PlayerInfo.m_Name == aPlayerName)
                    {
                        // Tell this client it needs to re-spawn
                        //client.m_PlayerInfo.CreateCorpse();
                        client.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerReSpawn, aPlayerName );
                        break;
                    }
                }
            }
        }

        // Spawn a single player who died
        public void SpawnPlayer(gameClient aClient, int aSpawnPointId)
        {
            string data;
            data = aClient.m_PlayerInfo.m_Name + ":" + aSpawnPointId.ToString();
            aClient.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerSpawnData, data );
        }

        // Spawn everyone at the start of a game
        public void SpawnAllPlayers()
        {
            // Get the number of players
            int numPlayers = m_Clients.Count+1; // +1 for server local player

            // Get spawn points for each player
            int[] spawnPointIds = new int[numPlayers];
            g.level.get_unique_spawns(ref spawnPointIds);

            int count = 1;
            foreach (gameClient client in m_Clients)
            {
                // Tell this client to spawn!
                SpawnPlayer(client, spawnPointIds[count++]);
            }

            // Spawn our local server player and tell everyone about it
            g.playerInfo.Spawn(spawnPointIds[0]);
        }

        public void AddPacketToQueue(gameClient aClient, PacketHeader aHeader, byte[] aData)
        {
            // This can be called from any thread!!
            queuedPacket tempPkt = new queuedPacket();
            tempPkt.m_Client = aClient;
            tempPkt.m_Data = aData;
            tempPkt.m_Header = aHeader;

            lock ( m_PacketQueue )
            {
                m_PacketQueue.Add(tempPkt);
            }

        }

        public void ProcessPacketsInQueue()
        {
            lock ( m_PacketQueue )
            {
                foreach (queuedPacket packet in m_PacketQueue)
                {
                    OnClientMessage(packet.m_Client, packet.m_Header, packet.m_Data);
                }
                m_PacketQueue.Clear();
            }
        }

        private void OnClientMessage(gameClient aClient, PacketHeader aHeader, byte[] aData)
        {
            bool forwardMessage = true;

            switch (aHeader.m_Family)
            {
                case PacketFams.EMovementData:
                    {
                        switch (aHeader.m_Subtype)
                        {
                            case PacketSubTypes.ESessionPlayerDead:
                                {
                                    System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                                    string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);
                                    string[] parts = data.Split(':');
                                    g.GameRef.HandlePlayerDeath(parts[0], parts[1]);
                                }
                                break;

                            case PacketSubTypes.EMovementRequestFireProjectile:
                                {
                                    //Console.WriteLine("Client requested to spawn a projectile spawning it and telling clients about this new projectile");

                                    // Get the information on where the projectile
                                    System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                                    string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);

                                    // Spawn it
                                    ProjectilePackage pkg = new ProjectilePackage(data);
                                    g.projectile_manager.Add(pkg);
                                    

                                    //m_fire_sound.Play(0.8f);

                                    // Tell the clients about this projectile
                                    SendToAllClients(PacketFams.EMovementData, PacketSubTypes.EMovementSpawnProjectile, data );
                                }
                                break;

                            case PacketSubTypes.EPlayerSpawnData:
                                {
                                    System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                                    string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);

                                    // Split the string at ":" because we have "playername:spawndata"
                                    string[] parts = data.Split(':');

                                    // Find the old player name in the list of players
                                    foreach (gameClient client in m_Clients)
                                    {
                                        if (client.m_PlayerInfo.m_Name == parts[0])
                                        {
                                            // Spawn the player
                                            client.m_PlayerInfo.Spawn(Convert.ToInt32(parts[1]));

                                            // Exit the loop
                                            break;
                                        }
                                    }

                                    // Check if it was our name that we where updating
                                    if (g.playerInfo.m_Name == parts[0])
                                    {
                                        g.playerInfo.Spawn(Convert.ToInt32(parts[1]));
                                    }
                                }
                                break;

                            case PacketSubTypes.EMovementUpdatePlayer:
                                {
                                    // Populate CompletePackage from aData
                                    System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                                    string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);
                                    CompletePackage packageIn = CompletePackage.CreateFromString(data);

                                    // Find the player that this update belongs to
                                    foreach (gameClient client in m_Clients)
                                    {
                                        // Is it this player?
                                        if (client.m_PlayerInfo.m_Name == packageIn.playerPackage.Name)
                                        {
                                            client.m_PlayerInfo.ProcessUpdateFromServer(packageIn);

                                            // Exit the loop
                                            break;
                                        }
                                    }

                                    // Is it the local player?
                                    if (g.playerInfo.m_Name == packageIn.playerPackage.Name)
                                    {
                                        g.playerInfo.ProcessUpdateFromServer(packageIn);
                                    }

                                    // Manually forward the data because we don't want to send it back
                                    // to who it came from!
                                    forwardMessage = false;

                                    foreach (gameClient client in m_Clients)
                                    {
                                        if (client != aClient)
                                        {
                                            client.SendPrebuiltMessage(aHeader, aData);
                                        }
                                    }
                                }
                                break;
                        }
                    }
                    break;


                case PacketFams.EPlayerManagement:
                    {
                        switch (aHeader.m_Subtype)
                        {
                            case PacketSubTypes.EPlayerMessage:  // New message
                                {
                                    // A new message packet is just a string for now
                                    System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                                    string message = enc.GetString(aData, 0, aHeader.m_DecompressedLength);

                                    // Add it to the window 
                                    g.chat_window.add_message(message, true);

                                }
                                break;

                            case PacketSubTypes.EPlayerSettingsUpdate:
                                {
                                    System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                                    string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);

                                    // Split the string at semi colons
                                    string[] parts = data.Split(':');

                                    // Check if the name has actually changed for this client
                                    if (aClient.m_PlayerInfo.m_Name != parts[0])
                                    {
                                        // Update the name with a number if its already taken
                                        while (GetUniqueName(ref parts[0]) != false) { };

                                        g.GameRef.HandleAccouncement(aClient.m_PlayerInfo.m_Name + " is now known as " + parts[0]);

                                        // Tell the clients about the new name
                                        foreach (gameClient client in m_Clients)
                                        {
                                            client.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerNameUpdate, aClient.m_PlayerInfo.m_Name + ":" + parts[0]);
                                            client.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerAnnouce, aClient.m_PlayerInfo.m_Name + " is now known as " + parts[0] );
                                        }

                                        // Set the new name on the server
                                        aClient.m_PlayerInfo.m_Name = parts[0];
                                    }

                                    // Update the team and tell the clients about the new team
                                    aClient.m_PlayerInfo.m_team = Convert.ToInt32(parts[1]);
                                    SendToAllClients(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerTeamUpdate, aClient.m_PlayerInfo.m_Name + ":" + parts[1] );

                                    // Update the player model and tell the clients about the new model
                                    aClient.m_PlayerInfo.SetModel(Convert.ToInt32(parts[2]));
                                    SendToAllClients(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerModelUpdate, aClient.m_PlayerInfo.m_Name + ":" + parts[2]);

                                }
                                break;

                            case PacketSubTypes.EPlayerPingResponse:
                                {
                                    if (aClient.m_PlayerInfo != null)
                                    {
                                        aClient.m_PlayerInfo.m_Ping = System.Environment.TickCount - aClient.m_PingMillisecondsElapsed;

                                        aClient.m_PingReponsePending = false;
                                        forwardMessage = false;

                                        // Let all the other clients update their player list pings
                                        string data = aClient.m_PlayerInfo.m_Name + ":" + aClient.m_PlayerInfo.m_Ping.ToString();

                                        // Tell the clients about this ping update (including who sent it because they need to know their own ping!)
                                        foreach (gameClient client in m_Clients)
                                        {
                                            client.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerPingUpdate, data);
                                        }
                                    }

                                }
                                break;

                            case PacketSubTypes.EPlayerConnected:
                                {
                                    System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                                    string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);

                                    string nameUnquie = data;

                                    while (GetUniqueName(ref nameUnquie) != false) { };
                                    //Console.WriteLine("SERVER: New player with name: " + nameUnquie + " Connected");
                                    if (nameUnquie != data)
                                    {
                                        //Console.WriteLine("The new clients name had to be changed to make it unique sending it back to the client");
                                        aClient.SendMessage(PacketFams.ESessionManagement, PacketSubTypes.ESessionForceNewName, nameUnquie);
                                    }

                                    aClient.m_PlayerInfo = new Player(nameUnquie);

                                    // Tell the client who we are
                                    //Console.WriteLine("SERVER: Telling new client who THIS player is (" + g.playerInfo.m_Name + ")");
                                    aClient.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerConnected, g.playerInfo.m_Name);
                                    aClient.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerTeamUpdate, g.playerInfo.m_Name + ":" + g.playerInfo.m_team.ToString());
                                    aClient.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerModelUpdate, g.playerInfo.m_Name + ":" + g.playerInfo.m_Model.ToString());

                                    // Tell the client who else is connected
                                    foreach( gameClient client in m_Clients )
                                    {
                                        if (client.m_PlayerInfo != null)
                                        {
                                            if (client != aClient)
                                            {
                                                //Console.WriteLine("SERVER: Tell new client about player: " + client.m_PlayerInfo.m_Name);
                                                aClient.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerConnected, client.m_PlayerInfo.m_Name);

                                                // Update the team and tell the clients about the new team
                                                SendToAllClients(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerTeamUpdate, client.m_PlayerInfo.m_Name + ":" + client.m_PlayerInfo.m_team.ToString());

                                                // Update the player model and tell the clients about the new model
                                                SendToAllClients(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerModelUpdate, client.m_PlayerInfo.m_Name + ":" + client.m_PlayerInfo.m_Model.ToString());
                                            }
                                        }
                                    }

                                    // Tell the other clients about this new player
                                    //Console.WriteLine("SERVER: Tell old clients about player: " + nameUnquie);
                                    foreach (gameClient client in m_Clients)
                                    {
                                        if (client != aClient)
                                        {
                                            client.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerConnected, nameUnquie);
                                            client.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerAnnouce, nameUnquie + " has joined the game");
                                        }
                                    }
                                    g.GameRef.HandleAccouncement(nameUnquie + " has joined the game");
                                    forwardMessage = false;
                                }
                                break;
                        }
                    }
                    break;

                case PacketFams.ESessionManagement:
                    {
                        switch (aHeader.m_Subtype)
                        {
                            case PacketSubTypes.ESessionIsServerFull:  // Client inital check to see if server is full
                                {
                                    if ( m_Clients.Count >= m_MaxClients )
                                    {
                                        // Tell the client that the server is full
                                        aClient.SendMessage(PacketFams.ESessionManagement, PacketSubTypes.ESessionServerFull, "" );
                                        // TODO: Remove the client from the list to close down the connection?
                                    }
                                    else
                                    {
                                        aClient.SendMessage(PacketFams.ESessionManagement, PacketSubTypes.ESessionServerNotFull, "" );
                                    }
                                    // Don't send this packet to other clients!
                                    forwardMessage = false;
                                }
                                break;

                            case PacketSubTypes.ESessionForceNewName:
                                {
                                    forwardMessage = false;
                                }
                                break;

                            default:
                                {
                                    // Unknown sub type
                                }
                                break;
                        } // End switch
                    }
                    break;

                default:
                    {
                        // Unknown packet fam
                    }
                    break;
            } // End switch

            if (forwardMessage)
            {
                // Pass to message to everyone else
                foreach (gameClient client in m_Clients)
                {
                    // Don't send the message back to who it came from!
                    if (client != aClient)
                    {
                        //Console.WriteLine("Forwarding message from " + aClient.m_Socket.RemoteEndPoint.ToString() + " to " + client.m_Socket.RemoteEndPoint.ToString());
                        client.SendPrebuiltMessage(aHeader, aData);
                    }
                }
            }
        }

        private void CheckForNewClients()
        {
            if (m_Listening)
            {
                if (m_BroadCastTimer.ElapsedMilliseconds > 1000)
                {
                    // Reset the timer
                    m_BroadCastTimer.Reset();
                    m_BroadCastTimer.Start();

                    // Send a broad cast msg
                    m_Broadcaster.BroadCast();
                }

                // Check if anyone is trying to connect
                if (m_Listener.Pending())
                {
                    // There is a client waiting to connect
                    //Console.WriteLine("Client waiting, constructing a new client to deal with it!");

                    // So construct a new client to accept the connection
                    gameClient newClient = new gameClient(ref m_Listener);

                    // Add the client to the list
                    m_Clients.Add(newClient);

                    // Start reading data from the client, the client will continue reading data by itself
                    newClient.ReadNewData();
                }
            }
        }

    }
}
