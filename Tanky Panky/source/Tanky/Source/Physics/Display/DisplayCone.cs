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
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;
using BEPUphysics;

namespace Tanky.Source.Physics.Display
{
    /// <remarks>
    /// Simple display object for cones.  Creates a triangulated representation and keeps track of it during simulation.
    /// </remarks>
    public class DisplayCone : DisplayObject
    {
        internal Cone cone;


        /// <summary>
        /// Creates the display object for the entity.
        /// </summary>
        /// <param name="c">Entity to draw.</param>
        /// <param name="graphics">Manager to use.</param>
        /// <param name="initShape">Whether or not to initialize the local shape vertices.</param>
        /// <param name="initVertexBuffer">Whether or not to initialize the local vertex buffer.</param>
        public DisplayCone(Cone c, GraphicsDeviceManager graphics, bool initShape, bool initVertexBuffer)
        {

            cone = c;
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
        /// <summary>
        /// Number of sides to build geometry with.
        /// </summary>
        public static int numSides = 16;

        void initializeShape()
        {
            Vector3 bottomCenter = new Vector3(0, -cone.height / 4 - cone.collisionMargin + cone.allowedPenetration, 0);
            Vector3 apex = new Vector3(0, 3 * cone.height / 4 + cone.collisionMargin - cone.allowedPenetration, 0);
            Vector3 currentVector;
            Vector3 nextVector;
            float xPos1, xPos2, zPos1, zPos2;
            float radius = cone.radius + cone.collisionMargin - cone.allowedPenetration;
            shapeVertices = new VertexPositionNormalTexture[numSides * 6];
            float angleChange = (float)Math.PI / (numSides / 2);
            float angle;
            for (int k = 0; k < numSides; k++)
            {
                angle = k * angleChange;
                xPos1 = (float)Math.Cos(angle);
                xPos2 = (float)Math.Cos(angle + angleChange);
                zPos1 = (float)Math.Sin(angle);
                zPos2 = (float)Math.Sin(angle + angleChange);
                currentVector = new Vector3(xPos1, 0, zPos1);
                nextVector = new Vector3(xPos2, 0, zPos2);

                shapeVertices[k * 6] = new VertexPositionNormalTexture(bottomCenter, Vector3.Down, textureCoords[0]);
                shapeVertices[k * 6 + 2] = new VertexPositionNormalTexture(bottomCenter + currentVector * radius, Vector3.Down, textureCoords[1]);
                shapeVertices[k * 6 + 1] = new VertexPositionNormalTexture(bottomCenter + nextVector * radius, Vector3.Down, textureCoords[2]);

                shapeVertices[k * 6 + 3] = new VertexPositionNormalTexture(apex, Vector3.Up, textureCoords[0]);
                shapeVertices[k * 6 + 4] = new VertexPositionNormalTexture(bottomCenter + currentVector * radius, currentVector, textureCoords[1]);
                shapeVertices[k * 6 + 5] = new VertexPositionNormalTexture(bottomCenter + nextVector * radius, nextVector, textureCoords[2]);

            }

        }

        /// <summary>
        /// Recalculates and returns the vertices of the object.
        /// </summary>
        /// <returns>Vertices composing the display object.</returns>
        public override VertexPositionNormalTexture[] getVertices()
        {
            Vector3 bottomCenter = new Vector3(0, -cone.height / 4 - cone.collisionMargin + cone.allowedPenetration, 0);
            Vector3 apex = new Vector3(0, 3 * cone.height / 4 + cone.collisionMargin - cone.allowedPenetration, 0);
            Vector3 currentVector;
            Vector3 nextVector;
            float xPos1, xPos2, zPos1, zPos2;
            float radius = cone.radius + cone.collisionMargin - cone.allowedPenetration;
            VertexPositionNormalTexture[] toReturn = new VertexPositionNormalTexture[numSides * 6];
            float angleChange = (float)Math.PI / (numSides / 2);
            float angle;
            for (int k = 0; k < numSides; k++)
            {
                angle = k * angleChange;
                xPos1 = (float)Math.Cos(angle);
                xPos2 = (float)Math.Cos(angle + angleChange);
                zPos1 = (float)Math.Sin(angle);
                zPos2 = (float)Math.Sin(angle + angleChange);
                currentVector = new Vector3(xPos1, 0, zPos1);
                nextVector = new Vector3(xPos2, 0, zPos2);

                toReturn[k * 6] = new VertexPositionNormalTexture(bottomCenter, Vector3.Down, textureCoords[0]);
                toReturn[k * 6 + 2] = new VertexPositionNormalTexture(bottomCenter + currentVector * radius, Vector3.Down, textureCoords[1]);
                toReturn[k * 6 + 1] = new VertexPositionNormalTexture(bottomCenter + nextVector * radius, Vector3.Down, textureCoords[2]);

                toReturn[k * 6 + 3] = new VertexPositionNormalTexture(apex, Vector3.Up, textureCoords[0]);
                toReturn[k * 6 + 4] = new VertexPositionNormalTexture(bottomCenter + currentVector * radius, currentVector, textureCoords[1]);
                toReturn[k * 6 + 5] = new VertexPositionNormalTexture(bottomCenter + nextVector * radius, nextVector, textureCoords[2]);

            }
            return toReturn;
        }


        /// <summary>
        /// Gets the entity wrapped by this display object.
        /// </summary>
        /// <returns>Entity wrapped by this display object.</returns>
        public override Entity getEntity()
        {
            return cone;
        }
    }
}
