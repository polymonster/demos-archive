using System;
using System.IO;
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

namespace Tanky
{
    public enum widget_type
    {
        BUTTON = 0,
        ACTIVE_CONSTANT,
        INPUT,
        GRAPHIC
    }

    public enum widget_states
    {
        NONE = 0,
        HOVER,
        ACTIVE,
        PRESSED,
    }

    class Widget
    {
        public String m_identifier;

        public int m_state;

        Rectangle m_button_rect;
        
        public ButtonTextureStruct Textures{get; private set;}

        TextOutput m_text_outputter;

        TypeReader m_type_reader;

        public String m_contents;

        public widget_type m_type;

        public Widget() { }

        public Widget(Vector2 position, Vector2 size, String identifier, ButtonTextureStruct textures,widget_type wType)
        {
            m_button_rect.Height = (int)size.Y;
            m_button_rect.Width = (int)size.X;

            m_button_rect.X = (int)position.X;
            m_button_rect.Y = (int)position.Y;

            m_identifier = identifier;
            Textures = textures;

            m_text_outputter = new TextOutput();

            m_type = wType;

            if (wType == widget_type.INPUT)
            {
                m_contents = String.Empty;
                m_type_reader = new TypeReader();
            }
        }
        public Widget(Vector2 position, Vector2 size, String identifier, ButtonTextureStruct textures, widget_type wType,string initialText)
        {
            m_button_rect.Height = (int)size.Y;
            m_button_rect.Width = (int)size.X;

            m_button_rect.X = (int)position.X;
            m_button_rect.Y = (int)position.Y;

            m_identifier = identifier;
            Textures = textures;

            m_text_outputter = new TextOutput();

            m_type = wType;

            if (wType == widget_type.INPUT)
            {
                m_contents = initialText;
                m_type_reader = new TypeReader();
            }
        }

        public String check()
        {
            //check mouse over
            Vector2 mouse_pos;
            mouse_pos.X = Mouse.GetState().X;
            mouse_pos.Y = Mouse.GetState().Y;

            Vector2 min_bound;
            Vector2 max_bound;

            min_bound.X = m_button_rect.Left;
            min_bound.Y = m_button_rect.Top;

            max_bound.X = m_button_rect.Right;
            max_bound.Y = m_button_rect.Bottom;

            if (m_state != (int) widget_states.ACTIVE)
            {
                m_state = (int)widget_states.NONE;
            }

            if (m_type != widget_type.GRAPHIC)
            {
                if (mouse_pos.X > min_bound.X && mouse_pos.X < max_bound.X
                    && mouse_pos.Y > min_bound.Y && mouse_pos.Y < max_bound.Y)
                {
                    if (m_type == widget_type.INPUT && m_state != (int)widget_states.ACTIVE)
                    {
                        m_state = (int)widget_states.HOVER;
                    }
                    else if(m_type != widget_type.INPUT)
                    {
                        m_state = (int)widget_states.HOVER;
                    }

                    // Check if the mouse is held
                    if (Mouse.GetState().LeftButton == ButtonState.Pressed && !g.mouse_held)
                    {
                        m_state = (int)widget_states.PRESSED;

                        if(m_type == widget_type.INPUT)
                        {
                            if(m_state != (int) widget_states.ACTIVE)
                            {
                                m_type_reader.reset();
                            }

                            m_state = (int) widget_states.ACTIVE;
                        }

                        g.mouse_held = true;

                        return m_identifier;
                    }
                }

                if (m_type == widget_type.INPUT && m_state == (int)widget_states.ACTIVE)
                {
                    m_type_reader.keyboard_input(ref m_contents);
                }
            }

            return "NO_ACTION";
        }

        public void render()
        {
            Color color = Color.White;

            switch (m_state)
            {
                case (int)widget_states.NONE:

                    g.sprite_batch.Draw(Textures.Up, m_button_rect, color);

                    break;

                case (int)widget_states.HOVER:

                    g.sprite_batch.Draw(Textures.Over, m_button_rect, color);

                    break;

                case (int)widget_states.ACTIVE:

                    g.sprite_batch.Draw(Textures.Down, m_button_rect, color);

                    break;

                case (int)widget_states.PRESSED:

                    g.sprite_batch.Draw(Textures.Down, m_button_rect, color);

                    break;

            }

            if (m_type == widget_type.INPUT)
            {
                String display = m_contents;

                if (m_state == (int) widget_states.ACTIVE) display = m_contents + "*";

                m_text_outputter.output_string(display, new Vector2(m_button_rect.Left + 5, m_button_rect.Top + 2), Color.White, 1, false);
            }


        
        }

    }



    class MouseCursor
    {
        Texture2D m_cursor;
        Rectangle m_cursor_rect;

        public MouseCursor()
        {
            m_cursor = g.content.Load<Texture2D>("UI\\mouse");

            m_cursor_rect.Width = 32;
            m_cursor_rect.Height = 32;

            m_cursor_rect.X = Mouse.GetState().X;
            m_cursor_rect.Y = Mouse.GetState().Y;
        }

        public void update()
        {
            m_cursor_rect.X = Mouse.GetState().X;
            m_cursor_rect.Y = Mouse.GetState().Y;

            if (Mouse.GetState().LeftButton == ButtonState.Released)
            {
                g.mouse_held = false;
            }
        }

        public void render()
        {
            g.sprite_batch.Draw(m_cursor, m_cursor_rect, Color.White);
        }


    }


