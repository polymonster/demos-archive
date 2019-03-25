using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Net;
using Microsoft.Xna.Framework.Storage;

namespace tankGame
{
    public class MenuButton
    {
        static String[] mapThumbString = { "map1thumb", "map2thumb", "map3thumb", "map4thumb", "map5thumb" };
        Rectangle buttonRect1;
        Rectangle buttonRect2;
        Texture2D buttonImg;
        Texture2D buttonImgOvr;
        Texture2D buttonArrowL;
        Texture2D buttonArrowLOvr;
        Texture2D buttonArrowR;
        Texture2D buttonArrowROvr;
        Texture2D[] mapThumbs = new Texture2D[5];
        bool m_hit1;
        bool m_hit2;
        public int m_list_size;
        bool m_dynamicCol;

        public globals.buttonTypes m_type;
        public Color m_col;
        public bool m_mouseOver1;
        public bool m_mouseOver2;
        public bool m_hideButton;
        public bool m_selected;
        public int m_list_value;
        public int[] m_list_array = new int[5];

        public void loadSprites( ContentManager content, String normal, String mouseover )
        {
            buttonImg = content.Load<Texture2D>(normal);

            if (m_type < globals.buttonTypes.BUTTON_TOGGLE)
            {
                buttonImgOvr = content.Load<Texture2D>(mouseover);
                // button rect based on image
                buttonRect1.Height = buttonImg.Height;
                buttonRect1.Width = buttonImg.Width;
                buttonRect1.X -= buttonRect1.Width / 2;
                buttonRect1.Y -= buttonImg.Height / 2;
            }
            else
            {
                loadArrowSprites(content);
                if( m_type == globals.buttonTypes.BUTTON_IMG_SELECT )
                    for( int i = 0; i < 5; i++ )
                        mapThumbs[i] = content.Load<Texture2D>(mapThumbString[i]);
            }
        }

        public void loadArrowSprites(ContentManager content)
        {
            if (m_type == globals.buttonTypes.BUTTON_TOGGLE)
            {
                buttonArrowL = content.Load<Texture2D>("tickcross_no");
                buttonArrowLOvr = content.Load<Texture2D>("tickcross_no-yes");
                buttonArrowR = content.Load<Texture2D>("tickcross_yes");
                buttonArrowROvr = content.Load<Texture2D>("tickcross_yes-no");

                buttonRect1.Height = buttonArrowL.Height;
                buttonRect1.Width = buttonArrowL.Width;
                buttonRect1.X -= (buttonRect1.Width / 2)-20;
                buttonRect1.Y -= buttonRect1.Height / 2;
            }
            else
            {
                buttonArrowR = content.Load<Texture2D>("arrowr");
                buttonArrowROvr = content.Load<Texture2D>("arrowrovr");
                buttonArrowL = content.Load<Texture2D>("arrowl");
                buttonArrowLOvr = content.Load<Texture2D>("arrowlovr");

                // button rect based on image
                buttonRect2.Height = buttonArrowR.Height;
                buttonRect2.Width = buttonArrowR.Width;
                buttonRect2.X -= buttonRect2.Width / 2;
                buttonRect2.Y -= buttonRect2.Height / 2;

                buttonRect1.Height = buttonArrowL.Height;
                buttonRect1.Width = buttonArrowL.Width;
                buttonRect1.X -= buttonRect1.Width / 2;
                buttonRect1.Y -= buttonRect1.Height / 2;
            }
        }

        // init variables
        public MenuButton( int x, int y, globals.buttonTypes type, bool dynamicColour )
        {
            m_dynamicCol = dynamicColour;
            m_list_size = 5;
            m_type = type;
            m_col = Color.White;
            m_mouseOver1 = false;
            m_mouseOver2 = false;
            m_hit1 = false;
            m_hideButton = false;
            m_selected = false;

            set_pos(x, y);

            m_hit1 = false;
            m_hit2 = false;
        }

