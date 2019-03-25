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
    public class TankSelectDevice
    {
        
        Texture2D deviceIco;
        public int m_schemeID;
        public bool m_activeDevice;
        public globals.Vector2i m_pos;
        globals.deviceTypes deviceType;
        bool selectThumb;

        public void loadTexture(ContentManager content, String image )
        {
            deviceIco = content.Load<Texture2D>(image);
        }

        // constructor
        public TankSelectDevice(bool active, globals.deviceTypes type, int scheme)
        {
            m_activeDevice = active;
            deviceType = type;
            m_schemeID = scheme;
            selectThumb = true;

            m_pos.X = 0;
            m_pos.Y = 0;
        }

        // update
        public void update( bool [] slot )
        {
            // check for active 360 pads. If not a pad control device, keep active
            if( deviceType == globals.deviceTypes.DEVICE_TYPE_360PAD )
                m_activeDevice = GamePad.GetState((PlayerIndex)m_schemeID).IsConnected;
            else
                m_activeDevice = true;

            if (m_activeDevice)
            {
                if (deviceType == globals.deviceTypes.DEVICE_TYPE_360PAD)
                {
                    // move selection pos
                    if (GamePad.GetState((PlayerIndex)m_schemeID).ThumbSticks.Left.Y <= -0.8f)
                    {
                        if (selectThumb)
                        {
                            int old_y = m_pos.Y;
                            // while slots are full, move through
                            while (m_pos.Y > 0 && slot[m_pos.Y])
                                m_pos.Y--;
                            // slot never changed, move to next free slot
                            if (old_y > 0 && m_pos.Y == old_y)
                                m_pos.Y--;
                            // if a slot change occured
                            if (old_y != m_pos.Y)
                            {
                                slot[old_y] = false;
                                if (m_pos.Y > 0)
                                    slot[m_pos.Y] = true;
                            }
                        }
                        selectThumb = false;
                    }
                    else if (GamePad.GetState((PlayerIndex)m_schemeID).ThumbSticks.Left.Y >= 0.8f)
                    {
                        if (selectThumb)
                        {
                            int old_y = m_pos.Y;
                            // enter visible slot
                            if (m_pos.Y == 0)
                                m_pos.Y++;
                            // while slots are full, move through
                            while (m_pos.Y < 4 && slot[m_pos.Y])
                                m_pos.Y++;
                            // slot never changed, move to next free slot
                            if (m_pos.Y < 4 && m_pos.Y == old_y)
                                m_pos.Y++;
                            // if a slot change occured
                            if (old_y != m_pos.Y)
                            {
                                // new slot empty?
                                if (!slot[m_pos.Y])
                                {
                                    slot[old_y] = false;
                                    if (m_pos.Y > 0)
                                        slot[m_pos.Y] = true;
                                }
                                // else revert
                                else
                                    m_pos.Y = old_y;
                            }
                        }
                        selectThumb = false;
                    }
                    else
                        selectThumb = true;
                }
                else
                {
                    Keys upkey;
                    Keys downkey;
                    if (m_schemeID == 4)
                    {
                        upkey = Keys.W;
                        downkey = Keys.S;
                    }
                    else
                    {
                        upkey = Keys.NumPad8;
                        downkey = Keys.NumPad5;
                    }

                    if (selectThumb)
                    {
                        if (Keyboard.GetState().IsKeyDown(upkey))
                        {
                            int old_y = m_pos.Y;
                            // enter visible slot
                            if (m_pos.Y == 0)
                                m_pos.Y++;
                            // while slots are full, move through
                            while (m_pos.Y < 4 && slot[m_pos.Y])
                                m_pos.Y++;
                            // slot never changed, move to next free slot
                            if (m_pos.Y < 4 && m_pos.Y == old_y)
                                m_pos.Y++;
                            // if a slot change occured
                            if (old_y != m_pos.Y)
                            {
                                // new slot empty?
                                if (!slot[m_pos.Y])
                                {
                                    slot[old_y] = false;
                                    if (m_pos.Y > 0)
                                        slot[m_pos.Y] = true;
                                }
                                // else revert
                                else
                                    m_pos.Y = old_y;
                            }
                            selectThumb = false;
                        }
                        else if (Keyboard.GetState().IsKeyDown(downkey))
                        {
                            int old_y = m_pos.Y;
                            // while slots are full, move through
                            while (m_pos.Y > 0 && slot[m_pos.Y])
                                m_pos.Y--;
                            // slot never changed, move to next free slot
                            if (old_y > 0 && m_pos.Y == old_y)
                                m_pos.Y--;
                            // if a slot change occured
                            if (old_y != m_pos.Y)
                            {
                                slot[old_y] = false;
                                if (m_pos.Y > 0)
                                    slot[m_pos.Y] = true;
                            }

                            selectThumb = false;
                        }
                    }

                    if (Keyboard.GetState().IsKeyUp(upkey) && Keyboard.GetState().IsKeyUp(downkey))
                        selectThumb = true;

                }
            }
        }

        // draw
        public void draw()
        {
            if (m_activeDevice)
            {
                Vector2 devicePos;
                devicePos.X = (m_pos.X * 100.0f) + 130.0f;
                devicePos.Y = 458.0f - (m_pos.Y * 100.0f);

                globals.spriteBatch.Begin(SpriteBlendMode.AlphaBlend);

                globals.spriteBatch.Draw
                (
                    deviceIco,
                    devicePos,
                    null,
                    Color.White,
                    0,
                    Vector2.Zero,
                    1.0f,
                    SpriteEffects.None,
                    0
                );

                globals.spriteBatch.End();
            }
        }

    }
}
