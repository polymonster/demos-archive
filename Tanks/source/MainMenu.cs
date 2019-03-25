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
    public class MainMenu
    {
        public Texture2D background;
//        public Texture2D logo;
        public MenuButton startButton;
        public MenuButton quitButton;

        public void loadTexture(ContentManager content, String texture)
        {
            background = content.Load<Texture2D>(texture);
//           logo = content.Load<Texture2D>("mainlogo");
        }

        public MainMenu( int startx, int starty, int quitx, int quity )
        {
            startButton = new MenuButton(startx, starty, globals.buttonTypes.BUTTON_ONETAP, false);
            quitButton = new MenuButton(quitx, quity, globals.buttonTypes.BUTTON_ONETAP, false);
        }

        public void update()
        {
            if( globals.gameState == globals.states.GAME_STATE_MAIN_MENU )
                startButton.showButton( true );
            startButton.update();
            quitButton.update();
        }

        public void draw()
        {
            globals.spriteBatch.Begin(SpriteBlendMode.AlphaBlend);

            Vector2 bgPos;
            bgPos.X = 0;
            bgPos.Y = 0;

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

            bgPos.X = 20;
            bgPos.Y = 20;

            globals.spriteBatch.End();

            startButton.draw();
            quitButton.draw();
        }

    }
}