    class Menu
    {
        List<Widget> m_widget_list;

        MouseCursor m_mouse_cursor;

        TextOutput m_text_outputter;

        Texture2D m_background;

        String m_error_string = String.Empty;

        //button Texture Hashtable
        Dictionary<string,ButtonTextureStruct> m_button_textures;
        //backgrounds list
        Dictionary<string, Texture2D> m_background_textures;

        //server list
        Dictionary<string, string> m_servers;

        Vector2 m_big_button_dimensions;
        Vector2 m_small_button_dimensions;
        Vector2 m_chat_box_dimensions;
        Vector2 m_nav_dimensions;

        int m_selected_vehicle = 0;
        int m_selected_team = 1;
        int m_selected_level = 1;
        int m_selected_server;

        g.game_types m_selected_gametype = g.game_types.DEATHMATCH_10;

        bool error_to_main = false;

        private UDPBroadCastCheck m_BroadCastCheck;

        public Menu()
        {
            m_widget_list = new List<Widget>();

            m_big_button_dimensions = new Vector2(200, 100);
            m_small_button_dimensions = new Vector2(125, 25);
            m_chat_box_dimensions = new Vector2(500, 35);
            m_nav_dimensions = new Vector2(20, 20);

            m_button_textures = new Dictionary<string, ButtonTextureStruct>();
            load_button_textures();
            m_background_textures = new Dictionary<string, Texture2D>();
            m_background_textures.Add("Background1",g.content.Load<Texture2D>("Gui\\Backgrounds\\ConceptBackground_1"));
            m_background_textures.Add("Background2", g.content.Load<Texture2D>("Gui\\Backgrounds\\ConceptBackground_2"));
            m_background_textures.Add("Background3", g.content.Load<Texture2D>("Gui\\Backgrounds\\ConceptBackground_3"));
            m_background_textures.Add("Background4", g.content.Load<Texture2D>("Gui\\Backgrounds\\ConceptBackground_4"));
            m_background_textures.Add("Background5", g.content.Load<Texture2D>("Gui\\Backgrounds\\ConceptBackground_5"));
            m_background_textures.Add("Ese", g.content.Load<Texture2D>("Gui\\Backgrounds\\ESE Edition"));

            m_background_textures.Add("Background6", g.content.Load<Texture2D>("Gui\\Backgrounds\\ConceptBackground_6"));
            m_background_textures.Add("Background7", g.content.Load<Texture2D>("Gui\\Backgrounds\\ConceptBackground_7"));
            m_background_textures.Add("Background8", g.content.Load<Texture2D>("Gui\\Backgrounds\\ConceptBackground_8"));
            m_background_textures.Add("Background9", g.content.Load<Texture2D>("Gui\\Backgrounds\\ConceptBackground_9"));

            m_background_textures.Add("Background10", g.content.Load<Texture2D>("Gui\\Backgrounds\\ConceptBackground_10"));
            m_background_textures.Add("Background11", g.content.Load<Texture2D>("Gui\\Backgrounds\\ConceptBackground_11"));
            m_background_textures.Add("Background12", g.content.Load<Texture2D>("Gui\\Backgrounds\\ConceptBackground_12"));
            m_background_textures.Add("Background13", g.content.Load<Texture2D>("Gui\\Backgrounds\\ConceptBackground_13"));
            m_background_textures.Add("Ese2", g.content.Load<Texture2D>("Gui\\Backgrounds\\Ese2"));

           


            m_mouse_cursor = new MouseCursor();

            m_text_outputter = new TextOutput();

            m_background = m_background_textures["Ese2"];

            g.mouse_held = false;

            go_to_main();

            
        }

