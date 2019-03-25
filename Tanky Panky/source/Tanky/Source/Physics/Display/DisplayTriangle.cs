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
using BEPUphysics;

namespace Tanky.Source.Physics.Display
{
    /// <remarks>
    /// Simple display class for triangles.  Creates a polygonal figure based on the triangle's data and keeps track of it during the simulation.
    /// </remarks>
    public class DisplayTriangle : DisplayObject
    {
        Triangle tri;


        /// <summary>
        /// Creates the display object for the entity.
        /// </summary>
        /// <param name="t">Entity to draw.</param>
        /// <param name="graphics">Manager to use.</param>
        /// <param name="initShape">Whether or not to initialize the local shape vertices.</param>
        /// <param name="initVertexBuffer">Whether or not to initialize the local vertex buffer.</param>
        public DisplayTriangle(Triangle t, GraphicsDeviceManager graphics, bool initShape, bool initVertexBuffer)
        {

            tri = t;
            myGraphics = graphics;
            textureCoords[0] = new Vector2(0, 0);
            textureCoords[1] = new Vector2(1, 0);
            textureCoords[2] = new Vector2(0, 1);
            initializeEffect();

            if (initShape || initVertexBuffer)
                initializeShape();
            if (initVertexBuffer)
            {
                vertexBuffer = new VertexBuffer(myGraphics.GraphicsDevice, VertexPositionNormalTexture.SizeInBytes * shapeVertices.Length, BufferUsage.WriteOnly);
                vertexBuffer.SetData<VertexPositionNormalTexture>(shapeVertices);
            }
        }

        /// <summary>
        /// Configures the effect used to display the body to default values.
        /// </summary>
        void initializeEffect()
        {

        }
        void initializeShape()
        {
            vertexBuffer = new VertexBuffer(myGraphics.GraphicsDevice, VertexPositionNormalTexture.SizeInBytes * 6, BufferUsage.WriteOnly);

            shapeVertices = new VertexPositionNormalTexture[6];
            //Triangles are doubled up due to backface culling killing off the opposite side.
            shapeVertices[0] = new VertexPositionNormalTexture(tri.localVertices[0], tri.normal, textureCoords[0]);
            shapeVertices[1] = new VertexPositionNormalTexture(tri.localVertices[1], tri.normal, textureCoords[1]);
            shapeVertices[2] = new VertexPositionNormalTexture(tri.localVertices[2], tri.normal, textureCoords[2]);
            shapeVertices[3] = new VertexPositionNormalTexture(tri.localVertices[0], tri.normal, textureCoords[0]);
            shapeVertices[5] = new VertexPositionNormalTexture(tri.localVertices[1], tri.normal, textureCoords[1]);
            shapeVertices[4] = new VertexPositionNormalTexture(tri.localVertices[2], tri.normal, textureCoords[2]);
            vertexBuffer.SetData<VertexPositionNormalTexture>(shapeVertices);
        }
        /// <summary>
        /// Recalculates and returns the vertices of the object.
        /// </summary>
        /// <returns>Vertices composing the display object.</returns>
        public override VertexPositionNormalTexture[] getVertices()
        {
            VertexPositionNormalTexture[] toReturn = new VertexPositionNormalTexture[6];

            toReturn[0] = new VertexPositionNormalTexture(tri.vertices[0], tri.normal, textureCoords[0]);
            toReturn[1] = new VertexPositionNormalTexture(tri.vertices[1], tri.normal, textureCoords[1]);
            toReturn[2] = new VertexPositionNormalTexture(tri.vertices[2], tri.normal, textureCoords[2]);
            toReturn[3] = new VertexPositionNormalTexture(tri.vertices[0], tri.normal, textureCoords[0]);
            toReturn[5] = new VertexPositionNormalTexture(tri.vertices[1], tri.normal, textureCoords[1]);
            toReturn[4] = new VertexPositionNormalTexture(tri.vertices[2], tri.normal, textureCoords[2]);

            return toReturn;
        }


        /// <summary>
        /// Gets the entity wrapped by this display object.
        /// </summary>
        /// <returns>Entity wrapped by this display object.</returns>
        public override Entity getEntity()
        {
            return tri;
        }
    }
}
