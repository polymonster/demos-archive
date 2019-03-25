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

namespace Tanky
{
    public class Camera
    {
        //orientation
        public Vector3 m_shadow_position = Vector3.Zero;
        public Vector3 m_position = Vector3.Zero;
        public Vector3 m_look_at = Vector3.Zero;

        public Vector3 m_center_offset;
        public Vector3 m_fixed_rotation;

        //matrices for affine transformation
        public Matrix m_view_matrix;
        public Matrix m_projection_matrix;

        public Matrix m_shadow_view;
        public Matrix m_shadow_projection;

        Vector2 m_camera_rotation = Vector2.Zero;
        Vector3 m_camera_position = Vector3.Zero;

        Vector2 m_old_mouse;

        private AudioListener m_audio;
        public AudioListener Audio
        {
            get
            {
                m_audio.Position = m_camera_position;
                m_audio.Up = Vector3.Up;
                m_audio.Forward = m_view_matrix.Forward;
                return m_audio;
            }
        }

        int cameraMode = 0; 

        //[vehicle select hack]
        public bool m_Jeep = false;

        public void initialise()
        {
            m_center_offset = new Vector3(0.0f, 45.0f, 45.0f);
            m_fixed_rotation = new Vector3(45.0f, 0.0f, 0.0f);
            m_position = m_center_offset;
            m_look_at = new Vector3(0.0f, 0.0f, 0.0f);
            
            //setup the look at matrix
            m_view_matrix = Matrix.CreateTranslation(-m_position) * 
                Matrix.CreateRotationY(MathHelper.ToRadians(m_fixed_rotation.Y)) * 
                Matrix.CreateRotationX(MathHelper.ToRadians(m_fixed_rotation.X)) ;

            //setup the projection matrix - divide by w and all that
            m_projection_matrix = Matrix.CreatePerspectiveFieldOfView
            (
                MathHelper.ToRadians(45.0f),
                g.graphics.GraphicsDevice.Viewport.AspectRatio,
                1.0f,
                500.0f
            );

            //set shadow position reltive to the main camera position
            m_shadow_position = m_position;

            //setup the shadow look at matrix
            m_shadow_view = Matrix.CreateTranslation(-m_shadow_position) * 
                Matrix.CreateRotationY(MathHelper.ToRadians(m_fixed_rotation.Y - 20.0f)) * 
                Matrix.CreateRotationX(MathHelper.ToRadians(m_fixed_rotation.X - 20.0f)) ;

            //orthogonal projection for the shadow map
            m_shadow_projection = Matrix.CreateOrthographic(300, 300, 10, 300);

            m_audio = new AudioListener();
        }

        public void move()
        {
            //get the radians value of the camera rotation
            float yrotrad = (float) MathHelper.ToRadians(m_camera_rotation.Y);
            float xrotrad = (float) MathHelper.ToRadians(m_camera_rotation.X);

            //store the mouse position
            Vector2 new_mouse;
            new_mouse.X = Mouse.GetState().X;
            new_mouse.Y = Mouse.GetState().Y;

            //calculate the vector in which the mouse has moved since last time
            Vector2 mouse_diff = new_mouse - m_old_mouse;

            if(Keyboard.GetState().IsKeyDown(Keys.Y))
            {
                //move forward, using trigenmoetry
                m_camera_position.X += (float)Math.Sin(yrotrad) * 1;
                m_camera_position.Y -= (float)Math.Sin(xrotrad) * 1;
                m_camera_position.Z -= (float)Math.Cos(yrotrad) * 1;
            }

            if(Keyboard.GetState().IsKeyDown(Keys.H))
            {
                //move backward using trigenmoetry
                m_camera_position.X -= (float)Math.Sin(yrotrad) * 1;
                m_camera_position.Y += (float)Math.Sin(xrotrad) * 1;
                m_camera_position.Z += (float)Math.Cos(yrotrad) * 1;
            }

            if(Keyboard.GetState().IsKeyDown(Keys.G))
            {
                //strafe left, using trigenmoetry 
                m_camera_position.X -= (float)Math.Cos(yrotrad) * 1;
                m_camera_position.Z -= (float)Math.Sin(yrotrad) * 1;
            }

            if(Keyboard.GetState().IsKeyDown(Keys.J))
            {
                //strafe right, using trigenmoetry
                m_camera_position.X += (float)Math.Cos(yrotrad) * 1;
                m_camera_position.Z += (float)Math.Sin(yrotrad) * 1;
            }

            if(Mouse.GetState().MiddleButton == ButtonState.Pressed)
            {
                //if the player is holding the mouse increment the
                //mouse rotation by the difference vector
                m_camera_rotation.X += mouse_diff.Y;
                m_camera_rotation.Y += mouse_diff.X;
            }

            //set the last mouse position to the current one
            //ready for the next frame
            m_old_mouse.X = (float) Mouse.GetState().X;
            m_old_mouse.Y = (float) Mouse.GetState().Y;
        }

