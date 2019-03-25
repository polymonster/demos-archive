using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;
using System.Xml;
using System.IO;

namespace Tanky
{
    [Serializable()]
    class MainPhysicsSettings
    {
        /// <summary>
        /// Value of the gravity in game
        /// </summary>
        public float Gravity;
        /// <summary>
        /// the size of the cubes in the Physics broad phase stage
        /// </summary>
        public float BroadPhaseGridSize;

        /// <summary>
        /// a set of vehicle settings
        /// </summary>
        List<VehicleSettings> VehicleSettingsSets;

        MainPhysicsSettings()
        {
            Gravity = -300;
            BroadPhaseGridSize = 40;
            VehicleSettingsSets = new List<VehicleSettings>();
        }
        MainPhysicsSettings(string theFilePath)
        {
            try
            {
                XmlSerializer Seri = new XmlSerializer(typeof(MainPhysicsSettings));
                XmlReader reader = XmlReader.Create(new StreamReader(theFilePath));

                if (Seri.CanDeserialize(reader))
                {
                    MainPhysicsSettings temp = (MainPhysicsSettings)Seri.Deserialize(reader);

                    Gravity = temp.Gravity;
                    BroadPhaseGridSize = temp.BroadPhaseGridSize;
                    VehicleSettingsSets = temp.VehicleSettingsSets;

                }
                else
                {
                    throw new SystemException("Could not load physics Settings");
                }
            }
            catch (Exception e)
            {
                throw new SystemException("Error Loading File: Exception - " + e.Message);
            }
        }

        void SaveToFile(string theFilePath)
        {
            XmlSerializer Seri = new XmlSerializer(typeof(MainPhysicsSettings));

            TextWriter TW = new StreamWriter(theFilePath);
            Seri.Serialize(TW, this);
            TW.Close();
        }
        



    }
}
