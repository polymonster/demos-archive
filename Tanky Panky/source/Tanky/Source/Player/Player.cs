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
using BEPUphysics;

namespace Tanky
{
    public enum VehicleType
    {
        Tank = 0,
        Jeep = 1
    }

    public class Player
    {
        // The players name in the game
        public string m_Name;

        // The players last known ping time
        public float m_Ping;

        // The players model
        public int m_Model;

        // The players vehicle
        public VehiclePhysics m_Vehicle;

        // The team this player is on
        public int m_team { get; set; }

        // The players health
        public int m_health { get; set; }

        //the players kills
        public int m_kills { get; set; }

        //the players deaths
        public int m_deaths { get; set; }

        //the players score
        public int m_score { get; set; }

        //the players value
        public int m_value { get; private set; }

        // emitter for damaged vehicles
        private bool m_damage_effects;
        public ParticleEffect m_damage_emitter;

        // The inital health value, and what m_Health is reset to
        private const int m_MaxHealth = 100;

        public bool IsAlive()
        {
            bool alive = true;
            if (m_health <= 0)
            {
                alive = false;
            }
            return alive;
        }

        /// <summary>
        /// adds damage to the vehicle
        /// if the damage kills the vehicle
        /// it returns true
        /// false if not
        /// </summary>
        /// <param name="aDamage">amount of damage to inflict</param>
        /// <returns>Vehicle Killed</returns>
        public void TakeDamage( int aDamage )
        {
            m_health = m_health - aDamage;

            if (m_health < 0)
            {
                m_health = 0;
                m_Vehicle.m_vehicle_physics.moveTo(new Vector3(0, -3000, 0));
            }
        }

        public void AddScore(int theScore)
        {
            m_score += theScore;
            AddValue(theScore /10);
        }

        public void AddValue(int theValue)
        {
            m_value += theValue;
        }

        public void Spawn(int aSpawnPointId)
        {
            // And we're alive again!
            m_health = m_MaxHealth;
            m_value = 1000;

            // reset visible damage flag
            m_damage_effects = false;

            // Reset any forces
            m_Vehicle.m_vehicle_physics.body.clearForces();
            m_Vehicle.m_vehicle_physics.body.linearMomentum = new Vector3(0, 0, 0);
            m_Vehicle.m_vehicle_physics.body.linearVelocity = new Vector3(0, 0, 0);
            m_Vehicle.m_vehicle_physics.body.orientationMatrix = Matrix.Identity;
            m_Vehicle.m_vehicle_physics.body.orientationQuaternion = Quaternion.Identity;

            // Move ourself into place
            if(m_Name == g.playerInfo.m_Name)
            {
                g.level.spawn_player_at_point(this, aSpawnPointId);
            }
            
            // Send an update for this player to the server to let it know we're alive again
            if (!g.is_server)
            {
                g.clientData.client.SendMessage(PacketFams.EMovementData, PacketSubTypes.EMovementUpdatePlayer, Bundle().ToString());
            }

        }
        public void Spawn()
        {
            // And we're alive again!
            m_health = m_MaxHealth;
            
            //reset value
            m_value = 1000;

            // reset visible damage flag
            m_damage_effects = false;

            // Update the dead players deaths
            m_deaths++;

            // Reset any forces
            m_Vehicle.m_vehicle_physics.body.clearForces();
            m_Vehicle.m_vehicle_physics.body.linearMomentum = new Vector3(0, 0, 0);
            m_Vehicle.m_vehicle_physics.body.linearVelocity = new Vector3(0, 0, 0);
            m_Vehicle.m_vehicle_physics.body.orientationMatrix = Matrix.Identity;
            m_Vehicle.m_vehicle_physics.body.orientationQuaternion = Quaternion.Identity;

            // Move ourself into place(away from corpse)
            g.level.spawn_by_distance();

            // Send an update for this player to the server to let it know we're alive again
            if (!g.is_server)
            {
                g.clientData.client.SendMessage(PacketFams.EMovementData, PacketSubTypes.EMovementUpdatePlayer, Bundle().ToString());
            }
        }

        public int GetVehicleType()
        {
            return m_Model;
        }

        public void UpdateDamageEmitter()
        {
            // if the player is dead, kill off existing damage emitter
            if (!IsAlive())
            {
                if (m_damage_emitter != null)
                {
                    g.effect_sys.RemoveEffect(m_damage_emitter);
                    m_damage_emitter = null;
                }
            }
            else
            {

                // damage effect off?
                if (!m_damage_effects)
                {
                    // if health is under 30%
                    if (m_health < m_MaxHealth * 0.3f)
                    {
                        // spawn damage effect
                        Matrix mat = m_Vehicle.m_vehicle_physics.body.orientationMatrix *
                                     Matrix.CreateTranslation(m_Vehicle.m_vehicle_physics.body.centerPosition) *
                                     Matrix.CreateTranslation(new Vector3(5.0f, 0.0f, 2.0f));
                        m_damage_emitter = g.effect_sys.CreateEmitter(mat.Translation, Vector3.UnitY, "effects/smokeemitter1.txt", g.effect_sys.GetTexture(EffectSystem.textures_e.TEX_FLAME_1));
                        m_damage_effects = true;
                    }
                }
                else
                {
                    if (m_damage_emitter != null)
                    {
                        // update position of emitter from player vehicle position
                        m_damage_emitter.m_pos = m_Vehicle.m_vehicle_physics.body.centerPosition;
                    }
                    else if( m_health >= m_MaxHealth * 0.3f )
                    {
                        m_damage_effects = false;
                    }
                }
            }
        }

