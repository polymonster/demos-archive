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
using Microsoft.Xna.Framework.Input;
using System.Diagnostics;
using BEPUphysics;

namespace Tanky.Source.Physics.Display
{
    /// <remarks>
    /// Simple display class for fluids.  Creates a representation based on the surface triangles and keeps track of it during the simulation.
    /// </remarks>
    public class DisplayFluid : DisplayObject
    {
        FluidVolume fluid;

        /// <summary>
        /// Creates the display object for the entity.
        /// </summary>
        /// <param name="b">Entity to draw.</param>
        /// <param name="graphics">Manager to use.</param>
        /// <param name="initShape">Whether or not to initialize the local shape vertices.</param>
        /// <param name="initVertexBuffer">Whether or not to initialize the local vertex buffer.</param>
        public DisplayFluid(FluidVolume volume, GraphicsDeviceManager graphics, bool initShape, bool initVertexBuffer)
        {

            fluid = volume;
            myGraphics = graphics;
            textureCoords[0] = new Vector2(0, 0);
            textureCoords[1] = new Vector2(1, 0);
            textureCoords[2] = new Vector2(0, 1);

            if (initShape || initVertexBuffer)
                initializeShape();
            if (initVertexBuffer)
            {
                vertexBuffer = new VertexBuffer(myGraphics.GraphicsDevice, VertexPositionNormalTexture.SizeInBytes * shapeVertices.Length, BufferUsage.WriteOnly);
                vertexBuffer.SetData<VertexPositionNormalTexture>(shapeVertices);
            }

        }

        void initializeShape()
        {
            shapeVertices = new VertexPositionNormalTexture[6 * fluid.triangles.Count];

            for (int k = 0; k < fluid.triangles.Count; k++)
            {
                shapeVertices[k * 6] = (new VertexPositionNormalTexture(fluid.triangles[k][0], fluid.normal, textureCoords[0]));
                shapeVertices[k * 6 + 1] = (new VertexPositionNormalTexture(fluid.triangles[k][1], fluid.normal, textureCoords[1]));
                shapeVertices[k * 6 + 2] = (new VertexPositionNormalTexture(fluid.triangles[k][2], fluid.normal, textureCoords[2]));
                shapeVertices[k * 6 + 3] = (new VertexPositionNormalTexture(fluid.triangles[k][0], fluid.normal, textureCoords[0]));
                shapeVertices[k * 6 + 4] = (new VertexPositionNormalTexture(fluid.triangles[k][2], fluid.normal, textureCoords[2]));
                shapeVertices[k * 6 + 5] = (new VertexPositionNormalTexture(fluid.triangles[k][1], fluid.normal, textureCoords[1]));
            }


        }

        /// <summary>
        /// Recalculates and returns the vertices of the object.
        /// </summary>
        /// <returns>Vertices composing the display object.</returns>
        public override VertexPositionNormalTexture[] getVertices()
        {

            VertexPositionNormalTexture[] toReturn = new VertexPositionNormalTexture[6 * fluid.triangles.Count];

            for (int k = 0; k < fluid.triangles.Count; k++)
            {
                toReturn[k * 6] = (new VertexPositionNormalTexture(fluid.triangles[k][0], fluid.normal, textureCoords[0]));
                toReturn[k * 6 + 1] = (new VertexPositionNormalTexture(fluid.triangles[k][1], fluid.normal, textureCoords[1]));
                toReturn[k * 6 + 2] = (new VertexPositionNormalTexture(fluid.triangles[k][2], fluid.normal, textureCoords[2]));
                toReturn[k * 6 + 3] = (new VertexPositionNormalTexture(fluid.triangles[k][0], fluid.normal, textureCoords[0]));
                toReturn[k * 6 + 4] = (new VertexPositionNormalTexture(fluid.triangles[k][2], fluid.normal, textureCoords[2]));
                toReturn[k * 6 + 5] = (new VertexPositionNormalTexture(fluid.triangles[k][1], fluid.normal, textureCoords[1]));
            }
            return toReturn;
        }


        /// <summary>
        /// Not a valid operation for fluid volumes, returns null.
        /// </summary>
        /// <returns>Null.</returns>
        public override Entity getEntity()
        {
            return null;
        }

        public override BoundingBox getBoundingBox()
        {
            return fluid.boundingBox;
        }
        /// <summary>
        /// Returns Vector3.Zero at all times; positions are defined directly by the triangles composing the surface.
        /// </summary>
        /// <returns>Vector3.Zero.</returns>
        public override Vector3 getPosition()
        {
            return Vector3.Zero;
        }
        /// <summary>
        /// Returns Matrix.Identity at all times; positions are defined directly by the triangles composing the surface.
        /// </summary>
        /// <returns>Matrix.Identity.</returns>
        public override Matrix getRotation()
        {
            return Matrix.Identity;
        }

    }
}
