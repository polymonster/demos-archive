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
using BEPUphysics;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;
using System.Diagnostics;

namespace Tanky.Source.Physics.Display
{
    /// <remarks>
    /// Manages displaying and moving a model.
    /// </remarks>
    public class DisplayModel
    {
        /// <summary>
        /// Entity to base the model's world matrix on.
        /// </summary>
        public Entity entity;
        /// <summary>
        /// Model to display.
        /// </summary>
        public Model model;
        /// <summary>
        /// Matrix to transform the position of the model with.
        /// </summary>
        public Matrix worldMatrix = Matrix.Identity;
        /// <summary>
        /// Bone transformations of meshes in the model.
        /// </summary>
        Matrix[] transforms;

        /// <summary>
        /// Creates a DisplayModel, following an entity.
        /// </summary>
        /// <param name="modelToDraw">Model to draw.</param>
        /// <param name="entityToFollow">Entity to base the model's world matrix on.</param>
        public DisplayModel(Model modelToDraw, Entity entityToFollow)
        {
            model = modelToDraw;
            entity = entityToFollow;
            transforms = new Matrix[modelToDraw.Bones.Count];
            foreach (ModelMesh mesh in model.Meshes)
            {
                foreach (BasicEffect effect in mesh.Effects)
                {
                    effect.EnableDefaultLighting();
                }
            }
        }
        /// <summary>
        /// Creates a DisplayModel, following an entity.
        /// </summary>
        /// <param name="modelToDraw">Model to draw.</param>
        public DisplayModel(Model modelToDraw)
        {
            model = modelToDraw;
            transforms = new Matrix[modelToDraw.Bones.Count];
            foreach (ModelMesh mesh in model.Meshes)
            {
                foreach (BasicEffect effect in mesh.Effects)
                {
                    effect.EnableDefaultLighting();
                }
            }
        }

        /// <summary>
        /// Sets the texture of the model to the given texture.
        /// </summary>
        /// <param name="texture">Texture to use.</param>
        public void setTexture(Texture2D texture)
        {
            foreach (ModelMesh mesh in model.Meshes)
            {
                foreach (BasicEffect effect in mesh.Effects)
                {
                    effect.TextureEnabled = true;
                    effect.Texture = texture;
                }
            }
        }

        /// <summary>
        /// Draws the model.
        /// </summary>
        /// <param name="viewMatrix">View matrix to use.</param>
        /// <param name="projectionMatrix">Projection matrix to use.</param>
        public void draw(Matrix viewMatrix, Matrix projectionMatrix)
        {
            
            Matrix worldMatrixToUse;
            if (entity != null)//Update the world matrix if the model is following an entity.
                worldMatrixToUse = worldMatrix * entity.orientationMatrix * Matrix.CreateTranslation(entity.centerPosition);
            else
                worldMatrixToUse = worldMatrix;
            

            model.CopyAbsoluteBoneTransformsTo(transforms);

            foreach (ModelMesh mesh in model.Meshes)
            {
                foreach (BasicEffect effect in mesh.Effects)
                {
                    effect.World = transforms[mesh.ParentBone.Index] * worldMatrixToUse;
                    effect.View = viewMatrix;
                    effect.Projection = projectionMatrix;
                }
                mesh.Draw();
            }
        }
    }
}
