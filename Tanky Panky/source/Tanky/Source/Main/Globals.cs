using System;
using System.Net;
using System.Net.Sockets;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Net;
using Microsoft.Xna.Framework.Storage;
using Tanky.Source.Physics.Display;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using Tanky.Source.Fun.Particle;
using System.Diagnostics;

namespace Tanky
{
    // game globals
    public class g
    {
        public static GraphicsDeviceManager graphics;
        public static Camera camera;
        public static ContentManager content;
        public static GameTime game_time;
        public static Level level;
        public static EffectSystem effect_sys;
        public static Shadowmap shadow_map;
        public static Effect terrain_shader;
        public static Effect model_shader;
        public static List<ModelEntry> model_list;
        public static Game GameRef;
        public static Physics physics;
        public static SpriteBatch sprite_batch;
        public static ChatWindow chat_window;
        public static TextOutput text_outputter;
        public static HUD hud;
        public static CountDownTimer game_timer;
        public static int game_kill_limit;
        public static CorpseManager corpse_man;
        public static Sound sound_man;
        public static Dictionary<string, string> m_server_list;

        public static ProjectileManager projectile_manager;

        public static Vector2 WindowCentre
        {
            get
            {
                //return new Vector2(GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Width / 2, GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Height / 2);
                return new Vector2(g.graphics.GraphicsDevice.Viewport.Width / 2, g.graphics.GraphicsDevice.Viewport.Height / 2);
            }
            set
            {

            }
        }

        public static float scaleFactor = 0.1f;
        public static KeyboardState oldKeyBoardState;
        public static KeyboardState keyboard_state;
        public static GamePadState gamepad_state;

        public struct mouse_data
        {
            public MouseState m_state;
            private Vector2 m_last_pos;
            public Vector2 m_relative_pos;

            public void update_mouse()
            {
                // check edges of screen to continue moving past
                if (graphics.IsFullScreen)
                {
                    if (m_state.X >= GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Width - 1)
                    {
                        Mouse.SetPosition(0, GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Height / 2);
                    }
                    else if (m_state.X <= 0)
                    {
                        Mouse.SetPosition(GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Width - 2, GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Height / 2);
                    }
                }

                // update relative pos (since last mouse pos)
                m_relative_pos.X = m_state.X - m_last_pos.X;
                m_relative_pos.Y = m_state.Y - m_last_pos.Y;

                m_last_pos.X = m_state.X;
                m_last_pos.Y = m_state.Y;

            }
        };

        public static mouse_data mouse;

        // Local player info, when running as a server we send this to clients when they connect
        // when running as a client we send it to the server when we're told it isnt full.
        public static Player playerInfo;

        // Used when running as a server
        public static gameServer server;

        // Used when running as a client
        public struct clientDataStruct
        {
            public gameClient client;
            public List<Player> players;
        }

        public static clientDataStruct clientData;


        public enum schemes
        {
            CONTROL_SCHEME_360PAD = 0,
            CONTROL_SCHEME_KEYBOARD
        };

        public enum game_states
        {
            MENU_LOOP = 0,
            LOBBY_LOOP,
            GAME_LOOP,
            EXIT,
            GAME_LOBBY_LOOP,
            STATS_LOOP
        };

        public enum game_types
        {
            DEATHMATCH_10 = 0,
            DEATHMATCH_20 = 1,
            DEATHMATCH_50 = 2,
            DEATHMATCH_100 = 3,
            TIMED_2 = 4,
            TIMED_5 = 5,
            TIMED_10 = 6,
            TIMED_25 = 7
        }

        public static bool is_server;

        public static int current_state;

        public static bool mouse_held;

        public static void PacketDeserialise<T>(byte[] arrayIn, out T classOut)
        {
            MemoryStream ms = new MemoryStream(arrayIn);
            BinaryFormatter bf = new BinaryFormatter();

            classOut = (T)bf.Deserialize(ms);
        }
        public static byte[] PacketSerialise<T>(T classIn)
        {
            MemoryStream ms = new MemoryStream();
            BinaryFormatter bf = new BinaryFormatter();
            bf.Serialize(ms, classIn);
            return ms.ToArray();
        }
        public static VehiclePackage client_vehicle_package;
        public static string SnapshotString;

        public static String[] effect_strings =
        {
            "#",
            "color",
            "speed",
            "acceleration",
            "duration",
            "falloff",
            "minsize",
            "maxsize",
            "expansionrate",
            "gravity",
            "rdrift",
            "rforce",
            "drag",
            "dragdir",
            "minparticles",
            "maxparticles",
            "billboarded",
            "hemspread",
            "isemitter",
            "emitterlife",
            "emitterfrequency",
            "spawndims",
            "maxpool"
        };

    }
}
