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
    /// Simple display class for boxes.  Creates and manages a triangulated represenation of the entity.
    /// </remarks>
    public class DisplayBox : DisplayObject
    {
        Box box;

        /// <summary>
        /// Creates the display object for the entity.
        /// </summary>
        /// <param name="b">Entity to draw.</param>
        /// <param name="graphics">Manager to use.</param>
        /// <param name="initShape">Whether or not to initialize the local shape vertices.</param>
        /// <param name="initVertexBuffer">Whether or not to initialize the local vertex buffer.</param>
        public DisplayBox(Box b, GraphicsDeviceManager graphics, bool initShape, bool initVertexBuffer)
        {

            box = b;
            myGraphics = graphics;
            textureCoords[0] = new Vector2(0, 0);
            textureCoords[1] = new Vector2(1, 0);
            textureCoords[2] = new Vector2(1, 1);
            textureCoords[3] = new Vector2(0, 1);

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
            float slop = box.collisionMargin - box.allowedPenetration;
            BoundingBox boundingBox = new BoundingBox(new Vector3(-box.halfWidth - slop, -box.halfHeight - slop, -box.halfLength - slop), new Vector3(box.halfWidth + slop, box.halfHeight + slop, box.halfLength + slop));


            Vector3[] corners = boundingBox.GetCorners();
            shapeVertices = new VertexPositionNormalTexture[36];


            shapeVertices[0] = new VertexPositionNormalTexture(corners[0], Vector3.Backward, textureCoords[0]);
            shapeVertices[1] = new VertexPositionNormalTexture(corners[1], Vector3.Backward, textureCoords[1]);
            shapeVertices[2] = new VertexPositionNormalTexture(corners[2], Vector3.Backward, textureCoords[2]);
            shapeVertices[3] = new VertexPositionNormalTexture(corners[0], Vector3.Backward, textureCoords[0]);
            shapeVertices[4] = new VertexPositionNormalTexture(corners[2], Vector3.Backward, textureCoords[2]);
            shapeVertices[5] = new VertexPositionNormalTexture(corners[3], Vector3.Backward, textureCoords[3]);

            shapeVertices[6] = new VertexPositionNormalTexture(corners[1], Vector3.Right, textureCoords[0]);
            shapeVertices[7] = new VertexPositionNormalTexture(corners[5], Vector3.Right, textureCoords[1]);
            shapeVertices[8] = new VertexPositionNormalTexture(corners[6], Vector3.Right, textureCoords[2]);
            shapeVertices[9] = new VertexPositionNormalTexture(corners[1], Vector3.Right, textureCoords[0]);
            shapeVertices[10] = new VertexPositionNormalTexture(corners[6], Vector3.Right, textureCoords[2]);
            shapeVertices[11] = new VertexPositionNormalTexture(corners[2], Vector3.Right, textureCoords[3]);

            shapeVertices[12] = new VertexPositionNormalTexture(corners[5], Vector3.Forward, textureCoords[0]);
            shapeVertices[13] = new VertexPositionNormalTexture(corners[4], Vector3.Forward, textureCoords[1]);
            shapeVertices[14] = new VertexPositionNormalTexture(corners[7], Vector3.Forward, textureCoords[2]);
            shapeVertices[15] = new VertexPositionNormalTexture(corners[5], Vector3.Forward, textureCoords[0]);
            shapeVertices[16] = new VertexPositionNormalTexture(corners[7], Vector3.Forward, textureCoords[2]);
            shapeVertices[17] = new VertexPositionNormalTexture(corners[6], Vector3.Forward, textureCoords[3]);

            shapeVertices[18] = new VertexPositionNormalTexture(corners[4], Vector3.Left, textureCoords[0]);
            shapeVertices[19] = new VertexPositionNormalTexture(corners[0], Vector3.Left, textureCoords[1]);
            shapeVertices[20] = new VertexPositionNormalTexture(corners[3], Vector3.Left, textureCoords[2]);
            shapeVertices[21] = new VertexPositionNormalTexture(corners[4], Vector3.Left, textureCoords[0]);
            shapeVertices[22] = new VertexPositionNormalTexture(corners[3], Vector3.Left, textureCoords[2]);
            shapeVertices[23] = new VertexPositionNormalTexture(corners[7], Vector3.Left, textureCoords[3]);

            shapeVertices[24] = new VertexPositionNormalTexture(corners[0], Vector3.Up, textureCoords[2]);
            shapeVertices[25] = new VertexPositionNormalTexture(corners[5], Vector3.Up, textureCoords[0]);
            shapeVertices[26] = new VertexPositionNormalTexture(corners[1], Vector3.Up, textureCoords[3]);
            shapeVertices[27] = new VertexPositionNormalTexture(corners[0], Vector3.Up, textureCoords[2]);
            shapeVertices[28] = new VertexPositionNormalTexture(corners[4], Vector3.Up, textureCoords[1]);
            shapeVertices[29] = new VertexPositionNormalTexture(corners[5], Vector3.Up, textureCoords[0]);

            shapeVertices[30] = new VertexPositionNormalTexture(corners[3], Vector3.Down, textureCoords[0]);
            shapeVertices[31] = new VertexPositionNormalTexture(corners[2], Vector3.Down, textureCoords[1]);
            shapeVertices[32] = new VertexPositionNormalTexture(corners[6], Vector3.Down, textureCoords[2]);
            shapeVertices[33] = new VertexPositionNormalTexture(corners[3], Vector3.Down, textureCoords[0]);
            shapeVertices[34] = new VertexPositionNormalTexture(corners[6], Vector3.Down, textureCoords[2]);
            shapeVertices[35] = new VertexPositionNormalTexture(corners[7], Vector3.Down, textureCoords[3]);

        }

        /// <summary>
        /// Recalculates and returns the vertices of the object.
        /// </summary>
        /// <returns>Vertices composing the display object.</returns>
        public override VertexPositionNormalTexture[] getVertices()
        {
            VertexPositionNormalTexture[] toReturn = new VertexPositionNormalTexture[36];

            float slop = box.collisionMargin - box.allowedPenetration;
            BoundingBox boundingBox = new BoundingBox(new Vector3(-box.halfWidth - slop, -box.halfHeight - slop, -box.halfLength - slop), new Vector3(box.halfWidth + slop, box.halfHeight + slop, box.halfLength + slop));


            Vector3[] corners = boundingBox.GetCorners();
            for (int k = 0; k < 8; k++)
            {
                corners[k] = Vector3.Transform(corners[k], box.orientationMatrix) + box.centerPosition;
            }
            Vector3 forward = Vector3.Transform(Vector3.Forward, box.orientationMatrix);
            Vector3 backward = Vector3.Transform(Vector3.Backward, box.orientationMatrix);
            Vector3 left = Vector3.Transform(Vector3.Left, box.orientationMatrix);
            Vector3 right = Vector3.Transform(Vector3.Right, box.orientationMatrix);
            Vector3 up = Vector3.Transform(Vector3.Up, box.orientationMatrix);
            Vector3 down = Vector3.Transform(Vector3.Down, box.orientationMatrix);


            toReturn[0] = new VertexPositionNormalTexture(corners[0], backward, textureCoords[0]);
            toReturn[1] = new VertexPositionNormalTexture(corners[1], backward, textureCoords[1]);
            toReturn[2] = new VertexPositionNormalTexture(corners[2], backward, textureCoords[2]);
            toReturn[3] = new VertexPositionNormalTexture(corners[0], backward, textureCoords[0]);
            toReturn[4] = new VertexPositionNormalTexture(corners[2], backward, textureCoords[1]);
            toReturn[5] = new VertexPositionNormalTexture(corners[3], backward, textureCoords[2]);

            toReturn[6] = new VertexPositionNormalTexture(corners[1], right, textureCoords[0]);
            toReturn[7] = new VertexPositionNormalTexture(corners[5], right, textureCoords[1]);
            toReturn[8] = new VertexPositionNormalTexture(corners[6], right, textureCoords[2]);
            toReturn[9] = new VertexPositionNormalTexture(corners[1], right, textureCoords[0]);
            toReturn[10] = new VertexPositionNormalTexture(corners[6], right, textureCoords[1]);
            toReturn[11] = new VertexPositionNormalTexture(corners[2], right, textureCoords[2]);

            toReturn[12] = new VertexPositionNormalTexture(corners[5], forward, textureCoords[0]);
            toReturn[13] = new VertexPositionNormalTexture(corners[4], forward, textureCoords[1]);
            toReturn[14] = new VertexPositionNormalTexture(corners[7], forward, textureCoords[2]);
            toReturn[15] = new VertexPositionNormalTexture(corners[5], forward, textureCoords[0]);
            toReturn[16] = new VertexPositionNormalTexture(corners[7], forward, textureCoords[1]);
            toReturn[17] = new VertexPositionNormalTexture(corners[6], forward, textureCoords[2]);

            toReturn[18] = new VertexPositionNormalTexture(corners[4], left, textureCoords[0]);
            toReturn[19] = new VertexPositionNormalTexture(corners[0], left, textureCoords[1]);
            toReturn[20] = new VertexPositionNormalTexture(corners[3], left, textureCoords[2]);
            toReturn[21] = new VertexPositionNormalTexture(corners[4], left, textureCoords[0]);
            toReturn[22] = new VertexPositionNormalTexture(corners[3], left, textureCoords[1]);
            toReturn[23] = new VertexPositionNormalTexture(corners[7], left, textureCoords[2]);

            toReturn[24] = new VertexPositionNormalTexture(corners[0], up, textureCoords[0]);
            toReturn[25] = new VertexPositionNormalTexture(corners[5], up, textureCoords[1]);
            toReturn[26] = new VertexPositionNormalTexture(corners[1], up, textureCoords[2]);
            toReturn[27] = new VertexPositionNormalTexture(corners[0], up, textureCoords[0]);
            toReturn[28] = new VertexPositionNormalTexture(corners[4], up, textureCoords[1]);
            toReturn[29] = new VertexPositionNormalTexture(corners[5], up, textureCoords[2]);

            toReturn[30] = new VertexPositionNormalTexture(corners[3], down, textureCoords[0]);
            toReturn[31] = new VertexPositionNormalTexture(corners[2], down, textureCoords[1]);
            toReturn[32] = new VertexPositionNormalTexture(corners[6], down, textureCoords[2]);
            toReturn[33] = new VertexPositionNormalTexture(corners[3], down, textureCoords[0]);
            toReturn[34] = new VertexPositionNormalTexture(corners[6], down, textureCoords[1]);
            toReturn[35] = new VertexPositionNormalTexture(corners[7], down, textureCoords[2]);

            return toReturn;
        }

        /// <summary>
        /// Gets the entity wrapped by this display object.
        /// </summary>
        /// <returns>Entity wrapped by this display object.</returns>
        public override Entity getEntity()
        {
            return box;
        }

    }
}
