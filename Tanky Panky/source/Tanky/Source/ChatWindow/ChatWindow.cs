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

namespace Tanky
{
    public class TextOutput
    {
        SpriteFont font;

        public TextOutput()
        {
            load_textures();
        }

        public void load_textures()
        {
            font = g.content.Load<SpriteFont>("Fonts\\Font");
        }

        public void output_string(String final_output, Vector2 pos, Color color, float scale, bool center)
        {
            Vector2 middle = Vector2.Zero;

            if (center) middle = font.MeasureString(final_output) * 0.5f;

            try
            {
                g.sprite_batch.DrawString(font, final_output, pos, color, 0, middle, scale, SpriteEffects.None, 0);
            }
            catch (Exception)
            {

            }
        }
        public void ChangeFont(SpriteFont theFont)
        {
            font = theFont;
        }
    }

    public class TypeReader
    {
        String last_press;
        float type_rate = 0.0f;
        Keys last_key;

        const float REPEAT_RATE = 0.1f;
        const int MAX_TYPE_LENGTH = 70;

        bool m_caps = false;

        public static char translate_char(Keys key, bool shift)
        {
            switch (key)
            {
                case Keys.A: return (shift) ? 'A' : 'a';
                case Keys.B: return (shift) ? 'B' : 'b';
                case Keys.C: return (shift) ? 'C' : 'c';
                case Keys.D: return (shift) ? 'D' : 'd';
                case Keys.E: return (shift) ? 'E' : 'e';
                case Keys.F: return (shift) ? 'F' : 'f';
                case Keys.G: return (shift) ? 'G' : 'g';
                case Keys.H: return (shift) ? 'H' : 'h';
                case Keys.I: return (shift) ? 'I' : 'i';
                case Keys.J: return (shift) ? 'J' : 'j';
                case Keys.K: return (shift) ? 'K' : 'k';
                case Keys.L: return (shift) ? 'L' : 'l';
                case Keys.M: return (shift) ? 'M' : 'm';
                case Keys.N: return (shift) ? 'N' : 'n';
                case Keys.O: return (shift) ? 'O' : 'o';
                case Keys.P: return (shift) ? 'P' : 'p';
                case Keys.Q: return (shift) ? 'Q' : 'q';
                case Keys.R: return (shift) ? 'R' : 'r';
                case Keys.S: return (shift) ? 'S' : 's';
                case Keys.T: return (shift) ? 'T' : 't';
                case Keys.U: return (shift) ? 'U' : 'u';
                case Keys.V: return (shift) ? 'V' : 'v';
                case Keys.W: return (shift) ? 'W' : 'w';
                case Keys.X: return (shift) ? 'X' : 'x';
                case Keys.Y: return (shift) ? 'Y' : 'y';
                case Keys.Z: return (shift) ? 'Z' : 'z';

                case Keys.D0: return (shift) ? ')' : '0';
                case Keys.D1: return (shift) ? '!' : '1';
                case Keys.D2: return (shift) ? '"' : '2';
                case Keys.D3: return (shift) ? '3' : '3';
                case Keys.D4: return (shift) ? '$' : '4';
                case Keys.D5: return (shift) ? '%' : '5';
                case Keys.D6: return (shift) ? '^' : '6';
                case Keys.D7: return (shift) ? '&' : '7';
                case Keys.D8: return (shift) ? '*' : '8';
                case Keys.D9: return (shift) ? '(' : '9';

                case Keys.Add: return '+';
                case Keys.Divide: return '/';
                case Keys.Multiply: return '*';
                case Keys.Subtract: return '-';

                case Keys.Space: return ' ';

                case Keys.Decimal:  return '.';
                case Keys.NumPad0:  return '0';
                case Keys.NumPad1:  return '1'; 
                case Keys.NumPad2:  return '2'; 
                case Keys.NumPad3:  return '3';
                case Keys.NumPad4:  return '4';
                case Keys.NumPad5:  return '5';
                case Keys.NumPad6:  return '6';
                case Keys.NumPad7:  return '7';
                case Keys.NumPad8:  return '8';
                case Keys.NumPad9:  return '9';

                case Keys.OemBackslash: return (shift) ? '|' : '\\';
                case Keys.OemCloseBrackets: return (shift) ? '}' : ']';
                case Keys.OemComma: return (shift) ? '<' : ',';
                case Keys.OemMinus: return (shift) ? '_' : '-';
                case Keys.OemOpenBrackets: return (shift) ? '{' : '[';
                case Keys.OemPeriod: return (shift) ? '>' : '.';
                case Keys.OemPipe: return (shift) ? '|' : '\\';
                case Keys.OemPlus: return (shift) ? '+' : '=';
                case Keys.OemQuestion: return (shift) ? '?' : '/';
                case Keys.OemQuotes: return (shift) ? '~' : '#';
                case Keys.OemSemicolon: return (shift) ? ':' : ';';
                case Keys.OemTilde: return (shift) ? '@' : '\'';
            }

            return (char)0;
        }

