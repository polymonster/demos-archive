using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using Microsoft.Xna.Framework.Audio;

namespace Tanky
{
    public partial class Game
    {
        // The list of packets waiting to be processed
        private List<queuedPacket> m_PacketQueue;

        public void AddPacketToQueue(gameClient aClient, PacketHeader aHeader, byte[] aData)
        {
            // This can be called from any thread!!
            queuedPacket tempPkt = new queuedPacket();
            tempPkt.m_Client = aClient;
            tempPkt.m_Data = aData;
            tempPkt.m_Header = aHeader;

            lock (m_PacketQueue)
            {
                m_PacketQueue.Add(tempPkt);
            }
        }

        public void ProcessPacketsInQueue()
        {
            lock (m_PacketQueue)
            {
                foreach (queuedPacket packet in m_PacketQueue)
                {
                    HandleNetworkMessage(packet.m_Client, packet.m_Header, packet.m_Data);
                }
                m_PacketQueue.Clear();
            }
        }

        public void OnClientConnectError(string aError)
        {
            //Console.WriteLine(aError);
            g.current_state = (int)g.game_states.MENU_LOOP;
            m_main_menu.handle_errors(aError);
        }

        private void HandleSessionMessage(gameClient aClient, PacketHeader aHeader, byte[] aData)
        {
            switch (aHeader.m_Subtype)
            {
                case PacketSubTypes.ESessionTimerUpdate:
                    {
                        System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                        string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);


                        //sync timer;
                        float newDuration = 0;
                        float elapsedTime = Convert.ToSingle(data);


                        switch (m_GameType)
                        {
                            case g.game_types.TIMED_2:
                                {
                                    newDuration = (1000 * 60 * 2) - elapsedTime;
                                    break;
                                }
                            case g.game_types.TIMED_5:
                                {
                                    newDuration = (1000 * 60 * 5) - elapsedTime;
                                    break;
                                }
                            case g.game_types.TIMED_10:
                                {
                                    newDuration = (1000 * 60 * 10) - elapsedTime;
                                    break;
                                }
                            case g.game_types.TIMED_25:
                                {
                                    newDuration = (1000 * 60 * 25) - elapsedTime;
                                    break;
                                }
                        }
                        g.game_timer = new CountDownTimer(newDuration);
                    }
                    break;

                case PacketSubTypes.ESeesionEndGame:
                    {
                        System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                        string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);
                        string[] parts = data.Split(':');
                        g.GameRef.OnGameEnded(parts[0], Convert.ToBoolean(parts[1]));
                    }
                    break;

                case PacketSubTypes.ESessionForceNewName:
                    {
                        System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                        string playerName = enc.GetString(aData, 0, aHeader.m_DecompressedLength);
                        g.playerInfo.m_Name = playerName;
                        m_main_menu.UpdatePlayerName();
                    }
                    break;

                case PacketSubTypes.ESessionStartGame:
                    {
                        System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                        string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);
                        string[] parts = data.Split(':');
                        int level = Convert.ToInt32(parts[0]);
                        g.game_types type = (g.game_types)Convert.ToInt32(parts[1]);
                        OnGameStart(level,type);
                        m_main_menu.go_to_game();
                        
                    }
                    break;

                case PacketSubTypes.ESessionServerFull:
                    {
                        //Console.WriteLine("SERVER: Server is full!");
                        OnClientConnectError("Server is full, try again later or pick another server");
                    }
                    break;

