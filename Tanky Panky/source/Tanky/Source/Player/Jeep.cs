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
using BEPUphysics;
using Tanky.Source.Physics.Display;
using System.Diagnostics;

namespace Tanky
{
    public class Jeep : VehiclePhysics
    {
        protected ModelTP m_jeep_body_model;
        protected ModelTP m_jeep_turret_model;
        protected ModelTP m_jeep_fl_wheel;
        protected ModelTP m_jeep_fr_wheel;
        protected ModelTP m_jeep_rl_wheel;
        protected ModelTP m_jeep_rr_wheel;

        Matrix m_turret_matrix;

        public Jeep(Vector3 pos, Vector3 dir, g.schemes control_scheme,Player theParent)
        {
            m_turretOffset = new Vector3(0.0f, 0.0f, 0.0f);

            m_control_scheme = control_scheme;

            m_Parent = theParent;
            //m_position = pos;
            m_base_rotation = dir;
            m_turret_rotation = dir;

            m_forwardAcceleration = 10;
            m_backwardsAcceleration = -12;

            m_flip_time = 10000;

            m_weapon_damage = 10;
            m_weapon_speed = 950f;
            m_weapon_rate = 350;     //in millseconds

            load();
            SetUpPhysics();
        }

        public override void load()
        {
            m_new_turret_rotation = m_turret_rotation;
            m_new_base_rotation = m_base_rotation;
            m_rotation_offset = new Vector3(0, 0, 15.0f);

            m_jeep_body_model = new ModelTP();
            m_jeep_body_model.load("Models\\jeep_body");

            m_jeep_turret_model = new ModelTP();
            m_jeep_turret_model.load("Models\\jeep_turret");

            m_jeep_fl_wheel = new ModelTP();
            m_jeep_fl_wheel.load("Models\\jeep_wheel");

            m_jeep_fr_wheel = new ModelTP();
            m_jeep_fr_wheel.load("Models\\jeep_wheel");

            m_jeep_rl_wheel = new ModelTP();
            m_jeep_rl_wheel.load("Models\\jeep_wheel");

            m_jeep_rr_wheel = new ModelTP();
            m_jeep_rr_wheel.load("Models\\jeep_wheel");

            m_fire_sound = g.content.Load<SoundEffect>("Sound\\Explosion");
        }

        public override void draw(Matrix projection, Matrix view)
        {
            Vector3 turretOffset = new Vector3(0.0f, 0.0f, -30.0f);

            Matrix World = new Matrix();
            World = Matrix.CreateScale(0.05f) * m_vehicle_physics.body.orientationMatrix * 
                Matrix.CreateTranslation(m_vehicle_physics.body.centerPosition);

            m_turret_matrix = Matrix.Identity * Matrix.CreateRotationY(MathHelper.ToRadians(m_turret_rotation.Y))
                * Matrix.CreateTranslation(turretOffset)
                * World;
               
            m_jeep_turret_model.draw(projection, view, m_turret_matrix);
            m_jeep_body_model.draw(projection, view, World);

            //WHEELS
            World = Matrix.CreateScale(0.05f) 
                * Matrix.CreateRotationZ(MathHelper.ToRadians(90))
                * m_vehicle_physics.wheels[0].worldMatrix;
            m_jeep_fl_wheel.draw(projection, view, World);

            World = Matrix.CreateScale(0.05f)
                * Matrix.CreateRotationZ(MathHelper.ToRadians(-90))
                * m_vehicle_physics.wheels[1].worldMatrix;
            m_jeep_fr_wheel.draw(projection, view, World);

            World = Matrix.CreateScale(0.05f)
                * Matrix.CreateRotationZ(MathHelper.ToRadians(90))
                * m_vehicle_physics.wheels[2].worldMatrix;
            m_jeep_rl_wheel.draw(projection, view, World);

            World = Matrix.CreateScale(0.05f)
                * Matrix.CreateRotationZ(MathHelper.ToRadians(-90))
                * m_vehicle_physics.wheels[3].worldMatrix;
            m_jeep_rr_wheel.draw(projection, view, World);
        }

