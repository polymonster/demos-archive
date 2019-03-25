using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using System.Diagnostics;
using Microsoft.Xna.Framework.Graphics;
using BEPUphysics;

namespace Tanky
{
    public struct ModelBox
    {
        public ModelTP model;
        public Box box;
    }
    public class Corpse
    {
        private Stopwatch m_timer;
        public VehiclePhysics m_vehicle;
        public VehiclePhysics Vehicle
        {
            get
            {
                return m_vehicle;
            }
        }
        private float m_time;
        private VehicleType Type;
        public bool Alive
        {
            get
            {
                return m_timer.ElapsedMilliseconds < m_time;
            }
        }
        public Dictionary<string,ModelBox> ModelList;
        private Explosion m_explosion;

        public Corpse(VehicleType theType,VehiclePhysics theVehicle,float theTime)
        {
            ModelList = new Dictionary<string, ModelBox>();
            ModelBox temp_modelbox;
            Box temp_box;

            if (theType == VehicleType.Tank)
            {
                m_vehicle = theVehicle;

                //part 1
                temp_modelbox.model = new ModelTP();
                temp_modelbox.model.load("Models\\tank_pieces_1");
                temp_box = temp_modelbox.model.GetBepuBox();
                temp_modelbox.box = new BEPUphysics.Box(m_vehicle.m_vehicle_physics.body.centerPosition,
                                                        temp_box.width * g.scaleFactor,
                                                        temp_box.height * g.scaleFactor,
                                                        temp_box.length * g.scaleFactor,
                                                        10);
                ModelList.Add("Models\\tank_pieces_1", temp_modelbox);

                //part 2
                temp_modelbox.model = new ModelTP();
                temp_modelbox.model.load("Models\\tank_pieces_2");
                temp_box = temp_modelbox.model.GetBepuBox();
                temp_modelbox.box = new BEPUphysics.Box(m_vehicle.m_vehicle_physics.body.centerPosition,
                                                        temp_box.width * g.scaleFactor,
                                                        temp_box.height * g.scaleFactor,
                                                        temp_box.length * g.scaleFactor,
                                                        10);
                ModelList.Add("Models\\tank_pieces_2", temp_modelbox);
                
                //part 3
                temp_modelbox.model = new ModelTP();
                temp_modelbox.model.load("Models\\tank_pieces_3");
                temp_box = temp_modelbox.model.GetBepuBox();
                temp_modelbox.box = new BEPUphysics.Box(m_vehicle.m_vehicle_physics.body.centerPosition,
                                                        temp_box.width * g.scaleFactor,
                                                        temp_box.height * g.scaleFactor,
                                                        temp_box.length * g.scaleFactor,
                                                        10);
                ModelList.Add("Models\\tank_pieces_3", temp_modelbox);

                //part 4
                temp_modelbox.model = new ModelTP();
                temp_modelbox.model.load("Models\\tank_pieces_4");
                temp_box = temp_modelbox.model.GetBepuBox();
                temp_modelbox.box = new BEPUphysics.Box(m_vehicle.m_vehicle_physics.body.centerPosition,
                                                        temp_box.width * g.scaleFactor,
                                                        temp_box.height * g.scaleFactor,
                                                        temp_box.length * g.scaleFactor,
                                                        10);
                ModelList.Add("Models\\tank_pieces_4", temp_modelbox);

                //part 5
                temp_modelbox.model = new ModelTP();
                temp_modelbox.model.load("Models\\tank_pieces_5");
                temp_box = temp_modelbox.model.GetBepuBox();
                temp_modelbox.box = new BEPUphysics.Box(m_vehicle.m_vehicle_physics.body.centerPosition,
                                                        temp_box.width * g.scaleFactor,
                                                        temp_box.height * g.scaleFactor,
                                                        temp_box.length * g.scaleFactor,
                                                        10);
                ModelList.Add("Models\\tank_pieces_5", temp_modelbox);
            }
            else
            {
                m_vehicle = theVehicle;

                //part 1
                temp_modelbox.model = new ModelTP();
                temp_modelbox.model.load("Models\\jeep_pieces_1");
                temp_box = temp_modelbox.model.GetBepuBox();
                temp_modelbox.box = new BEPUphysics.Box(m_vehicle.m_vehicle_physics.body.centerPosition,
                                                        temp_box.width * g.scaleFactor,
                                                        temp_box.height * g.scaleFactor,
                                                        temp_box.length * g.scaleFactor,
                                                        10);
                ModelList.Add("Models\\jeep_pieces_1", temp_modelbox);

                //part 2
                temp_modelbox.model = new ModelTP();
                temp_modelbox.model.load("Models\\jeep_pieces_2");
                temp_box = temp_modelbox.model.GetBepuBox();
                temp_modelbox.box = new BEPUphysics.Box(m_vehicle.m_vehicle_physics.body.centerPosition,
                                                        temp_box.width * g.scaleFactor,
                                                        temp_box.height * g.scaleFactor,
                                                        temp_box.length * g.scaleFactor,
                                                        10);
                ModelList.Add("Models\\jeep_pieces_2", temp_modelbox);

                //part 3
                temp_modelbox.model = new ModelTP();
                temp_modelbox.model.load("Models\\jeep_pieces_3");
                temp_box = temp_modelbox.model.GetBepuBox();
                temp_modelbox.box = new BEPUphysics.Box(m_vehicle.m_vehicle_physics.body.centerPosition,
                                                        temp_box.width * g.scaleFactor,
                                                        temp_box.height * g.scaleFactor,
                                                        temp_box.length * g.scaleFactor,
                                                        10);
                ModelList.Add("Models\\jeep_pieces_3", temp_modelbox);

                //part 4
                temp_modelbox.model = new ModelTP();
                temp_modelbox.model.load("Models\\jeep_pieces_4");
                temp_box = temp_modelbox.model.GetBepuBox();
                temp_modelbox.box = new BEPUphysics.Box(m_vehicle.m_vehicle_physics.body.centerPosition,
                                                        temp_box.width * g.scaleFactor,
                                                        temp_box.height * g.scaleFactor,
                                                        temp_box.length * g.scaleFactor,
                                                        10);
                ModelList.Add("Models\\jeep_pieces_4", temp_modelbox);

                //part 5
                temp_modelbox.model = new ModelTP();
                temp_modelbox.model.load("Models\\jeep_pieces_1");
                temp_box = temp_modelbox.model.GetBepuBox();
                temp_modelbox.box = new BEPUphysics.Box(m_vehicle.m_vehicle_physics.body.centerPosition,
                                                        temp_box.width * g.scaleFactor,
                                                        temp_box.height * g.scaleFactor,
                                                        temp_box.length * g.scaleFactor,
                                                        10);
                ModelList.Add("Models\\jeep_pieces_5", temp_modelbox);
            }

            foreach (KeyValuePair<string, ModelBox> m in ModelList)
            {
                m.Value.box.centerPosition = m_vehicle.m_vehicle_physics.body.centerPosition;
            }
            

            m_timer = new Stopwatch();
            m_timer.Start();
            m_time = theTime;
            Type = theType;
            Vector3 pos = m_vehicle.m_vehicle_physics.body.centerPosition;
            pos.Y -= 1;
            m_explosion = new Explosion(pos,
                                        400,
                                        15,
                                        g.physics.m_collisionSpace,
                                        ExplosionFalloffType.Quadratic);
            m_explosion.explode();
        }
        public void Draw()
        {
            Matrix World = new Matrix();
            foreach (KeyValuePair<string, ModelBox> m in ModelList)
            {
                World = Matrix.CreateScale(0.05f) *
                    m.Value.box.orientationMatrix *
                    Matrix.CreateTranslation(m.Value.box.centerPosition);

                m.Value.model.draw(g.camera.m_projection_matrix,g.camera.m_view_matrix,World);
            }
        }
        
    }

