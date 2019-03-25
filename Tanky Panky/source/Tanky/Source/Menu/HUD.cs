using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;
using BEPUphysics;

namespace Tanky
{
    public enum Orientation
    {
        Horiz = 0,
        Vert = 1
    }
    public enum HUD_state
    {
        game = 0,
        stats = 1
    }


    public class Announcement
    {
        public string Message;
        private float m_time;
        private CountDownTimer m_timer;

        public bool IsFinished
        {
            get
            {
                return m_timer.IsFinished;
            }
        }
        public Announcement(string theMessage,float thetime)
        {
            Message = theMessage;
            m_time = thetime;
            m_timer = new CountDownTimer(m_time);
        }
        
    }
    public class HUDIcon
    {

    }
    //Generics Suck Balls
    //so i'm using derivatives
    public class HUDIconInt: HUDIcon
    {
        
        Texture2D m_icon;
        public int Value;
        public Vector2 Position;
        private float m_scale;
        private Vector2 m_spacing;
        private Orientation m_orientation;
        private SpriteFont m_font;

        public HUDIconInt(Texture2D theTexture,int theValue,Vector2 thePosition,float theScale,Orientation theOri,SpriteFont theFont)
        {
            m_icon = theTexture;
            Value = theValue;
            Position = thePosition;
            m_scale = theScale;
            m_spacing = new Vector2(10, 2);
            m_orientation = theOri;
            m_font = theFont;
        }
        public void Draw()
        {
            Vector2 dim = new Vector2(m_icon.Width*m_scale,m_icon.Height* m_scale);
            string stringValue = Value.ToString();

            if(m_orientation == Orientation.Horiz)
            {
                g.sprite_batch.Draw(m_icon, new Rectangle((int)Position.X, (int)Position.Y, (int)dim.X, (int)dim.Y), Color.White);
                g.sprite_batch.DrawString(m_font, 
                                          stringValue,
                                          new Vector2(Position.X + dim.X + m_spacing.X, Position.Y),
                                          Color.White);
            }
            else
            {
                Vector2 stringPos = new Vector2(Position.X + (dim.X / 2) - m_font.MeasureString(stringValue).X/2,
                                                Position.Y + ((dim.Y/100)* 60));
                
                g.sprite_batch.Draw(m_icon, new Rectangle((int)Position.X, (int)Position.Y, (int)dim.X, (int)dim.Y), Color.White);
                g.sprite_batch.DrawString(m_font,
                                          stringValue,
                                          stringPos,
                                          Color.White);
            }
            
        }
        
    }
    public class HUDIconString : HUDIcon
    {

        Texture2D m_icon;
        public string Value;
        public Vector2 Position;
        private float m_scale;
        private Vector2 m_spacing;
        private Orientation m_orientation;
        private SpriteFont m_font;

        public HUDIconString(Texture2D theTexture, string theValue, Vector2 thePosition, float theScale, Orientation theOri, SpriteFont theFont)
        {
            m_icon = theTexture;
            Value = theValue;
            Position = thePosition;
            m_scale = theScale;
            m_spacing = new Vector2(10, 5);
            m_orientation = theOri;
            m_font = theFont;
        }
        public void Draw()
        {
            Vector2 dim = new Vector2(m_icon.Width * m_scale, m_icon.Height * m_scale);
            string stringValue = Value.ToString();

            if (m_orientation == Orientation.Horiz)
            {
                g.sprite_batch.Draw(m_icon, new Rectangle((int)Position.X, (int)Position.Y, (int)dim.X, (int)dim.Y), Color.White);
                g.sprite_batch.DrawString(m_font,
                                          stringValue,
                                          new Vector2(Position.X + dim.X + m_spacing.X, Position.Y),
                                          Color.White);
            }
            else
            {
                Vector2 stringPos = new Vector2(Position.X + (dim.X / 2) - m_font.MeasureString(stringValue).X,
                                                Position.Y + m_font.MeasureString(stringValue).Y + m_spacing.Y);
                g.sprite_batch.DrawString(m_font,
                                          stringValue,
                                          stringPos,
                                          Color.White);
                g.sprite_batch.Draw(m_icon, new Rectangle((int)Position.X, (int)Position.Y, (int)dim.X, (int)dim.Y), Color.White);
            }

        }

    }
    public class HUD
    {
        private bool m_display_scoreboard;

