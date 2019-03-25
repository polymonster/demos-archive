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
    public class Tank : VehiclePhysics
    {
        Matrix m_turret_matrix;
        Matrix m_muzzle_matrix;

        public Tank(Vector3 pos, Vector3 dir, g.schemes control_scheme,Player theParent) 
        {
            m_turretOffset = new Vector3(0f, 0f, -10f);

            m_control_scheme = control_scheme;

            m_Parent = theParent;
            m_position = pos;
            m_base_rotation = dir;
            m_turret_rotation = dir;

            m_forwardAcceleration = 8;
            m_backwardsAcceleration = -8;

            m_weapon_damage = 18;
            m_weapon_speed = 60f;
            m_weapon_rate = 640;     //in millseconds

            load();
            SetUpPhysics();
        }

        public override void load()
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

            m_fire_sound = g.content.Load<SoundEffect>("Sound\\Explosion");
        }

        public override void draw(Matrix projection, Matrix view)
        {

            Matrix World = new Matrix();

            //its not drawing the models in the right place so I'm adding offsets for fine tuning - its a pain but it works

            Vector3 bodyOffset = new Vector3(0f, -1.1f, 0f);

            //new matrix values
            World = Matrix.CreateScale(0.05f) * 
                    ((CompoundBody)m_vehicle_physics.body).getBodies()[0].orientationMatrix * 
                    Matrix.CreateTranslation(((CompoundBody)m_vehicle_physics.body).getBodies()[0].centerPosition) * 
                    Matrix.CreateTranslation(bodyOffset);

            m_turret_matrix =  Matrix.CreateRotationY(MathHelper.ToRadians(m_turret_rotation.Y))*
                              Matrix.CreateTranslation(m_turretOffset) * World;
            
            //old matrix values if something goes wrong
            // World = Matrix.Identity * m_physicsVehicle.body.orientationMatrix * Matrix.CreateTranslation(m_physicsVehicle.body.centerPosition);
            //m_turret_matrix = Matrix.CreateRotationY(MathHelper.ToRadians(m_turret_rotation.Y)) * World;

            m_tracks_model.draw(projection, view, World);
            m_base_model.draw(projection, view, World);
            m_turret_model.draw(projection, view, m_turret_matrix);

            //m_Effects.draw();

        }

        public override void shadow_map_render(Matrix projection, Matrix view)
        {
            Matrix World = new Matrix();
            World = Matrix.CreateScale(0.05f) * m_vehicle_physics.body.orientationMatrix * Matrix.CreateTranslation(m_vehicle_physics.body.centerPosition);

            m_turret_matrix = Matrix.CreateRotationY(MathHelper.ToRadians(m_turret_rotation.Y)) *
                                Matrix.CreateTranslation(m_turretOffset) * World;

            m_tracks_model.shadow_map_render(projection, view, World);
            m_base_model.shadow_map_render(projection, view, World);
            m_turret_model.shadow_map_render(projection, view, m_turret_matrix);
        }

        public override Vector3 calculate_bullet_pos()
        {
            Vector3[] vertex_data = new Vector3[1];
            m_muzzle.get_vertex_data(ref vertex_data, 0, 1);

            m_muzzle_matrix =
            Matrix.CreateRotationY(MathHelper.ToRadians(m_turret_rotation.Y)) *
            Matrix.CreateTranslation(m_turretOffset * 0.05f) *
            m_vehicle_physics.body.orientationMatrix *
            Matrix.CreateTranslation(m_vehicle_physics.body.centerPosition);

            return m_muzzle_matrix.Translation;
        }

        public override void control() 
        {
            if (m_Parent.IsAlive())
            {
                m_uprightConstraint.isUpdating = false;

                // control flags
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
                bool i_Fire = false;
                bool i_Flip = false;

                // check for vehicle flipping
                //Don't bother righting the vehicle if it's flipping through the air, or if all of its wheels are grounded anyway.
                int support = m_vehicle_physics.supportedWheelCount;

                if (support == 1 || support == 2 || support == 3)
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

                // key assignments
                if (m_control_scheme == g.schemes.CONTROL_SCHEME_KEYBOARD)
                {
                    //debug control keys
                    KeyboardState state = Keyboard.GetState(PlayerIndex.One);

                    Keys[] keys = state.GetPressedKeys();


                    // //Console.WriteLine("Num keys pressed = " + keys.Length.ToString());

                    //WASD KEys
                    i_Forward = state.IsKeyDown(Keys.W);
                    i_Backward = state.IsKeyDown(Keys.S);
                    i_LeftTurn = state.IsKeyDown(Keys.A);
                    i_RightTurn = state.IsKeyDown(Keys.D);

                    i_Brake = state.IsKeyDown(Keys.LeftShift);

                    i_Fire = state.IsKeyDown(Keys.Space) || g.mouse.m_state.LeftButton == ButtonState.Pressed;
                    i_Flip = state.IsKeyDown(Keys.Z);

                    ////Console.WriteLine("Fire:" + i_Fire.ToString() + " Forward: " + i_Forward.ToString() + " Left:" + i_LeftTurn.ToString() + " Right:" + i_RightTurn.ToString()); 

                    //i_Brake = g.keyboard_state.IsKeyDown(Keys.Space);
                    //i_TurretLeft = g.keyboard_state.IsKeyDown(Keys.Q);
                    //i_TurretRight = g.keyboard_state.IsKeyDown(Keys.E);
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

                    i_TurretLeft = g.gamepad_state.IsButtonDown(Buttons.LeftShoulder);
                    i_TurretRight = g.gamepad_state.IsButtonDown(Buttons.RightShoulder);
                }

                // execute inputs
                m_ContTimeStep.Update();
                if (m_ContTimeStep.Elapsed >= 17)
                {
                    if (i_Forward)
                    {
                        //if turning then accelerate at a slower speed
                        if (i_LeftTurn || i_RightTurn)
                        {
                            Accelerate(4.0f);
                        }
                        else
                        {
                            Accelerate(2.5f);
                        }
                    }
                    else if (i_Backward)
                    {
                        Reverse();
                    }
                    else if (i_Brake)
                    {
                        Brake();
                    }
                    //if not moving anyway then brake
                    else
                    {
                        m_vehicle_physics.brake(1.8f);
                    }

                    if (i_LeftTurn)
                    {
                        TurnLeft();
                    }
                    if (i_RightTurn)
                    {
                        TurnRight();
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

                    m_ContTimeStep.Reset();
                }

                // flip timer
                m_flip_timer.Update();
                if (m_flip_timer.Elapsed > m_flip_time && i_Flip && m_is_upside_down)
                {
                    Vector3 old_pos = m_vehicle_physics.body.centerPosition;
                    old_pos.Y += 3;


                    m_vehicle_physics.body.orientationMatrix = Matrix.Identity;
                    m_vehicle_physics.body.centerPosition = old_pos;

                    m_flip_timer.Reset();
                }

                // weapon speed timer
                m_weapon_timer.Update();
                if (m_weapon_timer.Elapsed > m_weapon_rate && i_Fire)
                {

                    shoot_projectile(calculate_bullet_pos(), m_muzzle_matrix.Forward);
                    m_weapon_timer.Reset();
                }
            }
            else
            {
                // Slow down when dead?
                Brake();
            }
        }

        void SetUpPhysics()
        {
            Model wheelModel = g.content.Load<Model>("models\\carWheel");
            Texture2D wheelTexture = g.content.Load<Texture2D>("textures\\wheel");

            CompoundBody body = new CompoundBody();
            //body
            body.addBody(new Box(new Vector3(0, 0, 0), 2.8f, 1.5f, 5.0f, 270));
            //turret
            body.addBody(new Box(new Vector3(0, 1.0f, -0.4f), 2.0f, 0.9f, 2.0f, 20));


            body.moveTo(new Vector3(0.0f, 20.0f, 0.0f)); //At first, just keep it out of the way.
            m_vehicle_physics = new BEPUphysics.Vehicle(body, 20, 20);
            //Increase the inertia parallel to the car to prevent rolling a little.
            Matrix inertiaTensorInverse = m_vehicle_physics.body.localSpaceInertiaTensorInverse;
            inertiaTensorInverse.M31 *= .5f;
            inertiaTensorInverse.M32 *= .5f;
            inertiaTensorInverse.M33 *= .5f;
            m_vehicle_physics.body.localSpaceInertiaTensorInverse = inertiaTensorInverse;
            m_vehicle_physics.body.collisionMargin = .1f;

            //adjust centre of gravity for less tipping
            m_vehicle_physics.body.centerOfMass = new Vector3(0, 3, 0);

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
            const float wheelWidth = 0.4f;
            const float wheelYPos = -0.8f;
            const float frontWheelsPos = 2.6f;
            const float backWheelsPos = -2.4f;

            const float suspensionLen = 0.2f;       //1f
            const float suspensionConst = 20;        //7
            const float suspensionDamp = 0.8f;      //0.8f
            const float tireGrip = 1.5f;            //0.5f
            const float slidingFritction = 20.8f;   //4.8f
            const float rollFriction = 0.02f;        //0.02f
            const float maxSusForce = 1500;         //1500

            const float wheelLeft = 1.8f;
            const float wheelRight = -wheelLeft;

            //front right wheel
            Entity wheelShape = new Cylinder(Vector3.Zero, wheelWidth, wheelRadius);
            wheelShape.collisionMargin = 0;
            m_vehicle_physics.addWheel(new ConvexCastWheel(new Vector3(wheelRight, wheelYPos, frontWheelsPos), initialWheelRotation,
                wheelShape, wheelRadius, new Vector3(0, -1, 0), new Vector3(0, 0, 1), suspensionLen, suspensionConst, slidingFritction, rollFriction, tireGrip, maxSusForce, suspensionDamp));

            //front left wheel:
            wheelShape = new Cylinder(Vector3.Zero, wheelWidth, wheelRadius);
            wheelShape.collisionMargin = 0;
            m_vehicle_physics.addWheel(new ConvexCastWheel(new Vector3(wheelLeft, wheelYPos, frontWheelsPos), initialWheelRotation,
                wheelShape, wheelRadius, new Vector3(0, -1, 0), new Vector3(0, 0, 1), suspensionLen, suspensionConst, slidingFritction, rollFriction, tireGrip, maxSusForce, suspensionDamp));

            //mid right wheel:
            wheelShape = new Cylinder(Vector3.Zero, wheelWidth, wheelRadius);
            wheelShape.collisionMargin = 0;
            m_vehicle_physics.addWheel(new ConvexCastWheel(new Vector3(wheelRight, wheelYPos, 0), initialWheelRotation,
                wheelShape, wheelRadius, new Vector3(0, -1, 0), new Vector3(0, 0, 1), suspensionLen, suspensionConst, slidingFritction, rollFriction, tireGrip, maxSusForce, suspensionDamp));

            //mid left wheel:
            wheelShape = new Cylinder(Vector3.Zero, wheelWidth, wheelRadius);
            wheelShape.collisionMargin = 0;
            m_vehicle_physics.addWheel(new ConvexCastWheel(new Vector3(wheelLeft, wheelYPos, 0), initialWheelRotation,
                wheelShape, wheelRadius, new Vector3(0, -1, 0), new Vector3(0, 0, 1), suspensionLen, suspensionConst, slidingFritction, rollFriction, tireGrip, maxSusForce, suspensionDamp));


            //back right wheel:
            wheelShape = new Cylinder(Vector3.Zero, wheelWidth, wheelRadius);
            wheelShape.collisionMargin = 0;
            m_vehicle_physics.addWheel(new ConvexCastWheel(new Vector3(wheelRight, wheelYPos, backWheelsPos), initialWheelRotation,
                wheelShape, wheelRadius, new Vector3(0, -1, 0), new Vector3(0, 0, 1), suspensionLen, suspensionConst, slidingFritction, rollFriction, tireGrip, maxSusForce, suspensionDamp));

            //back left wheel:
            wheelShape = new Cylinder(Vector3.Zero, wheelWidth, wheelRadius);
            wheelShape.collisionMargin = 0;
            m_vehicle_physics.addWheel(new ConvexCastWheel(new Vector3(wheelLeft, wheelYPos, backWheelsPos), initialWheelRotation,
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
            for (int k = 0; k < m_vehicle_physics.wheels.Count; k++)
            {
                m_vehicle_physics.wheels[k].detectorShape.tag = "noDisplayObject";
                model = new DisplayModel(wheelModel);
                m_entityDrawer.displayModels.Add(model);
                m_wheelModels.Add(model);
                model.setTexture(wheelTexture);
            }
        }

        public override void TurnLeft()
        {
            m_vehicle_physics.body.orientationMatrix *= Matrix.CreateRotationY(MathHelper.ToRadians(1.5f));
        }
        public override void TurnRight()
        {
            m_vehicle_physics.body.orientationMatrix *= Matrix.CreateRotationY(MathHelper.ToRadians(-1.5f));
        }

        public virtual void RotateTurretLeft()
        {
            m_turret_rotation.Y += 3f;
        }
        public virtual void RotateTurretLRight()
        {
            m_turret_rotation.Y -= 3f;
        }

        public virtual void RotateTurretAnalogue()
        {
            m_turret_rotation.Y -= g.mouse.m_relative_pos.X * 1.5f;
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
                //Console.WriteLine("Bullet fired, we are the server so telling clients");
                g.server.SendToAllClients(PacketFams.EMovementData, PacketSubTypes.EMovementSpawnProjectile, pkg.PackData() );  
            }
            else
            {
                // Ask the server to fire a bullet for us
                //Console.WriteLine("Bullet fired, we are the client so asking the server to spawn it");
                g.clientData.client.SendMessage(PacketFams.EMovementData, PacketSubTypes.EMovementRequestFireProjectile, pkg.PackData() );
            }
        }
    }
}
