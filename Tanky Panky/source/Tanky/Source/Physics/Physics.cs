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
using System.IO;
using BEPUphysics;
using Tanky.Source.Physics.Display;

namespace Tanky
{
    public class Physics
    {
        // Physics collision space
        public Space m_collisionSpace;

        // Physics terrian collision mesh
        private BEPUphysics.Terrain m_terrain;  // Explicit because we already have a class in Tanky called Terrain

        // Graphical display of terrian collision mesh
        private DisplayTerrain m_terrian_display;

        // Draws the physics stuff
        public EntityDrawer m_entityDrawer;

        List<Texture2D> m_entityTextures;

        public Physics()
        {
            SetUpSpace(); 
        }

        private void SetUpSpace()
        {
            m_collisionSpace = new BEPUphysics.Space(new PersistentUniformGrid(100)); //Create the world for the objects to live in, and use the PersistentUniformGrid broadphase system.  Try the BruteForce method for comparison.

            //Creates the vehicle.
            //vehicle = new VehicleInput(space, camera, entityRenderer, wheelModel, wheelTexture);
            
            //The following variables all come preset and don't have to be assigned values, but they're here for quick changing.
            //Each default has been defined with a general case in mind, so they work in the majority of situations- but they're available to change if needed.
            m_collisionSpace.simulationSettings.gravity = new Vector3(0, -10.81f, 0f); //If left unset, the default value is (0,0,0).

            m_collisionSpace.simulationSettings.timeScale = 1f; //If left unset, the default value is 1.
            m_collisionSpace.simulationSettings.iterations = 15; //If left unset, the default value is 15.
            //Fiddling with margins and position correction is one way to tune the engine for the specific game/simulation.
            m_collisionSpace.simulationSettings.defaultMargin = .04f;//Defaults to .04f
            m_collisionSpace.simulationSettings.defaultAllowedPenetration = .025f;//Defaults to .025f
            m_collisionSpace.simulationSettings.useSplitImpulsePositionCorrection = false; //Defaults to false.

            m_entityTextures = new List<Texture2D>();
            m_entityTextures.Add(g.content.Load<Texture2D>("Textures//wheel"));

            // Create the physics renderer
            m_entityDrawer = new EntityDrawer(m_entityTextures, false); // TODO: Needs a list of textures!?

            SetUpTerrianCollisionMesh();
        }

        public void SetUpTerrianCollisionMesh()
        {
            //Create the terrain.

            // TODO: These prob shouldnt be hard-coded!
            float aYSpace = 50 * g.scaleFactor;
            float aXSpace = 50 * g.scaleFactor;

            int aXLength = 100;
            int aYLength = 100;

            m_terrain = new BEPUphysics.Terrain(.5f,                                                                    
                                                0,
                                                new Vector3(-aXLength * aXSpace / 2, 0, -aYLength * aYSpace / 2), 
                                                .02f,
                                                .01f,
                                                2.0f,    //Friction 0.5f
                                                0,      //Bounciness
                                                50);

            m_terrain.setData(g.level.get_terrain_heights(), QuadFormats.lowerLeftUpperRight, aXSpace, aYSpace);


            m_collisionSpace.add(m_terrain);
            m_terrain.useFaceNormalWithinAngle = (float)Math.PI / 2; //This terrain shape is very flat, so the triangle normals can be safely used in every case.
            //Generally, it is fine to use the triangle normals, but if the terrain has extremely sharp cliffs and features you may notice
            //some 'stickiness' or other more severe problems.  Triangles can be set to use the normal contact normals always by setting the terrain.tryToUseTriangleNormals to false.
            //The benefit of using the triangles' normals is smoother behavior for sliding objects at the boundary between triangles.

            // Graphical display of terrian collision mesh
            m_terrian_display = new DisplayTerrain(m_terrain, g.graphics);

            // Add it to the list of stuff to draw
            m_entityDrawer.addDisplayObject(m_terrian_display);
            m_entityDrawer.isWireFrameMode = true;

        }

        public void Draw()
        {
            g.graphics.GraphicsDevice.RenderState.DepthBufferEnable = true;
            g.graphics.GraphicsDevice.RenderState.AlphaBlendEnable = false;

            //List<VertexPositionColor> boundingBoxLines = new List<VertexPositionColor>();
            m_entityDrawer.draw(ref g.camera.m_view_matrix, ref g.camera.m_projection_matrix, g.camera.frustum);

        }

        public void Update(GameTime gameTime)
        {
            m_collisionSpace.update(gameTime);
        }
    }
}
