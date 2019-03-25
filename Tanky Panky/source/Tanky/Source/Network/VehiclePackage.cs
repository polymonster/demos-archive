using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using BEPUphysics;
using Microsoft.Xna.Framework.Graphics.PackedVector;

namespace Tanky
{
    [Serializable]
    public class EntityPackage
    {
        public Vector3 correctiveAngularVelocity;

        public Vector3 correctiveLinearVelocity;

        public Vector3 angularMomentum { get; set; }

        public Vector3 angularVelocity { get; set; }

        public Vector3 centerPosition { get; set; }

        public Vector3 internalAngularMomentum { get; set; }

        public Vector3 internalAngularVelocity { get; set; }

        public Vector3 internalCenterPosition { get; set; }

        public Vector3 internalLinearMomentum { get; set; }

        public Vector3 internalLinearVelocity { get; set; }

        public Matrix internalOrientationMatrix { get; set; }

        public Quaternion internalOrientationQuaternion { get; set; }

        public Vector3 linearMomentum { get; set; }

        public Vector3 linearVelocity { get; set; }

        public Matrix localSpaceInertiaTensor { get; set; }

        public Matrix localSpaceInertiaTensorInverse { get; set; }

        public Matrix orientationMatrix { get; set; }

        public Quaternion orientationQuaternion { get; set; }

        public override string ToString()
        {
            char delim = '|';

            StringBuilder sb = new StringBuilder();

            sb.Append(correctiveAngularVelocity.ToString());
            sb.Append(delim);

            sb.Append(correctiveAngularVelocity.ToString());
            sb.Append(delim);

            sb.Append(correctiveLinearVelocity.ToString());
            sb.Append(delim);

            sb.Append(angularMomentum.ToString());
            sb.Append(delim);

            sb.Append(angularVelocity.ToString());
            sb.Append(delim);

            sb.Append(centerPosition.ToString());
            sb.Append(delim);

            sb.Append(internalAngularMomentum.ToString());
            sb.Append(delim);

            sb.Append(internalAngularVelocity.ToString());
            sb.Append(delim);

            sb.Append(internalCenterPosition.ToString());
            sb.Append(delim);

            sb.Append(internalLinearMomentum.ToString());
            sb.Append(delim);

            sb.Append(internalLinearVelocity.ToString());
            sb.Append(delim);

            sb.Append(internalOrientationMatrix.ToString());
            sb.Append(delim);

            sb.Append(internalOrientationQuaternion.ToString());
            sb.Append(delim);

            sb.Append(linearMomentum.ToString());
            sb.Append(delim);

            sb.Append(linearVelocity.ToString());
            sb.Append(delim);

            sb.Append(localSpaceInertiaTensor.ToString());
            sb.Append(delim);

            sb.Append(localSpaceInertiaTensorInverse.ToString());
            sb.Append(delim);

            sb.Append(orientationMatrix.ToString());
            sb.Append(delim);

            sb.Append(orientationQuaternion.ToString());

            return sb.ToString();

        }
        public static EntityPackage CreateFromString(string input)
        {
            EntityPackage returnPackage = new EntityPackage();

            string[] tempStringArray = input.Split('|');

            returnPackage.correctiveAngularVelocity = Tools.fromVec3String(tempStringArray[0]);
            returnPackage.correctiveAngularVelocity = Tools.fromVec3String(tempStringArray[1]);
            returnPackage.correctiveLinearVelocity = Tools.fromVec3String(tempStringArray[2]);
            returnPackage.angularMomentum = Tools.fromVec3String(tempStringArray[3]);
            returnPackage.angularVelocity = Tools.fromVec3String(tempStringArray[4]);
            returnPackage.centerPosition = Tools.fromVec3String(tempStringArray[5]);
            returnPackage.internalAngularMomentum = Tools.fromVec3String(tempStringArray[6]);
            returnPackage.internalAngularVelocity = Tools.fromVec3String(tempStringArray[7]);
            returnPackage.internalCenterPosition = Tools.fromVec3String(tempStringArray[8]);
            returnPackage.internalLinearMomentum = Tools.fromVec3String(tempStringArray[9]);
            returnPackage.internalLinearVelocity = Tools.fromVec3String(tempStringArray[10]);
            returnPackage.internalOrientationMatrix = Tools.fromMatString(tempStringArray[11]);
            returnPackage.internalOrientationQuaternion = Tools.fromQuatString(tempStringArray[12]);
            returnPackage.linearMomentum = Tools.fromVec3String(tempStringArray[13]);
            returnPackage.linearVelocity = Tools.fromVec3String(tempStringArray[14]);
            returnPackage.localSpaceInertiaTensor = Tools.fromMatString(tempStringArray[15]);
            returnPackage.localSpaceInertiaTensorInverse = Tools.fromMatString(tempStringArray[16]);
            returnPackage.orientationMatrix = Tools.fromMatString(tempStringArray[17]);
            returnPackage.orientationQuaternion = Tools.fromQuatString(tempStringArray[18]);

            return returnPackage;
        }
    }

    [Serializable]
    public class WheelPackage
    {
        // Summary:
        // Change in the velocity of the wheel per second.
        public float acceleration;
        
        public Vector3 forward;   
       
        public Matrix initialLocalRotation;
        
        public Vector3 localRayDirection;
        
        public Matrix steeringRotation;
        
        public Matrix totalLocalRotation;
        
        public float wheelFacingAngle { get; set; }

