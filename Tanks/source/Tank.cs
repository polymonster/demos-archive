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
    public class Tank
    {
        int max_projectiles = 10;

        Vector2 position = Vector2.Zero;

        float baseRotation = 0;
        float newBaseRotation = 0;
        float turretRotation = 0;
        float newTurretRotation = 0;

        float trackSpawnTimer;

        Vector2 turretPosition = Vector2.Zero;
        Texture2D sprite;
        Texture2D turretSprite;
        Vector2 updatedPos;

        float m_fireRate = 1;
        float m_fireTimer = 0;
        int dir = 1;
        int controlScheme;

        int radius;
        int score;

        int playerID;

        float respawnTimer;

        bool m_alive;
        bool m_collidedThisRecursion;
        bool respawn;

        bool registered;

        Projectile[] projectiles;

        float trackSpawnRate = 0.1f;

        public Vector2 getPos()
        {
            return position;
        }

        public bool isRegistered() { return registered; }

        public int getScore() { return score; }

        public int getID(){return playerID;}
     
        public bool collidedThisRecursion(){return m_collidedThisRecursion;}

        public int getRadius()
        {
            return radius;
        }

        public void kill()
        {
            position.X = 1000;
            position.Y = 1000;
            m_alive = false;
            respawnTimer = 5;
        }

        public bool isAlive(){return m_alive;}

        public void spawn(Vector2 spawnPos)
        {
            position = spawnPos;
            updatedPos = position;
            trackSpawnTimer = trackSpawnRate;

            m_alive = true;

        }

        public void spawn()
        {
            position = globals.level.getSpawnPos();
            updatedPos = position;
            trackSpawnTimer = trackSpawnRate;

            m_alive = true;

        }

        public void dontRespawn(bool theTruth) { respawn = theTruth; }

        public void init(int scheme, int id)
        {
            playerID = id;

            radius = 25;

            score = 0;

            respawn = true;

            controlScheme = scheme;

            if(scheme != -1) registered = true;
            else registered = false;
        }

        public void loadTexture(ContentManager content, String baseString, String turretString)
        {
            sprite = content.Load<Texture2D>(baseString);
            turretSprite = content.Load<Texture2D>(turretString);
        }

        public void fire()
        {
            if (m_fireTimer <= 0)
            {
                float turretRadRot = (float)turretRotation / 180 * (float)Math.PI;

                Vector2 direction;
                direction.X = (float)Math.Sin(turretRadRot);
                direction.Y = -(float)Math.Cos(turretRadRot);

                Vector2 startPos = position + (direction * 35.0f);

                tankGame.globals.level.addNewProjectile(startPos, direction, turretRotation, 5.0f, playerID);

                m_fireTimer = m_fireRate;
            }
        }

        public void addPoints(int ammount)
        {
            score += ammount;
        }

        public bool spawnTracks(ref Vector2 pos, ref float rot)
        {
            pos = position;
            rot = baseRotation;

            if (m_alive)
            {

                if (trackSpawnTimer <= 0)
                {
                    trackSpawnTimer = trackSpawnRate;
                    return true;
                }

            }

            return false;
        }

        public void controlSystem(int scheme, GameTime gameTime)
        {
            float radRot = (float)baseRotation / 180 * (float)Math.PI;
            Vector2 baseThumbstick;
            Vector2 turretThumbstick;
            float movement;

            if (scheme >= 0 && scheme < 4)
            {
            
                //Its a control pad zomg!
                baseThumbstick.X = GamePad.GetState((PlayerIndex)scheme).ThumbSticks.Left.X;
                baseThumbstick.Y = GamePad.GetState((PlayerIndex)scheme).ThumbSticks.Left.Y;

                turretThumbstick.X = GamePad.GetState((PlayerIndex)scheme).ThumbSticks.Right.X;
                turretThumbstick.Y = GamePad.GetState((PlayerIndex)scheme).ThumbSticks.Right.Y;

                newBaseRotation = (float)Math.Atan2((float)-baseThumbstick.Y, (float)baseThumbstick.X);
                newBaseRotation = MathHelper.ToDegrees(newBaseRotation);
                newBaseRotation += 90.0f;

                newTurretRotation = (float)Math.Atan2((float)-turretThumbstick.Y, (float)turretThumbstick.X);
                newTurretRotation = MathHelper.ToDegrees(newTurretRotation);
                newTurretRotation += 90.0f;

                //newTurretRotation += newBaseRotation;

                if (baseThumbstick.X != 0 || baseThumbstick.Y != 0)
                {
                    dir = 1;
                    float original = baseRotation;
                    interpolateRotation(ref baseRotation, ref newBaseRotation, 2.0f, 2.0f, true);
                    float diff = baseRotation - original;
                    turretRotation += diff;
                }

                if (turretThumbstick.X != 0 || turretThumbstick.Y != 0)
                {
                    float speed = Math.Abs(turretThumbstick.X) + Math.Abs(turretThumbstick.Y) * 0.5f;
                    interpolateRotation(ref turretRotation, ref newTurretRotation, 0.5f, 0.5f, false);
                }

                movement = Math.Abs(baseThumbstick.X) + Math.Abs(baseThumbstick.Y);
                if (movement > 1) movement = 1;
                position.X += movement * (float)Math.Sin(radRot) * dir;
                position.Y -= movement * (float)Math.Cos(radRot) * dir;
                globals.soundManager.play("shell");
                if (GamePad.GetState((PlayerIndex)scheme).Triggers.Right > 0) fire();

            }
            else
            {

                switch (scheme)
                {
                    case 4:

                        if (Keyboard.GetState().IsKeyDown(Keys.W))
                        {
                            position.X += (float)Math.Sin(radRot);
                            position.Y -= (float)Math.Cos(radRot);
                        }

                        if (Keyboard.GetState().IsKeyDown(Keys.S))
                        {
                            position.X -= (float)Math.Sin(radRot);
                            position.Y += (float)Math.Cos(radRot);
                        }

                        if (Keyboard.GetState().IsKeyDown(Keys.Q))
                        {
                            turretRotation -= 120.0f * (float)gameTime.ElapsedGameTime.TotalSeconds;
                        }

                        if (Keyboard.GetState().IsKeyDown(Keys.E))
                        {
                            turretRotation += 120.0f * (float)gameTime.ElapsedGameTime.TotalSeconds;
                        }

                        if (Keyboard.GetState().IsKeyDown(Keys.A))
                        {
                            baseRotation -= 140.0f * (float)gameTime.ElapsedGameTime.TotalSeconds;
                        }

                        if (Keyboard.GetState().IsKeyDown(Keys.D))
                        {
                            baseRotation += 140.0f * (float)gameTime.ElapsedGameTime.TotalSeconds;
                        }

                        if (Keyboard.GetState().IsKeyDown(Keys.Space)) fire();

                        break;

                    case 5:

                        if (Keyboard.GetState().IsKeyDown(Keys.NumPad8))
                        {
                            position.X += (float)Math.Sin(radRot);
                            position.Y -= (float)Math.Cos(radRot);
                        }

                        if (Keyboard.GetState().IsKeyDown(Keys.NumPad5))
                        {
                            position.X -= (float)Math.Sin(radRot);
                            position.Y += (float)Math.Cos(radRot);
                        }

                        if (Keyboard.GetState().IsKeyDown(Keys.NumPad7))
                        {
                            turretRotation -= 120.0f * (float)gameTime.ElapsedGameTime.TotalSeconds;
                        }

                        if (Keyboard.GetState().IsKeyDown(Keys.NumPad9))
                        {
                            turretRotation += 120.0f * (float)gameTime.ElapsedGameTime.TotalSeconds;
                        }

                        if (Keyboard.GetState().IsKeyDown(Keys.NumPad4))
                        {
                            baseRotation -= 140.0f * (float)gameTime.ElapsedGameTime.TotalSeconds;
                        }

                        if (Keyboard.GetState().IsKeyDown(Keys.NumPad6))
                        {
                            baseRotation += 140.0f * (float)gameTime.ElapsedGameTime.TotalSeconds;
                        }

                        if (Keyboard.GetState().IsKeyDown(Keys.NumPad0)) fire();

                        break;

                }

              }

        }

        public void update(GameTime gameTime)
        {
            if (m_alive)
            {
                updatedPos = position;

                controlSystem(controlScheme, gameTime);

                m_fireTimer -= (float)gameTime.ElapsedGameTime.TotalSeconds;

                trackSpawnTimer -= (float)gameTime.ElapsedGameTime.TotalSeconds;
            }
            else
            {
                if (respawn)
                {
                    respawnTimer -= (float)gameTime.ElapsedGameTime.TotalSeconds;
                    if (respawnTimer <= 0) spawn();
                }
            }
        }

        public void interpolateRotation(ref float oldRotation, ref float newRotation, float threshold, float speed, bool halfwayClamp)
        {

            if(newRotation - oldRotation > 180) oldRotation += 360;
            else if (newRotation - oldRotation < -180) oldRotation -= 360;     

            if (oldRotation != newRotation)
		    {
                float diff = oldRotation - newRotation;
                float absoluteDifference = Math.Abs(diff);

                if (halfwayClamp)
                {
                    if (diff > 90)
                    {
                        newRotation += 180;
                        dir = -1;
                    }
                    else if (diff < -90)
                    {
                        newRotation -= 180;
                        dir = -1;
                    }
                }
                else
                {
                    speed *= (absoluteDifference * 0.05f);
                }

                if (absoluteDifference < threshold)
                {
                    oldRotation = newRotation;
                }
                else if (oldRotation < newRotation)
			    {
                    oldRotation += speed;
			    }
                else if (oldRotation > newRotation)
			    {
                    oldRotation -= speed;
			    }
		    }
        }

        public double magnitude(Vector2 v)
        {
            return Math.Sqrt((v.X * v.X) + (v.Y * v.Y));
        }

        public void offsetPos(Vector2 offset, Vector2 slidingPlane, bool forceOffset)
        {
            bool slide = forceOffset;

            Vector2 movementVector = position - updatedPos;

            float dotProduct = Vector2.Dot(slidingPlane, movementVector);
            float magnitudeProduct = (float)magnitude(slidingPlane) * (float)magnitude(movementVector);

            float collisionAngle = (float) Math.Acos(dotProduct / magnitudeProduct);
            collisionAngle = (float) collisionAngle * 180.0f / (float) Math.PI;

            if (collisionAngle < 60.0f || collisionAngle > 120.0f) slide = true;
            
            if(offset != Vector2.Zero)
            {
                if (!slide) position = updatedPos;
                else position += offset;
            }

            updatedPos = position;

        }

        public void drawScore()
        {
            globals.spriteBatch.Begin(SpriteBlendMode.AlphaBlend);

            Vector2 tPos = Vector2.Zero;

            if (playerID == 0) tPos = new Vector2(10, 10);
            if (playerID == 1) tPos = new Vector2(750, 10);
            if (playerID == 2) tPos = new Vector2(10, 520);
            if (playerID == 3) tPos = new Vector2(750, 520);

            Color textCol = new Color(0, 0, 0, 164);

            switch (playerID)
            {
                case 0:

                    textCol = new Color(138, 32, 17, 164);

                    break;

                case 1:

                    textCol = new Color(16, 66, 135, 164);

                    break;

                case 2:

                    textCol = new Color(215, 179, 22, 164);
                    

                    break;

                case 3:


                    textCol = new Color(75, 118, 34, 164);

                    break;

            }

            Vector2 sPos = tPos;

            sPos.X += 4;
            sPos.Y += 2;

            globals.text.outputNumber(score, 0, sPos, new Color(0,0,0,90) ,1.0f);
            globals.text.outputNumber(score, 0, tPos, textCol,1.0f);

            globals.spriteBatch.End();

        }


        public void draw()
        {
            if (m_alive)
            {
                globals.spriteBatch.Begin(SpriteBlendMode.AlphaBlend);

                float scale = 0.6f;

                Vector2 rotationOrigin = Vector2.Zero;
                rotationOrigin.X = (sprite.Width / 2);
                rotationOrigin.Y = (sprite.Height / 2);

                float radRot = (float)baseRotation / 180 * (float)Math.PI; //MathHelper.ToRadians(baseRotation);
                float turretRadRot = (float)turretRotation / 180 * (float)Math.PI;

                Vector2 turretPos = position;

                globals.spriteBatch.Draw
                (
                    sprite,
                    position,
                    null,
                    Color.White,
                    radRot,
                    rotationOrigin,
                    scale,
                    SpriteEffects.None,
                    0
                );

                rotationOrigin.X = (turretSprite.Width / 2);
                rotationOrigin.Y = ((turretSprite.Height / 4) * 3);

                globals.spriteBatch.Draw
                (
                    turretSprite,
                    turretPos,
                    null,
                    Color.White,
                    turretRadRot,
                    rotationOrigin,
                    scale,
                    SpriteEffects.None,
                    0
                );

                globals.spriteBatch.End();
            }
        }
    }
}
