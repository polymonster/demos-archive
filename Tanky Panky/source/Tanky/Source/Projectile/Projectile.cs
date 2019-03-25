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
using BEPUphysics;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace Tanky
{
    public class Projectile
    {
        Vector3 m_position;
        Vector3 m_direction;

        Vector3 m_rotation;

        float m_speed;

        ModelTP m_model;

        public bool m_alive;

        public void load()
        {
            m_position = Vector3.Zero;
            m_direction = Vector3.Zero;
            m_rotation = Vector3.Zero;

            m_model = new ModelTP();
            m_model.load("Models\\shell_mesh");

            m_alive = false;
        }

        public void spawn(Vector3 position, Vector3 direction, float speed, Vector3 rotation)
        {
            m_position = position;
            m_direction = direction;
            m_rotation = rotation;
            m_speed = speed;

            m_alive = true;
        }

        public void update()
        {
            if (m_alive)
            {
                m_position += m_direction * m_speed;

                m_position.Y -= 1.0f;
            }
        }

        public void draw()
        {
            if (m_alive)
            {
                //m_model.draw(m_position, m_rotation,Vector3.Zero);
            }
        }
    }
    public class PhysicsProjectile
    {
        private ModelTP m_model;
        public int m_projectile_id;
        public CountDownTimer m_timer;

        private bool m_alive;
        public bool IsAlive
        {
            get
            {
                if (m_alive == false || m_timer.IsFinished)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            set
            {
                m_alive = value;
            }
        }

        public Vector3 m_start_pos;
        public Vector3 m_start_dir;
        public float m_life;
        public float m_scale;

        Box m_proj_box;
        public Box PhysicsEntity
        {
            get
            {
                return m_proj_box;
            }
        }

        public int m_damage { get; private set; }

        public Player m_parent { get; private set; }


        public PhysicsProjectile(Player parent,Vector3 pos, float force, Vector3 dir,int damage, float scale)
        {

            m_start_pos = pos;
            m_start_dir = dir;
            m_alive = true;
            m_damage = damage;
            m_scale = scale;

            m_model = new ModelTP();
            m_model.load("Models\\shell_mesh");

            // get dimensions of the projectile
            m_proj_box = m_model.GetBepuBox(5.0f, 0.05f*m_scale);

            // set initial pos and rotation
            m_proj_box.centerPosition = m_start_pos;

            // set bullet orientation
            Vector2 rotrad;
            rotrad.X = (float)-Math.Atan2((double)dir.Y, Math.Sqrt(((double)dir.X * (double)dir.X) + ((double)dir.Z * (double)dir.Z)));
            rotrad.Y = (float)Math.Atan2((double)dir.X, (double)dir.Z);

            m_proj_box.orientationMatrix = Matrix.CreateRotationX(rotrad.X) * Matrix.CreateRotationY(rotrad.Y);

            // give the projectile a starting force
            Vector3 dir_force = (dir*force);
            m_proj_box.internalLinearVelocity = dir_force;

            m_proj_box.bounciness = 0.1f;
            m_parent = parent;

            m_timer = new CountDownTimer(15000);
        }

        public void draw()
        {
            if (m_alive)
            {
                //m_model.draw(m_position, m_rotation,Vector3.Zero);
                Matrix world = Matrix.CreateScale(0.05f * m_scale) * m_proj_box.orientationMatrix * Matrix.CreateTranslation(m_proj_box.centerPosition);
                m_model.draw(g.camera.m_projection_matrix, g.camera.m_view_matrix, world);
            }
        }
    }

    public class ProjectileManager
    {
        public List<PhysicsProjectile> m_List;

        const float MAX_LIFE = 5.0f;
        
        public ProjectileManager()
        {
            m_List = new List<PhysicsProjectile>();
        }

        // lazy projectile spawning
        public void Add(ProjectilePackage data)
        {
            
            // The parent of this projectile is the local game client
            if (g.playerInfo.m_Name == data.m_name)
            {
                Spawn(data, g.playerInfo);
            }

            if (g.is_server)
            {
                foreach (gameClient client in g.server.m_Clients)
                {
                    if (client.m_PlayerInfo.m_Name == data.m_name)
                    {
                        Spawn(data, client.m_PlayerInfo);
                    }
                }
            }
            else
            {
                // find the parent from name string
                foreach (Player p in g.clientData.players)
                {
                    if (p.m_Name.Equals(data.m_name))
                    {
                        Spawn(data, p);
                    }
                }
            }

        }

        private void Spawn(ProjectilePackage data, Player owner)
        {
            int type = owner.GetVehicleType();
            Vector3 pos = data.m_pos;
            float scale = 1.0f;

            if (type == 0)
            {
                // tank-specific calibration of projectile initial position
                pos -= data.m_dir * 3.15f;
                pos.Y += 1.4f;
                scale = 1.9f;
            }
            else
            {
                // jeep-specific calibration of projectile initial position
                pos -= (data.m_dir * 42.5f);
                pos.Y += 2.2f;
                scale = 1.5f;
            }

            CreateProjectile(owner, pos, -data.m_dir, scale, data.m_mag, data.m_dmg);
            g.effect_sys.CreateEffect(pos, -data.m_dir, "effects/smoke1.txt", g.effect_sys.GetTexture(EffectSystem.textures_e.TEX_SMOKE_1));
            g.effect_sys.CreateFlash(pos, -data.m_dir, "effects/tankmuzzle.txt", g.effect_sys.GetTexture(EffectSystem.textures_e.TEX_MUZZLE_FLASH_1), g.effect_sys.GetTexture(EffectSystem.textures_e.TEX_MUZZLE_FLASH_2));
        }

        private void CreateProjectile(Player parent, Vector3 pos, Vector3 dir, float scale, float mag, int damage)
        {
            PhysicsProjectile proj = new PhysicsProjectile(parent,pos, mag, dir,damage, scale);

            //set it so that the parent entity isnt collidable with thsi bullet
            if (parent.m_Vehicle.m_vehicle_physics.body.compoundBody != null)
            {
                foreach (Entity e in ((CompoundBody)parent.m_Vehicle.m_vehicle_physics.body).getBodies())
                {
                    proj.PhysicsEntity.nonCollidableEntities.Add(e);
                }
            }
            else
            {
                proj.PhysicsEntity.nonCollidableEntities.Add(parent.m_Vehicle.m_vehicle_physics.body);
            }

            m_List.Add(proj);
            g.physics.m_collisionSpace.add(proj.PhysicsEntity);
            proj.PhysicsEntity.addEventHook((EventHandlerInitialCollisionDetected)OnCollisionStart);
        }

        public void Update()
        {
            if (m_List.Count > 0)
            {
                foreach (PhysicsProjectile p in m_List)
                {
                    float seconds = (float)g.game_time.ElapsedRealTime.Milliseconds * 0.001f;
                    p.m_life += seconds;
                    if (p.m_life > MAX_LIFE)
                    {
                        p.IsAlive = false;
                    }
                }

                TrimDead();

            }
        }

        public void Render()
        {
            foreach (PhysicsProjectile p in new List<PhysicsProjectile>(m_List))
            {
                if (p.IsAlive)
                {
                    p.draw();
                }
            }
        }

        // remove expired projectiles
        public void TrimDead()
        {
            if (m_List.Count > 0)
            {
                // convert projectile list to array format
                PhysicsProjectile[] p_array = m_List.ToArray();

                // check alive flag for each
                for (int i = m_List.Count - 1; i >= 0; i--)
                {
                    if (!p_array[i].IsAlive)
                    {
                        // remove physics box from physics manager
                        g.physics.m_collisionSpace.remove(p_array[i].PhysicsEntity);
                        // remove projectile from list
                        m_List.RemoveAt(i);
                    }
                }
            }

        }

        public PhysicsProjectile Contains(Entity theProj)
        {
            foreach (PhysicsProjectile p in m_List)
            {
                if(p.PhysicsEntity == theProj)
                {
                    return p;
                }
            }
            return null;
        }

        public static void OnCollisionStart(Entity sender, Controller controller)
        {
            //try to find projectile
            PhysicsProjectile tempProjectile;
            //if projectile is found
            if ((tempProjectile = g.projectile_manager.Contains(sender)) != null)
            {
                //here to make sure we dont get accidental hits from
                //projectiles that havent been killed off yet
                if (tempProjectile.IsAlive)
                {
                    //if thecollider is a player the "tag" property will be a PlayerTag type
                    if (controller.colliderB.tag != null && controller.colliderB.tag.GetType() == typeof(PlayerTag))
                    {
                        g.sound_man.MetalHit.Play();
                        if (g.is_server)
                        {
                            Player tempPlayer;
                            
                            //if the collider is a player
                            if ((tempPlayer = Tools.findPlayer(((PlayerTag)controller.colliderB.tag).name)) != null)
                            {
                                // If a team mate is shooting them then we ignore it
                                if (tempPlayer.m_team != tempProjectile.m_parent.m_team)
                                {
                                    // Is it the local player?
                                    if ( tempPlayer == g.playerInfo )
                                    {
                                        // Call the damage handler as if we had got the network message like a client
                                        g.GameRef.DamagePlayer( tempPlayer.m_Name, tempProjectile.m_damage, tempProjectile.m_parent.m_Name );
                                    }
                                    else
                                    {
                                        // Tell the client to take damage
                                        g.server.SendToAllClients(PacketFams.EMovementData,PacketSubTypes.EMovementProjectileHit, tempPlayer.m_Name + ":" + tempProjectile.m_damage.ToString() + ":" + tempProjectile.m_parent.m_Name );
                                    }
                                }
                            }
                        }
                    }
                    //if Prop
                    else if (controller.colliderA.tag != null && controller.colliderA.tag.GetType() == typeof(PropTag))
                    {
                        string model_name = ((PropTag)controller.colliderA.tag).ModelName;
                        switch (model_name)
                        {
                            case "Models\\barrel_1":
                            case "Models\\barrel_2":
                            case "Models\\barrel_3":
                            case "Models\\barrel_4":
                                {
                                    g.sound_man.MetalHit.Play();
                                    break;
                                }

                            case "Models\\container_1":
                            case "Models\\container_2":
                                {
                                    g.sound_man.MetalHit.Play();
                                    break;
                                }

                            case "Models\\tree":
                            case "Models\\tree_2":
                                {
                                    g.sound_man.WoodHit.Play();
                                    break;
                                }

                            case "Models\\woodbox_1":
                            case "Models\\woodbox_2":
                            case "Models\\woodbox_3":
                            case "Models\\woodbox_4":
                            case "Models\\pallet":
                                {
                                    g.sound_man.WoodHit.Play();
                                    break;
                                }
                            default:
                                {
                                    
                                    break;
                                }

                        }
                    }
                    //once we've done that we can kill it 
                    tempProjectile.IsAlive = false;
                    g.effect_sys.CreateEffect(tempProjectile.PhysicsEntity.internalCenterPosition, new Vector3(0.0f, 0.1f, 0.0f), "effects/smoke2.txt", g.effect_sys.GetTexture(EffectSystem.textures_e.TEX_SMOKE_1));
                    g.effect_sys.CreateEffect(tempProjectile.PhysicsEntity.internalCenterPosition, new Vector3(0.0f, 0.1f, 0.0f), "effects/explosion1.txt", g.effect_sys.GetTexture(EffectSystem.textures_e.TEX_FLAME_1));
                }
            }
        }
    }

    [Serializable]
    public class ProjectilePackage
    {
        public string m_name;
        public Vector3 m_pos;
        public Vector3 m_dir;
        public float m_mag;
        public int m_dmg;

        public ProjectilePackage(string data)
        {
            UnpackData(data);
        }

        public ProjectilePackage(string parent, Vector3 pos, Vector3 dir, float mag, int damage)
        {
            m_name = parent;
            m_pos = pos;
            m_dir = dir;
            m_mag = mag;
            m_dmg = damage;
        }


        public string PackData()
        {
            char delim = '#';

            StringBuilder sb = new StringBuilder();

            sb.Append(m_name);
            sb.Append(delim);

            sb.Append(m_pos.ToString());
            sb.Append(delim);

            sb.Append(m_dir.ToString());
            sb.Append(delim);

            sb.Append(m_mag.ToString());
            sb.Append(delim);

            sb.Append(m_dmg.ToString());

            return sb.ToString();
        }

        public void UnpackData(string input)
        {

            string[] tempString = input.Split('#');

            m_name = tempString[0];
            m_pos = Tools.fromVec3String(tempString[1]);
            m_dir = Tools.fromVec3String(tempString[2]);
            m_mag = Convert.ToSingle(tempString[3]);
            m_dmg = Convert.ToInt32(tempString[4]);
            
        }
    }
}
