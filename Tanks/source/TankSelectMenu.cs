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
    public class TankSelectMenu
    {
        Texture2D background;
        Texture2D title;
        Texture2D grid;

        public TankSelectDevice[] inputDevice = new TankSelectDevice[globals.MAX_DEVICES];

        public bool[] selected = new bool[5];

        Vector2 bgPos;

        public void loadTexture(ContentManager content)
        {
            title = content.Load<Texture2D>("title_select");
            background = content.Load<Texture2D>("selectbg");
            grid = content.Load<Texture2D>("tankselectgrid");

            // background size
            bgPos.X = 0;
            bgPos.Y = 0;

        }

        // constructor
        public TankSelectMenu()
        {
            for( int i = 0; i < 5; i++ )
                selected[i] = false;
            inputDevice[0] = new TankSelectDevice(true, globals.deviceTypes.DEVICE_TYPE_KEYBOARD, 5);
            inputDevice[1] = new TankSelectDevice(true, globals.deviceTypes.DEVICE_TYPE_KEYBOARD, 4);
            inputDevice[2] = new TankSelectDevice(GamePad.GetState(PlayerIndex.One).IsConnected, globals.deviceTypes.DEVICE_TYPE_360PAD, 0);
            inputDevice[3] = new TankSelectDevice(GamePad.GetState(PlayerIndex.Two).IsConnected, globals.deviceTypes.DEVICE_TYPE_360PAD, 1);
            inputDevice[4] = new TankSelectDevice(GamePad.GetState(PlayerIndex.Three).IsConnected, globals.deviceTypes.DEVICE_TYPE_360PAD, 2);
            inputDevice[5] = new TankSelectDevice(GamePad.GetState(PlayerIndex.Four).IsConnected, globals.deviceTypes.DEVICE_TYPE_360PAD, 3);

        }

        // update
        public void update()
        {
            for( int i = 0; i < 5; i++ )
                selected[i] = false;

            for (int i = 0; i < 6; i++)
                inputDevice[i].m_pos.X = 0;

            for (int i = 0; i < 6; i++)
            {
                for (int j = 0; j < i; j++)
                    if (inputDevice[i].m_activeDevice)
                        inputDevice[j].m_pos.X++;

                if (inputDevice[i].m_activeDevice && inputDevice[i].m_pos.Y > 0)
                    selected[inputDevice[i].m_pos.Y] = true;
            }

            selected[0] = false;

            for (int i = 0; i < 6; i++)
                inputDevice[i].update(selected);

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

            globals.spriteBatch.Draw
            (
                grid,
                bgPos,
                null,
                Color.White,
                0,
                Vector2.Zero,
                1.0f,
                SpriteEffects.None,
                0
            );

            globals.spriteBatch.End();

            for (int i = 0; i < 6; i++)
                inputDevice[i].draw();

        }
    }
}