        public override void shadow_map_render(Matrix projection, Matrix view)
        {
            Vector3 turretOffset = new Vector3(0.0f, 0.0f, -30.0f);

            Matrix World = new Matrix();
            World = Matrix.CreateScale(0.05f) * m_vehicle_physics.body.orientationMatrix * Matrix.CreateTranslation(m_vehicle_physics.body.centerPosition);

            m_turret_matrix = Matrix.Identity * Matrix.CreateRotationY(MathHelper.ToRadians(m_turret_rotation.Y))
            * Matrix.CreateTranslation(turretOffset)
            * World;

            m_jeep_turret_model.draw(projection, view, m_turret_matrix);
            m_jeep_body_model.shadow_map_render(projection, view, World);

            World = Matrix.CreateScale(0.05f)
                * Matrix.CreateRotationZ(MathHelper.ToRadians(90))
                * m_vehicle_physics.wheels[0].worldMatrix;
            m_jeep_fl_wheel.shadow_map_render(projection, view, World);

            World = Matrix.CreateScale(0.05f)
                * Matrix.CreateRotationZ(MathHelper.ToRadians(-90))
                * m_vehicle_physics.wheels[1].worldMatrix;
            m_jeep_fr_wheel.shadow_map_render(projection, view, World);

            World = Matrix.CreateScale(0.05f)
                * Matrix.CreateRotationZ(MathHelper.ToRadians(90))
                * m_vehicle_physics.wheels[2].worldMatrix;
            m_jeep_rl_wheel.shadow_map_render(projection, view, World);

            World = Matrix.CreateScale(0.05f)
                * Matrix.CreateRotationZ(MathHelper.ToRadians(-90))
                * m_vehicle_physics.wheels[3].worldMatrix;
            m_jeep_rr_wheel.shadow_map_render(projection, view, World);
        }

        public override Vector3 calculate_bullet_pos()
        {
            //Vector3[] vertex_data = new Vector3[1];
            //m_muzzle.get_vertex_data(ref vertex_data, 0, 1);
            //Vector3 tempMat = new Vector3();

            //Matrix tempMatrix =
            //Matrix.CreateRotationY(MathHelper.ToRadians(m_turret_rotation.Y)) *
            //m_vehicle_physics.body.internalOrientationMatrix *
            //Matrix.CreateTranslation(m_vehicle_physics.body.centerPosition);
            //tempMat = tempMatrix.Forward;
            return m_turret_matrix.Translation;
        }

