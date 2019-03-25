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
    public class Projectile
    {
        Vector2 m_position;
        float m_velocity;
        Vector2 m_direction;
        bool m_active;
        float m_life;
        float m_rotation;
        float m_radius;

        int m_ownerID;


        public int getOwner(){return m_ownerID;}
        public Vector2 getPos(){return m_position;}
        public float getRadius() { return m_radius; }

        public void kill()
        {
            m_active = false;
        }

        public Projectile(Vector2 position, Vector2 direction, float velocity)
        {
            direction.Normalize();

            m_position = position;
            m_direction = direction;
            m_velocity = velocity;

            m_radius = 5.0f;

        }

        public void spawn(Vector2 position, Vector2 direction, float rotation, float velocity, int ownerID)
        {
            direction.Normalize();

            m_position = position;
            m_direction = direction;
            m_velocity = velocity;
            m_rotation = rotation; 

            m_active = true;
            m_life = 100;

            m_ownerID = ownerID;
        }

        public bool isActive()
        {
            return m_active;
        }

        public void loadTexture(ContentManager content)
        {
            //m_sprite = content.Load<Texture2D>("tanksprite");
        }

        public void update()
        {
           if(m_active) m_position += m_direction * m_velocity;
           m_life -= 0.1f;

           if (m_life <= 0) m_active = false;
        }

        public void draw(Texture2D sprite)
        {
            if (m_active)
            {
                Vector2 center = Vector2.Zero;
                center.X += (sprite.Width / 2);
                center.Y += (sprite.Height / 2);

                float radRot = (float) MathHelper.ToRadians(m_rotation);

                globals.spriteBatch.Draw
                (
                      sprite,
                      m_position,
                      null,
                      Color.White,
                      radRot,
                      center,
                      0.5f,
                      SpriteEffects.None,
                      0
                 );
            }
        }
    }
}
