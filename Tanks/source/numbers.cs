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

namespace tankGame
{
    public class textOutput
    {
        Texture2D[] numberSheet;
        int numberOfSheets = 1;

        public void init()
        {
            numberSheet = new Texture2D[numberOfSheets];
        }

        public void loadTextures(ContentManager content)
        {
            globals.font = content.Load<SpriteFont>("Font");
        }

        public void outputNumber(float number, int style, Vector2 pos, Color scoreCol,float scale)
        {
            String finalOutput;

            finalOutput = number.ToString();

            //globals.spriteBatch.Begin();
            globals.spriteBatch.DrawString(globals.font, finalOutput, pos, scoreCol, 0, Vector2.Zero, scale, SpriteEffects.None, 0);
            //globals.spriteBatch.End();
        }

        public void outputString(String finalOutput, int style, Vector2 pos, Color scoreCol,float scale)
        {
            Vector2 middle = globals.font.MeasureString(finalOutput);
            middle *= 0.5f;
            //globals.spriteBatch.Begin();
            globals.spriteBatch.DrawString(globals.font, finalOutput, pos, scoreCol, 0, middle, scale, SpriteEffects.None, 0);
            //globals.spriteBatch.End();
        }
    }
}