        private float m_input_delay;
        private Timer m_input_timer;

        private bool m_display_map;
        private SmallMap m_small_map;

        private bool m_display_names;
        private bool m_display_name_scores;

        private bool m_display_HUD;

        private bool m_display_ping;

        private bool m_display_timer;

        private bool m_display_announce;

        private Texture2D m_background;

        private SpriteFont m_font;

        private Dictionary<string, Texture2D> m_texture_list;
        private TextOutput m_text;

        private Dictionary<string, HUDIconInt> m_icon_list;

        public HUD_state m_state = HUD_state.game;

        private Vector2 m_scoreboard_dimensions;

        private List<Announcement> m_announcements;

        public string WinString = string.Empty;

        public HUD()
        {
            m_texture_list = new Dictionary<string, Texture2D>();
            m_icon_list = new Dictionary<string, HUDIconInt>();

            m_input_timer = new Timer();
            m_input_delay = 200;

            //scoreboard
            m_display_scoreboard = false;
            m_scoreboard_dimensions = new Vector2(600, 400);
            //Map
            m_display_map = true;
            m_small_map = new SmallMap();
            //names
            m_display_names = true;
            m_display_name_scores = false;

            m_display_HUD = true;

            m_display_ping = true;

            m_display_timer = true;

            m_display_announce = true;

            //text rendering
            m_text = new TextOutput();
            m_font = g.content.Load<SpriteFont>("Fonts\\FontArmy_12pt");
            m_text.ChangeFont(m_font);

            m_announcements = new List<Announcement>();

            m_background = g.content.Load<Texture2D>("Gui\\Backgrounds\\ConceptBackground_11");

            LoadTextures();
            LoadHudIcons();
        }

        private void LoadTextures()
        {
            m_texture_list.Add("ScoreboardBackground", g.content.Load<Texture2D>("Gui\\Backgrounds\\InGame Scoreboard"));
        }

