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
    public class VehiclePhysics : Vehicle
    {
        public enum TPhysicsModels
        {
            EJeep,  // Use settings that simulate a jeep
            ETank,  // Use settings that simulate a tank
        };

        public Vector3 m_turretOffset;

        public BEPUphysics.Vehicle m_vehicle_physics;
        public Space m_space;
        public EntityDrawer m_entityDrawer;
        public Player m_Parent;

        //Movement accelerations
        protected float m_forwardAcceleration = 18;
        protected float m_backwardsAcceleration = -12;

        public float m_maxTurnAngles = (float)Math.PI / 5.5f;
        
        // Graphical models for the wheels (can't be scaled)
        public List<DisplayModel> m_wheelModels;        

        public UprightConstraint m_uprightConstraint;

        protected int m_weapon_damage;
        protected float m_weapon_speed;
        protected int m_weapon_rate;
        protected Timer m_weapon_timer;

        protected Timer m_flip_timer;
        protected float m_flip_time;
        protected bool m_is_upside_down;

        public VehiclePhysics()
        {
            m_ContTimeStep = new Timer();

            m_weapon_damage = 20;
            m_weapon_speed = 70f;
            m_weapon_rate = 500;
            m_weapon_timer = new Timer();

            m_flip_time = 10000;
            m_flip_timer = new Timer();
            m_is_upside_down = false;
        }

        public virtual void Accelerate()
        {
            m_vehicle_physics.releaseBrake();
            m_vehicle_physics.accelerate(m_forwardAcceleration);
        }
        public virtual void Reverse()
        {
            m_vehicle_physics.releaseBrake();
            m_vehicle_physics.accelerate(m_backwardsAcceleration);
        }
        public virtual void TurnLeft()
        {
            m_vehicle_physics.wheels[0].wheelFacingAngle = -m_maxTurnAngles;
            m_vehicle_physics.wheels[1].wheelFacingAngle = -m_maxTurnAngles;
        }
        public virtual void TurnRight()
        {
            m_vehicle_physics.wheels[0].wheelFacingAngle = m_maxTurnAngles;
            m_vehicle_physics.wheels[1].wheelFacingAngle = m_maxTurnAngles;
        }
        public virtual void Brake()
        {
            m_vehicle_physics.brake(1.5f);
        }

        protected EventHandlerInitialCollisionDetected OnCollisionFunc;
        protected EventHandlerCollisionEnded OnCollisionEnded;

        public virtual void Accelerate(float aValue)
        {
            m_vehicle_physics.releaseBrake();
            if (aValue <= m_forwardAcceleration && aValue >= 0)
            {
                m_vehicle_physics.accelerate(aValue);
            }
            else
            {
                m_vehicle_physics.accelerate(m_forwardAcceleration);
            }            
        }
        public virtual void Reverse(float aValue)
        {
            m_vehicle_physics.releaseBrake();
            if (aValue >= m_backwardsAcceleration && aValue <= 0)
            {
                m_vehicle_physics.accelerate(aValue);
            }
            else
            {
                m_vehicle_physics.accelerate(m_forwardAcceleration);
            }
        }
        public virtual void TurnLeft(float aValue)
        {
            if(aValue <= m_maxTurnAngles)
            {
                m_vehicle_physics.wheels[0].wheelFacingAngle = -aValue;
                m_vehicle_physics.wheels[1].wheelFacingAngle = -aValue;
            }
            else
            {
                m_vehicle_physics.wheels[0].wheelFacingAngle = -m_maxTurnAngles;
                m_vehicle_physics.wheels[1].wheelFacingAngle = -m_maxTurnAngles;
            }
        }
        public virtual void TurnRight(float aValue)
        {
            if (aValue <= m_maxTurnAngles)
            {
                m_vehicle_physics.wheels[0].wheelFacingAngle = aValue;
                m_vehicle_physics.wheels[1].wheelFacingAngle = aValue;
            }
            else
            {
                m_vehicle_physics.wheels[0].wheelFacingAngle = m_maxTurnAngles;
                m_vehicle_physics.wheels[1].wheelFacingAngle = m_maxTurnAngles;
            }
        }

        public virtual void Update(float dt)
        {
            
            for (int k = 0; k < m_wheelModels.Count; k++)
            {
                m_wheelModels[k].worldMatrix = m_vehicle_physics.wheels[k].worldMatrix;// Matrix.CreateFromAxisAngle(new Vector3(0, 0, 1), (float)Math.PI / 2) * vehicle.wheels[k].worldMatrix;
            }

            control();
           
        }

        public virtual VehiclePackage SnapShotOut()
        {
            VehiclePackage vReturn = new VehiclePackage();
            EntityPackage vOut = new EntityPackage();
            List<WheelPackage> wList = new List<WheelPackage>();

            vOut.correctiveAngularVelocity = m_vehicle_physics.body.correctiveAngularVelocity;
            vOut.correctiveLinearVelocity = m_vehicle_physics.body.correctiveLinearVelocity;
            vOut.angularMomentum = m_vehicle_physics.body.angularMomentum;
            vOut.angularVelocity = m_vehicle_physics.body.angularVelocity;
            vOut.centerPosition = m_vehicle_physics.body.centerPosition;
            vOut.internalAngularMomentum = m_vehicle_physics.body.internalAngularMomentum;
            vOut.internalAngularVelocity = m_vehicle_physics.body.internalAngularVelocity;
            vOut.internalCenterPosition = m_vehicle_physics.body.internalCenterPosition;
            vOut.internalLinearMomentum = m_vehicle_physics.body.internalLinearMomentum;
            vOut.internalLinearVelocity = m_vehicle_physics.body.internalLinearVelocity;
            vOut.internalOrientationMatrix = m_vehicle_physics.body.internalOrientationMatrix;
            vOut.internalOrientationQuaternion = m_vehicle_physics.body.internalOrientationQuaternion;
            vOut.linearMomentum = m_vehicle_physics.body.linearMomentum;
            vOut.linearVelocity = m_vehicle_physics.body.linearVelocity;
            vOut.localSpaceInertiaTensor = m_vehicle_physics.body.localSpaceInertiaTensor;
            vOut.localSpaceInertiaTensorInverse = m_vehicle_physics.body.localSpaceInertiaTensorInverse;
            vOut.orientationMatrix = m_vehicle_physics.body.orientationMatrix;
            vOut.orientationQuaternion = m_vehicle_physics.body.orientationQuaternion;

            foreach (Wheel w in m_vehicle_physics.wheels)
            {
                WheelPackage wheel = new WheelPackage();

                wheel.acceleration = w.acceleration;
                wheel.forward = w.forward;
                wheel.initialLocalRotation = w.initialLocalRotation;
                wheel.localRayDirection = w.localRayDirection;
                wheel.steeringRotation = w.steeringRotation;
                wheel.totalLocalRotation = w.totalLocalRotation;
                wheel.wheelFacingAngle = w.wheelFacingAngle;
                wList.Add(wheel);
            }

            vReturn.turretRotation = m_turret_rotation;
            vReturn.BodyPackage = vOut;
            vReturn.WheelsPackage = wList;

            return vReturn;
        }
        public virtual void SnapShotIn(VehiclePackage aPackage)
        {
            if (aPackage != null)
            {
                m_vehicle_physics.body.correctiveAngularVelocity = aPackage.BodyPackage.correctiveAngularVelocity;
                m_vehicle_physics.body.correctiveLinearVelocity = aPackage.BodyPackage.correctiveLinearVelocity;
                m_vehicle_physics.body.angularMomentum = aPackage.BodyPackage.internalAngularMomentum;
                m_vehicle_physics.body.angularVelocity = aPackage.BodyPackage.internalAngularVelocity;
                m_vehicle_physics.body.internalAngularMomentum = aPackage.BodyPackage.internalAngularMomentum;
                m_vehicle_physics.body.internalAngularVelocity = aPackage.BodyPackage.internalAngularVelocity;
                m_vehicle_physics.body.internalCenterPosition = aPackage.BodyPackage.internalCenterPosition;
                m_vehicle_physics.body.internalLinearMomentum = aPackage.BodyPackage.internalLinearMomentum;
                m_vehicle_physics.body.internalLinearVelocity = aPackage.BodyPackage.internalLinearVelocity;
                m_vehicle_physics.body.internalOrientationMatrix = aPackage.BodyPackage.internalOrientationMatrix;
                m_vehicle_physics.body.internalOrientationQuaternion = aPackage.BodyPackage.internalOrientationQuaternion;
                m_vehicle_physics.body.linearMomentum = aPackage.BodyPackage.internalLinearMomentum;
                m_vehicle_physics.body.linearVelocity = aPackage.BodyPackage.internalLinearVelocity;
                m_vehicle_physics.body.localSpaceInertiaTensor = aPackage.BodyPackage.localSpaceInertiaTensor;
                m_vehicle_physics.body.localSpaceInertiaTensorInverse = aPackage.BodyPackage.localSpaceInertiaTensorInverse;
                m_vehicle_physics.body.orientationMatrix = aPackage.BodyPackage.orientationMatrix;
                m_vehicle_physics.body.orientationQuaternion = aPackage.BodyPackage.orientationQuaternion;

                //Wheels
                for (int i = 0; i < aPackage.WheelsPackage.Count; i++)
                {
                    if (i < m_vehicle_physics.wheels.Count)
                    {
                        m_vehicle_physics.wheels[i].acceleration = aPackage.WheelsPackage[i].acceleration;
                        m_vehicle_physics.wheels[i].forward = aPackage.WheelsPackage[i].forward;
                        m_vehicle_physics.wheels[i].initialLocalRotation = aPackage.WheelsPackage[i].initialLocalRotation;
                        m_vehicle_physics.wheels[i].localRayDirection = aPackage.WheelsPackage[i].localRayDirection;
                        m_vehicle_physics.wheels[i].steeringRotation = aPackage.WheelsPackage[i].steeringRotation;
                        m_vehicle_physics.wheels[i].totalLocalRotation = aPackage.WheelsPackage[i].totalLocalRotation;
                        m_vehicle_physics.wheels[i].wheelFacingAngle = aPackage.WheelsPackage[i].wheelFacingAngle;
                    }
                }
                m_turret_rotation = aPackage.turretRotation;
            }

        }

    }

}