        public void render_menu()
        {
            g.sprite_batch.Begin();
            g.sprite_batch.Draw(m_background,new Rectangle(0,0,g.graphics.GraphicsDevice.Viewport.Width,g.graphics.GraphicsDevice.Viewport.Height),Color.White);

            foreach (Widget widget in m_widget_list)
            {
                widget.render();
            }

            switch(g.current_state)
            {
                case (int)g.game_states.LOBBY_LOOP:

                    foreach(Widget w in m_widget_list)
                    {
                        if (m_servers != null && m_servers.Count > 0)
                        {
                            if (w.m_identifier == "IP ADDRESS")
                            {
                                string server = null;
                                int i = 0;
                                foreach (KeyValuePair<string, string> s in m_servers)
                                {
                                    if (i == m_selected_server)
                                    {
                                        server = s.Key;
                                    }
                                }
                                if (server != null)
                                {
                                    w.m_contents = m_servers[server];
                                }
                            }
                        }
                    }
                break;

                case (int)g.game_states.GAME_LOBBY_LOOP:

                    Vector2 dim = new Vector2(125, 125);
                    Vector2 pos = new Vector2(g.graphics.PreferredBackBufferWidth - dim.X - 70 , 120);
                    

                    //////////////////////////////////////////////////////////////////////////
                    //Vehicle Select Picture
                    //////////////////////////////////////////////////////////////////////////
                    if (m_selected_vehicle == 1)
                    {
                        g.sprite_batch.Draw(m_button_textures["Jeep Image"].Up, new Rectangle((int)pos.X, (int)pos.Y, (int)dim.X, (int)dim.Y), Color.White);
                    } 
                    else
                    {
                        g.sprite_batch.Draw(m_button_textures["Tank Image"].Up, new Rectangle((int)pos.X, (int)pos.Y, (int)dim.X, (int)dim.Y), Color.White);
                    }

                    //////////////////////////////////////////////////////////////////////////
                    //Level Select Picture
                    //////////////////////////////////////////////////////////////////////////
                    dim = new Vector2(256, 128);
                    pos = new Vector2(g.graphics.PreferredBackBufferWidth - dim.X - 30, 280);
                    if (m_selected_level == 1)
                    {
                        g.sprite_batch.Draw(m_button_textures["Forest Thumb"].Up, new Rectangle((int)pos.X, (int)pos.Y, (int)dim.X, (int)dim.Y), Color.White);
                    }
                    else if (m_selected_level == 2)
                    {
                        g.sprite_batch.Draw(m_button_textures["Desert Thumb"].Up, new Rectangle((int)pos.X, (int)pos.Y, (int)dim.X, (int)dim.Y), Color.White);
                    }
                    else
                    {
                        g.sprite_batch.Draw(m_button_textures["Snow Thumb"].Up, new Rectangle((int)pos.X, (int)pos.Y, (int)dim.X, (int)dim.Y), Color.White);
                    }

                    m_text_outputter.output_string(m_selected_team.ToString(), new Vector2(g.graphics.PreferredBackBufferWidth - m_nav_dimensions.X - m_nav_dimensions.X - 50, 250), Color.Black, 1, false);

                    if(g.is_server)
                    {
                        SpriteFont font = g.content.Load<SpriteFont>("Fonts\\FontArmy_16pt");
                        m_text_outputter.ChangeFont(font);
                        string game_type = "";
                        #region get gamestring
                        switch (m_selected_gametype)
                        {
                            case g.game_types.DEATHMATCH_10:
                                {
                                    game_type = "Deathmatch - 10 Kills";
                                    break;
                                }
                            case g.game_types.DEATHMATCH_20:
                                {
                                    game_type = "Deathmatch - 20 Kills";
                                    break;
                                }
                            case g.game_types.DEATHMATCH_50:
                                {
                                    game_type = "Deathmatch - 50 Kills";
                                    break;
                                }
                            case g.game_types.DEATHMATCH_100:
                                {
                                    game_type = "Deathmatch - 100 Kills";
                                    break;
                                }
                            case g.game_types.TIMED_2:
                                {
                                    game_type = "Timed - 2 Minutes";
                                    break;
                                }
                            case g.game_types.TIMED_5:
                                {
                                    game_type = "Timed - 5 Minutes";
                                    break;
                                }
                            case g.game_types.TIMED_10:
                                {
                                    game_type = "Timed - 10 Minutes";
                                    break;
                                }
                            case g.game_types.TIMED_25:
                                {
                                    game_type = "Timed - 25 Minutes";
                                    break;
                                }
                        }
                        #endregion

                        pos = new Vector2(260, 280);
                        pos.X -= font.MeasureString(game_type).X/2;
                        m_text_outputter.output_string(game_type, pos, Color.Black, 1, false);
                        m_text_outputter.ChangeFont(g.content.Load<SpriteFont>("Fonts\\Font"));

                    }
                    g.chat_window.render_chat_window();

                    render_player_list();

                break;

                case (int)g.game_states.MENU_LOOP:

                if (m_error_string != String.Empty)
                {
                    m_text_outputter.output_string("Error : " + m_error_string, new Vector2(10, 10), Color.WhiteSmoke, 1, false);
                }

                break;
            }

            m_mouse_cursor.render();

            g.sprite_batch.End();

        }

        public void handle_errors(String error)
        {
            error_to_main = true;
            m_error_string = error;
        }

        public void UpdatePlayerName()
        {
            foreach (Widget widget in m_widget_list)
            {
                if (widget.m_identifier == "NAME INPUT")
                {
                    widget.m_contents = g.playerInfo.m_Name;
                }
            }
        }

