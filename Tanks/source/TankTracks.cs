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
    class TankTracks
    {
        float m_life;
        Vector2 m_position;
        float m_rotation;
        float m_time = 5;

        bool m_active;

        public TankTracks()
        {
            m_active = false;
        }

        public bool isActive() { return m_active; }

        public void spawn(Vector2 position, float rotation)
        {
            m_active = true;
            m_position = position;
            m_rotation = rotation;
            m_life = m_time;
        }

        public void update(GameTime gameTime)
        {
            m_life -= (float)gameTime.ElapsedGameTime.TotalSeconds;

            if(m_life <= 0)
            {
                m_active = false;
            }
        }

        public void draw(Texture2D sprite)
        {
            if (m_active)
            {
                float radRot = (float)m_rotation / 180 * (float)Math.PI;

                float scale = 0.6f;

                Vector2 rotationOrigin = Vector2.Zero;
                rotationOrigin.X = (sprite.Width / 2);
                rotationOrigin.Y = (sprite.Height / 2);


                float alpha;
                alpha = m_life / 5.0f;
                alpha *= 255.0f;

                globals.spriteBatch.Draw
                (
                    sprite,
                    m_position,
                    null,
                    new Color(255, 255, 255, (byte)alpha),
                    radRot,
                    rotationOrigin,
                    scale,
                    SpriteEffects.None,
                    0
                );
            }
        }
    }
}