        private void RenderScoreBoard()
        {
            float T1_Kills = 0;
            float T1_Deaths = 0;
            float T1_Score = 0;

            float T2_Kills = 0;
            float T2_Deaths = 0;
            float T2_Score = 0;

            float spacing = 20;



            //background
            g.sprite_batch.Draw(m_texture_list["ScoreboardBackground"], new Rectangle((int)(g.WindowCentre.X - (m_scoreboard_dimensions.X / 2)),
                                                                                      (int)(g.WindowCentre.Y - (m_scoreboard_dimensions.Y/2)),
                                                                                      (int)m_scoreboard_dimensions.X,
                                                                                      (int)m_scoreboard_dimensions.Y),
                                                                                      Color.White);

            
            //player
            Color playerColour = Color.LightBlue;
            Vector2 position = new Vector2(g.WindowCentre.X - (m_scoreboard_dimensions.X/2) + 45 ,g.WindowCentre.Y - (m_scoreboard_dimensions.Y/2) + 50);

            if (g.playerInfo != null)
            {
                if (g.playerInfo.m_Name.Length > 12)
                {
                    m_text.output_string(g.playerInfo.m_Name.Substring(0, 12), position, playerColour, 0.6f, false);
                }
                else
                {
                    m_text.output_string(g.playerInfo.m_Name, position, playerColour, 0.6f, false);
                }
                
                m_text.output_string(g.playerInfo.m_team.ToString(), new Vector2(position.X + 127, position.Y), playerColour, 0.6f, false);
                m_text.output_string(g.playerInfo.m_kills.ToString(), new Vector2(position.X + 215, position.Y), playerColour, 0.6f, false);
                m_text.output_string(g.playerInfo.m_deaths.ToString(), new Vector2(position.X + 308, position.Y), playerColour, 0.6f, false);
                m_text.output_string(g.playerInfo.m_score.ToString(), new Vector2(position.X + 401, position.Y), playerColour, 0.6f, false);
                m_text.output_string(g.playerInfo.m_Ping.ToString(), new Vector2(position.X + 480, position.Y), playerColour, 0.6f, false);

                if(g.playerInfo.m_team == 1 )
                {
                    T1_Deaths += g.playerInfo.m_deaths;
                    T1_Kills += g.playerInfo.m_kills;
                    T1_Score += g.playerInfo.m_score;
                }
                else
                {
                    T2_Deaths += g.playerInfo.m_deaths;
                    T2_Kills += g.playerInfo.m_kills;
                    T2_Score += g.playerInfo.m_score;
                }

            }

            position.Y += spacing;
            //other players
            playerColour = Color.Maroon;
            if (g.is_server == true)
            {

                foreach (gameClient player in g.server.m_Clients)
                {
                    if (player.m_PlayerInfo != null)
                    {
                        if (player.m_PlayerInfo.m_Name.Length > 12)
                        {
                            m_text.output_string(player.m_PlayerInfo.m_Name.Substring(0, 12), position, playerColour, 0.6f, false);
                        }
                        else
                        {
                            m_text.output_string(player.m_PlayerInfo.m_Name, position, playerColour, 0.6f, false);
                        }
                        m_text.output_string(player.m_PlayerInfo.m_team.ToString(), new Vector2(position.X + 127, position.Y), playerColour, 0.6f, false);
                        m_text.output_string(player.m_PlayerInfo.m_kills.ToString(), new Vector2(position.X + 215, position.Y), playerColour, 0.6f, false);
                        m_text.output_string(player.m_PlayerInfo.m_deaths.ToString(), new Vector2(position.X + 308, position.Y), playerColour, 0.6f, false);
                        m_text.output_string(player.m_PlayerInfo.m_score.ToString(), new Vector2(position.X + 401, position.Y), playerColour, 0.6f, false);
                        m_text.output_string(player.m_PlayerInfo.m_Ping.ToString(), new Vector2(position.X + 480, position.Y), playerColour, 0.6f, false);

                        if (g.playerInfo.m_team == 1)
                        {
                            T1_Deaths += player.m_PlayerInfo.m_deaths;
                            T1_Kills += player.m_PlayerInfo.m_kills;
                            T1_Score += player.m_PlayerInfo.m_score;
                        }
                        else
                        {
                            T2_Deaths += player.m_PlayerInfo.m_deaths;
                            T2_Kills += player.m_PlayerInfo.m_kills;
                            T2_Score += player.m_PlayerInfo.m_score;
                        }

                        position.Y += spacing;
                    }
                }
            }
            else
            {
                foreach (Player player in g.clientData.players)
                {
                    if (player.m_Name.Length > 12)
                    {
                        m_text.output_string(player.m_Name.Substring(0, 12), position, playerColour, 0.6f, false);
                    }
                    else
                    {
                        m_text.output_string(player.m_Name, position, playerColour, 0.6f, false);
                    }
                    m_text.output_string(player.m_team.ToString(), new Vector2(position.X + 127, position.Y), playerColour, 0.6f, false);
                    m_text.output_string(player.m_kills.ToString(), new Vector2(position.X + 215, position.Y), playerColour, 0.6f, false);
                    m_text.output_string(player.m_deaths.ToString(), new Vector2(position.X + 308, position.Y), playerColour, 0.6f, false);
                    m_text.output_string(player.m_score.ToString(), new Vector2(position.X + 401, position.Y), playerColour, 0.6f, false);
                    m_text.output_string(player.m_Ping.ToString(), new Vector2(position.X + 480, position.Y), playerColour, 0.6f, false);

                    if (player.m_team == 1)
                    {
                        T1_Deaths += player.m_deaths;
                        T1_Kills += player.m_kills;
                        T1_Score += player.m_score;
                    }
                    else
                    {
                        T2_Deaths += player.m_deaths;
                        T2_Kills += player.m_kills;
                        T2_Score += player.m_score;
                    }

                    position.Y += spacing;
                }
            }

            position = new Vector2(g.WindowCentre.X - (m_scoreboard_dimensions.X / 2) + 45, g.WindowCentre.Y - (m_scoreboard_dimensions.Y / 2) + 250);

            //Team 1;
            m_text.output_string("1", new Vector2(position.X + 40, position.Y), playerColour, 0.75f, false);
            m_text.output_string(T1_Kills.ToString(), new Vector2(position.X + 148, position.Y), playerColour, 0.6f, false);
            m_text.output_string(T1_Deaths.ToString(), new Vector2(position.X + 282, position.Y), playerColour, 0.6f, false);
            m_text.output_string(T1_Score.ToString(), new Vector2(position.X + 435, position.Y), playerColour, 0.6f, false);

            position.Y += spacing;

            //Team 2;
            m_text.output_string("2", new Vector2(position.X + 39, position.Y), playerColour, 0.75f, false);
            m_text.output_string(T2_Kills.ToString(), new Vector2(position.X + 148, position.Y), playerColour, 0.6f, false);
            m_text.output_string(T2_Deaths.ToString(), new Vector2(position.X + 282, position.Y), playerColour, 0.6f, false);
            m_text.output_string(T2_Score.ToString(), new Vector2(position.X + 435, position.Y), playerColour, 0.6f, false);
        }
        private void RenderNames()
        {
            if (g.playerInfo.IsAlive())
            {
                g.playerInfo.display_name(m_display_name_scores);
            }

            if (g.is_server)
            {
                foreach (gameClient client in g.server.m_Clients)
                {
                    if (client.m_PlayerInfo.m_Vehicle != null)
                    {
                        if (client.m_PlayerInfo.IsAlive())
                        {
                            client.m_PlayerInfo.display_name(m_display_name_scores);
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
                            player.display_name(m_display_name_scores);
                        }
                    }
                }
            }
        }
        private void RenderHUD()
        {
            foreach (KeyValuePair<string,HUDIconInt> h in m_icon_list)
            {
                h.Value.Draw();
            }
        }
        private void UpdateHUD()
        {
            m_icon_list["Health"].Value = g.playerInfo.m_health;
            //m_icon_list["Bullets"].Value = g.playerInfo.m_health;
            m_icon_list["Kills"].Value = g.playerInfo.m_kills;
            m_icon_list["Score"].Value = g.playerInfo.m_score;
            m_icon_list["Deaths"].Value = g.playerInfo.m_deaths;
            //m_icon_list["Team"].Value = g.playerInfo.m_team;
            m_icon_list["Value"].Value = g.playerInfo.m_value;
        }
        private void RenderPingList()
        {
            Vector2 start_pos = new Vector2(10, 10);

            float spacing_y = m_font.MeasureString("a").Y + 2;
            float spacing_x = 200;

            Color ping_colour = Color.Green;

            float scale =0.6f;


            //loacl player 
            float ping = g.playerInfo.m_Ping;
            //green if less than 200
            //orange if between 200 and 600
            //red if above 600
            ping_colour = ping < 200 ? Color.Green : 200 < ping && ping < 600 ? Color.Orange : Color.Red;

            g.sprite_batch.DrawString(m_font,
                                      g.playerInfo.m_Name.ToString(),
                                      start_pos,
                                      Color.White,
                                      0,
                                      Vector2.Zero,
                                      scale,
                                      SpriteEffects.None,
                                      0);
            g.sprite_batch.DrawString(m_font,
                                      ping.ToString(),
                                      new Vector2(start_pos.X + spacing_x,start_pos.Y),
                                      Color.White,
                                      0,
                                      Vector2.Zero,
                                      scale,
                                      SpriteEffects.None,
                                      0);
            start_pos.Y += spacing_y; 

            if (g.is_server == true)
            {

                foreach (gameClient player in g.server.m_Clients)
                {
                    ping = player.m_PlayerInfo.m_Ping;
                    ping_colour = ping < 200 ? Color.Green : 200 < ping && ping < 600 ? Color.Orange : Color.Red;

                    g.sprite_batch.DrawString(m_font,
                                              player.m_PlayerInfo.m_Name.ToString(),
                                              start_pos,
                                              Color.White,
                                              0,
                                              Vector2.Zero,
                                              scale,
                                              SpriteEffects.None,
                                              0);
                    g.sprite_batch.DrawString(m_font,
                                              ping.ToString(),
                                              new Vector2(start_pos.X + spacing_x, start_pos.Y),
                                              Color.White,
                                              0,
                                              Vector2.Zero,
                                              scale,
                                              SpriteEffects.None,
                                              0);
                    start_pos.Y += spacing_y; 
                }
            }
            else
            {
                foreach (Player player in g.clientData.players)
                {
                    ping = player.m_Ping;
                    ping_colour = ping < 200 ? Color.Green : 200 < ping && ping < 600 ? Color.Orange : Color.Red;

                    g.sprite_batch.DrawString(m_font,
                                              player.m_Name.ToString(),
                                              start_pos,
                                              Color.White,
                                              0,
                                              Vector2.Zero,
                                              scale,
                                              SpriteEffects.None,
                                              0);
                    g.sprite_batch.DrawString(m_font,
                                              ping.ToString(),
                                              new Vector2(start_pos.X + spacing_x, start_pos.Y),
                                              Color.White,
                                              0,
                                              Vector2.Zero,
                                              scale,
                                              SpriteEffects.None,
                                              0);
                    start_pos.Y += spacing_y; 
                }
            }
        }
        