                case PacketSubTypes.ESessionServerNotFull:
                    {
                        //Console.WriteLine("SERVER: Server not full");
                        //Console.WriteLine("CLIENT: Connected with user name: " + g.playerInfo.m_Name);
                        g.clientData.client.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerConnected, g.playerInfo.m_Name);
                    }
                    break;
            }
        }

        private void HandlePlayerMessage(gameClient aClient, PacketHeader aHeader, byte[] aData)
        {
            switch (aHeader.m_Subtype)
            {
                case PacketSubTypes.EPlayerScoreKillsUpdate:
                    {
                        System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                        string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);
                        string[] parts = data.Split(':');
                        if ( g.playerInfo.m_Name == parts[0] )
                        {
                            g.playerInfo.m_kills ++;
                            g.playerInfo.AddScore(Convert.ToInt32(parts[1]));

                            //Console.WriteLine("CLIENT: Got stat update from server");
                        }
                    }
                    break;

                case PacketSubTypes.EPlayerAnnouce:
                    {
                        System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                        string annoucement = enc.GetString(aData, 0, aHeader.m_DecompressedLength);
                        g.GameRef.HandleAccouncement(annoucement);
                    }
                    break;

                case PacketSubTypes.EPlayerReSpawn:
                    {
                        if (!g.is_server)
                        {
                            System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                            string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);

                            // Find the old player name in the list of players
                            foreach (Player player in g.clientData.players)
                            {
                                if (player.m_Name == data)
                                {
                                    // Spawn the player
                                    player.CreateCorpse();
                                    player.Spawn();

                                    // Exit the loop
                                    break;
                                }
                            }

                            // Check if it was our name that we where updating
                            if (g.playerInfo.m_Name == data)
                            {
                                g.playerInfo.CreateCorpse();
                                g.playerInfo.Spawn();
                            }

                        }
                    }
                    break;

                case PacketSubTypes.EPlayerSpawnData:
                    {
                        if (!g.is_server)
                        {
                            System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                            string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);

                            // Split the string at ":" because we have "playername:spawndata"
                            string[] parts = data.Split(':');

                            // Find the old player name in the list of players
                            foreach (Player player in g.clientData.players)
                            {
                                if (player.m_Name == parts[0])
                                {
                                    // Spawn the player
                                    player.Spawn(Convert.ToInt32(parts[1]));

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
                    }
                    break;

                case PacketSubTypes.EPlayerTeamUpdate:
                    {
                        if (!g.is_server)
                        {
                            System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                            string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);

                            // Split the string at ":" because we have "playername:newplayerteam"
                            string[] parts = data.Split(':');

                            // Find the old player name in the list of players
                            foreach (Player player in g.clientData.players)
                            {
                                if (player.m_Name == parts[0])
                                {
                                    // Update the model
                                    player.m_team = Convert.ToInt32(parts[1]);

                                    // Exit the loop
                                    break;
                                }
                            }

                            // Check if it was our name that we where updating
                            if (g.playerInfo.m_Name == parts[0])
                            {
                                g.playerInfo.m_team = Convert.ToInt32(parts[1]);
                            }

                        }
                    }
                    break;

                case PacketSubTypes.EPlayerModelUpdate:
                    {
                        if (!g.is_server)
                        {
                            System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                            string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);

                            // Split the string at ":" because we have "playername:newplayermodel"
                            string[] parts = data.Split(':');

                            // Find the old player name in the list of players
                            foreach (Player player in g.clientData.players)
                            {
                                if (player.m_Name == parts[0])
                                {
                                    // Update the model
                                    player.SetModel(Convert.ToInt32(parts[1]));

                                    // Exit the loop
                                    break;
                                }
                            }

                            // Check if it was our name that we where updating
                            if (g.playerInfo.m_Name == parts[0])
                            {
                                g.playerInfo.SetModel(Convert.ToInt32(parts[1]));
                            }

                        }
                    }
                    break;

                case PacketSubTypes.EPlayerNameUpdate:
                    {
                        if (!g.is_server)
                        {
                            System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                            string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);

                            // Split the string at ":" because we have "oldplayername:newplayername"
                            string[] parts = data.Split(':');

                            // Find the old player name in the list of players
                            foreach (Player player in g.clientData.players)
                            {
                                if (player.m_Name == parts[0])
                                {
                                    // Update the name
                                    player.m_Name = parts[1];

                                    // Exit the loop
                                    break;
                                }
                            }

                            // Check if it was our name that we where updating
                            if (g.playerInfo.m_Name == parts[0])
                            {
                                g.playerInfo.m_Name = parts[1];
                                m_main_menu.UpdatePlayerName();
                            }

                        }
                    }
                    break;

                case PacketSubTypes.EPlayerConnected:
                    {
                        // A player has connected so show a mesage or something?
                        System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                        string playerName = enc.GetString(aData, 0, aHeader.m_DecompressedLength);
                        //Console.WriteLine("Player: " + playerName + " has joined the game");

                        // We're not the server so we need to maintain the list of clients!
                        if (!g.is_server)
                        {
                            Player player = new Player(playerName);
                            g.clientData.players.Add(player);
                        }
                    }
                    break;

                case PacketSubTypes.EPlayerPingRequest:
                    {
                        // The server is requesting a ping
                        if (!g.is_server)
                        {
                            string data = g.playerInfo.m_Name;
                            if (g.clientData.client != null)
                            {
                                g.clientData.client.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerPingResponse, data);
                            }
                        }
                    }
                    break;

                case PacketSubTypes.EPlayerDisconnected:
                    {
                        if (!g.is_server)
                        {
                            System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                            string playerName = enc.GetString(aData, 0, aHeader.m_DecompressedLength);
                            foreach (Player player in g.clientData.players)
                            {
                                if (player.m_Name == playerName)
                                {
                                    g.clientData.players.Remove(player);
                                    break;
                                }
                            }
                        }
                    }
                    break;

                case PacketSubTypes.EPlayerPingUpdate:
                    {
                        if (!g.is_server)
                        {
                            System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                            string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);

                            // Split the string at ":" because we have "playername:ping"
                            string[] parts = data.Split(':');

                            // Is it our ping?
                            if (g.clientData.client.m_PlayerInfo != null)
                            {
                                if (g.clientData.client.m_PlayerInfo.m_Name == parts[0])
                                {
                                    g.clientData.client.m_PlayerInfo.m_Ping = Convert.ToInt32(parts[1]);
                                }
                            }

                            // Find out which client it is and update the ping
                            foreach (Player player in g.clientData.players)
                            {
                                if (player.m_Name == parts[0])
                                {
                                    player.m_Ping = Convert.ToInt32(parts[1]);
                                    break;
                                }
                            }

                        }
                    }
                    break;

                case PacketSubTypes.EPlayerMessage:  // New message
                    {
                        // A new message packet is just a string for now
                        System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                        string message = enc.GetString(aData, 0, aHeader.m_DecompressedLength);

                        // Add it to the window 
                        g.chat_window.add_message(message, true);

                    }
                    break;

                default:
                    {
                        // Unknown sub type
                    }
                    break;
            } // End switch
        }

        private void HandleMovementMessage(gameClient aClient, PacketHeader aHeader, byte[] aData)
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

                case PacketSubTypes.EMovementProjectileHit:
                    {
                        System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                        string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);
                        string[] parts = data.Split(':');
                        g.GameRef.DamagePlayer(parts[0], Convert.ToInt32(parts[1]),parts[2] );
                    }
                    break;

                case PacketSubTypes.EMovementSpawnProjectile:
                    {
                        //Console.WriteLine("Server telling us to spawn a projectile!");
                        // Get the information on where the projectile
                        System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                        string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);

                        // Spawn it
                        ProjectilePackage pkg = new ProjectilePackage(data);
                        g.projectile_manager.Add(pkg);
                        g.sound_man.Shot.Play();
                    }
                    break;

                case PacketSubTypes.EMovementUpdatePlayer:
                    {
                        if (!g.is_server)
                        {
                            // Populate CompletePackage from aData
                            System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                            string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);
                            CompletePackage packageIn = CompletePackage.CreateFromString(data);

                            // Find the player that this update belongs to
                            foreach (Player player in g.clientData.players)
                            {
                                // Is it this player?
                                if (player.m_Name == packageIn.playerPackage.Name)
                                {
                                    player.ProcessUpdateFromServer(packageIn);

                                    // Exit the loop
                                    break;
                                }
                            }

                            if (g.playerInfo.m_Name == packageIn.playerPackage.Name)
                            {
                                g.playerInfo.ProcessUpdateFromServer(packageIn);
                            }
                        }
                    }
                    break;

                case PacketSubTypes.EMovementUpdateProp:
                    {
                        //Console.WriteLine("Got prop update");
                        System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
                        string data = enc.GetString(aData, 0, aHeader.m_DecompressedLength);
                        string[] parts = data.Split(';');
                        foreach (string propData in parts)
                        {
                            // Unpack the data for this prop
                            PropPackage pkg = PropPackage.CreateFromString(propData);
                            
                            // Find which prop this data packet is for
                            foreach (Prop prop in g.level.m_prop_list)
                            {
                                if (pkg.propId == prop.m_prop_id)
                                {
                                    // Update the prop
                                    prop.SnapShotIn(pkg);

                                    // Exit the loop
                                    break;
                                }
                            }
                        }
                    }
                    break;
            } // End switch
        }

        private void HandleNetworkMessage(gameClient aClient, PacketHeader aHeader, byte[] aData)
        {
            switch (aHeader.m_Family)
            {
                case PacketFams.EMovementData:
                    {
                        HandleMovementMessage(aClient, aHeader, aData);
                    }
                    break;

                case PacketFams.EPlayerManagement:
                    {
                        HandlePlayerMessage(aClient, aHeader, aData);
                    }
                    break;

                case PacketFams.ESessionManagement:
                    {
                        HandleSessionMessage(aClient, aHeader, aData);
                    }
                    break;

                default:
                    {
                        // Unknown packet fam
                    }
                    break;
            } // End switch
        }
    }
}