        public void update_menu()
        {
            m_mouse_cursor.update();

            if (error_to_main)
            {
                go_to_main();
                error_to_main = false;
            }

            foreach (Widget widget in m_widget_list)
            {
                String action = widget.check();

                if (action != "NO_ACTION")
                {
                    if (widget.m_type == widget_type.INPUT || action == "CHAT")
                    {
                        foreach (Widget reset_widget in m_widget_list)
                        {
                            reset_widget.m_state = (int)widget_states.NONE;

                            if(reset_widget.m_identifier == "CHAT")
                            {
                                g.chat_window.talk = false;
                            }
                        }

                        widget.m_state = (int)widget_states.ACTIVE;

                        if (widget.m_identifier == "CHAT")
                        {
                            g.chat_window.talk = true;
                        }

                        return;
                    }

                    switch (action)
                    {
                        case "EXIT":

                            g.current_state = (int)g.game_states.EXIT;

                        return;

                        case "NETWORK START":

                            go_to_lobby();
                        
                        return;


                        case "HOST":

                            g.is_server = true;
                            go_to_game_lobby();
                        
                        return;

                        case "JOIN":

                            g.is_server = false;
                            go_to_game_lobby();
                        
                        return;

                        case "CHAT":

                            g.chat_window.talk = true;
                            g.chat_window.chat_boxes = true;

                        return;

                        case "START GAME":

                            go_to_game();

                        return;

                        case "BACK":

                            switch (g.current_state)
                            {
                                case (int)g.game_states.GAME_LOBBY_LOOP:


                                    if (g.is_server)
                                    {
                                        g.server.StopListening();
                                        g.server = null;
                                        g.is_server = false;
                                    }
                                    else
                                    {
                                        g.clientData.client = null;
                                    }

                                    go_to_lobby();

                                return;

                                case (int)g.game_states.LOBBY_LOOP:

                                    go_to_main();

                                return;
                            }

                        return;

                        case "NEXT VEHICLE":

                            m_selected_vehicle++;

                            if (m_selected_vehicle > 1)
                                m_selected_vehicle = 0;

                        return;

                        case "PREV VEHICLE":

                            m_selected_vehicle--;

                            if (m_selected_vehicle < 0)
                                m_selected_vehicle = 1;

                        return;
                        case "NEXT LEVEL":

                        m_selected_level++;

                        if (m_selected_level > 3)
                        {
                            m_selected_level = 1;
                        }

                        return;

                        case "PREV LEVEL":

                        m_selected_level--;

                        if (m_selected_level < 0)
                        {
                            m_selected_level = 3;
                        }

                        return;
                        case "NEXT TEAM":

                            m_selected_team++;

                            if (m_selected_team > 2)
                            {
                                m_selected_team = 1;
                            }

                        return;

                        case "PREV TEAM":

                            m_selected_team--;

                            if (m_selected_team < 1)
                            {
                                m_selected_team = 2;
                            }

                        return;
                        case "NEXT GAMETYPE":

                        m_selected_gametype++;

                        if ((int)m_selected_gametype > 7)
                        {
                            m_selected_gametype = (g.game_types)0;
                        }

                        return;

                        case "PREV GAMETYPE":

                        m_selected_gametype--;

                        if ((int)m_selected_gametype < 0)
                        {
                            m_selected_gametype = (g.game_types)7;
                        }

                        return;

                        case "APPLY SETTINGS":
                            // Collect the player info
                            string name = g.playerInfo.m_Name;
                            int team = m_selected_team;
                            int playerModel = m_selected_vehicle;
                            foreach (Widget control in m_widget_list)
                            {
                                switch (control.m_identifier)
                                {
                                    // Get the player name
                                    case "NAME INPUT":
                                        name = control.m_contents;
                                        break;
                                }
                            }

                            // Build a packet with this new data
                            if (g.is_server)
                            {
                                // Check if our name has changed
                                if (g.playerInfo.m_Name != name)
                                {
                                    // Update our name if its not already in use by another client
                                    while (g.server.GetUniqueName(ref name) != false) { };

                                    // Update our local text box
                                    foreach (Widget control in m_widget_list)
                                    {
                                        if (control.m_identifier == "NAME INPUT")
                                        {
                                            control.m_contents = name;
                                        }
                                    }

                                    // Send the new name to each client
                                    g.server.SendToAllClients(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerNameUpdate, g.playerInfo.m_Name + ":" + name);

                                    g.GameRef.HandleAccouncement(g.playerInfo.m_Name + " is now known as " + name);
                                    g.server.SendToAllClients(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerAnnouce, g.playerInfo.m_Name + " is now known as " + name );

                                    // Update local player info
                                    g.playerInfo.m_Name = name;
                                }

                                // Send a team update
                                g.playerInfo.m_team = team;
                                g.server.SendToAllClients(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerTeamUpdate, g.playerInfo.m_Name + ":" + g.playerInfo.m_team.ToString());

                                // Send a player model update
                                g.playerInfo.SetModel( playerModel );
                                g.server.SendToAllClients(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerModelUpdate, g.playerInfo.m_Name + ":" + g.playerInfo.m_Model.ToString());

                            }
                            else
                            {
                                string payload = name + ":" + team.ToString() + ":" + playerModel.ToString();
                                g.clientData.client.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerSettingsUpdate, payload);
                            }
                            break;
                        case "FIND":
                            {
                                // If there is no instance then create one to start listening for
                                // a UDP broadcast response
                                if (m_BroadCastCheck == null)
                                {
                                    m_BroadCastCheck = new UDPBroadCastCheck();
                                }
                                m_servers = g.m_server_list;
                            }
                            break;
                        case "NEXT B_IP":
                            {
                                if (m_servers != null && m_servers.Count > 0)
                                {
                                    //if (m_selected_server != null)
                                    {
                                        m_selected_server ++;
                                        if (m_selected_server > m_servers.Count)
                                        {
                                            m_selected_server = 0;
                                        }
                                    }
                                }
                            }
                            break;
                        case "PREV B_IP":
                            {
                                if (m_servers != null && m_servers.Count > 0)
                                {
                                    //if (m_selected_server != null)
                                    {
                                        m_selected_server--;
                                        if (m_selected_server < 0)
                                        {
                                            m_selected_server = m_servers.Count - 1;
                                        }
                                    }
                                }
                            }
                            break;


                    }
                }
            }
            switch (g.current_state)
            {
                case (int)g.game_states.LOBBY_LOOP:
                case (int)g.game_states.GAME_LOBBY_LOOP:

                    g.chat_window.type();

                break;
                case (int)g.game_states.STATS_LOOP:
                {
                    g.hud.Update();
                }
                break;
            }
        }