        private void RenderAnnoucements()
        {
            float spacing_y = 20;
            Vector2 Pos = new Vector2(0,100);
            foreach (Announcement a in m_announcements)
            {
                if (!a.IsFinished)
                {
                    Pos = new Vector2(g.WindowCentre.X - (m_font.MeasureString(a.Message).X / 2), Pos.Y);
                    g.sprite_batch.DrawString(m_font, a.Message, Pos, Color.White);

                    Pos.Y += spacing_y;
                }
            }
        }
        private void UpdateAnnoucements()
        {
            for(int i = 0;i < m_announcements.Count;i++)
            {
                if (m_announcements[i].IsFinished)
                {
                    m_announcements.RemoveAt(i);
                }
            }
        }
        public void AddAnnouncement(string theMessage)
        {
            float time = 5000;
            m_announcements.Add(new Announcement(theMessage,time));
        }

        private void RenderTime()
        {
            if (g.game_timer != null)
            {
                string Time = g.game_timer.ToString();
                Vector2 pos = new Vector2(g.WindowCentre.X - (m_font.MeasureString(Time).X / 2), 20);

                g.sprite_batch.DrawString(m_font, Time, pos, Color.White);
            }
        }
        public void AddToMap(Prop theProp)
        {
            m_small_map.AddProp(theProp);
        }
        public void AddToMap(VehiclePhysics theVehicle)
        {
            m_small_map.AddVehicle(theVehicle);
        }

