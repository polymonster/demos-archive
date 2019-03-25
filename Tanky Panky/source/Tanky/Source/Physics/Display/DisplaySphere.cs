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
    /// Simple display object for spheres.  Creates a triangulated representation and keeps track of it during simulation.
    /// </remarks>
    public class DisplaySphere : DisplayObject
    {
        internal Sphere sphere;


        /// <summary>
        /// Creates the display object for the entity.
        /// </summary>
        /// <param name="s">Entity to draw.</param>
        /// <param name="graphics">Manager to use.</param>
        /// <param name="initShape">Whether or not to initialize the local shape vertices.</param>
        /// <param name="initVertexBuffer">Whether or not to initialize the local vertex buffer.</param>
        public DisplaySphere(Sphere s, GraphicsDeviceManager graphics, bool initShape, bool initVertexBuffer)
        {

            sphere = s;
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
        public static int numSides = 24;

        void initializeShape()
        {

            Vector3 current;
            Vector3 nextH;
            Vector3 nextV;
            Vector3 nextHV;
            float xPos1, xPos2, xPos21, xPos22, yPos1, yPos2, zPos1, zPos2, zPos21, zPos22;
            int numVerticesInColumn = 3 * numSides - 6;
            shapeVertices = new VertexPositionNormalTexture[numVerticesInColumn * numSides];
            float radius = sphere.radius + sphere.collisionMargin - sphere.allowedPenetration;
            float angleChange = (float)Math.PI / (numSides / 2);
            float theta;
            float phi;
            for (int j = 0; j < numSides; j++)
            {//per column
                for (int k = 1; k < numSides / 2 - 1; k++)
                {//per non-tri row
                    theta = j * angleChange;
                    phi = k * angleChange;
                    xPos1 = (float)(Math.Cos(theta) * Math.Sin(phi));
                    xPos2 = (float)(Math.Cos(theta + angleChange) * Math.Sin(phi));

                    xPos21 = (float)(Math.Cos(theta) * Math.Sin(phi + angleChange));
                    xPos22 = (float)(Math.Cos(theta + angleChange) * Math.Sin(phi + angleChange));

                    yPos1 = (float)Math.Cos(phi);
                    yPos2 = (float)Math.Cos(phi + angleChange);

                    zPos1 = (float)(Math.Sin(theta) * Math.Sin(phi));
                    zPos2 = (float)(Math.Sin(theta + angleChange) * Math.Sin(phi));

                    zPos21 = (float)(Math.Sin(theta) * Math.Sin(phi + angleChange));
                    zPos22 = (float)(Math.Sin(theta + angleChange) * Math.Sin(phi + angleChange));

                    current = new Vector3(xPos1, yPos1, zPos1);
                    nextH = new Vector3(xPos2, yPos1, zPos2);
                    nextV = new Vector3(xPos21, yPos2, zPos21);
                    nextHV = new Vector3(xPos22, yPos2, zPos22);

                    shapeVertices[numVerticesInColumn * j + (k - 1) * 6] = new VertexPositionNormalTexture(current * radius, current, textureCoords[0]);
                    shapeVertices[numVerticesInColumn * j + (k - 1) * 6 + 1] = new VertexPositionNormalTexture(nextHV * radius, nextHV, textureCoords[1]);
                    shapeVertices[numVerticesInColumn * j + (k - 1) * 6 + 2] = new VertexPositionNormalTexture(nextH * radius, nextH, textureCoords[2]);

                    shapeVertices[numVerticesInColumn * j + (k - 1) * 6 + 3] = new VertexPositionNormalTexture(current * radius, current, textureCoords[0]);
                    shapeVertices[numVerticesInColumn * j + (k - 1) * 6 + 4] = new VertexPositionNormalTexture(nextV * radius, nextV, textureCoords[1]);
                    shapeVertices[numVerticesInColumn * j + (k - 1) * 6 + 5] = new VertexPositionNormalTexture(nextHV * radius, nextHV, textureCoords[2]);

                    if (k == 1)
                    {
                        shapeVertices[numVerticesInColumn * j + (numSides / 2 - 2) * 6 + 0] = new VertexPositionNormalTexture(Vector3.Up * radius, Vector3.Up, textureCoords[0]);
                        shapeVertices[numVerticesInColumn * j + (numSides / 2 - 2) * 6 + 1] = new VertexPositionNormalTexture(current * radius, current, textureCoords[1]);
                        shapeVertices[numVerticesInColumn * j + (numSides / 2 - 2) * 6 + 2] = new VertexPositionNormalTexture(nextH * radius, nextH, textureCoords[2]);
                    }
                    else if (k == numSides / 2 - 2)
                    {
                        shapeVertices[numVerticesInColumn * j + (k) * 6 + 3] = new VertexPositionNormalTexture(Vector3.Down * radius, Vector3.Down, textureCoords[0]);
                        shapeVertices[numVerticesInColumn * j + (k) * 6 + 5] = new VertexPositionNormalTexture(nextV * radius, nextV, textureCoords[1]);
                        shapeVertices[numVerticesInColumn * j + (k) * 6 + 4] = new VertexPositionNormalTexture(nextHV * radius, nextHV, textureCoords[2]);
                    }
                }
            }
        }
        /// <summary>
        /// Recalculates and returns the vertices of the object.
        /// </summary>
        /// <returns>Vertices composing the display object.</returns>
        public override VertexPositionNormalTexture[] getVertices()
        {
            Vector3 current;
            Vector3 nextH;
            Vector3 nextV;
            Vector3 nextHV;
            float xPos1, xPos2, xPos21, xPos22, yPos1, yPos2, zPos1, zPos2, zPos21, zPos22;
            int numVerticesInColumn = 3 * numSides - 6;
            VertexPositionNormalTexture[] toReturn = new VertexPositionNormalTexture[numVerticesInColumn * numSides];
            float radius = sphere.radius + sphere.collisionMargin - sphere.allowedPenetration;
            float angleChange = (float)Math.PI / (numSides / 2);
            float theta;
            float phi;
            for (int j = 0; j < numSides; j++)
            {//per column
                for (int k = 1; k < numSides / 2 - 1; k++)
                {//per non-tri row
                    theta = j * angleChange;
                    phi = k * angleChange;
                    xPos1 = (float)(Math.Cos(theta) * Math.Sin(phi));
                    xPos2 = (float)(Math.Cos(theta + angleChange) * Math.Sin(phi));

                    xPos21 = (float)(Math.Cos(theta) * Math.Sin(phi + angleChange));
                    xPos22 = (float)(Math.Cos(theta + angleChange) * Math.Sin(phi + angleChange));

                    yPos1 = (float)Math.Cos(phi);
                    yPos2 = (float)Math.Cos(phi + angleChange);

                    zPos1 = (float)(Math.Sin(theta) * Math.Sin(phi));
                    zPos2 = (float)(Math.Sin(theta + angleChange) * Math.Sin(phi));

                    zPos21 = (float)(Math.Sin(theta) * Math.Sin(phi + angleChange));
                    zPos22 = (float)(Math.Sin(theta + angleChange) * Math.Sin(phi + angleChange));

                    current = new Vector3(xPos1, yPos1, zPos1);
                    nextH = new Vector3(xPos2, yPos1, zPos2);
                    nextV = new Vector3(xPos21, yPos2, zPos21);
                    nextHV = new Vector3(xPos22, yPos2, zPos22);

                    toReturn[numVerticesInColumn * j + (k - 1) * 6] = new VertexPositionNormalTexture(current * radius, current, textureCoords[0]);
                    toReturn[numVerticesInColumn * j + (k - 1) * 6 + 1] = new VertexPositionNormalTexture(nextHV * radius, nextHV, textureCoords[1]);
                    toReturn[numVerticesInColumn * j + (k - 1) * 6 + 2] = new VertexPositionNormalTexture(nextH * radius, nextH, textureCoords[2]);

                    toReturn[numVerticesInColumn * j + (k - 1) * 6 + 3] = new VertexPositionNormalTexture(current * radius, current, textureCoords[0]);
                    toReturn[numVerticesInColumn * j + (k - 1) * 6 + 4] = new VertexPositionNormalTexture(nextV * radius, nextV, textureCoords[1]);
                    toReturn[numVerticesInColumn * j + (k - 1) * 6 + 5] = new VertexPositionNormalTexture(nextHV * radius, nextHV, textureCoords[2]);

                    if (k == 1)
                    {
                        toReturn[numVerticesInColumn * j + (numSides / 2 - 2) * 6 + 0] = new VertexPositionNormalTexture(Vector3.Up * radius, Vector3.Up, textureCoords[0]);
                        toReturn[numVerticesInColumn * j + (numSides / 2 - 2) * 6 + 1] = new VertexPositionNormalTexture(current * radius, current, textureCoords[1]);
                        toReturn[numVerticesInColumn * j + (numSides / 2 - 2) * 6 + 2] = new VertexPositionNormalTexture(nextH * radius, nextH, textureCoords[2]);
                    }
                    else if (k == numSides / 2 - 2)
                    {
                        toReturn[numVerticesInColumn * j + (k) * 6 + 3] = new VertexPositionNormalTexture(Vector3.Down * radius, Vector3.Down, textureCoords[0]);
                        toReturn[numVerticesInColumn * j + (k) * 6 + 5] = new VertexPositionNormalTexture(nextV * radius, nextV, textureCoords[1]);
                        toReturn[numVerticesInColumn * j + (k) * 6 + 4] = new VertexPositionNormalTexture(nextHV * radius, nextHV, textureCoords[2]);
                    }
                }
            }
            return toReturn;
        }

        /// <summary>
        /// Gets the entity wrapped by this display object.
        /// </summary>
        /// <returns>Entity wrapped by this display object.</returns>
        public override Entity getEntity()
        {
            return sphere;
        }
    }
}