        public void go_to_main()
        {
            m_widget_list.Clear();

            Widget widget_adder;

            widget_adder = new Widget(new Vector2((g.WindowCentre.X - m_big_button_dimensions.X) - 5,(g.WindowCentre.Y) + 5), new Vector2(m_big_button_dimensions.X, m_big_button_dimensions.Y), 
                                      "NETWORK START", m_button_textures["Start Big"], widget_type.BUTTON);
            m_widget_list.Add(widget_adder);

            widget_adder = new Widget(new Vector2((g.WindowCentre.X + 5) - 5, (g.WindowCentre.Y) + 5), new Vector2(m_big_button_dimensions.X, m_big_button_dimensions.Y), 
                                      "EXIT", m_button_textures["Exit Big"], widget_type.BUTTON);
            m_widget_list.Add(widget_adder);

            m_background = m_background_textures["Ese2"];

            g.current_state = (int)g.game_states.MENU_LOOP;

        }

        public void go_to_lobby()
        {
            m_widget_list.Clear();

            Widget widget_adder;
            m_background = m_background_textures["Background10"];

            widget_adder = new Widget(new Vector2(g.WindowCentre.X  -(m_big_button_dimensions.X) -5 , g.WindowCentre.Y -100), 
                m_big_button_dimensions, "HOST", m_button_textures["Host Big"], widget_type.BUTTON);

            m_widget_list.Add(widget_adder);

            widget_adder = new Widget(new Vector2(g.WindowCentre.X + 40, g.WindowCentre.Y ), 
                m_small_button_dimensions, "IP DISPLAY", m_button_textures["IP Small"], widget_type.GRAPHIC);

            m_widget_list.Add(widget_adder);

            widget_adder = new Widget(new Vector2(g.WindowCentre.X + 42, g.WindowCentre.Y + 25),
            m_small_button_dimensions, "IP ADDRESS", m_button_textures["Chat Box"], widget_type.INPUT, "86.4.1.160");

            m_widget_list.Add(widget_adder);

           

            widget_adder = new Widget(new Vector2(g.WindowCentre.X + 5, g.WindowCentre.Y -100), 
                m_big_button_dimensions, "JOIN", m_button_textures["Join Big"], widget_type.BUTTON);


            m_widget_list.Add(widget_adder);

            Vector2 pos = new Vector2(g.graphics.PreferredBackBufferWidth - m_small_button_dimensions.X - 10, 10);

            widget_adder = new Widget(pos, m_small_button_dimensions, "BACK", m_button_textures["Back Small"], widget_type.BUTTON);
            m_widget_list.Add(widget_adder);

            //////////////////////////////////////////////////////////////////////////
            //Broadcast stuff
            //////////////////////////////////////////////////////////////////////////
            widget_adder = new Widget(new Vector2(g.WindowCentre.X + 40, g.WindowCentre.Y + 50),
            m_small_button_dimensions, "FIND", m_button_textures["Find Small"], widget_type.BUTTON);

            m_widget_list.Add(widget_adder);

            pos = new Vector2(g.WindowCentre.X + 42 - m_nav_dimensions.X,
                              g.WindowCentre.Y + 25);

            widget_adder = new Widget(pos, m_nav_dimensions, "PREV B_IP", m_button_textures["Left"], widget_type.BUTTON);
            m_widget_list.Add(widget_adder);


            pos = new Vector2(g.WindowCentre.X + 42 + (m_small_button_dimensions.X) -2,
                              g.WindowCentre.Y + 25);

            widget_adder = new Widget(pos, m_nav_dimensions, "NEXT B_IP", m_button_textures["Right"], widget_type.BUTTON);
            m_widget_list.Add(widget_adder);






            g.current_state = (int)g.game_states.LOBBY_LOOP;
        }

