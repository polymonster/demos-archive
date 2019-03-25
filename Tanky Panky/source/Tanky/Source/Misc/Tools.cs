using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using BEPUphysics;
using System.Diagnostics;

namespace Tanky
{
    struct PlayerTag
    {
        public string name;
    }
    struct PropTag
    {
        public string ModelName;
    }

    public static class Tools
    {
        public static Vector2 fromVec2String(string input)
        {
            Vector2 returnVec = new Vector2();

            //remove braces
            string temp = input.Replace("{", "");
            temp = temp.Replace("}", "");

            //split remaining string
            string[] temparray = temp.Split(' ');

            returnVec.X = Convert.ToSingle(temparray[0].Split(':')[1]);
            returnVec.Y = Convert.ToSingle(temparray[1].Split(':')[1]);

            return returnVec;
        }
        public static Vector3 fromVec3String(string input)
        {
            Vector3 returnVec = new Vector3();

            //remove braces
            string temp = input.Replace("{", "");
            temp = temp.Replace("}", "");

            //split remaining string
            string[] temparray = temp.Split(' ');

            returnVec.X = Convert.ToSingle(temparray[0].Split(':')[1]);
            returnVec.Y = Convert.ToSingle(temparray[1].Split(':')[1]);
            returnVec.Z = Convert.ToSingle(temparray[2].Split(':')[1]);

            return returnVec;
        }
        public static Quaternion fromQuatString(string input)
        {
            Quaternion returnQuat = new Quaternion();

            //remove braces
            string temp = input.Replace("{", "");
            temp = temp.Replace("}", "");

            //split remaining string
            string[] temparray = temp.Split(' ');

            returnQuat.X = Convert.ToSingle(temparray[0].Split(':')[1]);
            returnQuat.Y = Convert.ToSingle(temparray[1].Split(':')[1]);
            returnQuat.Z = Convert.ToSingle(temparray[2].Split(':')[1]);
            returnQuat.W = Convert.ToSingle(temparray[3].Split(':')[1]);
            return returnQuat;
        }
        public static Matrix fromMatString(string input)
        {
            Matrix returnMat = new Matrix();

            //remove braces
            string temp = input.Remove(0,1);
            temp = temp.Remove(temp.Length-1);

            //split remaining string
            string[] temparray = temp.Split('}');

            string intTemp = temparray[0].Replace("{","");
            string[] intTempArray = intTemp.Remove(0,1).Split(' ');
            returnMat.M11 = Convert.ToSingle(intTempArray[0].Split(':')[1]);
            returnMat.M12 = Convert.ToSingle(intTempArray[1].Split(':')[1]);
            returnMat.M13 = Convert.ToSingle(intTempArray[2].Split(':')[1]);
            returnMat.M14 = Convert.ToSingle(intTempArray[3].Split(':')[1]);

            intTemp = temparray[1].Replace("{", "");
            intTempArray = intTemp.Remove(0, 1).Split(' ');
            returnMat.M21 = Convert.ToSingle(intTempArray[0].Split(':')[1]);
            returnMat.M22 = Convert.ToSingle(intTempArray[1].Split(':')[1]);
            returnMat.M23 = Convert.ToSingle(intTempArray[2].Split(':')[1]);
            returnMat.M24 = Convert.ToSingle(intTempArray[3].Split(':')[1]);

            intTemp = temparray[2].Replace("{", "");
            intTempArray = intTemp.Remove(0, 1).Split(' ');
            returnMat.M31 = Convert.ToSingle(intTempArray[0].Split(':')[1]);
            returnMat.M32 = Convert.ToSingle(intTempArray[1].Split(':')[1]);
            returnMat.M33 = Convert.ToSingle(intTempArray[2].Split(':')[1]);
            returnMat.M34 = Convert.ToSingle(intTempArray[3].Split(':')[1]);

            intTemp = temparray[3].Replace("{", "");
            intTempArray = intTemp.Remove(0, 1).Split(' ');
            returnMat.M41 = Convert.ToSingle(intTempArray[0].Split(':')[1]);
            returnMat.M42 = Convert.ToSingle(intTempArray[1].Split(':')[1]);
            returnMat.M43 = Convert.ToSingle(intTempArray[2].Split(':')[1]);
            returnMat.M44 = Convert.ToSingle(intTempArray[3].Split(':')[1]);


            return returnMat;
        }
        public static byte[] StrToByteArray(string str)
        {
            System.Text.ASCIIEncoding encoding = new System.Text.ASCIIEncoding();
            return encoding.GetBytes(str);
        }
        /// <summary>
        /// Searches the current player and the connected player and 
        /// returns the reference to the player associated with the input name
        /// Null if no player is found
        /// </summary>
        /// <param name="thePlayerName">Name of player to find</param>
        /// <returns>Player</returns>
        public static Player findPlayer(Player thePlayer)
        {
            if(thePlayer != null)
            {
                //is it current player
                if(g.playerInfo == thePlayer)
                {
                    return g.playerInfo;
                }
                //is it one of the connected players
                else
                {
                    if (g.is_server == true)
                    {

                        foreach (gameClient player in g.server.m_Clients)
                        {
                            if (player.m_PlayerInfo != null)
                            {
                                if(player.m_PlayerInfo == thePlayer)
                                {
                                    return player.m_PlayerInfo;
                                }
                            }
                        }
                    }
                    else
                    {
                        foreach (Player player in g.clientData.players)
                        {
                            if (player != null)
                            {
                                if(player == thePlayer)
                                {
                                    return player;
                                }
                            }
                        }
                    }
                }
                //didnt find player
                return null;
            }
            else
            {
                //didnt find player
                return null;
            }        
        }
        public static Player findPlayer(Entity thePlayerEntity)
        {
            if (thePlayerEntity != null)
            {
                //is it current player
                if (g.playerInfo.m_Vehicle.m_vehicle_physics.body == thePlayerEntity)
                {
                    return g.playerInfo;
                }
                //is it one of the connected players
                else
                {
                    if (g.is_server == true)
                    {

                        foreach (gameClient player in g.server.m_Clients)
                        {
                            if (player.m_PlayerInfo.m_Vehicle.m_vehicle_physics.body != null)
                            {
                                if (player.m_PlayerInfo.m_Vehicle.m_vehicle_physics.body == thePlayerEntity)
                                {
                                    return player.m_PlayerInfo;
                                }
                            }
                        }
                    }
                    else
                    {
                        foreach (Player player in g.clientData.players)
                        {
                            if (player != null)
                            {
                                if (player.m_Vehicle.m_vehicle_physics.body == thePlayerEntity)
                                {
                                    return player;
                                }
                            }
                        }
                    }
                }
                //didnt find player
                return null;
            }
            else
            {
                //didnt find player
                return null;
            }
        }
        public static Player findPlayer(string theName)
        {

            //is it current player
            if (g.playerInfo.m_Name == theName)
            {
                return g.playerInfo;
            }
            //is it one of the connected players
            else
            {
                if (g.is_server == true)
                {

                    foreach (gameClient player in g.server.m_Clients)
                    {

                        if (player.m_PlayerInfo.m_Name == theName)
                        {
                            return player.m_PlayerInfo;
                        }
                    }
                }
                else
                {
                    foreach (Player player in g.clientData.players)
                    {
                        if (player.m_Name == theName)
                        {
                            return player;
                        }
                    }
                }
            }
            //didnt find player
            return null;
        }
        public static List<Player> findHighestScore()
        {
            List<Player> returnList = new List<Player>();

            returnList.Add(g.playerInfo);

           
            if (g.is_server == true)
            {
                //find highest score
                foreach (gameClient player in g.server.m_Clients)
                {
                    if(player.m_PlayerInfo.m_score > returnList[0].m_score)
                    {
                        returnList[0] = player.m_PlayerInfo;
                    }
                }

                //check if there are any more with equal score
                foreach (gameClient player in g.server.m_Clients)
                {
                    if (player.m_PlayerInfo.m_score == returnList[0].m_score && returnList.Contains(player.m_PlayerInfo) == false)
                    {
                        returnList.Add(player.m_PlayerInfo);
                    }
                }
            }
            else
            {
                //find highest score
                foreach (Player player in g.clientData.players)
                {
                    if (player.m_score > returnList[0].m_score)
                    {
                        returnList[0] = player;
                    }
                }
                //check if there are any more with equal score
                foreach (Player player in g.clientData.players)
                {
                    if (player.m_score == returnList[0].m_score && returnList.Contains(player) == false)
                    {
                        returnList.Add(player);
                    }
                }


            }
            return returnList;
        }
        public static Player findHighestKills()
        {
            Player returnPlayer = g.playerInfo;

            if (g.is_server == true)
            {
                //find highest killcount
                foreach (gameClient player in g.server.m_Clients)
                {
                    if (player.m_PlayerInfo.m_kills > returnPlayer.m_kills)
                    {
                        returnPlayer = player.m_PlayerInfo;
                    }
                }
            }
            else
            {
                //find highest killcount
                foreach (Player player in g.clientData.players)
                {
                    if (player.m_kills > returnPlayer.m_kills)
                    {
                        returnPlayer = player;
                    }
                }
            }
            return returnPlayer;
        }
        public static string GetWinnerString()
        {

            string returnString;

            //if it was a timed game
            if (g.game_timer != null)
            {
                List<Player> temp = findHighestScore();
                //if it was a draw
                if (temp.Count > 1)
                {
                    returnString = "The Game Was a Tie Between  ";

                    for (int i = 0; i < temp.Count; i++)
                    {
                        if (i != 0)
                        {
                            returnString += " and ";
                        }
                        returnString += (temp[i].m_Name);
                    }
                }
                else
                {
                    returnString = "A Winrar is you ";

                    returnString += (temp[0].m_Name);
                }
            }
            //kill limit game
            else
            {
                Player temp = findHighestKills();
                returnString = "A Winrar is you ";

                returnString += (temp.m_Name);

            }

            return returnString;

        }

    }  
    public class CountDownTimer
    {
        Stopwatch m_timer;
        public float Duration { get; private set; }

        public bool IsFinished
        {
            get
            {
                if(m_timer.ElapsedMilliseconds >= Duration)
                {
                    m_timer.Stop();
                    return true;
                }
                return false;
            }
        }

        public float ElapsedTime 
        {
            get
            {
                return m_timer.ElapsedMilliseconds;
            }
        }

        public CountDownTimer(float theDuration)
        {
            Duration = theDuration;
            m_timer = new Stopwatch();
            m_timer.Start();
        }
        public override string ToString()
        {
            float display = (Duration - m_timer.ElapsedMilliseconds);

            int seconds = (int)(display / 1000);
            int minutes = seconds / 60;

            string s_minutes = minutes.ToString();
            string s_seconds = (seconds % 60) < 10 ? "0" + (seconds % 60).ToString() : (seconds % 60).ToString();


            return s_minutes + "." + s_seconds;
        }
    }
}