        public void set_pos(int x, int y)
        {
            buttonRect1.Y = y;
            buttonRect2.Y = y;
            switch (m_type)
            {
                case globals.buttonTypes.BUTTON_LIST:
                    buttonRect1.X = x - 40;
                    buttonRect2.X = x + 80;
                    break;
                case globals.buttonTypes.BUTTON_IMG_SELECT:
                    buttonRect1.X = x - 130;
                    buttonRect2.X = x + 130;
                    break;
                default:
                    buttonRect1.X = x;
                    break;
            }
        }

        public Vector2 get_pos()
        {
            Vector2 pos;
            pos.X = buttonRect1.X;
            pos.Y = buttonRect1.Y;
            return pos;
        }

        public void showButton(bool yesno)
        {
            m_hideButton = !yesno;
        }

        // button logic
        public void update()
        {
            if (!m_hideButton)
            {
                //if (m_dynamicCol && !m_mouseOver1)
                //{
                //    switch (globals.gameState)
                //    {
                //        case globals.states.GAME_STATE_MAIN_MENU:
                //            m_col = new Color(100, 200, 100);
                //            break;
                //        case globals.states.GAME_STATE_TANK_SELECT:
                //            m_col = new Color(80, 160, 220);
                //            break;
                //        case globals.states.GAME_STATE_GAME_SETUP:
                //            m_col = new Color(250, 70, 70);
                //            break;
                //    }
                //}
                //else
                {
                    m_col = Color.White;
                }

                int X1 = buttonRect1.X;
                int X2 = buttonRect1.X +  + buttonRect1.Width;
                if (m_type == globals.buttonTypes.BUTTON_TOGGLE)
                {
                    if (!m_selected)
                        X1 += (int)(buttonRect1.Width * 0.5f);
                    else
                        X2 -= (int)(buttonRect1.Width * 0.5f);
                }

                // check for mouse over constantly
                if ((Mouse.GetState().X > X1) &&
                    (Mouse.GetState().X < X2) &&
                    (Mouse.GetState().Y > buttonRect1.Y) &&
                    (Mouse.GetState().Y < buttonRect1.Y + buttonRect1.Height))
                    m_mouseOver1 = true;
                else
                {
                    m_mouseOver1 = false;
                    if (m_type >= globals.buttonTypes.BUTTON_LIST)
                    {
                        if ((Mouse.GetState().X > buttonRect2.X) &&
                            (Mouse.GetState().X < buttonRect2.X + buttonRect2.Width) &&
                            (Mouse.GetState().Y > buttonRect2.Y) &&
                            (Mouse.GetState().Y < buttonRect2.Y + buttonRect2.Height))
                            m_mouseOver2 = true;
                        else
                            m_mouseOver2 = false;
                    }
                }

                // mouseover and mouse button down = hit!
                if (m_mouseOver1 && Mouse.GetState().LeftButton == ButtonState.Pressed) m_hit1 = true;
                if (m_mouseOver2 && Mouse.GetState().LeftButton == ButtonState.Pressed) m_hit2 = true;

                if( m_hit1 || m_hit2 )
                {
                    if ((m_mouseOver1 || m_mouseOver2) && Mouse.GetState().LeftButton == ButtonState.Released)
                    {
                        switch( m_type )
                        {
                            case globals.buttonTypes.BUTTON_LIST:
                            case globals.buttonTypes.BUTTON_IMG_SELECT:
                                // check for button hit to alter list value
                                if (m_hit1)
                                {
                                    m_list_value--;
                                    if (m_list_value < 0)
                                        m_list_value = m_list_size - 1;
                                }
                                if (m_hit2)
                                {
                                    m_list_value++;
                                    if (m_list_value >= m_list_size)
                                        m_list_value = 0;
                                }
                            break;

                            case globals.buttonTypes.BUTTON_ONETAP:
                                // one chance to hit this button before deactivation
                                m_hideButton = true;
                                m_selected = true;
                            break;

                            case globals.buttonTypes.BUTTON_TOGGLE:
                                m_selected = !m_selected;
                            break;
                        }

                        m_hit1 = false;
                        m_hit2 = false;
                    }
                }
            }
        }

