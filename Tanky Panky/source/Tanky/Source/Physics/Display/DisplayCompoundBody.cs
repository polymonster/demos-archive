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
using System.Diagnostics;

namespace Tanky.Source.Physics.Display
{
    /// <remarks>
    /// Handles the appearance of compound bodies.
    /// </remarks>
    public class DisplayCompoundBody : DisplayObject
    {
        CompoundBody comp;
        internal List<DisplayObject> subDisplayObjects;

        /// <summary>
        /// Creates the display object for the entity.
        /// </summary>
        /// <param name="c">Entity to draw.</param>
        /// <param name="graphics">Manager to use.</param>
        /// <param name="initShape">Whether or not to initialize the local shape vertices.</param>
        /// <param name="initVertexBuffer">Whether or not to initialize the local vertex buffer.</param>
        public DisplayCompoundBody(CompoundBody c, GraphicsDeviceManager graphics, bool initShape, bool initVertexBuffer)
        {

            comp = c;
            myGraphics = graphics;
            if (initShape || initVertexBuffer)
                initializeShape();
            if (initVertexBuffer)
            {
                vertexBuffer = new VertexBuffer(myGraphics.GraphicsDevice, VertexPositionNormalTexture.SizeInBytes * shapeVertices.Length, BufferUsage.WriteOnly);
                vertexBuffer.SetData<VertexPositionNormalTexture>(shapeVertices);
            }
        }


        /// <summary>
        /// Assigns the texture of the compound.
        /// </summary>
        /// <param name="newTexture">New texture to use.</param>
        public void setTexture(Texture2D newTexture)
        {
            texture = newTexture;
            foreach (DisplayObject d in subDisplayObjects)
            {
                if (d is DisplayCompoundBody)
                    ((DisplayCompoundBody)d).setTexture(newTexture);
                else
                    d.texture = newTexture;
            }
        }


        void initializeShape()
        {
            subDisplayObjects = new List<DisplayObject>();
            DisplayObject toAdd;
            List<VertexPositionNormalTexture> shapeVerticesToAdd = new List<VertexPositionNormalTexture>();
            foreach (Entity e in comp.getBodies())
            {
                
                if (e is Sphere)
                    toAdd = new DisplaySphere((Sphere)e, myGraphics, true, true);
                else if (e is Cone)
                    toAdd = new DisplayCone((Cone)e, myGraphics, true, true);
                else if (e is Cylinder)
                    toAdd = new DisplayCylinder((Cylinder)e, myGraphics, true, true);
                else if (e is Capsule)
                    toAdd = new DisplayCapsule((Capsule)e, myGraphics, true, true);
                else if (e is Triangle)
                    toAdd = new DisplayTriangle((Triangle)e, myGraphics, true, true);
                else if (e is Box)
                    toAdd = new DisplayBox((Box)e, myGraphics, true, true);
                else if (e is MinkowskiSum)
                    toAdd = new DisplayMinkowskiSum((MinkowskiSum)e, myGraphics, true, true);
                else if (e is WrappedBody)
                    toAdd = new DisplayWrappedBody((WrappedBody)e, myGraphics, true, true);
                else if (e is ConvexHull)
                    toAdd = new DisplayConvexHull((ConvexHull)e, myGraphics, true, true);
                else
                    toAdd = new DisplayCompoundBody((CompoundBody)e, myGraphics, true, true);
                Matrix transposedCompoundMatrix = Matrix.Transpose(comp.orientationMatrix);
                foreach (VertexPositionNormalTexture v in toAdd.shapeVertices)
                {

                    Vector3 worldPos = Vector3.Transform(v.Position, e.orientationMatrix) + e.centerPosition;
                    Vector3 offset = worldPos - comp.centerPosition;
                    offset = Vector3.Transform(offset, transposedCompoundMatrix);
                    Vector3 norm = Vector3.Transform(Vector3.Transform(v.Normal, e.orientationMatrix), transposedCompoundMatrix);
                    shapeVerticesToAdd.Add(new VertexPositionNormalTexture(offset, norm, v.TextureCoordinate));
                }
                subDisplayObjects.Add(toAdd);
                
            }
            shapeVertices = shapeVerticesToAdd.ToArray();

        }
        /// <summary>
        /// Recalculates and returns the vertices of the object.
        /// Not implemented for Compound Bodies.
        /// </summary>
        /// <returns>Vertices composing the display object.</returns>
        public override VertexPositionNormalTexture[] getVertices()
        {
            return null;
        }

        /// <summary>
        /// Draws the display object.
        /// </summary>
        /// <param name="effect">Effect used to draw</param>
        public override void draw(Effect effect)
        {
            foreach (DisplayObject d in subDisplayObjects)
                d.draw(effect);
        }

        /// <summary>
        /// Gets the entity wrapped by this display object.
        /// </summary>
        /// <returns>Entity wrapped by this display object.</returns>
        public override Entity getEntity()
        {
            return comp;
        }
    }
}