        public override string ToString()
        {
            char delim = '<';

            StringBuilder sb = new StringBuilder();

            sb.Append(acceleration.ToString());
            sb.Append(delim);

            sb.Append(forward.ToString());
            sb.Append(delim);

            sb.Append(initialLocalRotation.ToString());
            sb.Append(delim);

            sb.Append(localRayDirection.ToString());
            sb.Append(delim);

            sb.Append(steeringRotation.ToString());
            sb.Append(delim);

            sb.Append(totalLocalRotation.ToString());
            sb.Append(delim);

            sb.Append(wheelFacingAngle.ToString());
            sb.Append(delim);

            return sb.ToString();

        }
        public static WheelPackage CreateFromString(string input)
        {
            WheelPackage returnPackage = new WheelPackage();

            string[] tempStringArray = input.Split('<');

            returnPackage.acceleration = Convert.ToSingle(tempStringArray[0]);
            returnPackage.forward = Tools.fromVec3String(tempStringArray[1]);
            returnPackage.initialLocalRotation = Tools.fromMatString(tempStringArray[2]);
            returnPackage.localRayDirection = Tools.fromVec3String(tempStringArray[3]);
            returnPackage.steeringRotation = Tools.fromMatString(tempStringArray[4]);
            returnPackage.totalLocalRotation = Tools.fromMatString(tempStringArray[5]);
            returnPackage.wheelFacingAngle = Convert.ToSingle(tempStringArray[6]);

            return returnPackage;
        }

    }

    [Serializable]
    public class VehiclePackage
    {
        public EntityPackage BodyPackage;
        public List<WheelPackage> WheelsPackage;
        public Vector3 turretRotation;

        public override string ToString()
        {
            char delim = '~';

            StringBuilder sb = new StringBuilder();

            sb.Append(BodyPackage.ToString());
            sb.Append(delim);

            foreach (WheelPackage w in WheelsPackage)
            {
                sb.Append(w.ToString());
                sb.Append(delim);
            }

            sb.Append(turretRotation.ToString());

            return sb.ToString();
        }
        public static VehiclePackage CreateFromString(string input)
        {
            VehiclePackage returnPackage = new VehiclePackage();
            List<WheelPackage> returnWheels = new List<WheelPackage>();

            string[] tempString = input.Split('~');

            returnPackage.BodyPackage = EntityPackage.CreateFromString(tempString[0]);
            for (int i = 1; i < tempString.Length - 1; i++)
            {
                returnWheels.Add(WheelPackage.CreateFromString(tempString[i]));
            }

            returnPackage.turretRotation = Tools.fromVec3String(tempString[tempString.Length - 1]);

            returnPackage.WheelsPackage = returnWheels;

            return returnPackage;
        }
    }
    [Serializable]
    public class PlayerPackage
    {
        public string Name;
        public int Score;
        public int Health;
        public int Kills;
        public int Deaths;
        public int Value;


        public override string ToString()
        {
            char delim = '>';

            StringBuilder sb = new StringBuilder();

            sb.Append(Name);
            sb.Append(delim);

            sb.Append(Score.ToString());
            sb.Append(delim);

            sb.Append(Health.ToString());
            sb.Append(delim);

            sb.Append(Kills.ToString());
            sb.Append(delim);

            sb.Append(Deaths.ToString());
            sb.Append(delim);

            sb.Append(Value.ToString());


            return sb.ToString();
        }
        public static PlayerPackage CreateFromString(string input)
        {
            PlayerPackage returnPackage = new PlayerPackage();

            string[] tempString = input.Split('>');

            returnPackage.Name = tempString[0];
            returnPackage.Score = Convert.ToInt32(tempString[1]);
            returnPackage.Health = Convert.ToInt32(tempString[2]);
            returnPackage.Kills = Convert.ToInt32(tempString[3]);
            returnPackage.Deaths = Convert.ToInt32(tempString[4]);
            returnPackage.Value = Convert.ToInt32(tempString[5]);

            return returnPackage;
        }
    }

    [Serializable]
    public class CompletePackage
    {
        public PlayerPackage playerPackage;
        public VehiclePackage vehiclePackage;

        public override string ToString()
        {
            char delim = '^';

            StringBuilder sb = new StringBuilder();

            sb.Append(playerPackage.ToString());
            sb.Append(delim);

            sb.Append(vehiclePackage.ToString());

            return sb.ToString();
        }
        public static CompletePackage CreateFromString(string input)
        {
            CompletePackage returnPackage = new CompletePackage();

            string[] tempString = input.Split('^');

            returnPackage.playerPackage = PlayerPackage.CreateFromString(tempString[0]);

            returnPackage.vehiclePackage = VehiclePackage.CreateFromString(tempString[1]);

            return returnPackage;
        }
    }

    [Serializable]
    public class PropPackage
    {
        public string modelName;
        public int propId;
        public EntityPackage entityPackage;
        public override string ToString()
        {
            char delim = '#';

            StringBuilder sb = new StringBuilder();

            sb.Append(modelName.ToString());
            sb.Append(delim);

            sb.Append(propId.ToString());
            sb.Append(delim);

            sb.Append(entityPackage.ToString());

            return sb.ToString();
        }
        public static PropPackage CreateFromString(string input)
        {
            PropPackage returnPackage = new PropPackage();

            string[] tempString = input.Split('#');

            returnPackage.modelName = tempString[0];

            returnPackage.propId = Convert.ToInt32(tempString[1]);

            returnPackage.entityPackage = EntityPackage.CreateFromString(tempString[2]);

            return returnPackage;
        }
    }

}