    public class CorpseManager
    {
        List<Corpse> m_list;
        Texture2D SmokeTex;

        public CorpseManager()
        {
            m_list = new List<Corpse>();
            SmokeTex = g.content.Load<Texture2D>("Particles\\Smoke");
        }

        public void AddCorpse(VehicleType theType,VehiclePhysics theVehicle,float theTime)
        {

            Corpse temp = new Corpse(theType,theVehicle,theTime);


            //add to physics space
            foreach (KeyValuePair<string, ModelBox> m in temp.ModelList)
            {
                g.physics.m_collisionSpace.add(m.Value.box);
                g.physics.m_entityDrawer.addEntity(m.Value.box);
            }

            m_list.Add(temp);

        }
        public void AddCorpse(Corpse theCorpse)
        {
            foreach (KeyValuePair<string, ModelBox> m in theCorpse.ModelList)
            {
                g.physics.m_collisionSpace.add(m.Value.box);
            }
            m_list.Add(theCorpse);
        }

        public void Update()
        {
            for (int i = 0; i < m_list.Count; i++)
            {
                if (m_list[i].Alive == false)
                {
                    foreach (KeyValuePair<string, ModelBox> m in m_list[i].ModelList)
                    {
                        g.physics.m_collisionSpace.remove(m.Value.box);
                    }
                    m_list.RemoveAt(i);
                }
            }
        }
        public void Draw()
        {
            foreach (Corpse c in m_list)
            {
                if (c.Alive)
                {
                    c.Draw();
                }
            }
        }
    }
}