        public void Draw()
        {
            if (m_state == HUD_state.game)
            {
                if (m_display_scoreboard) { RenderScoreBoard(); }
                if (m_display_map) { m_small_map.Draw(); }
                if (m_display_names) { RenderNames(); }
                if (m_display_HUD) { RenderHUD(); }
                if (m_display_ping) { RenderPingList(); }
                if (m_display_timer) { RenderTime(); }
                if (m_display_announce) { RenderAnnoucements(); }
            }
            else
            {
                g.sprite_batch.Begin();
                g.sprite_batch.Draw(m_background, new Rectangle(0, 0, g.graphics.GraphicsDevice.Viewport.Width, g.graphics.GraphicsDevice.Viewport.Height), Color.White);
                RenderScoreBoard();
                //RenderTime();
                if(WinString != string.Empty)
                {
                    Vector2 length = m_font.MeasureString(WinString);

                    m_text.output_string(WinString, new Vector2(g.WindowCentre.X - (length.X / 2),
                                                                          50),
                                                                          Color.White,
                                                                          0.6f,
                                                                          false);
                }
                
                g.sprite_batch.End();
            }
        }
        public void Update()
        {
            if (m_state == HUD_state.game)
            {
                //internal updates

                m_small_map.Update();

                UpdateHUD();
                if (!g.chat_window.talk)
                {
                    #region control variables

                    //scoreboard - Hold Tab
                    bool i_scoreboard = g.keyboard_state.IsKeyDown(Keys.Tab);

                    //Map - Toggle M
                    bool i_map = g.keyboard_state.IsKeyDown(Keys.M);

                    //Names - Toggle N
                    bool i_names = g.keyboard_state.IsKeyDown(Keys.N);

                    //Name Scores - Toggle B
                    bool i_name_scores = g.keyboard_state.IsKeyDown(Keys.B);

                    //Vitals - Toggle V
                    bool i_HUD = g.keyboard_state.IsKeyDown(Keys.V);

                    //timer
                    bool i_Timer = g.keyboard_state.IsKeyDown(Keys.X);

                    bool i_announce = g.keyboard_state.IsKeyDown(Keys.P);
                    #endregion

                    #region control switches
                    //hud controls
                    m_display_scoreboard = i_scoreboard ? true : false;

                    m_input_timer.Update();
                    if ((m_input_timer.Elapsed > m_input_delay))
                    {
                        if (i_map)
                        {
                            m_display_map = !m_display_map;
                        }

                        if (i_names)
                        {
                            m_display_names = !m_display_names;
                        }

                        if (i_name_scores)
                        {
                            m_display_name_scores = !m_display_name_scores;
                        }

                        if (i_HUD)
                        {
                            m_display_HUD = !m_display_HUD;
                        }

                        if (i_Timer)
                        {
                            m_display_timer = !m_display_timer;
                        }

                        if (i_announce)
                        {
                            m_display_announce = !m_display_announce;
                        }


                        m_input_timer.Reset();
                    }

                    #endregion
                }
            }
            else
            {
                if(g.keyboard_state.IsKeyDown(Keys.Escape))
                {
                    m_state = HUD_state.game;
                    g.current_state = (int)g.game_states.MENU_LOOP;
                        
                }
            }


        }

