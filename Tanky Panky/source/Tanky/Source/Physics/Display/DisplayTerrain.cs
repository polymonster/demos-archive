/*
   Tanky.Source.Physics.Display Project

      Copyright (C) 2008 Bepu Entertainment LLC

      This demonstration software, excluding the BEPUphysics.dll and BEPUphysics.xml (which are handled independently by their accompanying license),
      is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages
      arising from the use of this software.

      Permission is granted to anyone to use this software for any purpose,
      including commercial applications, and to alter it and redistribute it
      freely, subject to the following restrictions:

      1. The origin of this software must not be misrepresented; you must not
         claim that you wrote the original software. If you use this software
         in a product, an acknowledgment in the product documentation would be
         appreciated but is not required.
      2. Altered source versions must be plainly marked as such, and must not be
         misrepresented as being the original software.
      3. This notice may not be removed or altered from any source distribution.

    Contact us at:
    contact@bepu-games.com
    Ross Nordby
    norbo@bepu-games.com
    Harrison Nordby
    squashwell@bepu-games.com
 */

using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Tanky.Source.Physics.Display
{
    /// <remarks>
    /// Handles the graphical representation of a terrain.
    /// </remarks>
    public class DisplayTerrain : DisplayObject
    {
        BEPUphysics.Terrain terrain;
        /// <summary>
        /// Creates the display object for the terrain.
        /// </summary>
        /// <param name="t">Terrain to draw.</param>
        /// <param name="graphics">Manager to use.</param>
        public DisplayTerrain(BEPUphysics.Terrain t, GraphicsDeviceManager graphics)
        {

            terrain = t;
            myGraphics = graphics;
            textureCoords[0] = new Vector2(0, 0);
            textureCoords[1] = new Vector2(1, 0);
            textureCoords[2] = new Vector2(0, 1);
            initializeEffect();
            initializeShape();
        }

        /// <summary>
        /// Configures the effect used to display the body to default values.
        /// </summary>
        void initializeEffect()
        {

        }
        void initializeShape()
        {



            Vector3[] localVertices;
            Vector3 normal;
            List<VertexPositionNormalTexture> shapeVerts = new List<VertexPositionNormalTexture>();
            for (int i = 0; i < terrain.heights.GetLength(0) - 1; i++)
            {
                for (int k = 0; k < terrain.heights.GetLength(1) - 1; k++)
                {
                    localVertices = new Vector3[] {
                                    new Vector3(i * terrain.getSpacingX(),       terrain.heights[i, k],          k * terrain.getSpacingZ()), 
                                    new Vector3((i + 1) * terrain.getSpacingX(), terrain.heights[i + 1, k],      k * terrain.getSpacingZ()),  
                                    new Vector3(i * terrain.getSpacingX(),       terrain.heights[i, k + 1],      (k + 1) * terrain.getSpacingZ()), 
                                    new Vector3((i + 1) * terrain.getSpacingX(), terrain.heights[i + 1, k + 1],  (k + 1) * terrain.getSpacingZ())};



                    for (int j = 0; j < terrain.quadTriangles.Length; j++)
                    {

                        if (terrain.quadTriangles[j] == 0)
                            normal = terrain.getNormal(i, k);
                        else if (terrain.quadTriangles[j] == 1)
                            normal = terrain.getNormal(i + 1, k);
                        else if (terrain.quadTriangles[j] == 2)
                            normal = terrain.getNormal(i, k + 1);
                        else
                            normal = terrain.getNormal(i + 1, k + 1);
                        shapeVerts.Add(new VertexPositionNormalTexture(localVertices[terrain.quadTriangles[j]], normal, new Vector2(i, k)));
                    }

                }
            }

            vertexBuffer = new VertexBuffer(myGraphics.GraphicsDevice, VertexPositionNormalTexture.SizeInBytes * shapeVerts.Count, BufferUsage.WriteOnly);
            shapeVertices = shapeVerts.ToArray();
            vertexBuffer.SetData<VertexPositionNormalTexture>(shapeVertices);
        }
        /// <summary>
        /// Returns the vertices of the object.
        /// </summary>
        /// <returns>Vertices composing the display object.</returns>
        public override VertexPositionNormalTexture[] getVertices()
        {
            return shapeVertices;
        }


        /// <summary>
        /// Not applicable to DisplayTerrain.
        /// </summary>
        /// <returns>Null.</returns>
        public override BEPUphysics.Entity getEntity()
        {
            return null;
        }
        /// <summary>
        /// Gets the bounding box of the terrain.
        /// </summary>
        /// <returns>Bounding box of the terrain..</returns>
        public override BoundingBox getBoundingBox()
        {
            return terrain.boundingBox;
        }
        /// <summary>
        /// Gets the position of the terrain.
        /// </summary>
        /// <returns>Position of the terrain.</returns>
        public override Vector3 getPosition()
        {
            return terrain.getPosition();
        }
        /// <summary>
        /// Gets the orientation of the terrain, which as of v0.6.1 is always identity.
        /// </summary>
        /// <returns>Matrix.Identity.</returns>
        public override Matrix getRotation()
        {
            return Matrix.Identity;
        }
    }
}
