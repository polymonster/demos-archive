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
    class Particles
    {
        Vector2 m_position;
        float m_scale;
        float m_life;
        bool m_active;
        float m_life_scalar;
        float m_rotation;
        int m_type;
        float m_innerScale;

        public bool isActive(){return m_active;}

        public Particles()
        {
            m_active = false;
        }

        public void update(GameTime gameTime)
        {
            m_life -= (float)gameTime.ElapsedGameTime.TotalSeconds;
            m_scale += (float)gameTime.ElapsedGameTime.TotalSeconds;
            m_innerScale -= (float)gameTime.ElapsedGameTime.TotalSeconds * 0.3f;
            m_rotation += (float)gameTime.ElapsedGameTime.TotalSeconds * 20;

            if(m_life <= 0)
            {
                m_active = false;
            }


        }

        public void spawn(Vector2 pos, float scale, float life,int type)
        {
            m_position = pos;
            m_scale = scale;
            m_innerScale = scale;
            m_life = life;
            m_life_scalar = life;
            m_active = true;
            m_rotation = 0;
            m_type = type;
        }

        public void draw(Texture2D sprite, Texture2D explode)
        {
            if (m_active)
            {
                float radRot = (float)m_rotation / 180 * (float)Math.PI;

                Vector2 rotationOrigin = Vector2.Zero;
                rotationOrigin.X = (sprite.Width / 2);
                rotationOrigin.Y = (sprite.Height / 2);

                float alpha;
                alpha = m_life / m_life_scalar;
                alpha *= 255.0f;

                globals.spriteBatch.Draw
                (
                        sprite,
                        m_position,
                        null,
                        new Color(255, 255, 255, (byte) alpha),
                        radRot,
                        rotationOrigin,
                        m_scale,
                        SpriteEffects.None,
                        0
                );

                globals.spriteBatch.Draw
                (
                        sprite,
                        m_position,
                        null,
                        new Color(255, 255, 255, (byte)alpha),
                        -radRot,
                        rotationOrigin,
                        m_scale,
                        SpriteEffects.None,
                        0
                );

                if (m_type == 1)
                {
                    alpha = m_life / (m_life_scalar * 2.0f);
                    alpha *= 255.0f;

                    if (m_innerScale < 0) m_innerScale = 0;

                    //if (alpha > 0)
                    //{
                        globals.spriteBatch.Draw
                        (
                                explode,
                                m_position,
                                null,
                                new Color(255, 255, 255, (byte) alpha),
                                radRot * 0.5f,
                                rotationOrigin,
                                m_innerScale,
                                SpriteEffects.None,
                                0
                        );
                    //}

                }
            }
        }
    }
}