        public void LoadHudIcons()
        {
            float distanceFromBottom = 20;
            float numberOfIcons = 5;
            float scale = 0.3f;
            float spacing_x = 5;
            Vector2 icon_dimensions = new Vector2(200, 200);
            icon_dimensions.X *= scale;
            icon_dimensions.Y *= scale;

            Vector2 pos;

            bool centreAlign = false;

            if(centreAlign)
            {
                pos = new Vector2(g.WindowCentre.X - ((icon_dimensions.X * numberOfIcons) / 2) + spacing_x,
                                        g.graphics.PreferredBackBufferHeight - icon_dimensions.Y - distanceFromBottom);
            }
            else
            {
                pos = new Vector2(g.graphics.PreferredBackBufferWidth - ((icon_dimensions.X * numberOfIcons)) - 50,
                                        g.graphics.PreferredBackBufferHeight - icon_dimensions.Y - distanceFromBottom);
            }

            


            //health
            HUDIconInt tempInt = new HUDIconInt(g.content.Load<Texture2D>("Gui\\HUD\\HUD Icons-01"),
                                             g.playerInfo.m_health,
                                             pos,
                                             scale,
                                             Orientation.Vert,
                                             m_font);

            m_icon_list.Add("Health", (tempInt));

            ////bullets(not used atm)
            //tempInt = new HUDIconInt(g.content.Load<Texture2D>("Gui\\HUD\\HUD Icons-02"),
            //                                 g.playerInfo.m_health,
            //                                 Vector2.Zero,
            //                                 1,
            //                                 Orientation.Vert,
            //                                 m_font);


            //m_icon_list.Add("Bullets", (tempInt));

            pos.X += icon_dimensions.X + spacing_x;

            //Score
            tempInt = new HUDIconInt(g.content.Load<Texture2D>("Gui\\HUD\\HUD Icons-07"),
                                             g.playerInfo.m_health,
                                             pos,
                                             scale,
                                             Orientation.Vert,
                                             m_font);
            m_icon_list.Add("Score", (tempInt));

            pos.X += icon_dimensions.X + spacing_x;

            //Kills
            tempInt = new HUDIconInt(g.content.Load<Texture2D>("Gui\\HUD\\HUD Icons-06"),
                                             g.playerInfo.m_health,
                                             pos,
                                             scale,
                                             Orientation.Vert,
                                             m_font);

            m_icon_list.Add("Kills", (tempInt));

            pos.X += icon_dimensions.X + spacing_x;

            //Deaths
            tempInt = new HUDIconInt(g.content.Load<Texture2D>("Gui\\HUD\\HUD Icons-04"),
                                             g.playerInfo.m_health,
                                             pos,
                                             scale,
                                             Orientation.Vert,
                                             m_font);

            m_icon_list.Add("Deaths", (tempInt));

            pos.X += icon_dimensions.X + spacing_x;

            //value
            tempInt = new HUDIconInt(g.content.Load<Texture2D>("Gui\\HUD\\HUD Icons-03"),
                                             g.playerInfo.m_health,
                                             pos,
                                             scale,
                                             Orientation.Vert,
                                             m_font);

            m_icon_list.Add("Value", (tempInt));

            

            
        }

      
    }
}
