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
    /// Superclass for the various display object types.
    /// </remarks>
    public abstract class DisplayObject
    {
        internal VertexPositionNormalTexture[] shapeVertices;
        internal VertexBuffer vertexBuffer;
        internal GraphicsDeviceManager myGraphics;
        /// <summary>
        /// Texture to use for noninstanced and CPU instaced drawing.
        /// </summary>
        public Texture2D texture;
        /// <summary>
        /// Index of texture to use for shader instanced drawing.
        /// </summary>
        public int textureIndex;
        /// <summary>
        /// Texture cooordinate array reused to slap on textures.
        /// </summary>
        protected Vector2[] textureCoords = new Vector2[4];


        /// <summary>
        /// Recalculates and returns the vertices of the object.
        /// </summary>
        /// <returns>Vertices composing the display object.</returns>
        public abstract VertexPositionNormalTexture[] getVertices();
        /// <summary>
        /// Draws the display object.
        /// </summary>
        /// <param name="effect">Effect used to draw.</param>
        public virtual void draw(Effect effect)
        {
            if (effect is BasicEffect)
            {
                ((BasicEffect)effect).World = getRotation() * Matrix.CreateTranslation(getPosition());
                if (((BasicEffect)effect).TextureEnabled)
                    ((BasicEffect)effect).Texture = texture;
            }
            myGraphics.GraphicsDevice.Vertices[0].SetSource(vertexBuffer, 0, VertexPositionNormalTexture.SizeInBytes);
            effect.Begin();
            foreach (EffectPass pass in effect.CurrentTechnique.Passes)
            {
                pass.Begin();

                myGraphics.GraphicsDevice.DrawPrimitives(PrimitiveType.TriangleList, 0, shapeVertices.Length / 3);

                pass.End();
            }
            effect.End();


        }

        /// <summary>
        /// Gets the entity wrapped by this display object.
        /// </summary>
        /// <returns>Entity wrapped by this display object.</returns>
        public abstract Entity getEntity();
        /// <summary>
        /// Gets the bounding box of the entity wrapped by this display object.
        /// </summary>
        /// <returns>Bounding box of the entity wrapped by this display object.</returns>
        public virtual BoundingBox getBoundingBox()
        {
            return getEntity().boundingBox;
        }
        /// <summary>
        /// Gets the position of the entity wrapped by this display object.
        /// </summary>
        /// <returns>Gets the position of the entity wrapped by this display object.</returns>
        public virtual Vector3 getPosition()
        {
            return getEntity().centerPosition;
        }
        /// <summary>
        /// Gets the rotation of the entity wrapped by this display object.
        /// </summary>
        /// <returns>Rotation of the entity wrapped by this display object.</returns>
        public virtual Matrix getRotation()
        {
            return getEntity().orientationMatrix;
        }




    }
}