        public void go_to_game_lobby()
        {
            m_background = m_background_textures["Background11"];

            if(g.is_server)
            {
                g.server = new gameServer();
            }
            else
            {
                //setup networking
                String address = String.Empty;

                foreach (Widget widget in m_widget_list)
                {
                    if (widget.m_identifier == "IP ADDRESS")
                    {
                        address = widget.m_contents;
                    }
                }

                //return false if invalid IP is typed
                if (address == String.Empty) return;

                g.clientData.players = new List<Player>();
                g.clientData.client = new gameClient(address);
            }

            g.chat_window.chat_boxes = true;
            //setup lobby menu page
            m_widget_list.Clear();

            Widget widget_adder;
            Vector2 dim = new Vector2(m_chat_box_dimensions.X, 200);
            Vector2 pos = new Vector2(10, g.graphics.PreferredBackBufferHeight - 20 - dim.Y - 15);

            dim = new Vector2(m_chat_box_dimensions.X, 200);
            pos = new Vector2(10, 57);

            widget_adder = new Widget(pos, dim, "Settings", m_button_textures["Settings"], widget_type.GRAPHIC);
            m_widget_list.Add(widget_adder);

            pos = new Vector2(10, g.graphics.PreferredBackBufferHeight - 40);

            widget_adder = new Widget(pos, m_chat_box_dimensions, "CHAT", m_button_textures["Chat Box"], widget_type.BUTTON,"Click here to Type");
            m_widget_list.Add(widget_adder);



            pos = new Vector2(g.graphics.PreferredBackBufferWidth - m_small_button_dimensions.X - 10, 10);

            widget_adder = new Widget(pos, m_small_button_dimensions, "BACK", m_button_textures["Back Small"], widget_type.BUTTON);
            m_widget_list.Add(widget_adder);

            //////////////////////////////////////////////////////////////////////////
            //player settings stuff
            //////////////////////////////////////////////////////////////////////////
            pos = new Vector2(g.graphics.PreferredBackBufferWidth - m_small_button_dimensions.X - m_small_button_dimensions.X - 20, 50);

            widget_adder = new Widget(pos, m_small_button_dimensions, "NAME DISPLAY", m_button_textures["Name Small"], widget_type.GRAPHIC);
            m_widget_list.Add(widget_adder);

            pos = new Vector2(g.graphics.PreferredBackBufferWidth - m_small_button_dimensions.X - 10, 50);

            widget_adder = new Widget(pos, m_small_button_dimensions, "NAME INPUT", m_button_textures["Chat Box"], widget_type.INPUT, g.playerInfo.m_Name);

            m_widget_list.Add(widget_adder);

            pos = new Vector2(g.graphics.PreferredBackBufferWidth - m_small_button_dimensions.X - 40, 430);

            widget_adder = new Widget(pos, m_small_button_dimensions, "APPLY SETTINGS", m_button_textures["Apply Small"], widget_type.BUTTON);
            m_widget_list.Add(widget_adder);

            //////////////////////////////////////////////////////////////////////////
            //Vehicle Select
            //////////////////////////////////////////////////////////////////////////
            pos = new Vector2(g.graphics.PreferredBackBufferWidth - m_small_button_dimensions.X - m_small_button_dimensions.X - 20, 90);

            widget_adder = new Widget(pos, m_small_button_dimensions, "NEXT VEHICLE", m_button_textures["Next Small"], widget_type.BUTTON);
            m_widget_list.Add(widget_adder);

            pos = new Vector2(g.graphics.PreferredBackBufferWidth - m_small_button_dimensions.X - 10, 90);

            widget_adder = new Widget(pos, m_small_button_dimensions, "PREV VEHICLE", m_button_textures["Previous Small"], widget_type.BUTTON);
            m_widget_list.Add(widget_adder);


            //////////////////////////////////////////////////////////////////////////
            //Team Stuff
            //////////////////////////////////////////////////////////////////////////
            pos = new Vector2(g.graphics.PreferredBackBufferWidth - m_nav_dimensions.X - m_nav_dimensions.X - 50 - 30, 250);

            widget_adder = new Widget(pos, m_nav_dimensions, "PREV TEAM", m_button_textures["Left"], widget_type.BUTTON);
            m_widget_list.Add(widget_adder);


            pos = new Vector2(g.graphics.PreferredBackBufferWidth - m_nav_dimensions.X - 50, 250);

            widget_adder = new Widget(pos, m_nav_dimensions, "NEXT TEAM", m_button_textures["Right"], widget_type.BUTTON);
            m_widget_list.Add(widget_adder);

            pos = new Vector2(g.graphics.PreferredBackBufferWidth - m_small_button_dimensions.X - m_small_button_dimensions.X - 20, 250);

            widget_adder = new Widget(pos, m_small_button_dimensions, "TEAM DISPLAY", m_button_textures["Team Small"], widget_type.GRAPHIC);
            m_widget_list.Add(widget_adder);

            //////////////////////////////////////////////////////////////////////////
            //Level Select
            //////////////////////////////////////////////////////////////////////////
            if(g.is_server)
            {
                pos = new Vector2(g.graphics.PreferredBackBufferWidth - m_nav_dimensions.X - m_nav_dimensions.X - 256 -9, 330);

                widget_adder = new Widget(pos, m_nav_dimensions, "PREV LEVEL", m_button_textures["Left"], widget_type.BUTTON);
                m_widget_list.Add(widget_adder);


                pos = new Vector2(g.graphics.PreferredBackBufferWidth - m_nav_dimensions.X - 11, 330);

                widget_adder = new Widget(pos, m_nav_dimensions, "NEXT LEVEL", m_button_textures["Right"], widget_type.BUTTON);
                m_widget_list.Add(widget_adder);


                //////////////////////////////////////////////////////////////////////////
                //Game Type
                //////////////////////////////////////////////////////////////////////////
                pos = new Vector2(20, 280);

                widget_adder = new Widget(pos, m_nav_dimensions, "PREV GAMETYPE", m_button_textures["Left"], widget_type.BUTTON);
                m_widget_list.Add(widget_adder);


                pos = new Vector2(480, 280);

                widget_adder = new Widget(pos, m_nav_dimensions, "NEXT GAMETYPE", m_button_textures["Right"], widget_type.BUTTON);
                m_widget_list.Add(widget_adder);


                //////////////////////////////////////////////////////////////////////////
                //id server get a start button
                //////////////////////////////////////////////////////////////////////////
              
                pos = new Vector2(g.graphics.PreferredBackBufferWidth - m_big_button_dimensions.X - 10, g.graphics.PreferredBackBufferHeight - 100);

                widget_adder = new Widget(pos, m_big_button_dimensions, "START GAME", m_button_textures["Start Big"], widget_type.BUTTON);
                m_widget_list.Add(widget_adder);
            }

            g.current_state = (int)g.game_states.GAME_LOBBY_LOOP;
        }