        public void apply_transformation()
        {
            //setup the look at matrix
            //m_view_matrix = Matrix.CreateLookAt(m_position, m_look_at, Vector3.Up);
            if(Keyboard.GetState().IsKeyDown(Keys.F1)) cameraMode = 0;
            if(Keyboard.GetState().IsKeyDown(Keys.F2)) cameraMode = 1;

            //[vehicle select hack]
            if (cameraMode == 0)
            {
                /*
                if(m_Jeep){m_position = (g.tank.m_vehicle.body.centerPosition + m_center_offset);}
                else{m_position = (g.jeep.m_vehicle.body.centerPosition + m_center_offset);}
                */

                // Only follow the player if it isnt dead
                if (g.playerInfo.IsAlive())
                {
                    m_position = (g.playerInfo.m_Vehicle.m_vehicle_physics.body.centerPosition + m_center_offset);
                }

                //FOLLOW CAM
                m_view_matrix = Matrix.CreateTranslation(-m_position) *
                   Matrix.CreateRotationY(MathHelper.ToRadians(m_fixed_rotation.Y)) *
                   Matrix.CreateRotationX(MathHelper.ToRadians(m_fixed_rotation.X));
            }
            else
            {
                //FPSCAM
                m_view_matrix = Matrix.CreateTranslation(-m_camera_position) *
                  Matrix.CreateRotationY(MathHelper.ToRadians(m_camera_rotation.Y)) *
                  Matrix.CreateRotationX(MathHelper.ToRadians(m_camera_rotation.X));
            }

            m_shadow_position = m_position;

            m_shadow_position.X = (int) m_shadow_position.X;
            m_shadow_position.Y = (int) m_shadow_position.Y;
            m_shadow_position.Z = (int) m_shadow_position.Z;

            m_shadow_position.X -= 20;
            m_shadow_position.Y += 20;
            m_shadow_position.Z += 20;

            //setup the look at matrix
            m_shadow_view = Matrix.CreateTranslation(-m_shadow_position) *
                Matrix.CreateRotationY(MathHelper.ToRadians(m_fixed_rotation.Y + 20.0f)) *
                Matrix.CreateRotationX(MathHelper.ToRadians(m_fixed_rotation.X));

        }

        public Matrix rotationMatrix_XYZ
        {
            get
            {
                return Matrix.CreateRotationX(MathHelper.ToRadians(m_fixed_rotation.X)) *
                       Matrix.CreateRotationY(MathHelper.ToRadians(m_fixed_rotation.Y)) *
                       Matrix.CreateRotationZ(MathHelper.ToRadians(m_fixed_rotation.Z));

            }
            private set{}

        }
        public Matrix rotationMatrix_XY
        {
            get
            {
                return Matrix.CreateRotationX(MathHelper.ToRadians(m_fixed_rotation.X)) *
                       Matrix.CreateRotationY(MathHelper.ToRadians(m_fixed_rotation.Y));

            }
            private set { }

        }

        public BoundingFrustum frustum
        {
            get
            {
                return new BoundingFrustum(m_view_matrix * m_projection_matrix);
            }
        }
    }
}
