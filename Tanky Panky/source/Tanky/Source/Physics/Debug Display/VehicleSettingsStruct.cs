using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using System.Xml.Serialization;
using System.Xml;
using System.IO;

namespace Tanky
{
    [Serializable()]
    public class VehicleSettings
    {
        /// <summary>
        /// Weight of corresponding body block
        /// </summary>
        [XmlElement()]
        public float FrontLeftWeight;
        /// <summary>
        /// Weight of corresponding body block
        /// </summary>
        [XmlElement()]
        public float FrontRightWeight;
        /// <summary>
        /// Weight of corresponding body block
        /// </summary>
        [XmlElement()]
        public float BackLeftWeight;
        /// <summary>
        /// Weight of corresponding body block
        /// </summary>
        [XmlElement()]
        public float BackRightWeight;

        [XmlElement()]
        public Vector3 CentreOfGravity;

        [XmlElement()]
        public float AdditionalWeight;

        [XmlArray("Wheels")]
        [XmlArrayItem("Wheel",typeof(WheelSettings))]
        public List<WheelSettings> WheelList { get; set; }

        public VehicleSettings()
        {
            FrontLeftWeight = 30f;
            FrontRightWeight = 30f;
            BackLeftWeight = 10f;
            BackRightWeight = 10f;

            CentreOfGravity = new Vector3(0, 0, 0); ;
            AdditionalWeight = 100f;

            WheelList = new List<WheelSettings>();
        }

        public VehicleSettings(string theFilePath)
        {
            LoadFromFile(theFilePath);
        }

        public void AddWheel()
        {
            WheelSettings temp = new WheelSettings();

            temp.Radius = 15f;
            temp.Width = 20f;
            temp.Distance = 10f;
            temp.SuspensionDirection = new Vector3(0, -1, 0);
            temp.ForwardVector = new Vector3(0, 0, 1);
            temp.SuspensionLen = 2f;
            temp.SuspensionConstant = 1f;
            temp.SlidingFriction = 200f;
            temp.RollFriction = 10.0f;
            temp.Grip = 800.0f;
            temp.MaxSuspensionForce = 15000;
            temp.SuspensionDamp = 15f;

            WheelList.Add(temp);

        }

        public void AddWheel(WheelSettings theSettings)
        {
            if (theSettings != null)
            {
                WheelList.Add(theSettings);
            }
        }

        public void SavetoFile(string theFileName)
        {
            TextWriter tr = new StreamWriter(theFileName);
            XmlSerializer sr = new XmlSerializer(typeof(VehicleSettings));
            sr.Serialize(tr, this);
            tr.Close();
        }

        public void LoadFromFile(string theFilePath)
        {
            try
            {
                XmlSerializer Seri = new XmlSerializer(typeof(VehicleSettings));
                XmlReader reader = XmlReader.Create(new StreamReader(theFilePath));

                if (Seri.CanDeserialize(reader))
                {
                    VehicleSettings temp = (VehicleSettings)Seri.Deserialize(reader);

                    FrontLeftWeight = temp.FrontLeftWeight;
                    FrontRightWeight = temp.FrontRightWeight;
                    BackLeftWeight = temp.BackLeftWeight;
                    BackRightWeight = temp.BackRightWeight;

                    CentreOfGravity = temp.CentreOfGravity;
                    AdditionalWeight = temp.AdditionalWeight;

                    WheelList = temp.WheelList;

                }
                else
                {
                    throw new SystemException("Could not load vehicle Settings");
                }
            }
            catch (Exception e)
            {
                throw new SystemException("Error Loading File: Exception - " + e.Message);
            }
        }
        
    }

    [Serializable()]
    public class WheelSettings
    {
        [XmlElement()]
        public float Radius { get; set; }
        [XmlElement()]
        public float Width { get; set; }
        [XmlElement()]
        public float Distance{ get; set; }
        [XmlElement()]
        public Vector3 SuspensionDirection{ get; set; }
        [XmlElement()]
        public Vector3 ForwardVector{ get; set; }
        [XmlElement()]
        public float SuspensionLen{ get; set; }
        [XmlElement()]
        public float SuspensionConstant{ get; set; }
        [XmlElement()]
        public float SlidingFriction{ get; set; }
        [XmlElement()]
        public float RollFriction{ get; set; }
        [XmlElement()]
        public float Grip{ get; set; }
        [XmlElement()]
        public float MaxSuspensionForce{ get; set; }
        [XmlElement()]
        public float SuspensionDamp{ get; set; }
        [XmlElement()]
        public Vector3 WheelPosition { get; set; }

        public WheelSettings()
        {
            SuspensionDirection = Vector3.Zero;
            ForwardVector = Vector3.Zero;
            WheelPosition = Vector3.Zero;
        }

        public WheelSettings(WheelSettings theSettings)
        {
             Radius = theSettings.Radius;
             Width  = theSettings.Width;
             Distance  = theSettings.Distance;
             SuspensionDirection  = theSettings.SuspensionDirection;
             ForwardVector  = theSettings.ForwardVector;
             SuspensionLen  = theSettings.SuspensionLen;
             SuspensionConstant  = theSettings.SuspensionConstant;
             SlidingFriction  = theSettings.SlidingFriction;
             RollFriction  = theSettings.RollFriction;
             Grip  = theSettings.Grip;
             MaxSuspensionForce  = theSettings.MaxSuspensionForce;
             SuspensionDamp  = theSettings.SuspensionDamp;
             WheelPosition = theSettings.WheelPosition; 
        }
    }
}
