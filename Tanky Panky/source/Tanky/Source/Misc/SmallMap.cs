using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Tanky
{
    public class SmallMap
    {
        private List<VehiclePhysics> m_VechicleList;
        private List<Prop> m_PropList;

        private Vector2 m_TerrainDimensions;
        private Vector2 m_MapDimensions;
        private Vector2 m_Scale;
        private SpriteBatch m_Sprite;

        private Vector2 m_MarkerDimensions;

        public bool ShowProps = true;
        public bool ShowVehicles = true;

        //offset for map in top right corner
        private Vector2 m_Offset;

        Texture2D m_MapBackground;
        Texture2D m_Marker;
        Texture2D m_VehicleMarker;

        public SmallMap()
        {
            m_MapBackground = g.content.Load<Texture2D>("Textures\\Map\\Map_Back");
            m_Marker = g.content.Load<Texture2D>("Textures\\Map\\Marker");
            m_VehicleMarker = g.content.Load<Texture2D>("Textures\\Map\\Vehicle_Marker");

            m_VechicleList = new List<VehiclePhysics>();
            m_PropList = new List<Prop>();
            m_TerrainDimensions = new Vector2(500 , 500);
            m_MapDimensions = new Vector2(150, 150);
            m_MarkerDimensions = new Vector2(2, 2);
            m_Sprite = new SpriteBatch(g.graphics.GraphicsDevice);

            m_Scale = m_MapDimensions / m_TerrainDimensions;

            float border = 10;
            m_Offset = new Vector2(g.graphics.GraphicsDevice.Viewport.Width - ((m_MapDimensions.X) + border) ,
                                   border);
        }
        public void AddVehicle(VehiclePhysics theVehicle)
        {
            if(!m_VechicleList.Contains(theVehicle))
            {
                m_VechicleList.Add(theVehicle);
            }
        }
        public void AddProp(Prop theProp)
        {
            if (!m_PropList.Contains(theProp))
            {
                m_PropList.Add(theProp);
            }
        }
        public Vector2 toMapScale(Vector2 thePos)
        {
            return thePos * m_Scale;
        }
        public Vector2 toMapScale(Vector3 thePos)
        {
            return m_Scale * new Vector2(thePos.X, thePos.Z);
        }

        public void Draw()
        {
            //draw background
            m_Sprite.Begin(SpriteBlendMode.AlphaBlend);
            
            m_Sprite.Draw(m_MapBackground,
                          new Rectangle((int)m_Offset.X, (int)m_Offset.Y, (int)m_MapDimensions.X, (int)m_MapDimensions.Y),
                          Color.White);

            //draw prop markers
            if (ShowProps)
            {
                foreach (Prop p in m_PropList)
                {
                    Vector2 temp = toMapScale(p.PhysicsModel.centerPosition);
                    Color tempColour;

                    int XPos = (int)(m_Offset.X +  (m_MapDimensions.X / 2) + temp.X);
                    int YPos = (int)(m_Offset.Y +  (m_MapDimensions.Y / 2) + temp.Y);

                    #region Choose Texture Based on Model
                    switch (p.m_model.m_name)
                    {
                        case "Models\\barrel_1":
                        case "Models\\barrel_2":
                        case "Models\\barrel_3":
                        case "Models\\barrel_4":
                            {
                                tempColour = Color.Yellow;
                                break;
                            }
                        case "Models\\container_1":
                        case "Models\\container_2":
                            {
                                tempColour = Color.Yellow;
                                break;
                            }
                        case "Models\\tree_1":
                            {
                                tempColour = Color.Green;
                                break;
                            }
                        case "Models\\wood_box_1":
                        case "Models\\wood_box_2":
                        case "Models\\wood_box_3":
                        case "Models\\wood_box_4":
                            {
                                tempColour = Color.Yellow;
                                break;
                            }
                        default:
                            {
                                tempColour = Color.Yellow;
                                break;
                            }
                    }
                    #endregion

                    m_Sprite.Draw(m_Marker,
                          new Rectangle(0 + XPos,0 + YPos, (int)m_MarkerDimensions.X, (int)m_MarkerDimensions.Y),
                          tempColour);
                }
            }

            //draw vehicle markers
            if (ShowVehicles)
            {
                foreach (VehiclePhysics v in m_VechicleList)
                {
                    if (v.m_Parent.IsAlive())
                    {
                        Vector2 temp = toMapScale(v.m_vehicle_physics.body.centerPosition);

                        int XPos = (int)(m_Offset.X + (m_MapDimensions.X / 2) + temp.X);
                        int YPos = (int)(m_Offset.Y + (m_MapDimensions.Y / 2) + temp.Y);

                        if (v.m_Parent.m_team == 1)
                        {
                            m_Sprite.Draw(m_VehicleMarker,
                              new Rectangle(0 + XPos, 0 + YPos, (int)m_MarkerDimensions.X + 4, (int)m_MarkerDimensions.Y + 4),
                              Color.Red);
                        }
                        else
                        {
                            m_Sprite.Draw(m_VehicleMarker,
                              new Rectangle(0 + XPos, 0 + YPos, (int)m_MarkerDimensions.X + 4, (int)m_MarkerDimensions.Y + 4),
                              Color.CadetBlue);
                        }
                    }
                }
            }

            m_Sprite.End();

            //reset variables
            g.graphics.GraphicsDevice.RenderState.DepthBufferEnable = true;
            g.graphics.GraphicsDevice.RenderState.AlphaBlendEnable = false;
            g.graphics.GraphicsDevice.RenderState.AlphaTestEnable = false;

        }
        public void Update()
        {

        }
    }
}
