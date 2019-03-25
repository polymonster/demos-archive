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
using System.IO;

namespace Tanky
{
    public class Vehicle
    {
        protected Vector3 m_turret_pos;

        //rotation variables
        protected Vector3 m_base_rotation;
        protected Vector3 m_new_base_rotation;

        protected Vector3 m_turret_rotation;
        protected Vector3 m_new_turret_rotation;

        protected Vector3 m_rotation_offset = new Vector3(0, 0, 20.0f);

        //models
        protected ModelTP m_tracks_model;
        protected ModelTP m_base_model;
        protected ModelTP m_turret_model;
        protected ModelTP m_muzzle;

        //control mechanics
        protected g.schemes m_control_scheme;

        protected int m_dir;
        protected float m_movement = 0.0f;

        //positioning variables
        public Vector3 m_position;

        //Control TimeStep Timer
        protected Timer m_ContTimeStep;

        protected SoundEffect m_fire_sound;

        public Vehicle()
        {

        }

        public Vehicle(Vector3 pos, Vector3 dir, g.schemes control_scheme)
        {
            m_control_scheme = control_scheme;

            m_position = pos;
            m_base_rotation = dir;
            m_turret_rotation = dir;

            load();
        }

        public virtual void load()
        {

            m_new_turret_rotation = m_turret_rotation;
            m_new_base_rotation = m_base_rotation;
            m_rotation_offset = new Vector3(0, 0, 15.0f);

            m_base_model = new ModelTP();
            m_base_model.load("Models\\base_mesh");

            m_tracks_model = new ModelTP();
            m_tracks_model.load("Models\\tracks_mesh");

            m_turret_model = new ModelTP();
            m_turret_model.load("Models\\turret_mesh");

            m_muzzle = new ModelTP();
            m_muzzle.load("Models\\muzzle_mesh");


        }

        public void interpolateRotation(ref float oldRotation, ref float newRotation, float threshold, float speed, bool halfwayClamp)
        {
            if (newRotation - oldRotation > 180) oldRotation += 360;
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
                        m_dir = -1;
                    }
                    else if (diff < -90)
                    {
                        newRotation -= 180;
                        m_dir = -1;
                    }
                }
                else
                {
                    speed *= (absoluteDifference * 0.1f); //* g.game_time.ElapsedGameTime.Seconds);
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
        /*
        public void update()
        {
            control();
            m_proj.update();
        }
        */

        public virtual void control()
        {

        }

        public virtual Vector3 calculate_bullet_pos()
        {
            return Vector3.Zero;
        }

        public virtual void draw(Matrix projection, Matrix view)
        {
            // Implemented in derived type
        }

        public virtual void shadow_map_render(Matrix projection, Matrix view)
        {

        }

    }
}