        // button rendering
        public void draw()
        {
            if (!m_hideButton)
            {
                globals.spriteBatch.Begin(SpriteBlendMode.AlphaBlend);

                switch( m_type )
                {
                    case globals.buttonTypes.BUTTON_ONETAP:
                        if( m_hit1 )
                            globals.spriteBatch.Draw(buttonImg, buttonRect1, m_col);
                        else if( m_mouseOver1 )
                            globals.spriteBatch.Draw(buttonImgOvr, buttonRect1, m_col);
                        else
                            globals.spriteBatch.Draw(buttonImg, buttonRect1, m_col);
                        break;

                    case globals.buttonTypes.BUTTON_TOGGLE:
                        Vector2 postPos;
                        postPos.X = buttonRect1.X - 50 - buttonImg.Width;
                        postPos.Y = buttonRect1.Y;
                        globals.spriteBatch.Draw(buttonImg, postPos, m_col);

                        if( m_selected && !m_mouseOver1 )
                            globals.spriteBatch.Draw(buttonArrowR, buttonRect1, m_col);
                        else if( m_selected && m_mouseOver1 )
                            globals.spriteBatch.Draw(buttonArrowROvr, buttonRect1, m_col);
                        else if (!m_selected && !m_mouseOver1)
                            globals.spriteBatch.Draw(buttonArrowL, buttonRect1, m_col);
                        else if (!m_selected && m_mouseOver1)
                            globals.spriteBatch.Draw(buttonArrowLOvr, buttonRect1, m_col);
                        break;

                    case globals.buttonTypes.BUTTON_LIST:
                        postPos.X = buttonRect1.X - 20 - buttonImg.Width;
                        postPos.Y = buttonRect1.Y;
                        globals.spriteBatch.Draw(buttonImg, postPos, m_col);
                        if (m_hit1)
                            globals.spriteBatch.Draw(buttonArrowL, buttonRect1, m_col);
                        else if (m_mouseOver1)
                            globals.spriteBatch.Draw(buttonArrowLOvr, buttonRect1, m_col);
                        else
                            globals.spriteBatch.Draw(buttonArrowL, buttonRect1, m_col);

                        if (m_hit2)
                            globals.spriteBatch.Draw(buttonArrowR, buttonRect2, m_col);
                        else if (m_mouseOver2)
                            globals.spriteBatch.Draw(buttonArrowROvr, buttonRect2, m_col);
                        else
                            globals.spriteBatch.Draw(buttonArrowR, buttonRect2, m_col);

                        postPos.X = buttonRect1.X + 60;
                        globals.spriteBatch.DrawString(globals.font, m_list_array[m_list_value].ToString(), postPos, Color.Silver);
                        break;

                    case globals.buttonTypes.BUTTON_IMG_SELECT:
                        postPos.X = buttonRect1.X - 60;
                        postPos.Y = buttonRect1.Y - 140;
                        globals.spriteBatch.Draw(buttonImg, postPos, m_col);
                        if (m_hit1)
                            globals.spriteBatch.Draw(buttonArrowL, buttonRect1, m_col);
                        else if (m_mouseOver1)
                            globals.spriteBatch.Draw(buttonArrowLOvr, buttonRect1, m_col);
                        else
                            globals.spriteBatch.Draw(buttonArrowL, buttonRect1, m_col);

                        if (m_hit2)
                            globals.spriteBatch.Draw(buttonArrowR, buttonRect2, m_col);
                        else if (m_mouseOver2)
                            globals.spriteBatch.Draw(buttonArrowROvr, buttonRect2, m_col);
                        else
                            globals.spriteBatch.Draw(buttonArrowR, buttonRect2, m_col);
                        postPos.Y = buttonRect1.Y - 60;
                        postPos.X += 120;
                        globals.spriteBatch.Draw(mapThumbs[m_list_value], postPos, Color.White);
                        break;

                }
                globals.spriteBatch.End();
            }
        }
    }
}
