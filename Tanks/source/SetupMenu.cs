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
    public class SetupMenu
    {
        Texture2D background;
        Texture2D title;

        public MenuButton gameButtonTime;
        public MenuButton gameButtonPoints;
        public MenuButton gameButtonLast;
        public MenuButton gameButtonMap;

        Vector2 bgPos;

        public void loadTexture(ContentManager content)
        {
            title = content.Load<Texture2D>("title_setup");
            background = content.Load<Texture2D>("setupbg");

            // background size
            bgPos.X = 0;
            bgPos.Y = 0;

        }

        // constructor
        public SetupMenu()
        {
            gameButtonTime = new MenuButton(550, 180-40, globals.buttonTypes.BUTTON_LIST, false);
            gameButtonTime.m_list_size = 5;
            gameButtonTime.m_list_array = globals.gameTimes;
            gameButtonPoints = new MenuButton(550, 250 - 40, globals.buttonTypes.BUTTON_LIST, false);
            gameButtonPoints.m_list_size = 5;
            gameButtonPoints.m_list_array = globals.gamePoints;
            gameButtonLast = new MenuButton(550, 320 - 40, globals.buttonTypes.BUTTON_TOGGLE, false);
            gameButtonMap = new MenuButton(400, 510, globals.buttonTypes.BUTTON_IMG_SELECT, false);
            gameButtonPoints.m_list_size = 5;
        }

        // update
        public void update()
        {
            gameButtonTime.showButton(true);
            gameButtonPoints.showButton(true);
            gameButtonLast.showButton(true);
            gameButtonMap.showButton(true);
            gameButtonTime.update();
            gameButtonPoints.update();
            gameButtonLast.update();
            gameButtonMap.update();
            
        }

        // draw
        public void draw()
        {
            globals.spriteBatch.Begin(SpriteBlendMode.AlphaBlend);

            globals.spriteBatch.Draw
            (
                background,
                bgPos,
                null,
                Color.White,
                0,
                Vector2.Zero,
                1.0f,
                SpriteEffects.None,
                0
            );

            Vector2 titlePos;
            titlePos.X = 110;
            titlePos.Y = 10;
            globals.spriteBatch.Draw
            (
                title,
                titlePos,
                null,
                Color.White,
                0,
                Vector2.Zero,
                1.0f,
                SpriteEffects.None,
                0
            );

            globals.spriteBatch.End();

            gameButtonTime.draw();
            gameButtonPoints.draw();
            gameButtonLast.draw();
            gameButtonMap.draw();
        }
    }
}
