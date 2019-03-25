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
    /// Simple display class for wrapped bodies.  Creates a triangulated representation and keeps track of it during the simulation.
    /// </remarks>
    public class DisplayWrappedBody : DisplayObject
    {
        WrappedBody body;



        /// <summary>
        /// Creates the display object for the entity.
        /// </summary>
        /// <param name="b">Entity to draw.</param>
        /// <param name="graphics">Manager to use.</param>
        /// <param name="initShape">Whether or not to initialize the local shape vertices.</param>
        /// <param name="initVertexBuffer">Whether or not to initialize the local vertex buffer.</param>
        public DisplayWrappedBody(WrappedBody b, GraphicsDeviceManager graphics, bool initShape, bool initVertexBuffer)
        {

            body = b;
            myGraphics = graphics;

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
            List<Vector3> bodyPoints = new List<Vector3>();
            Vector3 extreme;
            int numSides = 20;
            Vector3 dir;
            float angleChange = (float)Math.PI / (numSides / 2);
            float theta = 0;
            float phi = 0;
            for (int j = 0; j < numSides; j++)
            {//per column
                for (int k = 1; k < numSides / 2 - 1; k++)
                {//per row
                    theta = j * angleChange;
                    phi = k * angleChange;
                    dir = new Vector3((float)(Math.Cos(theta) * Math.Sin(phi)), (float)Math.Cos(phi), (float)(Math.Sin(theta) * Math.Sin(phi)));
                    body.getExtremePoint(ref dir, body.collisionMargin, out extreme);
                    bodyPoints.Add(extreme - body.centerPosition);
                }
            }
            body.getExtremePoint(ref Toolbox.upVector, body.collisionMargin, out extreme);
            bodyPoints.Add(extreme - body.centerPosition);
            body.getExtremePoint(ref Toolbox.downVector, body.collisionMargin, out extreme);
            bodyPoints.Add(extreme - body.centerPosition);

            List<Vector3> hullTriangleVertices = new List<Vector3>();
            List<int> hullTriangleIndices = new List<int>();
            Toolbox.getConvexHull(bodyPoints, hullTriangleIndices, hullTriangleVertices); //The hull triangle vertices are used as a dummy to get the unnecessary hull vertices, which are cleared afterwards.
            hullTriangleVertices.Clear();
            foreach (int i in hullTriangleIndices)
            {
                hullTriangleVertices.Add(bodyPoints[i]);
            }
            textureCoords[0] = new Vector2(0, 0);
            textureCoords[1] = new Vector2(1, 0);
            textureCoords[2] = new Vector2(0, 1);
            shapeVertices = new VertexPositionNormalTexture[hullTriangleVertices.Count];
            Vector3 normal;
            for (int i = 0; i < hullTriangleVertices.Count; i += 3)
            {
                normal = Vector3.Normalize(Vector3.Cross(hullTriangleVertices[i + 2] - hullTriangleVertices[i], hullTriangleVertices[i + 1] - hullTriangleVertices[i]));
                shapeVertices[i] = new VertexPositionNormalTexture(hullTriangleVertices[i], normal, textureCoords[0]);
                shapeVertices[i + 1] = new VertexPositionNormalTexture(hullTriangleVertices[i + 1], normal, textureCoords[1]);
                shapeVertices[i + 2] = new VertexPositionNormalTexture(hullTriangleVertices[i + 2], normal, textureCoords[2]);
            }
            Quaternion inverseOrientation = Quaternion.Conjugate(body.orientationQuaternion);
            for (int i = 0; i < shapeVertices.Length; i++)
            {
                shapeVertices[i].Position = Vector3.Transform(shapeVertices[i].Position, inverseOrientation);
            }
        }

        /// <summary>
        /// Recalculates and returns the vertices of the object.
        /// </summary>
        /// <returns>Vertices composing the display object.</returns>
        public override VertexPositionNormalTexture[] getVertices()
        {
            List<Vector3> bodyPoints = new List<Vector3>();
            List<Vector3> hullTriangleVertices = new List<Vector3>();
            List<int> hullTriangleIndices = new List<int>();
            Toolbox.getConvexHull(bodyPoints, hullTriangleIndices, hullTriangleVertices); //The hull triangle vertices are used as a dummy to get the unnecessary hull vertices, which are cleared afterwards.
            hullTriangleVertices.Clear();
            foreach (int i in hullTriangleIndices)
            {
                hullTriangleVertices.Add(bodyPoints[i]);
            }
            textureCoords[0] = new Vector2(0, 0);
            textureCoords[1] = new Vector2(1, 0);
            textureCoords[2] = new Vector2(0, 1);
            VertexPositionNormalTexture[] toReturn = new VertexPositionNormalTexture[hullTriangleVertices.Count];
            Vector3 normal;
            for (int i = 0; i < hullTriangleVertices.Count; i += 3)
            {
                normal = Vector3.Normalize(Vector3.Cross(hullTriangleVertices[i + 1] - hullTriangleVertices[i], hullTriangleVertices[i + 2] - hullTriangleVertices[i]));
                toReturn[i] = new VertexPositionNormalTexture(hullTriangleVertices[i] - body.centerPosition, normal, textureCoords[0]);
                toReturn[i + 1] = new VertexPositionNormalTexture(hullTriangleVertices[i + 1] - body.centerPosition, normal, textureCoords[1]);
                toReturn[i + 2] = new VertexPositionNormalTexture(hullTriangleVertices[i + 2] - body.centerPosition, normal, textureCoords[2]);
            }
            Quaternion inverseOrientation = Quaternion.Conjugate(body.orientationQuaternion);
            for (int i = 0; i < toReturn.Length; i++)
            {
                toReturn[i].Position = Vector3.Transform(toReturn[i].Position, inverseOrientation);
            }

            return toReturn;
        }


        /// <summary>
        /// Gets the entity wrapped by this display object.
        /// </summary>
        /// <returns>Entity wrapped by this display object.</returns>
        public override Entity getEntity()
        {
            return body;
        }
    }
}