        public override void control()
        {
            if (m_Parent.IsAlive())
            {
                m_uprightConstraint.isUpdating = false;

                #region Variable definition
                bool userInput = false;
                bool i_Forward = false;
                bool i_Backward = false;
                bool i_LeftTurn = false;
                bool i_RightTurn = false;
                bool i_Brake = false;
                bool i_TurretLeft = false;
                bool i_TurretRight = false;
                float i_ForwardValue = 0;
                float i_BackwardValue = 0;
                float i_TurnValue = 0;
                Vector2 i_Turn = Vector2.Zero;
                bool i_Fire = false;
                bool i_Flip = false;
                #endregion

                #region uprighting / flip
                //Don't bother righting the vehicle if it's flipping through the air, or if all of its wheels are grounded anyway.
                int support = m_vehicle_physics.supportedWheelCount;

                if (support == 1 || support == 2)
                {
                    m_uprightConstraint.isUpdating = true;
                }
                else
                {
                    m_uprightConstraint.isUpdating = false;
                }

                //if no wheels on the ground
                if (support == 0)
                {
                    if (m_is_upside_down == false)
                    {
                        m_flip_timer.Reset();
                    }
                    m_is_upside_down = true;
                }
                else
                {
                    m_is_upside_down = false;
                }
                #endregion

                #region Check keyboard/gamepad
                if (m_control_scheme == g.schemes.CONTROL_SCHEME_KEYBOARD)
                {
                    KeyboardState state = Keyboard.GetState(PlayerIndex.One);

                    //WASD KEys
                    i_Forward = state.IsKeyDown(Keys.W);
                    i_Backward = state.IsKeyDown(Keys.S);
                    i_LeftTurn = state.IsKeyDown(Keys.A);
                    i_RightTurn = state.IsKeyDown(Keys.D);

                    i_Brake = state.IsKeyDown(Keys.LeftShift);

                    i_Fire = state.IsKeyDown(Keys.Space) || g.mouse.m_state.LeftButton == ButtonState.Pressed;
                    i_Flip = state.IsKeyDown(Keys.Z);
                }
                else if (m_control_scheme == g.schemes.CONTROL_SCHEME_360PAD)
                {
                    //not sure what deadzone on the gamepad works like tbh
                    //could one of you guys adjust this to a proper value ..cheers
                    float DeadZone = 0;

                    // if stick is above the deadzone on the y axis then make value true
                    if (i_Forward = g.gamepad_state.ThumbSticks.Left.Y > (0 + DeadZone))
                    {
                        //and set how much
                        i_ForwardValue = g.gamepad_state.ThumbSticks.Left.Y;
                    }

                    // if stick is below the deadzone on the y axis then make value true
                    if (i_Backward = g.gamepad_state.ThumbSticks.Left.Y < (0 - DeadZone))
                    {
                        //and set how much
                        i_BackwardValue = g.gamepad_state.ThumbSticks.Left.Y;
                    }

                    // if stick is above the deadzone on the y axis then make value true
                    if (i_LeftTurn = g.gamepad_state.ThumbSticks.Left.X > (0 + DeadZone))
                    {
                        //and set how much
                        i_TurnValue = g.gamepad_state.ThumbSticks.Left.X;
                    }

                    // if stick is below the deadzone on the y axis then make value true
                    if (i_RightTurn = g.gamepad_state.ThumbSticks.Left.X < (0 - DeadZone))
                    {
                        //and set how much
                        i_TurnValue = -g.gamepad_state.ThumbSticks.Left.X;
                    }

                    i_Brake = g.gamepad_state.IsButtonDown(Buttons.B);

                }
                #endregion

                #region control region
                m_ContTimeStep.Update();
                if (m_ContTimeStep.Elapsed >= 17)
                {
                    foreach (Wheel w in m_vehicle_physics.wheels)
                    {
                        w.wheelFacingAngle = 0;
                    }
                    if (i_Forward)
                    {
                        Accelerate();
                        userInput = true;
                    }
                    else if (i_Backward)
                    {
                        Reverse();
                        userInput = true;
                    }
                    else if (i_Brake)
                    {
                        Brake();
                        userInput = true;
                    }

                    if (i_LeftTurn)
                    {
                        TurnLeft();
                        userInput = true;
                    }
                    if (i_RightTurn)
                    {
                        TurnRight();
                        userInput = true;
                    }

                    if (i_TurretLeft)
                    {
                        RotateTurretLeft();
                    }
                    if (i_TurretRight)
                    {
                        RotateTurretLRight();
                    }

                    RotateTurretAnalogue();

                    if (!userInput)
                    {
                        //Tell the vehicle to brake if it's not being controlled.
                        m_vehicle_physics.brake(.5f);
                    }
                    m_ContTimeStep.Reset();
                }
                #endregion

                #region flip control
                m_flip_timer.Update();
                if (m_flip_timer.Elapsed > m_flip_time && i_Flip && m_is_upside_down)
                {
                    Vector3 old_pos = m_vehicle_physics.body.centerPosition;
                    old_pos.Y += 3;


                    m_vehicle_physics.body.orientationMatrix = Matrix.Identity;
                    m_vehicle_physics.body.centerPosition = old_pos;

                    m_flip_timer.Reset();
                }
                #endregion

                #region Fire control
                m_weapon_timer.Update();
                if (m_weapon_timer.Elapsed > m_weapon_rate && i_Fire)
                {
                    shoot_projectile(calculate_bullet_pos(), m_turret_matrix.Forward);
                    m_weapon_timer.Reset();
                }
                #endregion
            }
            else
            {
                // Slow down when dead?
                Brake();
            }
        }

