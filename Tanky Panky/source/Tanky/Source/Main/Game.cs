using System;
using System.Net;
using System.Net.Sockets;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Net;
using Microsoft.Xna.Framework.Storage;
using Tanky.Source.Physics.Display;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace Tanky
{
    public struct ModelEntry
    {
        public String name;
        public Texture2D[] texture;
        public int texture_count;
    }

    public partial class Game : Microsoft.Xna.Framework.Game
    {
        Menu m_main_menu;
        Timer inTime = new Timer();
        Timer propTime = new Timer();
        public g.game_types m_GameType;

        private struct spawnData
        {
            public spawnData(string aName)
            {
                m_PlayerName = aName;
                m_Timer = new CountDownTimer(1000 * 10); // respawn after 10 seconds
            }
            public string m_PlayerName;
            public CountDownTimer m_Timer;
        }

        private List<spawnData> m_ReSpawnList;

        public void HandleDoPlayerRespawns()
        {
            if (g.is_server)
            {
                foreach (spawnData spawn in m_ReSpawnList)
                {
                    // Check if its time to respawn this client
                    if (spawn.m_Timer.IsFinished)
                    {
                        // Respawn the client
                        g.server.SpawnPlayer(spawn.m_PlayerName);

                        // Remove it from the list
                        m_ReSpawnList.Remove(spawn);

                        // Exit the loop
                        break;
                    }
                }
            }
        }

        public void HandlePlayerDeath(string aPlayer, string aKiller )
        {
            // Find the who got hit
            Player player = Tools.findPlayer(aPlayer);

            if (g.is_server)
            {
                // Annouce its death
                string annoucement = player.m_Name + " was killed by " + aKiller + "!";
                g.GameRef.HandleAccouncement(annoucement);
                g.server.SendToAllClients(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerAnnouce, annoucement);

                // Add the dead client to the respawn queue
                spawnData spawn = new spawnData(player.m_Name);
                m_ReSpawnList.Add(spawn);

                // Find the player that did the hit
                Player otherPlayer = Tools.findPlayer(aKiller);

                // Send an update for this players kills and score
                if (aKiller == g.playerInfo.m_Name)
                {
                    g.playerInfo.m_kills++;
                    g.playerInfo.AddScore(player.m_value);
                    //Console.WriteLine("SERVER: Updated local stats");
                }
                else
                {
                    //amount to add to the player score
                    int killValue = player.m_value;
                    g.server.SendToAllClients(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerScoreKillsUpdate, aKiller + ": " + killValue);
                    //Console.WriteLine("SERVER: Sending stat update");
                }

                // Check if that player won in a kills based match
                bool playerWon = false;
                switch (g.GameRef.m_GameType)
                {
                    case g.game_types.DEATHMATCH_10:
                        {
                            if (otherPlayer.m_kills >= 10)
                            {
                                playerWon = true;
                            }
                        }
                        break;

                    case g.game_types.DEATHMATCH_20:
                        {
                            if (otherPlayer.m_kills >= 20)
                            {
                                playerWon = true;
                            }
                        }
                        break;

                    case g.game_types.DEATHMATCH_50:
                        {
                            if (otherPlayer.m_kills >= 50)
                            {
                                playerWon = true;
                            }
                        }
                        break;


                    case g.game_types.DEATHMATCH_100:
                        {
                            if (otherPlayer.m_kills >= 100)
                            {
                                playerWon = true;
                            }
                        }
                        break;
                }

                if (playerWon)
                {
                    g.GameRef.OnGameEnded("A Winrar is you " + otherPlayer.m_Name , false);
                    string data = "A Winrar is you " + otherPlayer.m_Name + ":false";
                    g.server.SendToAllClients(PacketFams.ESessionManagement, PacketSubTypes.ESeesionEndGame, data);
                }
            }

            // Spawn its corpse
            player.CreateCorpse();
            //Console.WriteLine("Doing corpse");
            
        }

        public void DamagePlayer(string aPlayerToDamage, int aDamage, string aPlayerFrom)
        {
            //Console.WriteLine("Do damage to " + aPlayerToDamage + " from " + aPlayerFrom );

            // Check if its our local player
            if (aPlayerToDamage == g.playerInfo.m_Name)
            {
                // Take some damage
                g.playerInfo.TakeDamage(aDamage);

                // Did it kill us?
                if (!g.playerInfo.IsAlive())
                {
                    if (g.is_server)
                    {
                        // Handle player death
                        HandlePlayerDeath(aPlayerToDamage, aPlayerFrom);
                        
                        // Tell the clients we died
                        g.server.SendToAllClients(PacketFams.EMovementData, PacketSubTypes.ESessionPlayerDead, aPlayerToDamage + ":" + aPlayerFrom);

                        // Spawn our corpse locally as we we wont get HandlePlayerDeath() as a result of sending the message
                        g.playerInfo.CreateCorpse();
                    }
                    else
                    {
                        // Tell the server we died!
                        g.clientData.client.SendMessage(PacketFams.EMovementData, PacketSubTypes.ESessionPlayerDead, aPlayerToDamage + ":" + aPlayerFrom);
                    }

                    // Either way the local player has died, so we can create their corpse now
                    g.playerInfo.CreateCorpse();
                }
            }
        }

        public void OnFoundServer(string aServerName, string aServerIP)
        {
            //Console.WriteLine("Found server: " + aServerName + " at ip " + aServerIP);
            if (!g.m_server_list.ContainsKey(aServerName))
            {
                g.m_server_list.Add(aServerName, aServerIP);
            }
        }

        public Game()
        {
            //handy global reference of the game things
            g.content = Content;
            g.graphics = new GraphicsDeviceManager(this);
            g.GameRef = this;
            g.content.RootDirectory = "Content";
            m_PacketQueue = new List<queuedPacket>();
            m_ReSpawnList = new List<spawnData>();
        }

        public void go_fullscreen()
        {        
            //set the screen to fullscreen at the current desktop resolution

            g.graphics.PreferredBackBufferHeight = GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Height;            
            g.graphics.PreferredBackBufferWidth = GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Width;            
            g.graphics.IsFullScreen = true;            
            g.graphics.ApplyChanges();            
        }

        protected override void Initialize()
        {
            g.oldKeyBoardState = Keyboard.GetState();

            go_fullscreen();

            //rendering setups
            g.sprite_batch = new SpriteBatch(g.graphics.GraphicsDevice);
            g.terrain_shader = g.content.Load<Effect>("Effects\\Terrain");
            g.model_shader = g.content.Load<Effect>("Effects\\Model");
            g.model_list = new List<ModelEntry>();

            //setup the menu and state to loop
            g.current_state = (int)g.game_states.MENU_LOOP;
            m_main_menu = new Menu();

            g.chat_window = new ChatWindow();
            g.playerInfo = new Player();

            g.text_outputter = new TextOutput();

            g.projectile_manager = new ProjectileManager();
            g.effect_sys = new EffectSystem();

            g.hud = new HUD();
            g.corpse_man = new CorpseManager();
            g.sound_man = new Sound();

            g.m_server_list = new Dictionary<string, string>();
           
            base.Initialize();
        }

        protected override void Update(GameTime gameTime)
        {
            g.game_time = gameTime;

            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed) g.current_state = (int)g.game_states.EXIT;
            if (Keyboard.GetState().IsKeyDown(Keys.Escape)) g.current_state = (int)g.game_states.EXIT;

            // Are we running as the server?
            if (g.is_server)
            {
                // Update all clients
                g.server.Update();
            }
            else
            {
                // Handle packets from the server
                ProcessPacketsInQueue();
            }

            switch (g.current_state)
            {
                case (int) g.game_states.MENU_LOOP:
                case (int)g.game_states.LOBBY_LOOP:
                case (int)g.game_states.GAME_LOBBY_LOOP:

                    m_main_menu.update_menu();

                break;

                case (int)g.game_states.GAME_LOOP:

                    game_update_loop();

                break;
                case (int)g.game_states.STATS_LOOP:
                {
                    g.hud.Update();
                }
                break;

                case (int)g.game_states.EXIT:

                    this.Exit(); 

                break;
            }

            base.Update(gameTime);
        }

        public void HandleAccouncement(string aAnouncement)
        {
            g.chat_window.add_message(aAnouncement, true);
            g.hud.AddAnnouncement(aAnouncement);
        }

        void game_update_loop()
        {
            // Respawn clients if needed!
            HandleDoPlayerRespawns();

            // TEMP FIX ME! only send when the player has pressed a key!
            inTime.Update();
            propTime.Update();

            // props have higher latency as they require more bandwidth to sync
            if (propTime.Elapsed > 250)
            {
                if (g.is_server)
                {
                     
                    // Sync the level props
                    string propsPacket = "";
                    foreach (Prop prop in g.level.m_prop_list)
                    {
                        if (prop.IsDynamic())
                        {
                            propsPacket = propsPacket + prop.SnapShotOut().ToString() + ";";
                        }
                    }

                    // Remove the trailing "£"
                    if (propsPacket[propsPacket.Length - 1] == ';')
                    {
                        propsPacket = propsPacket.Remove(propsPacket.Length - 1);
                    }


                    //g.server.SendToAllClients(PacketFams.EMovementData, PacketSubTypes.EMovementUpdateProp, propsPacket);
                    //Console.WriteLine("Syncing props..");
                }

                propTime.Reset();
            }

            if (inTime.Elapsed > 60)
            {
                // Sync the level timer, and check if it has expired
                if (g.is_server)
                {
                    bool sendTimerUpdate = false;
                    switch (m_GameType)
                    {
                        case g.game_types.TIMED_2:
                        case g.game_types.TIMED_5:
                        case g.game_types.TIMED_10:
                        case g.game_types.TIMED_25:
                            {
                                sendTimerUpdate = true;
                            }
                            break;
                    }
                    if (sendTimerUpdate)
                    {
                        Player high_kill = Tools.findHighestKills();
                        if (high_kill.m_kills >= g.game_kill_limit && g.game_kill_limit != -1)
                        {
                            bool draw = false;
                            string winner = "A winrar is you " + high_kill.m_Name;
                            string data = winner + ":" + draw.ToString();
                            g.server.SendToAllClients(PacketFams.ESessionManagement, PacketSubTypes.ESeesionEndGame, data);
                            OnGameEnded(winner, draw);
                        }
                        else if (g.game_timer.IsFinished)
                        {
                            // TODO: Find the player that won, or check if it was a draw
                            bool draw = false;


                            string tempWin = Tools.GetWinnerString();
                            if(tempWin.Split(" and ".ToCharArray()).Length > 1 )
                            {
                                draw = true;
                            }

                            string winner = tempWin;
                            string data = winner + ":" + draw.ToString();
                            g.server.SendToAllClients(PacketFams.ESessionManagement, PacketSubTypes.ESeesionEndGame, data);
                            OnGameEnded(winner, draw);
                        }
                        else
                        {
                            // Sync the clients game timers
                            g.server.SendToAllClients(PacketFams.ESessionManagement, PacketSubTypes.ESessionTimerUpdate, g.game_timer.ElapsedTime.ToString());
                        }
                    }

                    // Send local player movement of the server to all clients
                    g.server.SendToAllClients(PacketFams.EMovementData, PacketSubTypes.EMovementUpdatePlayer, g.playerInfo.Bundle().ToString());
                }
                else
                {
                    // Send local player movement of the client to the server so all other clients get it
                    g.clientData.client.SendMessage(PacketFams.EMovementData, PacketSubTypes.EMovementUpdatePlayer, g.playerInfo.Bundle().ToString());
                }
                inTime.Reset();
            }

            g.chat_window.type();

            g.effect_sys.Update();

            g.level.update();

            g.physics.Update(g.game_time);

            g.keyboard_state = Keyboard.GetState();
            g.gamepad_state = GamePad.GetState(0);
            g.mouse.m_state = Mouse.GetState();
            g.mouse.update_mouse();

            if (!g.chat_window.talk)
            {
                if (g.playerInfo.IsAlive())
                {
                    g.playerInfo.m_Vehicle.Update(g.physics.m_collisionSpace.simulationSettings.timeStep);
                }
            }

            if (g.is_server)
            {
                foreach (gameClient client in g.server.m_Clients)
                {
                    if (client.m_PlayerInfo.m_Vehicle != null)
                    {
                        client.m_PlayerInfo.UpdateDamageEmitter();

                        if (client.m_PlayerInfo.m_Name == g.playerInfo.m_Name)
                        {
                            if (client.m_PlayerInfo.IsAlive())
                            {
                                client.m_PlayerInfo.m_Vehicle.Update(g.physics.m_collisionSpace.simulationSettings.timeStep);
                            }
                        }
                    }
                }
            }
            else
            {
                foreach (Player player in g.clientData.players)
                {
                    if (player.m_Vehicle != null)
                    {
                        player.UpdateDamageEmitter();

                        if (player.m_Name == g.playerInfo.m_Name)
                        {
                            if (player.IsAlive())
                            {
                                player.m_Vehicle.Update(g.physics.m_collisionSpace.simulationSettings.timeStep);
                            }
                        }
                    }
                }
            }

            g.playerInfo.UpdateDamageEmitter();

            g.camera.move();
            g.camera.apply_transformation();

            g.projectile_manager.Update();
            g.hud.Update();
            g.corpse_man.Update();


            #region Vehicle Select Hack
            //////////////////////////////////////////////////////////////////////////////
            //Switch Vehicle Hack - can be removed when were are done
            //////////////////////////////////////////////////////////////////////////////
            if (g.keyboard_state.IsKeyDown(Keys.F8))
            {
                g.SnapshotString = g.playerInfo.Bundle().ToString();
                
            }
            if (g.keyboard_state.IsKeyDown(Keys.F9))
            {
                g.playerInfo.ProcessUpdateFromServer(CompletePackage.CreateFromString(g.SnapshotString));
            }

            //////////////////////////////////////////////////////////////////////////////
            //End Hack
            //////////////////////////////////////////////////////////////////////////////
            #endregion
        }
        protected override void Draw(GameTime gameTime)
        {
            //clear the screen before any rendering
            g.graphics.GraphicsDevice.Clear(Color.CornflowerBlue);

            switch (g.current_state)
            {
                //at some state of the menu and let menu deal with it
                case (int)g.game_states.MENU_LOOP:
                case (int)g.game_states.LOBBY_LOOP:
                case (int)g.game_states.GAME_LOBBY_LOOP:

                    m_main_menu.render_menu();

                break;

                //in the game
                case (int)g.game_states.GAME_LOOP:

                    game_render_loop();

                break;
                case (int)g.game_states.STATS_LOOP:

                    g.hud.Draw();

                break;
            }

            base.Draw(gameTime);
        }

        // This is called when the game is over with the name of the player who won
        public void OnGameEnded(string aWinningPlayer, bool aDraw)
        {
            
            g.hud.WinString = aWinningPlayer;
            g.current_state = (int)g.game_states.STATS_LOOP;
            g.hud.m_state = HUD_state.stats;
            //m_main_menu.go_to_main();
        }

        // This is called when the network match begins
        public void OnGameStart(int levelToLoad, g.game_types aGameType )
        {
            // Set the game type
            m_GameType = aGameType;

            switch (m_GameType)
            {
                case g.game_types.TIMED_2:
                    {
                        g.game_timer = new CountDownTimer(1000 * 60 * 2);
                        g.game_kill_limit = -1;
                    }
                    break;

                case g.game_types.TIMED_5:
                    {
                        g.game_timer = new CountDownTimer(1000 * 60 * 5);
                        g.game_kill_limit = -1;
                    }
                    break;

                case g.game_types.TIMED_10:
                    {
                        g.game_timer = new CountDownTimer(1000 * 60 * 10);
                        g.game_kill_limit = -1;
                    }
                    break;

                case g.game_types.TIMED_25:
                    {
                        g.game_timer = new CountDownTimer(1000 * 60 * 25);
                        g.game_kill_limit = -1;
                    }
                    break;
                case g.game_types.DEATHMATCH_10:
                    {
                        g.game_kill_limit = 10;
                        g.game_timer = null;
                    }
                    break;

                case g.game_types.DEATHMATCH_20:
                    {
                        g.game_kill_limit = 20;
                        g.game_timer = null;
                    }
                    break;

                case g.game_types.DEATHMATCH_50:
                    {
                        g.game_kill_limit = 50;
                        g.game_timer = null;
                    }
                    break;

                case g.game_types.DEATHMATCH_100:
                    {
                        g.game_kill_limit = 100;
                        g.game_timer = null;
                    }
                    break;
            }

            //load the level from text file
            g.level = new Level();

            string LoadLevel = "Maps\\level_1";
            switch (levelToLoad)
            {
                case 1:
                    LoadLevel = "Maps\\level_1";
                    g.effect_sys.CreateEmitter(new Vector3(0.0f, 60.0f, 0.0f), -Vector3.UnitY, "effects/rain1.txt", g.effect_sys.GetTexture(EffectSystem.textures_e.TEX_RAIN_1));
                    break;
                case 2:
                    LoadLevel = "Maps\\level_2";
                    break;
                case 3:
                    LoadLevel="Maps\\level_3";
                    g.effect_sys.CreateEmitter(new Vector3(0.0f, 60.0f, 0.0f), -Vector3.UnitY, "effects/snow1.txt", g.effect_sys.GetTexture(EffectSystem.textures_e.TEX_SNOW_1));
                    break;
            }

            g.level.load(LoadLevel);

            // Create the physics core, this will manage all collision volumes and physics bodies
            g.physics = new Physics();

            g.level.addPropsToPhysicsSpace();

            //create a shadow map
            g.shadow_map = new Shadowmap();
            g.shadow_map.initialise();

            //new camera
            g.camera = new Camera();
            g.camera.initialise();

            // Load the player model
            g.playerInfo.LoadModel();

            //add vehicles
            g.hud.AddToMap(g.playerInfo.m_Vehicle);

            if (g.is_server)
            {
                foreach (gameClient client in g.server.m_Clients)
                {
                    if (client.m_PlayerInfo != null)
                    {
                        client.m_PlayerInfo.LoadModel();
                        if (client.m_PlayerInfo.m_Vehicle != null)
                        {
                            g.hud.AddToMap(client.m_PlayerInfo.m_Vehicle);
                        }
                    }
                }
            }
            else
            {
                foreach (Player player in g.clientData.players)
                {
                    player.LoadModel();
                    if (player.m_Vehicle != null)
                    {
                        g.hud.AddToMap(player.m_Vehicle);
                    }
                }
            }

            //add props
            g.level.addPropsToMiniMap();

            // sync the physics
            g.physics.Update(g.game_time);
        }

        void game_render_loop()
        {
            g.graphics.GraphicsDevice.RenderState.DepthBufferEnable = true;

            #region Shadowmap Pass
            //SHADOW MAP PASS----------------------------------------------

            g.shadow_map.shadowmap_begin();

            if (g.playerInfo.IsAlive())
            {
                g.playerInfo.m_Vehicle.shadow_map_render
                    (g.camera.m_shadow_projection, g.camera.m_shadow_view);
            }

            if (g.is_server)
            {
                foreach (gameClient client in g.server.m_Clients)
                {
                    if (client.m_PlayerInfo.m_Vehicle != null)
                    {
                        if (client.m_PlayerInfo.IsAlive())
                        {
                            client.m_PlayerInfo.m_Vehicle.shadow_map_render
                                (g.camera.m_shadow_projection, g.camera.m_shadow_view);
                        }
                    }
                }
            }
            else
            {
                foreach (Player player in g.clientData.players)
                {
                    if (player.m_Vehicle != null)
                    {
                        if (player.IsAlive())
                        {
                            player.m_Vehicle.shadow_map_render
                                (g.camera.m_shadow_projection, g.camera.m_shadow_view);
                        }
                    }
                }
            }


            g.level.shadow_map_render
             (g.camera.m_shadow_projection, g.camera.m_shadow_view);

            g.shadow_map.shadowmap_end();

            //END SHADOW PASS----------------------------------------------
            #endregion
            
            #region Mainrender Pass
            //MAIN RENDER PASS---------------------------------------------

            g.level.draw(g.camera.m_projection_matrix, g.camera.m_view_matrix,true);

            if (g.playerInfo.IsAlive())
            {
                g.playerInfo.m_Vehicle.draw(g.camera.m_projection_matrix, g.camera.m_view_matrix);
            }

            //g.projectile_mgr.Render();

            if (g.is_server)
            {
                foreach (gameClient client in g.server.m_Clients)
                {
                    if (client.m_PlayerInfo.m_Vehicle != null)
                    {
                        if (client.m_PlayerInfo.IsAlive())
                        {
                            client.m_PlayerInfo.m_Vehicle.draw(g.camera.m_projection_matrix, g.camera.m_view_matrix);
                        }
                    }
                }
            }
            else
            {
                foreach (Player player in g.clientData.players)
                {
                    if (player.m_Vehicle != null)
                    {
                        if (player.IsAlive())
                        {
                            player.m_Vehicle.draw(g.camera.m_projection_matrix, g.camera.m_view_matrix);
                        }
                    }
                }
            }

            g.effect_sys.Draw();

            g.projectile_manager.Render();

            g.corpse_man.Draw();
            //END MAIN PASS------------------------------------------------
            #endregion
            
            #region Aux Pass 2D / HUD / ORTHO
            //AUX PASS - 2D / HUD / ORTHO----------------------------------

            g.sprite_batch.Begin();

            g.chat_window.render_chat_window();

            #region HUD

            //draw hud
            g.hud.Draw();

            g.sprite_batch.End();
            #endregion

            //END AUX PASS-------------------------------------------------
            #endregion

            // DEBUG PASS
            //g.physics.Draw();


        }
    }
}

