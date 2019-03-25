using System;
using System.IO;
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


namespace Tanky
{
    public class SpawnPoint
    {
        public Vector3 m_position;

        public SpawnPoint()
        {
            m_position = Vector3.Zero;
        }
    }

    public class Level
    {
        public List<Prop> m_prop_list = new List<Prop>();
        List<Projectile> m_projectile_list = new List<Projectile>();
        List<SpawnPoint> m_spawn_points = new List<SpawnPoint>();

        Terrain m_terrain;

        public BEPUphysics.Terrain PhysicsTerrain;

        public void load(String filename)
        {
            int id = 0;
            FileStream fin = new FileStream(filename + "_objects.txt", FileMode.Open, FileAccess.Read, FileShare.Read);
            StreamReader stream = new StreamReader(fin);

            while (!stream.EndOfStream)
            {
                //read in a line from the file and split it
                String line = stream.ReadLine();
                String[] splitted = line.Split(' ');

                //if the name is spawn then its a spawn point, otherwise 
                if (splitted[0] == "spawn")
                {
                    SpawnPoint spawn_addition = new SpawnPoint();

                    spawn_addition.m_position.X = System.Convert.ToSingle(splitted[1]) * 0.1f;
                    spawn_addition.m_position.Y = System.Convert.ToSingle(splitted[2]) * 0.1f;
                    spawn_addition.m_position.Z = System.Convert.ToSingle(splitted[3]) * 0.1f;

                    m_spawn_points.Add(spawn_addition);
                }
                else
                {
                    Prop prop_addition = new Prop();

                    String model_name = "Models\\";
                    Vector3 position = Vector3.Zero;

                    model_name += splitted[0];
                    position.X = System.Convert.ToSingle(splitted[1]);
                    position.Y = System.Convert.ToSingle(splitted[2]);
                    position.Z = System.Convert.ToSingle(splitted[3]);

                    prop_addition.load(model_name, position);
                    prop_addition.m_prop_id = id;
                    m_prop_list.Add(prop_addition);

                    id++;

                }
            }

            m_terrain = new Terrain();
            m_terrain.load(filename + "_terrain.txt");
        }

        public void get_unique_spawns(ref int[] spawns)
        {
            int number = spawns.Length;

            Random rand = new Random();

            for (int i = 0; i < number; i++)
            {
                spawns[i] = -1;

                bool in_use = true;

                while(in_use)
                {
                    int id = rand.Next(m_spawn_points.Count);

                    bool duplicated = false;

                    for (int j = 0; j < i; j++)
                    {
                         if (id == spawns[j])
                         {
                             duplicated = true;
                         }
                     }

                    if (!duplicated)
                    {
                        in_use = false;
                        spawns[i] = id;
                    }
                }
            }
        }

        public void spawn_player_at_point(Player aPlayer, int aSpawnPointId)
        {
            aPlayer.m_Vehicle.m_vehicle_physics.moveTo(m_spawn_points[aSpawnPointId].m_position);
        }

        public void spawn_by_distance()
        {
            //get the positions of all the players in a list
            List<Vector3> position_list = new List<Vector3>();

            if (g.is_server)
            {
                foreach (gameClient client in g.server.m_Clients)
                {
                    position_list.Add(client.m_PlayerInfo.m_Vehicle.m_vehicle_physics.body.centerPosition);
                }
            }
            else
            {
                foreach (Player player in g.clientData.players)
                {
                    if (player.m_Vehicle != null)
                    {
                        position_list.Add(player.m_Vehicle.m_vehicle_physics.body.centerPosition);
                    }
                }
            }

            List<float> shortest_distance_list = new List<float>();

            //compare distances to all spawn points
            foreach (SpawnPoint spawn in m_spawn_points)
            {
                float distance = 10000000.0f;
                bool first = true;

                foreach (Vector3 position in position_list)
                {
                    if (first)
                    {
                        distance = Math.Abs(Vector3.Distance(position, spawn.m_position));
                        first = false;
                    }
                    else
                    {
                        float new_distance = Math.Abs(Vector3.Distance(position, spawn.m_position));

                        if (new_distance < distance) distance = new_distance;
                    }

                }

                shortest_distance_list.Add(distance);
            }

            int furthest_id = 0;

            for (int i = 1; i < shortest_distance_list.Count; i++)
            {
                int j = i - 1;

                if (shortest_distance_list[i] > shortest_distance_list[j])
                {
                    furthest_id = i;
                }
            }

            g.playerInfo.m_Vehicle.m_vehicle_physics.moveTo(m_spawn_points[furthest_id].m_position);
        }

        public void spawnProjectile(Projectile projectile)
        {
            m_projectile_list.Add(projectile);
        }

        public void update()
        {
            foreach (Projectile proj in m_projectile_list)
            {
                proj.update();
            }

        }

        public void draw(Matrix projection, Matrix view, bool terrain)
        {
            g.graphics.GraphicsDevice.RenderState.AlphaTestEnable = true;
            g.graphics.GraphicsDevice.RenderState.ReferenceAlpha = 0x222222;
            g.graphics.GraphicsDevice.RenderState.AlphaFunction = CompareFunction.Greater;
            g.graphics.GraphicsDevice.RenderState.CullMode = CullMode.None;
            
            if(terrain) m_terrain.draw();

            foreach (Prop prop in m_prop_list)
            {
                prop.draw(projection, view);
            }
        }

        public void shadow_map_render(Matrix projection, Matrix view)
        {
            g.graphics.GraphicsDevice.RenderState.AlphaTestEnable = true;
            g.graphics.GraphicsDevice.RenderState.ReferenceAlpha = 0x222222;
            g.graphics.GraphicsDevice.RenderState.AlphaFunction = CompareFunction.Greater;
            g.graphics.GraphicsDevice.RenderState.CullMode = CullMode.None;

            foreach (Prop prop in m_prop_list)
            {
                prop.shadow_map_render(projection, view);
            }
        }

        public float[,] get_terrain_heights()
        {
            return m_terrain.get_heights();
        }

        public void addPropsToPhysicsSpace()
        {
            //add to physics space
            foreach(Prop p in m_prop_list)
            {
                if (p.HasPhysicsModel())
                {
                    g.physics.m_collisionSpace.add(p.PhysicsModel);
                    g.physics.m_entityDrawer.addEntity(p.PhysicsModel);
                }
            }
            
        }

        public void addPropsToMiniMap()
        {
            foreach (Prop p in m_prop_list)
            {
                if (p.HasPhysicsModel())
                {
                    g.hud.AddToMap(p);
                }
            }
        }

        
    }
}