        public void CreateCorpse()
        {
            g.corpse_man.AddCorpse((VehicleType)m_Model, m_Vehicle, 1000*15);

            g.effect_sys.CreateEffect(m_Vehicle.m_vehicle_physics.body.centerPosition, Vector3.UnitY, "effects/bigexplosion.txt", g.effect_sys.GetTexture(EffectSystem.textures_e.TEX_FLAME_1));
            g.effect_sys.CreateEffect(m_Vehicle.m_vehicle_physics.body.centerPosition, Vector3.UnitY, "effects/bigsmoke.txt", g.effect_sys.GetTexture(EffectSystem.textures_e.TEX_SMOKE_1));
        }

        public Player()
        {
            // This is used when a local client starts up, we set the player name 
            // to the computer name by default
            m_Name = Environment.MachineName;

            // Default to tank
            SetModel(0);

            // reset visible damage flag
            m_damage_effects = false;

            // Default to team 1
            m_team = 1;

            // We start alive:)
            m_health = 100;

            //we start Poor :(
            m_score = 0;

            //we start off worth not a great deal
            m_value = 1000;
        }

        public Player(string aName)
        {
            // This is used when the server constructs a player
            m_Name = aName;

            // Defaul to tank
            SetModel(0);

            // reset visible damage flag
            m_damage_effects = false;

            // Default to team 1
            m_team = 1;

            // We start alive:)
            m_health = 100;

            //we start Poor :(
            m_score = 0;

            //we start off worth not a great deal
            m_value = 1000;

        }

        public void SetModel(int aModel)
        {
            m_Model = aModel;
        }

        public void display_name(bool display_score)
        {
            //player names
            Vector2 screen_space;

            Vector3 world_space = m_Vehicle.m_vehicle_physics.body.centerPosition;
            world_space.Y += 5;
            Vector4 screen_space_conversion = new Vector4(world_space, 1);

            screen_space_conversion = Vector4.Transform(screen_space_conversion, g.camera.m_view_matrix);
            screen_space_conversion = Vector4.Transform(screen_space_conversion, g.camera.m_projection_matrix);
            screen_space_conversion /= screen_space_conversion.W;

            screen_space.X = screen_space_conversion.X;
            screen_space.Y = -screen_space_conversion.Y;

            screen_space.X = (screen_space.X * g.WindowCentre.X) + g.WindowCentre.X;
            screen_space.Y = (screen_space.Y * g.WindowCentre.Y) + g.WindowCentre.Y;

            g.text_outputter.output_string(m_Name, screen_space, Color.SkyBlue, 1, true);    
        }

        public void LoadModel()
        {
            
            if (m_Vehicle == null)
            {
                if (m_Model == 1)
                {
                    // The player is a jeep!
                    m_Vehicle = new Jeep(new Vector3(0.0f, -30.0f, 0.0f), Vector3.Zero, g.schemes.CONTROL_SCHEME_KEYBOARD,this);
                }
                else
                {
                    // The player is a tank!
                    m_Vehicle = new Tank(new Vector3(0.0f, -30.0f, 0.0f), Vector3.Zero, g.schemes.CONTROL_SCHEME_KEYBOARD, this);
                }

                PlayerTag tag;
                tag.name = m_Name;

                foreach (Entity e in ((CompoundBody)m_Vehicle.m_vehicle_physics.body).getBodies())
                {
                    e.addEventHook((EventHandlerInitialCollisionDetected)OnCollisionStart);
                    e.tag = tag;
                }
            }
        }

        public CompletePackage Bundle()
        {
            //complete package for return
            CompletePackage temp_complete = new CompletePackage();

            //create a new player package for this player
            PlayerPackage temp_player = new PlayerPackage();

            //fill player package
            temp_player.Name = m_Name;
            temp_player.Deaths = m_deaths;
            temp_player.Kills = m_kills;
            temp_player.Score = m_score;
            temp_player.Value = m_value;
            temp_player.Health = m_health;

            //add vehicle package
            temp_complete.vehiclePackage = m_Vehicle.SnapShotOut();
            //add vehicle package
            temp_complete.playerPackage = temp_player;

            //return complete package
            return temp_complete;
        }

        public void ProcessUpdateFromServer(CompletePackage packageIn)
        {
            // load vehicle details
            if (m_Vehicle != null)
            {
                // Sync the position
                m_Vehicle.SnapShotIn(packageIn.vehiclePackage);
            }

            // Sync the score
            m_score = packageIn.playerPackage.Score;

            // Sync health
            m_health = packageIn.playerPackage.Health;

            // Sync deaths
            m_deaths = packageIn.playerPackage.Deaths;

            // Sync kills
            m_kills = packageIn.playerPackage.Kills;
        }

        public static void OnCollisionStart(Entity sender, Controller controller)
        {
           
        }

        public static void OnCollisionEnd(Entity sender, Controller controller)
        {

        }
    }
}
