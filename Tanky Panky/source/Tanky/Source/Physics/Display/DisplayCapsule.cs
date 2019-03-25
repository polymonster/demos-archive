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
    /// Simple display object for capsules.  Creates a triangulated representation and keeps track of it during simulation.
    /// </remarks>
    public class DisplayCapsule : DisplayObject
    {
        internal Capsule capsule;


        /// <summary>
        /// Creates the display object for the entity.
        /// </summary>
        /// <param name="c">Entity to draw.</param>
        /// <param name="graphics">Manager to use.</param>
        /// <param name="initShape">Whether or not to initialize the local shape vertices.</param>
        /// <param name="initVertexBuffer">Whether or not to initialize the local vertex buffer.</param>
        public DisplayCapsule(Capsule c, GraphicsDeviceManager graphics, bool initShape, bool initVertexBuffer)
        {

            capsule = c;
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
            Vector3 lowerCurrent;
            Vector3 lowerNextH;
            Vector3 lowerNextV;
            Vector3 lowerNextHV;
            Vector3 currentNormal;
            Vector3 nextHNormal;
            Vector3 nextVNormal;
            Vector3 nextHVNormal;
            Vector3 lowerCurrentNormal;
            Vector3 lowerNextHNormal;
            Vector3 lowerNextVNormal;
            Vector3 lowerNextHVNormal;
            float xPos1, xPos2, xPos21, xPos22, yPos1, yPos2, zPos1, zPos2, zPos21, zPos22;
            int numVerticesInColumn = 3 * numSides;
            shapeVertices = new VertexPositionNormalTexture[numVerticesInColumn * numSides];
            float halfLength = capsule.length / 2f;
            float angleChange = (float)Math.PI / (numSides / 2);
            float theta;
            float phi;
            float radius = capsule.radius + capsule.collisionMargin - capsule.allowedPenetration;
            for (int j = 0; j < numSides; j++)
            {//per column
                for (int k = 1; k < numSides / 4 - 0; k++)
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

                    current = new Vector3(xPos1, yPos1 + halfLength, zPos1);
                    nextH = new Vector3(xPos2, yPos1 + halfLength, zPos2);
                    nextV = new Vector3(xPos21, yPos2 + halfLength, zPos21);
                    nextHV = new Vector3(xPos22, yPos2 + halfLength, zPos22);
                    lowerCurrent = new Vector3(xPos1, -yPos1 - halfLength, zPos1);
                    lowerNextH = new Vector3(xPos2, -yPos1 - halfLength, zPos2);
                    lowerNextV = new Vector3(xPos21, -yPos2 - halfLength, zPos21);
                    lowerNextHV = new Vector3(xPos22, -yPos2 - halfLength, zPos22);

                    currentNormal = new Vector3(xPos1, yPos1, zPos1);
                    nextHNormal = new Vector3(xPos2, yPos1, zPos2);
                    nextVNormal = new Vector3(xPos21, yPos2, zPos21);
                    nextHVNormal = new Vector3(xPos22, yPos2, zPos22);
                    lowerCurrentNormal = new Vector3(xPos1, -yPos1, zPos1);
                    lowerNextHNormal = new Vector3(xPos2, -yPos1, zPos2);
                    lowerNextVNormal = new Vector3(xPos21, -yPos2, zPos21);
                    lowerNextHVNormal = new Vector3(xPos22, -yPos2, zPos22);

                    //Upper Cap
                    shapeVertices[numVerticesInColumn * j + (k - 1) * 12] = new VertexPositionNormalTexture(current * radius, currentNormal, textureCoords[0]);
                    shapeVertices[numVerticesInColumn * j + (k - 1) * 12 + 1] = new VertexPositionNormalTexture(nextHV * radius, nextHVNormal, textureCoords[1]);
                    shapeVertices[numVerticesInColumn * j + (k - 1) * 12 + 2] = new VertexPositionNormalTexture(nextH * radius, nextHNormal, textureCoords[2]);

                    shapeVertices[numVerticesInColumn * j + (k - 1) * 12 + 3] = new VertexPositionNormalTexture(current * radius, currentNormal, textureCoords[0]);
                    shapeVertices[numVerticesInColumn * j + (k - 1) * 12 + 4] = new VertexPositionNormalTexture(nextV * radius, nextVNormal, textureCoords[1]);
                    shapeVertices[numVerticesInColumn * j + (k - 1) * 12 + 5] = new VertexPositionNormalTexture(nextHV * radius, nextHVNormal, textureCoords[2]);

                    if (k == 1)
                    {
                        shapeVertices[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 0] = new VertexPositionNormalTexture(new Vector3(0, radius + halfLength, 0), Vector3.Up, textureCoords[0]);
                        shapeVertices[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 1] = new VertexPositionNormalTexture(current * radius, currentNormal, textureCoords[1]);
                        shapeVertices[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 2] = new VertexPositionNormalTexture(nextH * radius, nextHNormal, textureCoords[2]);
                    }


                    //Body
                    if (k == numSides / 4 - 1)
                    {
                        shapeVertices[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 6] = new VertexPositionNormalTexture(lowerNextV * radius, lowerNextVNormal, textureCoords[0]);
                        shapeVertices[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 7] = new VertexPositionNormalTexture(lowerNextHV * radius, lowerNextHVNormal, textureCoords[1]);
                        shapeVertices[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 8] = new VertexPositionNormalTexture(nextV * radius, nextVNormal, textureCoords[2]);

                        shapeVertices[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 9] = new VertexPositionNormalTexture(nextV * radius, nextVNormal, textureCoords[0]);
                        shapeVertices[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 11] = new VertexPositionNormalTexture(nextHV * radius, nextHVNormal, textureCoords[1]);
                        shapeVertices[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 10] = new VertexPositionNormalTexture(lowerNextHV * radius, lowerNextHVNormal, textureCoords[2]);
                    }

                    //Lower Cap
                    shapeVertices[numVerticesInColumn * j + (k - 1) * 12 + 6] = new VertexPositionNormalTexture(lowerCurrent * radius, lowerCurrentNormal, textureCoords[0]);
                    shapeVertices[numVerticesInColumn * j + (k - 1) * 12 + 8] = new VertexPositionNormalTexture(lowerNextHV * radius, lowerNextHVNormal, textureCoords[1]);
                    shapeVertices[numVerticesInColumn * j + (k - 1) * 12 + 7] = new VertexPositionNormalTexture(lowerNextH * radius, lowerNextHNormal, textureCoords[2]);

                    shapeVertices[numVerticesInColumn * j + (k - 1) * 12 + 9] = new VertexPositionNormalTexture(lowerCurrent * radius, lowerCurrentNormal, textureCoords[0]);
                    shapeVertices[numVerticesInColumn * j + (k - 1) * 12 + 11] = new VertexPositionNormalTexture(lowerNextV * radius, lowerNextVNormal, textureCoords[1]);
                    shapeVertices[numVerticesInColumn * j + (k - 1) * 12 + 10] = new VertexPositionNormalTexture(lowerNextHV * radius, lowerNextHVNormal, textureCoords[2]);


                    if (k == 1)
                    {
                        shapeVertices[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 3] = new VertexPositionNormalTexture(new Vector3(0, -radius - halfLength, 0), Vector3.Down, textureCoords[0]);
                        shapeVertices[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 5] = new VertexPositionNormalTexture(lowerCurrent * radius, lowerCurrentNormal, textureCoords[1]);
                        shapeVertices[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 4] = new VertexPositionNormalTexture(lowerNextH * radius, lowerNextHNormal, textureCoords[2]);
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
            Matrix world = capsule.orientationMatrix * Matrix.CreateTranslation(capsule.centerPosition);
            Vector3 current;
            Vector3 nextH;
            Vector3 nextV;
            Vector3 nextHV;
            Vector3 lowerCurrent;
            Vector3 lowerNextH;
            Vector3 lowerNextV;
            Vector3 lowerNextHV;
            Vector3 currentNormal;
            Vector3 nextHNormal;
            Vector3 nextVNormal;
            Vector3 nextHVNormal;
            Vector3 lowerCurrentNormal;
            Vector3 lowerNextHNormal;
            Vector3 lowerNextVNormal;
            Vector3 lowerNextHVNormal;
            float xPos1, xPos2, xPos21, xPos22, yPos1, yPos2, zPos1, zPos2, zPos21, zPos22;
            int numVerticesInColumn = 3 * numSides;
            VertexPositionNormalTexture[] toReturn = new VertexPositionNormalTexture[numVerticesInColumn * numSides];
            float halfLength = capsule.length / 2f;
            float angleChange = (float)Math.PI / (numSides / 2);
            float theta;
            float phi;
            for (int j = 0; j < numSides; j++)
            {//per column
                for (int k = 1; k < numSides / 4 - 0; k++)
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

                    current = Vector3.Transform(new Vector3(xPos1, yPos1 + halfLength, zPos1), world);
                    nextH = Vector3.Transform(new Vector3(xPos2, yPos1 + halfLength, zPos2), world);
                    nextV = Vector3.Transform(new Vector3(xPos21, yPos2 + halfLength, zPos21), world);
                    nextHV = Vector3.Transform(new Vector3(xPos22, yPos2 + halfLength, zPos22), world);
                    lowerCurrent = Vector3.Transform(new Vector3(xPos1, -yPos1 - halfLength, zPos1), world);
                    lowerNextH = Vector3.Transform(new Vector3(xPos2, -yPos1 - halfLength, zPos2), world);
                    lowerNextV = Vector3.Transform(new Vector3(xPos21, -yPos2 - halfLength, zPos21), world);
                    lowerNextHV = Vector3.Transform(new Vector3(xPos22, -yPos2 - halfLength, zPos22), world);

                    currentNormal = Vector3.Transform(new Vector3(xPos1, yPos1, zPos1), capsule.orientationMatrix);
                    nextHNormal = Vector3.Transform(new Vector3(xPos2, yPos1, zPos2), capsule.orientationMatrix);
                    nextVNormal = Vector3.Transform(new Vector3(xPos21, yPos2, zPos21), capsule.orientationMatrix);
                    nextHVNormal = Vector3.Transform(new Vector3(xPos22, yPos2, zPos22), capsule.orientationMatrix);
                    lowerCurrentNormal = Vector3.Transform(new Vector3(xPos1, -yPos1, zPos1), capsule.orientationMatrix);
                    lowerNextHNormal = Vector3.Transform(new Vector3(xPos2, -yPos1, zPos2), capsule.orientationMatrix);
                    lowerNextVNormal = Vector3.Transform(new Vector3(xPos21, -yPos2, zPos21), capsule.orientationMatrix);
                    lowerNextHVNormal = Vector3.Transform(new Vector3(xPos22, -yPos2, zPos22), capsule.orientationMatrix);

                    //Upper Cap
                    toReturn[numVerticesInColumn * j + (k - 1) * 12] = new VertexPositionNormalTexture(current * capsule.radius, currentNormal, textureCoords[0]);
                    toReturn[numVerticesInColumn * j + (k - 1) * 12 + 1] = new VertexPositionNormalTexture(nextHV * capsule.radius, nextHVNormal, textureCoords[1]);
                    toReturn[numVerticesInColumn * j + (k - 1) * 12 + 2] = new VertexPositionNormalTexture(nextH * capsule.radius, nextHNormal, textureCoords[2]);

                    toReturn[numVerticesInColumn * j + (k - 1) * 12 + 3] = new VertexPositionNormalTexture(current * capsule.radius, currentNormal, textureCoords[0]);
                    toReturn[numVerticesInColumn * j + (k - 1) * 12 + 4] = new VertexPositionNormalTexture(nextV * capsule.radius, nextVNormal, textureCoords[1]);
                    toReturn[numVerticesInColumn * j + (k - 1) * 12 + 5] = new VertexPositionNormalTexture(nextHV * capsule.radius, nextHVNormal, textureCoords[2]);

                    if (k == 1)
                    {
                        toReturn[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 0] = new VertexPositionNormalTexture(new Vector3(0, capsule.radius + halfLength, 0), Vector3.Up, textureCoords[0]);
                        toReturn[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 1] = new VertexPositionNormalTexture(current * capsule.radius, currentNormal, textureCoords[1]);
                        toReturn[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 2] = new VertexPositionNormalTexture(nextH * capsule.radius, nextHNormal, textureCoords[2]);
                    }


                    //Body
                    if (k == numSides / 4 - 1)
                    {
                        toReturn[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 6] = new VertexPositionNormalTexture(lowerNextV * capsule.radius, lowerNextVNormal, textureCoords[0]);
                        toReturn[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 7] = new VertexPositionNormalTexture(lowerNextHV * capsule.radius, lowerNextHVNormal, textureCoords[1]);
                        toReturn[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 8] = new VertexPositionNormalTexture(nextV * capsule.radius, nextVNormal, textureCoords[2]);

                        toReturn[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 9] = new VertexPositionNormalTexture(nextV * capsule.radius, nextVNormal, textureCoords[0]);
                        toReturn[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 11] = new VertexPositionNormalTexture(nextHV * capsule.radius, nextHVNormal, textureCoords[1]);
                        toReturn[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 10] = new VertexPositionNormalTexture(lowerNextHV * capsule.radius, lowerNextHVNormal, textureCoords[2]);
                    }

                    //Lower Cap
                    toReturn[numVerticesInColumn * j + (k - 1) * 12 + 6] = new VertexPositionNormalTexture(lowerCurrent * capsule.radius, lowerCurrentNormal, textureCoords[0]);
                    toReturn[numVerticesInColumn * j + (k - 1) * 12 + 8] = new VertexPositionNormalTexture(lowerNextHV * capsule.radius, lowerNextHVNormal, textureCoords[1]);
                    toReturn[numVerticesInColumn * j + (k - 1) * 12 + 7] = new VertexPositionNormalTexture(lowerNextH * capsule.radius, lowerNextHNormal, textureCoords[2]);

                    toReturn[numVerticesInColumn * j + (k - 1) * 12 + 9] = new VertexPositionNormalTexture(lowerCurrent * capsule.radius, lowerCurrentNormal, textureCoords[0]);
                    toReturn[numVerticesInColumn * j + (k - 1) * 12 + 11] = new VertexPositionNormalTexture(lowerNextV * capsule.radius, lowerNextVNormal, textureCoords[1]);
                    toReturn[numVerticesInColumn * j + (k - 1) * 12 + 10] = new VertexPositionNormalTexture(lowerNextHV * capsule.radius, lowerNextHVNormal, textureCoords[2]);


                    if (k == 1)
                    {
                        toReturn[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 3] = new VertexPositionNormalTexture(new Vector3(0, -capsule.radius - halfLength, 0), Vector3.Down, textureCoords[0]);
                        toReturn[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 5] = new VertexPositionNormalTexture(lowerCurrent * capsule.radius, lowerCurrentNormal, textureCoords[1]);
                        toReturn[numVerticesInColumn * j + (numSides / 4 - 1) * 12 + 4] = new VertexPositionNormalTexture(lowerNextH * capsule.radius, lowerNextHNormal, textureCoords[2]);
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
            return capsule;
        }
    }
}