        void shoot_projectile(Vector3 pos, Vector3 dir)
        {

            ProjectilePackage pkg = new ProjectilePackage(g.playerInfo.m_Name, pos, dir, m_weapon_speed, m_weapon_damage);
            if (g.is_server)
            {
                // Fire the projectile locally
                g.projectile_manager.Add(pkg);
                m_fire_sound.Play(0.8f);

                // Tell the clients about the shot
                g.server.SendToAllClients(PacketFams.EMovementData, PacketSubTypes.EMovementSpawnProjectile, pkg.PackData());
            }
            else
            {
                // Ask the server to fire a bullet for us
                g.clientData.client.SendMessage(PacketFams.EMovementData, PacketSubTypes.EMovementRequestFireProjectile, pkg.PackData());
            }
        }

        void SetUpPhysics()
        {
            Model wheelModel = g.content.Load<Model>("models\\carWheel");
            Texture2D wheelTexture = g.content.Load<Texture2D>("textures\\wheel");

            CompoundBody body = new CompoundBody();
            body.addBody(new Box(new Vector3(0, 0, 0), 3, 0.5f, 6, 170));
            body.addBody(new Box(new Vector3(0, 0.5f, -0.5f), 2, 0.5f, 03f, 10));

            body.moveTo(new Vector3(30.0f, -15f, 0.0f)); //At first, just keep it out of the way.
            m_vehicle_physics = new BEPUphysics.Vehicle(body, 35, 20);
            //Increase the inertia parallel to the car to prevent rolling a little.
            Matrix inertiaTensorInverse = m_vehicle_physics.body.localSpaceInertiaTensorInverse;
            inertiaTensorInverse.M31 *= .5f;
            inertiaTensorInverse.M32 *= .5f;
            inertiaTensorInverse.M33 *= .5f;
            m_vehicle_physics.body.localSpaceInertiaTensorInverse = inertiaTensorInverse;
            m_vehicle_physics.body.collisionMargin = .1f;

            //The two sections below correspond to ray cast wheels and convex cast wheels; 
            //Uncomment the one you'd like to use- note that the defaults are a little different, so handling won't be the same between the two.

            #region ConvexCastWheels

            //Convex cast wheels detect collisions in a smoother manner, preventing the wheel from 'falling into' a thin, deep crevice.
            //Similar recommendations apply in regards to keeping the starting position of the object within the car.  With convex cast wheels,
            //if your wheel is poking out to the sides and you drive near an obstacle/wall, the wheels will view that object as a support
            //which can cause interesting flips.
            //front left wheel:

            Quaternion initialWheelRotation = Quaternion.CreateFromAxisAngle(Vector3.Forward, (float)Math.PI / 2);

            const float wheelRadius = 0.375f;
            const float wheelYPos = -0.1f;
            const float wheelWidth = 0.2f;          //0.2f
            const float frontWheelsPos = 1.9f;      //1.8f
            const float backWheelsPos = -1.6f;      //-1.5f

            const float suspensionLen = 0.6f;       //0.6f
            const float suspensionConst = 42;       //42
            const float suspensionDamp = 0.99f;      //0.8f
            const float tireGrip = 0.5f;            //0.5f
            const float slidingFritction = 1.8f;   //0.8f
            const float rollFriction = 0.2f;       //0.2f
            const float maxSusForce = 800;         //500

            Entity wheelShape = new Cylinder(Vector3.Zero, wheelWidth, wheelRadius);
            wheelShape.collisionMargin = 0;
            m_vehicle_physics.addWheel(new ConvexCastWheel(new Vector3(-1.3f, wheelYPos, frontWheelsPos), initialWheelRotation,
                wheelShape, wheelRadius, new Vector3(0, -1, 0), new Vector3(0, 0, 1), suspensionLen, suspensionConst, slidingFritction, rollFriction, tireGrip, maxSusForce, suspensionDamp));

            //front right wheel:
            wheelShape = new Cylinder(Vector3.Zero, wheelWidth, wheelRadius);
            wheelShape.collisionMargin = 0;
            m_vehicle_physics.addWheel(new ConvexCastWheel(new Vector3(1.3f, wheelYPos, frontWheelsPos), initialWheelRotation,
                wheelShape, wheelRadius, new Vector3(0, -1, 0), new Vector3(0, 0, 1), suspensionLen, suspensionConst, slidingFritction, rollFriction, tireGrip, maxSusForce, suspensionDamp));

            //back left wheel:
            wheelShape = new Cylinder(Vector3.Zero, wheelWidth, wheelRadius);
            wheelShape.collisionMargin = 0;
            m_vehicle_physics.addWheel(new ConvexCastWheel(new Vector3(-1.3f, wheelYPos, backWheelsPos), initialWheelRotation,
                wheelShape, wheelRadius, new Vector3(0, -1, 0), new Vector3(0, 0, 1), suspensionLen, suspensionConst, slidingFritction, rollFriction, tireGrip, maxSusForce, suspensionDamp));

            //back right wheel:
            wheelShape = new Cylinder(Vector3.Zero, wheelWidth, wheelRadius);
            wheelShape.collisionMargin = 0;
            m_vehicle_physics.addWheel(new ConvexCastWheel(new Vector3(1.3f, wheelYPos, backWheelsPos), initialWheelRotation,
                wheelShape, wheelRadius, new Vector3(0, -1, 0), new Vector3(0, 0, 1), suspensionLen, suspensionConst, slidingFritction, rollFriction, tireGrip, maxSusForce, suspensionDamp));

            #endregion

            m_space = g.physics.m_collisionSpace;
            m_uprightConstraint = new UprightConstraint(body, Vector3.Up, (float)Math.PI / 8, (float)Math.PI / 2f, 20000);
            m_space.add(m_uprightConstraint);

            m_space.add(m_vehicle_physics);
            m_entityDrawer = g.physics.m_entityDrawer;
            m_entityDrawer.addEntity(m_vehicle_physics.body);
            DisplayModel model;
            m_wheelModels = new List<DisplayModel>();
            for (int k = 0; k < 4; k++)
            {
                m_vehicle_physics.wheels[k].detectorShape.tag = "noDisplayObject";
                model = new DisplayModel(wheelModel);
                m_entityDrawer.displayModels.Add(model);
                m_wheelModels.Add(model);
                model.setTexture(wheelTexture);
            }
        }

        public virtual void RotateTurretLeft()
        {
            m_turret_rotation.Y += 4f;
        }
        public virtual void RotateTurretLRight()
        {
            m_turret_rotation.Y -= g.mouse.m_relative_pos.X;
        }

        public virtual void RotateTurretAnalogue()
        {
            
            m_turret_rotation.Y -= g.mouse.m_relative_pos.X * 1.5f;

            //Vector2 dir = g.mouse.m_relative_pos;
            //if (dir.X != 0.0f || dir.Y != 0.0f)
            //{
            //    float rads = 57.2957795f;

            //    float rot;
            //    rot = (float)-Math.Atan2((double)dir.Y, (double)dir.X) * rads;

            //    m_turret_rotation.Y += (rot - m_turret_rotation.Y);
            //}
        }

    }

}