        public void go_to_game()
        {
            bool startGame = true;
            g.chat_window.chat_boxes = true;

            if (g.is_server)
            {
                // Must be a player to start the game!
                if (g.server.m_Clients.Count == 0)
                {
                    //startGame = false;
                    //Console.WriteLine("No clients connected but starting game anyway for testing!");
                }
                else
                {
                    // Once the game starts no one else can connect!
                    g.server.StopListening();
                }
            }

            if (startGame == true)
            {
                g.hud.m_state = HUD_state.game;
                m_widget_list.Clear();

                Widget widget_adder;
                Vector2 dim = new Vector2(m_chat_box_dimensions.X, 200);
                Vector2 pos = new Vector2(10, g.graphics.PreferredBackBufferHeight - 20 - dim.Y - 15);

                dim = new Vector2(m_chat_box_dimensions.X, 200);
                pos = new Vector2(10, 57);

                widget_adder = new Widget(pos, dim, "Settings", m_button_textures["Settings"], widget_type.GRAPHIC);
                m_widget_list.Add(widget_adder);

                g.chat_window.clear();
                g.chat_window.talk = false;
                g.chat_window.chat_boxes = false;

                g.current_state = (int)g.game_states.GAME_LOOP;

                // Tell all the clients to start!
                if (g.is_server)
                {
                    // This will load the map each for each client
                    g.server.SendToAllClients(PacketFams.ESessionManagement, PacketSubTypes.ESessionStartGame, m_selected_level.ToString() + ":" + ((int)m_selected_gametype).ToString() );

                    // To the same for us
                    g.GameRef.OnGameStart(m_selected_level, m_selected_gametype);

                    // This will cause the player models to spawn in the map
                    g.server.SpawnAllPlayers();  
                }
            }
            
        }

        public void go_to_stats()
        {
            g.hud.m_state = HUD_state.stats;
            g.current_state = (int)g.game_states.STATS_LOOP;
        }

        public void render_player_list()
        {
            Vector2 position = new Vector2(20, 90);
            Vector2 dimensions = new Vector2(30, 30);

            m_text_outputter.output_string(g.playerInfo.m_Name, position, Color.White, 1, false);
            m_text_outputter.output_string(g.playerInfo.m_Ping.ToString(), new Vector2(position.X + 150, position.Y), Color.White, 1, false);
            m_text_outputter.output_string(g.playerInfo.m_team.ToString(), new Vector2(position.X + 271, position.Y), Color.White, 1, false);

            if (g.playerInfo.m_Model == 1)
            {
                g.sprite_batch.Draw(m_button_textures["Jeep Image"].Up, new Rectangle((int)position.X + 410, (int)position.Y, (int)dimensions.X, (int)dimensions.Y), Color.White);
            }
            else
            {
                g.sprite_batch.Draw(m_button_textures["Tank Image"].Up, new Rectangle((int)position.X + 410, (int)position.Y, (int)dimensions.X, (int)dimensions.Y), Color.White);
            }
            position.Y += 20;

            if (g.is_server == true)
            {

                foreach (gameClient player in g.server.m_Clients)
                {
                    if (player.m_PlayerInfo != null)
                    {
                        m_text_outputter.output_string(player.m_PlayerInfo.m_Name, position, Color.White, 1, false);
                        m_text_outputter.output_string(player.m_PlayerInfo.m_Ping.ToString(), new Vector2(position.X + 150, position.Y), Color.White, 1, false);
                        m_text_outputter.output_string(player.m_PlayerInfo.m_team.ToString(), new Vector2(position.X + 271, position.Y), Color.White, 1, false);
                        if (player.m_PlayerInfo.m_Model == 1)
                        {
                            g.sprite_batch.Draw(m_button_textures["Jeep Image"].Up, new Rectangle((int)position.X + 410, (int)position.Y, (int)dimensions.X, (int)dimensions.Y), Color.White);
                        }
                        else
                        {
                            g.sprite_batch.Draw(m_button_textures["Tank Image"].Up, new Rectangle((int)position.X + 410, (int)position.Y, (int)dimensions.X, (int)dimensions.Y), Color.White);
                        }
                        position.Y += 20;
                    }
                }
            }
            else
            {
                foreach (Player player in g.clientData.players)
                {
                    m_text_outputter.output_string(player.m_Name, position, Color.White, 1, false);
                    m_text_outputter.output_string(player.m_Ping.ToString(), new Vector2(position.X + 150, position.Y), Color.White, 1, false);
                    m_text_outputter.output_string(player.m_team.ToString(), new Vector2(position.X + 271, position.Y), Color.White, 1, false);
                    if (player.m_Model == 1)
                    {
                        g.sprite_batch.Draw(m_button_textures["Jeep Image"].Up, new Rectangle((int)position.X + 410, (int)position.Y, (int)dimensions.X, (int)dimensions.Y), Color.White);
                    }
                    else
                    {
                        g.sprite_batch.Draw(m_button_textures["Tank Image"].Up, new Rectangle((int)position.X + 410, (int)position.Y, (int)dimensions.X, (int)dimensions.Y), Color.White);
                    }
                    position.Y += 20;
                }
            }
        }

