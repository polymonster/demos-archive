///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// F1-F12           Change the attribute you are adjusting
// Num0-Num9        Change the wheel you are adjusting(provided that wheel exists)
// Z                Turns the Debug mode on/off
// Up/Down Arrow    Adjusts the currently selected attribute(on currently selected wheel)
// PrtScr           Saves the setting to a .xml file called "Saved.xml" in the exe directory
// Left Alt         Loads Saved.xml From exe dir    (BROKEN ATM)
// Right Ctrl       Resets the tank position
// Left Ctrl        Resets the tank position and sets it upside Down(ish)
// M                Toggles the Adjust All/Adjust Selected Wheel (Default is All)
//
//
//  Once you have finished adjusting settings you can save rename the file to Tank.xml 
//  and overwrite the one in Content\VehicleSettings
//  or alternatively you can call it whatever you want and add it to teh project
//  and just change the target in the LoadFromFile() Method in PhysicsLoad() in the vehicle class
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;
using System.Xml.Serialization;
using System.IO;
/*
namespace Tanky
{
    public enum DebugState
    {
        AdditionalWeight = 0,
        WheelRadius = 1,
        WheelWidth = 2,
        WheelDistance = 3,
        SuspensionLength = 4,
        SuspensionConstant = 5,
        SlidingFriction = 6,
        RollFriction = 7,
        Grip = 8,
        MaxSusForce = 9,
        SusDamp = 10,
        CentreOfGravityZ = 11
    }

    public class PhysicsTweak
    {

        /// <summary>
        /// Turn Debug On and Off
        /// </summary>
        public bool DebugOn { get; private set; }

        public bool AdjustAll { get; private set; }

        /// <summary>
        /// Font to be used to display text
        /// </summary>
        public SpriteFont Font { get; private set; }

        public SpriteBatch Sprite { get; private set; }

        public Vehicle MyVehicle { get; private set; }

        public DebugState State { get; private set; }

        public int SelectedWheel { get; private set; }

        public string SelectedAttribute { get; private set; }

        public VehicleSettings Settings { get; private set; }

        private Timer MyTimer;
        private Timer ChangeTimer;
        private Timer CamTimer;


        private Camera OldCam;
        private Camera TweakCam;


        private BasicEffect BoxDraw;
        private List<VertexPositionColor> BoxVertices;
        VertexDeclaration BoxDrawDeclaration;



        public PhysicsTweak(SpriteBatch theSprite, SpriteFont theFont, Vehicle theVehicle)
        {
            DebugOn = true;
            AdjustAll = true;
            Font = theFont;
            MyVehicle = theVehicle;
            State = DebugState.AdditionalWeight;
            Sprite = theSprite;
            Settings = MyVehicle.m_vehicle_settings;

            SelectedAttribute = "Additional Weight";
            SelectedWheel = 0;

            MyTimer = new Timer();
            ChangeTimer = new Timer();
            CamTimer = new Timer();

            OldCam = g.camera;
            TweakCam = g.camera;
            if(DebugOn)
            {
                OldCam = g.camera;
            }

            BoxDraw = new BasicEffect(g.graphics.GraphicsDevice, null);
            BoxDrawDeclaration = new VertexDeclaration(g.graphics.GraphicsDevice, VertexPositionColor.VertexElements);
        }
        public void Control()
        {

            MyTimer.Update();
            if (MyTimer.Elapsed > 100)
            {
                #region Get Input
                bool F1 = Keyboard.GetState().IsKeyDown(Keys.F1);
                bool F2 = Keyboard.GetState().IsKeyDown(Keys.F2);
                bool F3 = Keyboard.GetState().IsKeyDown(Keys.F3);
                bool F4 = Keyboard.GetState().IsKeyDown(Keys.F4);
                bool F5 = Keyboard.GetState().IsKeyDown(Keys.F5);
                bool F6 = Keyboard.GetState().IsKeyDown(Keys.F6);
                bool F7 = Keyboard.GetState().IsKeyDown(Keys.F7);
                bool F8 = Keyboard.GetState().IsKeyDown(Keys.F8);
                bool F9 = Keyboard.GetState().IsKeyDown(Keys.F9);
                bool F10 = Keyboard.GetState().IsKeyDown(Keys.F10);
                bool F11 = Keyboard.GetState().IsKeyDown(Keys.F11);
                bool F12 = Keyboard.GetState().IsKeyDown(Keys.F12);

                bool Num0 = Keyboard.GetState().IsKeyDown(Keys.NumPad0);
                bool Num1 = Keyboard.GetState().IsKeyDown(Keys.NumPad1);
                bool Num2 = Keyboard.GetState().IsKeyDown(Keys.NumPad2);
                bool Num3 = Keyboard.GetState().IsKeyDown(Keys.NumPad3);
                bool Num4 = Keyboard.GetState().IsKeyDown(Keys.NumPad4);
                bool Num5 = Keyboard.GetState().IsKeyDown(Keys.NumPad5);
                bool Num6 = Keyboard.GetState().IsKeyDown(Keys.NumPad6);
                bool Num7 = Keyboard.GetState().IsKeyDown(Keys.NumPad7);
                bool Num8 = Keyboard.GetState().IsKeyDown(Keys.NumPad8);
                bool Num9 = Keyboard.GetState().IsKeyDown(Keys.NumPad9);

                bool Save = Keyboard.GetState().IsKeyDown(Keys.PrintScreen);
                bool Load = Keyboard.GetState().IsKeyDown(Keys.LeftAlt);
                bool Reset = Keyboard.GetState().IsKeyDown(Keys.RightControl);
                bool Reset_Upside = Keyboard.GetState().IsKeyDown(Keys.LeftControl);

                bool DebugToggle = Keyboard.GetState().IsKeyDown(Keys.Z);
                bool AdjustAllToggle = Keyboard.GetState().IsKeyDown(Keys.M);

                #endregion

                #region State Change
                if (F1)
                {
                    State = DebugState.AdditionalWeight;
                    SelectedAttribute = "Additional Weight";
                }
                else if (F2)
                {
                    State = DebugState.CentreOfGravityZ;
                    SelectedAttribute = "Centre of Gravity";
                }
                else if (F3)
                {
                    State = DebugState.Grip;
                    SelectedAttribute = "Grip";
                }
                else if (F4)
                {
                    State = DebugState.MaxSusForce;
                    SelectedAttribute = "Max Sus Force";
                }
                else if (F5)
                {
                    State = DebugState.RollFriction;
                    SelectedAttribute = "Roll Friction";
                }
                else if (F6)
                {
                    State = DebugState.SlidingFriction;
                    SelectedAttribute = "Sliding Friction";
                }
                else if (F7)
                {
                    State = DebugState.SusDamp;
                    SelectedAttribute = "Sus Damp";
                }
                else if (F8)
                {
                    //State = DebugState.SuspensionConstant;
                    SelectedAttribute = "Additional Weight";
                }
                else if (F9)
                {
                    State = DebugState.SuspensionLength;
                    SelectedAttribute = "Sus Length";
                }
                else if (F10)
                {
                    //State = DebugState.WheelDistance;
                    SelectedAttribute = "Additional Weight";
                }
                else if (F11)
                {
                    State = DebugState.WheelRadius;
                    SelectedAttribute = "Wheel Radius";
                }
                else if (F12)
                {
                    //State = DebugState.WheelWidth;
                    SelectedAttribute = "Additional Weight";
                }
                #endregion
                #region Save/Load
                else if (Save)
                {
                    SaveSettings("Saved.xml");
                }
                else if (Load)
                {
                    //LoadSettings("c:\\TestSettings.xml");
                }

                #endregion
                #region Reset
                else if (Reset)
                {
                    MyVehicle.moveTo(new Vector3(0, 150, 0));
                    MyVehicle.m_physicsVehicle.body.linearVelocity = Vector3.Zero;
                    MyVehicle.m_physicsVehicle.body.angularVelocity = Vector3.Zero;
                    MyVehicle.m_physicsVehicle.body.orientationQuaternion = Quaternion.Identity;
                }
                else if (Reset_Upside)
                {
                    MyVehicle.moveTo(new Vector3(0, 150, 0));
                    MyVehicle.m_physicsVehicle.body.linearVelocity = Vector3.Zero;
                    MyVehicle.m_physicsVehicle.body.angularVelocity = Vector3.Zero;
                    MyVehicle.m_physicsVehicle.body.orientationQuaternion = Quaternion.CreateFromYawPitchRoll(0, 0, 180);
                }
                #endregion
                #region WheelChange
                if (Num0)
                {
                    SelectedWheel = 0;
                }
                else if (Num1)
                {
                    SelectedWheel = 1;
                }
                else if (Num2)
                {
                    SelectedWheel = 2;
                }
                else if (Num3)
                {
                    SelectedWheel = 3;
                }
                else if (Num4)
                {
                    SelectedWheel = 4;
                }
                else if (Num5)
                {
                    SelectedWheel = 5;
                }
                else if (Num6)
                {
                    SelectedWheel = 6;
                }
                else if (Num7)
                {
                    SelectedWheel = 7;
                }
                else if (Num8)
                {
                    SelectedWheel = 8;
                }
                else if (Num9)
                {
                    SelectedWheel = 9;
                }


                #endregion

                #region Debug Toggle
                if (DebugToggle)
                {
                    DebugOn = !DebugOn;
                    if (DebugOn)
                    {
                        OldCam = g.camera;
                        g.camera = TweakCam;
                    }
                    else
                    {

                        g.camera = OldCam;
                        TweakCam = g.camera;
                    }
                }
                if (AdjustAllToggle)
                {
                    AdjustAll = !AdjustAll;
                }
                #endregion
                #region Exit
                if (Keyboard.GetState().IsKeyDown(Keys.Escape))
                {
                    g.GameRef.Exit();
                }
                #endregion
                MyTimer.Reset();
            }
            #region Camera Control
            CamTimer.Update();
            if (CamTimer.Elapsed > 30)
            {
                if (DebugOn)
                {
                    bool CamF = Keyboard.GetState().IsKeyDown(Keys.I);
                    bool CamB = Keyboard.GetState().IsKeyDown(Keys.K);

                    bool CamL = Keyboard.GetState().IsKeyDown(Keys.J);
                    bool CamR = Keyboard.GetState().IsKeyDown(Keys.L);

                    bool Cam1 = Keyboard.GetState().IsKeyDown(Keys.Y);
                    bool Cam2 = Keyboard.GetState().IsKeyDown(Keys.H);

                    Vector2 Centre = new Vector2(g.GameRef.Window.ClientBounds.Center.X, g.GameRef.Window.ClientBounds.Center.Y);
                    Vector2 MousePos = new Vector2(Mouse.GetState().X, Mouse.GetState().Y);


                    Vector2 Diff = MousePos - Centre;

                    g.camera.m_fixed_rotation.Y += Diff.X;
                    g.camera.m_fixed_rotation.X += Diff.Y;

                    #region Cap to 360
                    if (g.camera.m_fixed_rotation.Y > 360)
                    {
                        g.camera.m_fixed_rotation.Y = g.camera.m_fixed_rotation.Y % 360;
                    }
                    if (g.camera.m_fixed_rotation.Y < -360)
                    {
                        g.camera.m_fixed_rotation.Y = -(-g.camera.m_fixed_rotation.Y % 360);
                    }
                    if (g.camera.m_fixed_rotation.X > 360)
                    {
                        g.camera.m_fixed_rotation.X = g.camera.m_fixed_rotation.X % 360;
                    }
                    if (g.camera.m_fixed_rotation.X < -360)
                    {
                        g.camera.m_fixed_rotation.X = -(-g.camera.m_fixed_rotation.X % 360);
                    }
                    #endregion



                    Vector3 Forward = Vector3.Normalize(new Vector3((float)Math.Sin(-g.camera.m_fixed_rotation.Y), (float)Math.Sin(g.camera.m_fixed_rotation.X), (float)Math.Cos(-g.camera.m_fixed_rotation.Y)));
                    Vector3 Left = Vector3.Normalize(new Vector3((float)Math.Cos(g.camera.m_fixed_rotation.Y), 0f, (float)Math.Sin(g.camera.m_fixed_rotation.Y)));

                    float MoveSpeed = 10;

                    if (CamF)
                    {
                        g.camera.m_position.X += MoveSpeed;
                    }
                    else if (CamB)
                    {
                        g.camera.m_position.X -= MoveSpeed;
                    }
                    else if (CamL)
                    {
                        g.camera.m_position.Y += MoveSpeed;
                    }
                    else if (CamR)
                    {
                        g.camera.m_position.Y -= MoveSpeed;
                    }
                    else if (Cam1)
                    {
                        g.camera.m_position.Z += MoveSpeed;
                    }
                    else if (Cam2)
                    {
                        g.camera.m_position.Z -= MoveSpeed;
                    }

                    g.camera.apply_transformation();
                    CamTimer.Reset();
                }
            }

            //if active window move mouse to centre
            if (g.GameRef.IsActive)
            {
                Vector2 Centre = new Vector2(g.GameRef.Window.ClientBounds.Center.X, g.GameRef.Window.ClientBounds.Center.Y);
                Mouse.SetPosition((int)Centre.X, (int)Centre.Y);
            }
            #endregion
        }
        public void Update()
        {
            #region Change Settings

            Control();

            if (DebugOn)
            {
                bool Up;
                bool Down;
                ChangeTimer.Update();
                if (ChangeTimer.Elapsed > 100)
                {
                    Up = Keyboard.GetState().IsKeyDown(Keys.Up);
                    Down = Keyboard.GetState().IsKeyDown(Keys.Down);
                    ChangeTimer.Reset();

                    #region Change Attribute
                    switch (State)
                    {
                        case DebugState.AdditionalWeight:
                            {
                                float AdjustValue = 15f;
                                if (Up)
                                {
                                    MyVehicle.m_physicsVehicle.body.mass += AdjustValue;
                                    Settings.AdditionalWeight += AdjustValue;
                                }
                                else if (Down)
                                {
                                    MyVehicle.m_physicsVehicle.body.mass -= AdjustValue;
                                    Settings.AdditionalWeight -= AdjustValue;
                                }
                                break;
                            }
                        case DebugState.CentreOfGravityZ:
                            {
                                float AdjustValue = 1f;

                                Vector3 temp = MyVehicle.m_physicsVehicle.body.centerOfMass;

                                if (Up)
                                {
                                    MyVehicle.m_physicsVehicle.body.centerOfMass = new Vector3(temp.X, temp.Y, temp.Z + AdjustValue);
                                    Settings.CentreOfGravity.Z += AdjustValue;
                                }
                                else if (Down)
                                {
                                    MyVehicle.m_physicsVehicle.body.centerOfMass = new Vector3(temp.X, temp.Y, temp.Z - AdjustValue);
                                    Settings.CentreOfGravity.Z -= AdjustValue;
                                }
                                break;
                            }
                        case DebugState.Grip:
                            {
                                float AdjustValue = 10f;

                                if (AdjustAll)
                                {
                                    if (Up)
                                    {
                                        foreach (BEPUphysics.Wheel w in MyVehicle.m_physicsVehicle.wheels)
                                        {
                                            w.tireGrip += AdjustValue;
                                        }
                                        foreach (WheelSettings w in Settings.WheelList)
                                        {
                                            w.Grip += AdjustValue;
                                        }
                                    }
                                    else if (Down)
                                    {
                                        foreach (BEPUphysics.Wheel w in MyVehicle.m_physicsVehicle.wheels)
                                        {
                                            w.tireGrip -= AdjustValue;
                                        }
                                        foreach (WheelSettings w in Settings.WheelList)
                                        {
                                            w.Grip -= AdjustValue;
                                        }
                                    }
                                }
                                else
                                {
                                    if (SelectedWheel < MyVehicle.m_physicsVehicle.wheels.Count)
                                    {
                                        if (Up)
                                        {
                                            MyVehicle.m_physicsVehicle.wheels[SelectedWheel].tireGrip += AdjustValue;
                                            Settings.WheelList[SelectedWheel].Grip += AdjustValue;
                                        }
                                        else if (Down)
                                        {
                                            MyVehicle.m_physicsVehicle.wheels[SelectedWheel].tireGrip -= AdjustValue;
                                            Settings.WheelList[SelectedWheel].Grip -= AdjustValue;
                                        }
                                    }
                                }

                                break;
                            }
                        case DebugState.MaxSusForce:
                            {
                                float AdjustValue = 100f;


                                if (AdjustAll)
                                {
                                    if (Up)
                                    {
                                        foreach (BEPUphysics.Wheel w in MyVehicle.m_physicsVehicle.wheels)
                                        {
                                            w.maximumSuspensionForce += AdjustValue;
                                        }
                                        foreach (WheelSettings w in Settings.WheelList)
                                        {
                                            w.MaxSuspensionForce += AdjustValue;
                                        }
                                    }
                                    else if (Down)
                                    {
                                        foreach (BEPUphysics.Wheel w in MyVehicle.m_physicsVehicle.wheels)
                                        {
                                            w.maximumSuspensionForce -= AdjustValue;
                                        }
                                        foreach (WheelSettings w in Settings.WheelList)
                                        {
                                            w.MaxSuspensionForce -= AdjustValue;
                                        }
                                    }
                                }
                                else
                                {
                                    if (SelectedWheel < MyVehicle.m_physicsVehicle.wheels.Count)
                                    {
                                        if (Up)
                                        {
                                            MyVehicle.m_physicsVehicle.wheels[SelectedWheel].maximumSuspensionForce += AdjustValue;
                                            Settings.WheelList[SelectedWheel].MaxSuspensionForce += AdjustValue;
                                        }
                                        else if (Down)
                                        {
                                            MyVehicle.m_physicsVehicle.wheels[SelectedWheel].maximumSuspensionForce -= AdjustValue;
                                            Settings.WheelList[SelectedWheel].MaxSuspensionForce -= AdjustValue;
                                        }
                                    }
                                }

                                break;
                            }
                        case DebugState.RollFriction:
                            {
                                float AdjustValue = 10f;


                                if (AdjustAll)
                                {
                                    if (Up)
                                    {
                                        foreach (BEPUphysics.Wheel w in MyVehicle.m_physicsVehicle.wheels)
                                        {
                                            w.rollingFriction += AdjustValue;
                                        }
                                        foreach (WheelSettings w in Settings.WheelList)
                                        {
                                            w.RollFriction += AdjustValue;
                                        }
                                    }
                                    else if (Down)
                                    {
                                        foreach (BEPUphysics.Wheel w in MyVehicle.m_physicsVehicle.wheels)
                                        {
                                            w.rollingFriction -= AdjustValue;
                                        }
                                        foreach (WheelSettings w in Settings.WheelList)
                                        {
                                            w.RollFriction -= AdjustValue;
                                        }
                                    }
                                }
                                else
                                {
                                    if (SelectedWheel < MyVehicle.m_physicsVehicle.wheels.Count)
                                    {
                                        if (Up)
                                        {
                                            MyVehicle.m_physicsVehicle.wheels[SelectedWheel].rollingFriction += AdjustValue;
                                            Settings.WheelList[SelectedWheel].RollFriction += AdjustValue;
                                        }
                                        else if (Down)
                                        {
                                            MyVehicle.m_physicsVehicle.wheels[SelectedWheel].rollingFriction -= AdjustValue;
                                            Settings.WheelList[SelectedWheel].RollFriction -= AdjustValue;
                                        }
                                    }
                                }

                                break;
                            }
                        case DebugState.SlidingFriction:
                            {
                                float AdjustValue = 10f;


                                if (AdjustAll)
                                {
                                    if (Up)
                                    {
                                        foreach (BEPUphysics.Wheel w in MyVehicle.m_physicsVehicle.wheels)
                                        {
                                            w.slideFriction += AdjustValue;
                                        }
                                        foreach (WheelSettings w in Settings.WheelList)
                                        {
                                            w.SlidingFriction += AdjustValue;
                                        }
                                    }
                                    else if (Down)
                                    {
                                        foreach (BEPUphysics.Wheel w in MyVehicle.m_physicsVehicle.wheels)
                                        {
                                            w.slideFriction -= AdjustValue;
                                        }
                                        foreach (WheelSettings w in Settings.WheelList)
                                        {
                                            w.SlidingFriction -= AdjustValue;
                                        }
                                    }
                                }
                                else
                                {
                                    if (SelectedWheel < MyVehicle.m_physicsVehicle.wheels.Count)
                                    {
                                        if (Up)
                                        {
                                            MyVehicle.m_physicsVehicle.wheels[SelectedWheel].slideFriction += AdjustValue;
                                            Settings.WheelList[SelectedWheel].SlidingFriction += AdjustValue;
                                        }
                                        else if (Down)
                                        {
                                            MyVehicle.m_physicsVehicle.wheels[SelectedWheel].slideFriction -= AdjustValue;
                                            Settings.WheelList[SelectedWheel].SlidingFriction -= AdjustValue;
                                        }
                                    }
                                }

                                break;
                            }
                        case DebugState.SusDamp:
                            {
                                float AdjustValue = 2f;


                                if (AdjustAll)
                                {
                                    if (Up)
                                    {
                                        foreach (BEPUphysics.Wheel w in MyVehicle.m_physicsVehicle.wheels)
                                        {
                                            w.suspensionDamping += AdjustValue;
                                        }
                                        foreach (WheelSettings w in Settings.WheelList)
                                        {
                                            w.SuspensionDamp += AdjustValue;
                                        }
                                    }
                                    else if (Down)
                                    {
                                        foreach (BEPUphysics.Wheel w in MyVehicle.m_physicsVehicle.wheels)
                                        {
                                            w.suspensionDamping -= AdjustValue;
                                        }
                                        foreach (WheelSettings w in Settings.WheelList)
                                        {
                                            w.SuspensionDamp -= AdjustValue;
                                        }
                                    }
                                }
                                else
                                {
                                    if (SelectedWheel < MyVehicle.m_physicsVehicle.wheels.Count)
                                    {
                                        if (Up)
                                        {
                                            MyVehicle.m_physicsVehicle.wheels[SelectedWheel].suspensionDamping += AdjustValue;
                                            Settings.WheelList[SelectedWheel].SuspensionDamp += AdjustValue;
                                        }
                                        else if (Down)
                                        {
                                            MyVehicle.m_physicsVehicle.wheels[SelectedWheel].suspensionDamping -= AdjustValue;
                                            Settings.WheelList[SelectedWheel].SuspensionDamp -= AdjustValue;
                                        }
                                    }
                                }

                                break;
                            }
                        case DebugState.SuspensionConstant:
                            {
                                break;
                            }
                        case DebugState.SuspensionLength:
                            {
                                float AdjustValue = 1f;


                                if (AdjustAll)
                                {
                                    if (Up)
                                    {
                                        foreach (BEPUphysics.Wheel w in MyVehicle.m_physicsVehicle.wheels)
                                        {
                                            w.suspensionLength += AdjustValue;
                                        }
                                        foreach (WheelSettings w in Settings.WheelList)
                                        {
                                            w.SuspensionLen += AdjustValue;
                                        }
                                    }
                                    else if (Down)
                                    {
                                        foreach (BEPUphysics.Wheel w in MyVehicle.m_physicsVehicle.wheels)
                                        {
                                            w.suspensionLength -= AdjustValue;
                                        }
                                        foreach (WheelSettings w in Settings.WheelList)
                                        {
                                            w.SuspensionLen -= AdjustValue;
                                        }
                                    }
                                }
                                else
                                {
                                    if (SelectedWheel < MyVehicle.m_physicsVehicle.wheels.Count)
                                    {
                                        if (Up)
                                        {
                                            MyVehicle.m_physicsVehicle.wheels[SelectedWheel].suspensionLength += AdjustValue;
                                            Settings.WheelList[SelectedWheel].SuspensionLen += AdjustValue;
                                        }
                                        else if (Down)
                                        {
                                            MyVehicle.m_physicsVehicle.wheels[SelectedWheel].suspensionLength -= AdjustValue;
                                            Settings.WheelList[SelectedWheel].SuspensionLen -= AdjustValue;
                                        }
                                    }
                                }

                                break;
                            }
                        case DebugState.WheelDistance:
                            {

                                break;
                            }
                        case DebugState.WheelRadius:
                            {
                                float AdjustValue = 1f;


                                if (AdjustAll)
                                {
                                    if (Up)
                                    {
                                        foreach (BEPUphysics.Wheel w in MyVehicle.m_physicsVehicle.wheels)
                                        {
                                            w.radius += AdjustValue;
                                        }
                                        foreach (WheelSettings w in Settings.WheelList)
                                        {
                                            w.Radius += AdjustValue;
                                        }
                                    }
                                    else if (Down)
                                    {
                                        foreach (BEPUphysics.Wheel w in MyVehicle.m_physicsVehicle.wheels)
                                        {
                                            w.radius -= AdjustValue;
                                        }
                                        foreach (WheelSettings w in Settings.WheelList)
                                        {
                                            w.Radius -= AdjustValue;
                                        }
                                    }
                                }
                                else
                                {
                                    if (SelectedWheel < MyVehicle.m_physicsVehicle.wheels.Count)
                                    {
                                        if (Up)
                                        {
                                            MyVehicle.m_physicsVehicle.wheels[SelectedWheel].radius += AdjustValue;
                                            Settings.WheelList[SelectedWheel].Radius += AdjustValue;
                                        }
                                        else if (Down)
                                        {
                                            MyVehicle.m_physicsVehicle.wheels[SelectedWheel].radius -= AdjustValue;
                                            Settings.WheelList[SelectedWheel].Radius -= AdjustValue;
                                        }
                                    }
                                }

                                break;
                            }
                        case DebugState.WheelWidth:
                            {
                                break;
                            }
                    }
                    #endregion

                }
            }
            #endregion
        }
        public void DrawText(Vector2 thePos, string theText)
        {
            Vector2 centerOfString = Font.MeasureString(theText) / 2;
            Vector2 Pos = thePos;// - centerOfString;
            Sprite.DrawString(Font, theText, Pos, Color.White);
        }
        public void Draw()
        {
            #region Text
            float Spacing = 60;
            if (DebugOn)
            {
                Sprite.Begin();

                DrawText(new Vector2(10, 10), "Additional Weight: ");
                DrawText(new Vector2(200, 10), Settings.AdditionalWeight.ToString());

                DrawText(new Vector2(10, 30), "Centre of Gravity (Z): ");
                DrawText(new Vector2(200, 30), Settings.CentreOfGravity.Z.ToString());


                DrawText(new Vector2(10, 50), "Grip: ");
                for (int i = 0; i < Settings.WheelList.Count; i++)
                {
                    DrawText(new Vector2(200 + (Spacing * i), 50), ":" + Settings.WheelList[i].Grip.ToString());
                }

                DrawText(new Vector2(10, 70), "MaxSuspensionForce: ");
                for (int i = 0; i < Settings.WheelList.Count; i++)
                {
                    DrawText(new Vector2(200 + (Spacing * i), 70), ":" + Settings.WheelList[i].MaxSuspensionForce.ToString());
                }

                DrawText(new Vector2(10, 90), "RollFriction: ");
                for (int i = 0; i < Settings.WheelList.Count; i++)
                {
                    DrawText(new Vector2(200 + (Spacing * i), 90), ":" + Settings.WheelList[i].RollFriction.ToString());
                }

                DrawText(new Vector2(10, 110), "SlidingFriction: ");
                for (int i = 0; i < Settings.WheelList.Count; i++)
                {
                    DrawText(new Vector2(200 + (Spacing * i), 110), ":" + Settings.WheelList[i].SlidingFriction.ToString());
                }

                DrawText(new Vector2(10, 130), "SuspensionConstant: ");
                for (int i = 0; i < Settings.WheelList.Count; i++)
                {
                    DrawText(new Vector2(200 + (Spacing * i), 130), ":" + Settings.WheelList[i].SuspensionConstant.ToString());
                }

                DrawText(new Vector2(10, 150), "SuspensionDamp: ");
                for (int i = 0; i < Settings.WheelList.Count; i++)
                {
                    DrawText(new Vector2(200 + (Spacing * i), 150), ":" + Settings.WheelList[i].SuspensionDamp.ToString());
                }

                DrawText(new Vector2(10, 170), "SuspensionLen: ");
                for (int i = 0; i < Settings.WheelList.Count; i++)
                {
                    DrawText(new Vector2(200 + (Spacing * i), 170), ":" + Settings.WheelList[i].SuspensionLen.ToString());
                }

                DrawText(new Vector2(10, 190), "WheelRadius: ");
                for (int i = 0; i < Settings.WheelList.Count; i++)
                {
                    DrawText(new Vector2(200 + (Spacing * i), 190), ":" + Settings.WheelList[i].Radius.ToString());
                }

                DrawText(new Vector2(10, 210), "Current Attribute:  ");
                DrawText(new Vector2(200, 210), SelectedAttribute);

                DrawText(new Vector2(10, 230), "Selected Wheel:  ");
                DrawText(new Vector2(200, 230), SelectedWheel.ToString());

                DrawText(new Vector2(10, 250), "Adjust All:  ");
                DrawText(new Vector2(200, 250), AdjustAll.ToString());

                Sprite.End();

                g.graphics.GraphicsDevice.RenderState.DepthBufferEnable = true;
                g.graphics.GraphicsDevice.RenderState.AlphaBlendEnable = false;
                g.graphics.GraphicsDevice.RenderState.AlphaTestEnable = false;
            }
            #endregion
            
            #region Box 
            if (DebugOn)
            {
                BoxVertices.Clear();
                foreach (BEPUphysics.Wheel w in MyVehicle.m_physicsVehicle.wheels)
                {

                    Vector2 Min = new Vector2(w.acceleration]]]] -)
                    Vector2 Max;

                    BoxVertices.Add()
                }
                BoxDraw.LightingEnabled = false;
                BoxDraw.VertexColorEnabled = true;
                BoxDraw.World = Matrix.Identity;
                BoxDraw.View = g.camera.m_view_matrix;
                BoxDraw.Projection = g.camera.m_projection_matrix;




                BoxDraw.Begin();
                foreach (EffectPass pass in BoxDraw.CurrentTechnique.Passes)
                {
                    pass.Begin();

                    g.graphics.GraphicsDevice.DrawUserPrimitives(PrimitiveType.LineList, boundingBoxLines.ToArray(), 0, space.entities.Count * 12);

                    pass.End();
                }
                BoxDraw.End();
            }




            #endregion

        }

        public void ChangeVehicle(Vehicle theVehicle)
        {
            if (theVehicle != null)
            {
                MyVehicle = theVehicle;
            }
        }

        public void SaveSettings(string theFilePath)
        {
            Settings.SavetoFile(theFilePath);
        }

        private void BoxDraw()
        {


        }
        }
    }
}
*/