        public void keyboard_input(ref String type_buffer)
        {
            //obtain the keyboard input
            Keys[] keypresses = Keyboard.GetState().GetPressedKeys();

            if (Keyboard.GetState().IsKeyUp(last_key))
            {
                type_rate = 0;
                last_press = Keys.LaunchApplication1.ToString();
            }

            //if key's have been pressed
            if (keypresses.Length != 0)
            {
                int press_array_pos = keypresses.Length - 1;

                while(keypresses[press_array_pos] == Keys.LeftShift || keypresses[press_array_pos] == Keys.RightShift)
                {
                    press_array_pos--;

                    if (press_array_pos < 0) return;
                }

                if (type_rate <= 0 || keypresses[press_array_pos].ToString() != last_press)
                {
                    int type_len = type_buffer.Length;
                      

                    //flag if shit is being held
                    bool shift = (Keyboard.GetState().IsKeyDown(Keys.RightShift) 
                        || Keyboard.GetState().IsKeyDown(Keys.LeftShift)
                        || m_caps);

                    //translate the keypress into a char
                    char trans = translate_char(keypresses[press_array_pos], shift);

                    //if char is not 0
                    if (trans != 0 && type_len < MAX_TYPE_LENGTH)
                    {
                        //add to the type buffer
                        type_buffer += trans;
                    }

                    //typer rate just in case
                    type_rate = 0.3f;

                    //delete keys with back space
                    if (Keyboard.GetState().IsKeyDown(Keys.Back))
                    {
                        String new_buffer = String.Empty;

                        for (int i = 0; i < type_len - 1; i++)
                        {
                            new_buffer += type_buffer[i];
                        }

                        type_buffer = new_buffer;

                        type_rate = 0.1f;
                    }

                    //save the last key press to avoid quick double typing
                    last_press = keypresses[press_array_pos].ToString();
                    last_key = keypresses[press_array_pos];

                    if (Keyboard.GetState().IsKeyDown(Keys.CapsLock))
                    {
                        m_caps = !m_caps;

                        last_press = Keys.CapsLock.ToString();
                        last_key = Keys.CapsLock;
                    }
                }
            }

            //reduce the timer rate that prevents multiple entries
            type_rate -= (float)(g.game_time.ElapsedRealTime.Milliseconds * 0.001f);
        }

        public void reset()
        {
            last_press = Keys.BrowserBack.ToString();
            last_key = Keys.BrowserBack;

            type_rate = 0;
        }
    }


    public class ChatWindow
    {
        const int MAX_MESSAGES = 9;

        TextOutput text_outputter;

        Texture2D chat_box_texture;
        Texture2D chat_container_texture;

        List<String> message_list;

        Vector2 root_position;

        bool submit = false;
        public bool talk = false;

        public bool chat_boxes;

        String submit_buffer;
        String type_buffer;

        TypeReader type_reader;

        bool keyLocked = false;

        public ChatWindow()
        {
            //construct
            text_outputter = new TextOutput();

            root_position = new Vector2(15, g.graphics.PreferredBackBufferHeight - 30);

            message_list = new List<String>();

            type_buffer = String.Empty;

            type_reader = new TypeReader();

            chat_box_texture = g.content.Load<Texture2D>("Gui\\Buttons\\ChatBox-01");

            chat_container_texture = g.content.Load<Texture2D>("Gui\\Buttons\\ChatContainer-02");

            chat_boxes = false;
        }

        public void type()
        {
            if (Keyboard.GetState().IsKeyDown(Keys.F4) && !keyLocked)
            {
                talk = !talk;
                keyLocked = true;
                chat_boxes = !chat_boxes;
            }

            if (Keyboard.GetState().IsKeyUp(Keys.F4))
            {
                keyLocked = false;
            }

            if (!talk) return;

            type_reader.keyboard_input(ref type_buffer);

            //submit message with enter
            if (Keyboard.GetState().IsKeyDown(Keys.Enter) && !submit)
            {
               if (type_buffer == String.Empty) return;
       
               submit = true;
               submit_buffer = type_buffer;

               type_buffer = String.Empty;

               add_message( g.playerInfo.m_Name + ": " + submit_buffer, false);
            }

            if (submit && Keyboard.GetState().IsKeyUp(Keys.Enter))
            {
                submit = false;
            }
        }

        public void send_message()
        {
            //send a message
            if (submit)
            {
                //retriev the message from here somehow
                //"submit_buffer"
            }
        }

        public void clear()
        {
            message_list.Clear();
        }


        public void add_message(String msg, bool aFromNetwork)
        {   
            //add a new message
            message_list.Insert(0, msg);

            if (message_list.Count > MAX_MESSAGES)
            {
                message_list.RemoveAt(message_list.Count - 1);
            }

            // Our client has generated this message, so send it to everyone else
            if (aFromNetwork==false)
            {
                if (!g.is_server)
                {
                    g.clientData.client.SendMessage(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerMessage, msg);
                }
                else
                {
                    g.server.SendToAllClients(PacketFams.EPlayerManagement, PacketSubTypes.EPlayerMessage, msg);
                }
            }
        }

        public void render_chat_window()
        {
            //display messages

            Vector2 message_pos = root_position;

            //render chat boxes
            if (chat_boxes)
            {
                if (g.sprite_batch != null)
                {
                    g.sprite_batch.Draw(chat_box_texture, new Rectangle((int)root_position.X - 5,
                                        (int)root_position.Y - 10,500,35), Color.White);

                    g.sprite_batch.Draw(chat_container_texture, new Rectangle((int)root_position.X - 5, 
                                        (int)root_position.Y - 10 - 200, 500, 200), Color.White);
                }
            }

            String typed_message_display = type_buffer;

            if (talk)
            {
                text_outputter.output_string(typed_message_display + "*", message_pos, Color.Chartreuse, 1.0f, false);
            }
            else
            {
                text_outputter.output_string(typed_message_display, message_pos, Color.WhiteSmoke, 1.0f, false);
            }

            message_pos.Y -= 40;

            foreach(String s in message_list)
            {
                text_outputter.output_string(s, message_pos, Color.White, 1.0f, false);

                message_pos.Y -= 20;
            }

            
        }

    }
}