        public void load_button_textures()
        {

           ///Big Buttons
           
           //Start Button
            m_button_textures.Add("Start Big", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonStart-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonStart-03"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonStart-02")));
           //Exit Button
            m_button_textures.Add("Exit Big", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonExit-01"),
                                                                  g.content.Load<Texture2D>("Gui\\Buttons\\ButtonExit-03"),
                                                                  g.content.Load<Texture2D>("Gui\\Buttons\\ButtonExit-02")));
            //Chat Button
            m_button_textures.Add("Chat Big", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonChat-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonChat-03"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonChat-02")));
            //Host Button
            m_button_textures.Add("Host Big", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonHost-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonHost-03"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonHost-02")));
            //IP Button
            m_button_textures.Add("IP Big", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonIP-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonIP-03"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonIP-02")));
            //Join Button
            m_button_textures.Add("Join Big", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonJoin-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonJoin-03"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonJoin-02")));
            //Back Button
            m_button_textures.Add("Back Big", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonBack-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonBack-03"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonBack-02")));
            //Name Button
            m_button_textures.Add("Name Big", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonName-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonName-03"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonName-02")));
            //Apply Button
            m_button_textures.Add("Apply Big", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonApply-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonApply-03"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonApply-02")));
            //Previous Button
            m_button_textures.Add("Previous Big", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonPrevious-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonPrevious-03"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonPrevious-02")));
            //Next Button
            m_button_textures.Add("Next Big", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonNext-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonNext-03"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonNext-02")));
            //Team Button
            m_button_textures.Add("Team Big", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonTeam-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonTeam-03"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonTeam-02")));
            //Find Button
            m_button_textures.Add("Find Big", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonFind-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonFind-03"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonFind-02")));

            /// Small Notices
            //Start Button
            m_button_textures.Add("Start Small", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonStart-04"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonStart-06"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonStart-05")));
            //Exit Button
            m_button_textures.Add("Exit Small", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonExit-04"),
                                                                  g.content.Load<Texture2D>("Gui\\Buttons\\ButtonExit-06"),
                                                                  g.content.Load<Texture2D>("Gui\\Buttons\\ButtonExit-05")));
            //Chat Button
            m_button_textures.Add("Chat Small", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonChat-04"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonChat-06"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonChat-05")));
            //Host Button
            m_button_textures.Add("Host Small", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonHost-04"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonHost-06"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonHost-05")));
            //IP Button
            m_button_textures.Add("IP Small", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonIP-04"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonIP-06"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonIP-05")));
            //Join Button
            m_button_textures.Add("Join Small", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonJoin-04"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonJoin-06"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonJoin-05")));
            //Back Button
            m_button_textures.Add("Back Small", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonBack-04"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonBack-06"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonBack-05")));
            //Name Button
            m_button_textures.Add("Name Small", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonName-04"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonName-06"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonName-05")));
            //Apply Button
            m_button_textures.Add("Apply Small", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonApply-04"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonApply-06"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonApply-05")));
            //Previous Button
            m_button_textures.Add("Previous Small", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonPrevious-04"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonPrevious-06"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonPrevious-05")));
            //Next Button
            m_button_textures.Add("Next Small", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonNext-04"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonNext-06"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonNext-05")));
            //Team Button
            m_button_textures.Add("Team Small", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonTeam-04"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonTeam-06"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonTeam-05")));

            //Find Button
            m_button_textures.Add("Find Small", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonFind-04"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonFind-06"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonFind-05")));

            ///misc
            //Chat Box
            m_button_textures.Add("Chat Box", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ChatBox-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ChatBox-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ChatBox-01")));
            //Chat Container
            m_button_textures.Add("Chat Container", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ChatContainer-02"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ChatContainer-02"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ChatContainer-02")));
            //Right Button
            m_button_textures.Add("Right", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonRight-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonRight-03"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonRight-02")));
            //Left Button
            m_button_textures.Add("Left", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\ButtonLeft-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonLeft-03"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\ButtonLeft-02")));
            //Jeep Image
            m_button_textures.Add("Jeep Image", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\Vehicles-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\Vehicles-01"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\Vehicles-01")));
            //Tank Image
            m_button_textures.Add("Tank Image", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\Vehicles-02"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\Vehicles-02"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\Vehicles-02")));

            //Settings Image
            m_button_textures.Add("Settings", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Buttons\\Settings"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\Settings"),
                                                                 g.content.Load<Texture2D>("Gui\\Buttons\\Settings")));

            //Forest thumb
            m_button_textures.Add("Forest Thumb", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Other\\Forest Thumb"),
                                                                 g.content.Load<Texture2D>("Gui\\Other\\Forest Thumb"),
                                                                 g.content.Load<Texture2D>("Gui\\Other\\Forest Thumb")));
            //Desert Thumb
            m_button_textures.Add("Desert Thumb", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Other\\Desert Thumb"),
                                                                 g.content.Load<Texture2D>("Gui\\Other\\Desert Thumb"),
                                                                 g.content.Load<Texture2D>("Gui\\Other\\Desert Thumb")));

            //Snow Thumb
            m_button_textures.Add("Snow Thumb", new ButtonTextureStruct(g.content.Load<Texture2D>("Gui\\Other\\Snow Thumb"),
                                                                 g.content.Load<Texture2D>("Gui\\Other\\Snow Thumb"),
                                                                 g.content.Load<Texture2D>("Gui\\Other\\Snow Thumb")));



            
        }
    }


    class ButtonTextureStruct
    {
        public Texture2D Up { get; private set; }
        public Texture2D Down { get; private set; }
        public Texture2D Over { get; private set; }

        public ButtonTextureStruct(Texture2D theUp,Texture2D theDown,Texture2D theOver)
        {
            Up = theUp;
            Down = theDown;
            Over = theOver;
        }
    }


}
