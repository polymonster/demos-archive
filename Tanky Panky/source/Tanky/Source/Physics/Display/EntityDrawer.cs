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
using System.Diagnostics;
using BEPUphysics;

namespace Tanky.Source.Physics.Display
{
    /// <summary>
    /// Manages and draws the entities of the demos application.
    /// </summary>
    public class EntityDrawer
    {
        /// <summary>
        /// List of entity-based objects handled by the entity drawer.
        /// </summary>
        List<DisplayObject> displayObjects;
        /// <summary>
        /// List of models to be drawn by the drawer.
        /// </summary>
        public List<DisplayModel> displayModels;
        /// <summary>
        /// Used for shader instancing, due to terrain draw calls being handled separately.
        /// Keeps track of the number of instanced objects.
        /// </summary>
        int nonTerrainDisplayObjectsCount = 0;

        GraphicsDeviceManager graphics;
        /// <summary>
        /// Effect responsible for drawing non-instanced geometry.
        /// </summary>
        public BasicEffect drawer;
        VertexDeclaration declaration;
        /// <summary>
        /// Used in CPU instancing; stores the vertices of the current batch.
        /// </summary>
        List<VertexPositionNormalTexture> instanceVertices;
        /// <summary>
        /// Used in CPU instancing; holds vertices of the current batch.
        /// </summary>
        DynamicVertexBuffer instanceBuffer;
        VertexDeclaration instancedDeclaration;
        List<Texture2D> textures;
        Random randomColorGenerator = new Random();
        /// <summary>
        /// Whether or not the geometry should be rendered in wireframe.
        /// </summary>
        public bool isWireFrameMode = false;

        #region Instancing Fields
        /// <summary>
        /// Special effect used in shader instancing.
        /// </summary>
        public Effect instancedDrawer;
        List<List<InstancedVertex>> batches = new List<List<InstancedVertex>>(128);
        List<VertexBuffer> batchBuffers = new List<VertexBuffer>(128);
        bool shaderInstancing;
        /// <summary>
        /// Used in shader instancing; maximum number of instances to be drawn at once.  Must match InstancedEffect.fx MAX_INSTANCES.
        /// </summary>
        const int MAX_INSTANCES = 45;
        /// <summary>
        /// Used in shader instancing; number of textures being used.  Must match InstancedEffect.fx NUM_TEXTURES and available samplers.
        /// </summary>
        const int NUM_TEXTURES = 8;
        EffectParameter rotationsParameter;
        EffectParameter translationsParameter;
        EffectParameter textureIndicesParameter;
        EffectParameter viewParameter;
        EffectParameter viewPositionParameter;
        Matrix[] rotationMatrices = new Matrix[MAX_INSTANCES];
        Vector3[] translations = new Vector3[MAX_INSTANCES];
        float[] textureIndices = new float[MAX_INSTANCES];
        #endregion

        /// <summary>
        /// Creates a basic visualization object to display entities.
        /// </summary>
        /// <param name="manager">Manager to be used by the drawer.</param>
        /// <param name="texturesToUse">List of textures to be randomly applied to automatically created DisplayObjects.</param>
        /// <param name="useShaderInstancing">Whether or not to use shader instancing.  Setting to false falls back to normal CPU instancing methods.</param>
        public EntityDrawer(List<Texture2D> texturesToUse, bool useShaderInstancing)
        {
            graphics = g.graphics;
            drawer = new BasicEffect(graphics.GraphicsDevice, null);
            displayObjects = new List<DisplayObject>();
            displayModels = new List<DisplayModel>();
            if(useShaderInstancing)
                instanceBuffer = new DynamicVertexBuffer(graphics.GraphicsDevice, 32000, BufferUsage.WriteOnly);
            else
                instanceBuffer = new DynamicVertexBuffer(graphics.GraphicsDevice, 64000000, BufferUsage.WriteOnly);
            instanceVertices = new List<VertexPositionNormalTexture>();
            if (useShaderInstancing)
                instancedDeclaration = new VertexDeclaration(graphics.GraphicsDevice, InstancedVertex.VertexElements);
            declaration = new VertexDeclaration(graphics.GraphicsDevice, VertexPositionNormalTexture.VertexElements);
            shaderInstancing = useShaderInstancing;
            if (texturesToUse != null)
            {
                textures = texturesToUse;
                drawer.TextureEnabled = true;

            }
            if (!useShaderInstancing)
                initializeShaders();
        }

        /// <summary>
        /// Manually diposes of vertex buffers within the display objects.
        /// </summary>
        public void cleanUpBuffers()
        {
            foreach (DisplayObject d in displayObjects)
            {
                if (d.vertexBuffer != null)
                    d.vertexBuffer.Dispose();
            }
        }

        /// <summary>
        /// Sets up all the information required by the currently activated shader systems.
        /// </summary>
        public void initializeShaders()
        {

            drawer = new BasicEffect(graphics.GraphicsDevice, null);




            drawer.AmbientLightColor = Vector3.One / 2;

            drawer.DirectionalLight0.Enabled = true;
            drawer.DirectionalLight0.DiffuseColor = Vector3.One / 1.5f;
            drawer.DirectionalLight0.Direction = Vector3.Normalize(new Vector3(1f, -1.0f, -1f));
            drawer.DirectionalLight0.SpecularColor = Vector3.One / 3;

            drawer.PreferPerPixelLighting = true;
            drawer.LightingEnabled = true;

            if (shaderInstancing)
            {
                rotationsParameter = instancedDrawer.Parameters["Rotations"];
                translationsParameter = instancedDrawer.Parameters["Translations"];
                textureIndicesParameter = instancedDrawer.Parameters["TextureIndices"];
                viewParameter = instancedDrawer.Parameters["View"];
                viewPositionParameter = instancedDrawer.Parameters["ViewPosition"];

                instancedDrawer.Parameters["LightDirection"].SetValue(Vector3.Normalize(new Vector3(1, -1, -1)));
                instancedDrawer.Parameters["DiffuseColor"].SetValue(Vector4.One / 1.5f);
                instancedDrawer.Parameters["AmbientAmount"].SetValue(.5f);
                instancedDrawer.Parameters["SpecularColor"].SetValue(Vector4.One / 3);
                instancedDrawer.Parameters["Texture0"].SetValue(textures[0]);
                instancedDrawer.Parameters["Texture1"].SetValue(textures[1]);
                instancedDrawer.Parameters["Texture2"].SetValue(textures[2]);
                instancedDrawer.Parameters["Texture3"].SetValue(textures[3]);
                instancedDrawer.Parameters["Texture4"].SetValue(textures[4]);
                instancedDrawer.Parameters["Texture5"].SetValue(textures[5]);
                instancedDrawer.Parameters["Texture6"].SetValue(textures[6]);
                instancedDrawer.Parameters["Texture7"].SetValue(textures[7]);
            }
            else
            {
                drawer.PreferPerPixelLighting = false;
                drawer.TextureEnabled = true;
            }

        }

        /// <summary>
        /// Adds the given display object to the system.
        /// </summary>
        /// <param name="d">DisplayObject to add.</param>
        public void addDisplayObject(DisplayObject d)
        {
                if (!shaderInstancing || textures != null || d.texture == null)
                    d.texture = textures[randomColorGenerator.Next(textures.Count - 1)];
                if (textures != null && !drawer.TextureEnabled)
                    drawer.TextureEnabled = true;
                if (shaderInstancing && !(d is DisplayTerrain))
                {
                    d.textureIndex = randomColorGenerator.Next(NUM_TEXTURES);
                    addToBatches(d);
                }
                if (!(d is DisplayTerrain))
                    nonTerrainDisplayObjectsCount++;
                displayObjects.Add(d);
            

        }
        /// <summary>
        /// Removes the given display object from the system.
        /// </summary>
        /// <param name="d">DisplayObject to remove.</param>
        public void removeDisplayObject(DisplayObject d)
        {

                displayObjects.Remove(d);
                if (!(d is DisplayTerrain))
                    nonTerrainDisplayObjectsCount--;
                if (shaderInstancing)
                    remakeBatches();
            

        }
        /// <summary>
        /// Creates a display object for the given entity and adds it to the system.
        /// </summary>
        /// <param name="e">Entity to add.</param>
        public void addEntity(Entity e)
        {

                DisplayObject d;
                if (shaderInstancing)
                {
                    if (e is Sphere)
                        d = new DisplaySphere((Sphere)e, graphics, true, false);
                    else if (e is Cylinder)
                        d = new DisplayCylinder((Cylinder)e, graphics, true, false);
                    else if (e is Cone)
                        d = new DisplayCone((Cone)e, graphics, true, false);
                    else if (e is Triangle)
                        d = new DisplayTriangle((Triangle)e, graphics, true, false);
                    else if (e is Box)
                        d = new DisplayBox((Box)e, graphics, true, false);
                    else if (e is Capsule)
                        d = new DisplayCapsule((Capsule)e, graphics, true, false);
                    else if (e is ConvexHull)
                        d = new DisplayConvexHull((ConvexHull)e, graphics, true, false);
                    else if (e is MinkowskiSum)
                        d = new DisplayMinkowskiSum((MinkowskiSum)e, graphics, true, false);
                    else if (e is WrappedBody)
                        d = new DisplayWrappedBody((WrappedBody)e, graphics, true, false);
                    else// if (e is CompoundBody)
                        d = new DisplayCompoundBody((CompoundBody)e, graphics, true, false);
                    d.textureIndex = randomColorGenerator.Next(NUM_TEXTURES);

                    addToBatches(d);
                }
                else
                {
                    if (e is Sphere)
                        d = new DisplaySphere((Sphere)e, graphics, true, true);
                    else if (e is Cylinder)
                        d = new DisplayCylinder((Cylinder)e, graphics, true, true);
                    else if (e is Cone)
                        d = new DisplayCone((Cone)e, graphics, true, true);
                    else if (e is Triangle)
                        d = new DisplayTriangle((Triangle)e, graphics, false, false);
                    else if (e is Box)
                        d = new DisplayBox((Box)e, graphics, false, false);
                    else if (e is Capsule)
                        d = new DisplayCapsule((Capsule)e, graphics, true, true);
                    else if (e is ConvexHull)
                        d = new DisplayConvexHull((ConvexHull)e, graphics, true, true);
                    else if (e is MinkowskiSum)
                        d = new DisplayMinkowskiSum((MinkowskiSum)e, graphics, true, true);
                    else if (e is WrappedBody)
                        d = new DisplayWrappedBody((WrappedBody)e, graphics, true, true);
                    else// if (e is CompoundBody)
                        d = new DisplayCompoundBody((CompoundBody)e, graphics, true, true);
                    if (textures != null)
                    {
                        Texture2D texture;
                        texture = textures[randomColorGenerator.Next(textures.Count - 1)];
                        if (d is DisplayCompoundBody)
                            ((DisplayCompoundBody)d).setTexture(texture);
                        else
                            d.texture = texture;
                    }

                }
                nonTerrainDisplayObjectsCount++;
                displayObjects.Add(d);
            
        }

        /// <summary>
        /// Removes the display object associated with the given entity from the drawing system.
        /// </summary>
        /// <param name="e">Entity to remove.</param>
        public void removeEntity(Entity e)
        {

                DisplayObject d;

                for (int k = 0; k < displayObjects.Count; k++)
                {
                    d = displayObjects[k];
                    if (d.getEntity() == e)
                    {
                        displayObjects.RemoveAt(k);
                        nonTerrainDisplayObjectsCount--;
                        remakeBatches();
                        return;
                    }
                }
            
        }

        /// <summary>
        /// Adds the given object's vertices to the appropriate batch and batch buffer.
        /// </summary>
        /// <param name="d">Object to add.</param>
        void addToBatches(DisplayObject d)
        {

            int numBatches = (nonTerrainDisplayObjectsCount) / MAX_INSTANCES;
            int numInstancesInLastBatch = (nonTerrainDisplayObjectsCount) % MAX_INSTANCES;
            if (numInstancesInLastBatch < MAX_INSTANCES && numBatches < batches.Count)
            {
                foreach (VertexPositionNormalTexture v in d.shapeVertices)
                {
                    batches[numBatches].Add(new InstancedVertex(v.Position, v.Normal, v.TextureCoordinate, numInstancesInLastBatch));
                }
                batchBuffers[numBatches].Dispose();
                batchBuffers[numBatches] = new VertexBuffer(graphics.GraphicsDevice, InstancedVertex.SizeInBytes * batches[numBatches].Count, BufferUsage.WriteOnly);
                batchBuffers[numBatches].SetData<InstancedVertex>(batches[numBatches].ToArray());
            }
            else
            {
                batches.Add(new List<InstancedVertex>());
                foreach (VertexPositionNormalTexture v in d.shapeVertices)
                {
                    batches[numBatches].Add(new InstancedVertex(v.Position, v.Normal, v.TextureCoordinate, numInstancesInLastBatch));
                }
                batchBuffers.Add(new VertexBuffer(graphics.GraphicsDevice, InstancedVertex.SizeInBytes * batches[numBatches].Count, BufferUsage.WriteOnly));
                batchBuffers[numBatches].SetData<InstancedVertex>(batches[numBatches].ToArray());
            }

        }

        /// <summary>
        /// Recreates the batches and batch buffers.
        /// </summary>
        void remakeBatches()
        {//This could be optimized significantly if a simulation needs frequent removal of entities- one way would be to defer the creation of vertex buffers until the end by using a local construction method.

            batches.Clear();
            batchBuffers.Clear();
            int temp = nonTerrainDisplayObjectsCount;
            nonTerrainDisplayObjectsCount = 0;
            foreach (DisplayObject d in displayObjects)
            {
                addToBatches(d);
                if (!(d is DisplayTerrain))
                    nonTerrainDisplayObjectsCount++;
            }
            nonTerrainDisplayObjectsCount = temp;

        }

        /// <summary>
        /// Draws the geometry managed by the entity drawer.
        /// </summary>
        /// <param name="viewMatrix">Current view matrix.</param>
        /// <param name="projectionMatrix">Projection matrix.</param>
        /// <param name="frustum">View frustum of the camera used to cull objects in CPU instancing mode.</param>
        public void draw(ref Matrix viewMatrix, ref Matrix projectionMatrix, BoundingFrustum frustum)
        {

                if (isWireFrameMode)
                {
                    drawer.GraphicsDevice.RenderState.FillMode = FillMode.WireFrame;
                    drawer.GraphicsDevice.RenderState.CullMode = CullMode.None;
                }
                if (shaderInstancing)
                    drawShaderInstanced(ref viewMatrix, ref projectionMatrix);
                else
                    drawCPUInstanced(ref viewMatrix, ref projectionMatrix, frustum);
                //Draw models in the world.
                foreach (DisplayModel model in displayModels)
                    model.draw(viewMatrix, projectionMatrix);
                if (isWireFrameMode)
                {
                    drawer.GraphicsDevice.RenderState.FillMode = FillMode.Solid;
                    drawer.GraphicsDevice.RenderState.CullMode = CullMode.CullCounterClockwiseFace;
                }
            
        }

        /// <summary>
        /// Draws the entities using CPU instancing for appropriate types, and a draw-call per object for the others.
        /// </summary>
        /// <param name="viewMatrix">Current view matrix.</param>
        /// <param name="projectionMatrix">Projection matrix.</param>
        /// <param name="frustum">View frustum of the camera used to cull objects.</param>
        void drawCPUInstanced(ref Matrix viewMatrix, ref Matrix projectionMatrix, BoundingFrustum frustum)
        {
            instanceVertices.Clear();
            drawer.View = viewMatrix;
            drawer.Projection = projectionMatrix;
            bool useViewFrustumOcclusion = false;
            if (frustum != null)
                useViewFrustumOcclusion = true;
            bool useInstanceBuffer = false;
            for (int k = 0; k < displayObjects.Count; k++)
            {
                if (displayObjects[k] is DisplayBox || displayObjects[k] is DisplayTriangle || displayObjects[k] is DisplayFluid)
                {
                    if (!useViewFrustumOcclusion || (useViewFrustumOcclusion && frustum.Contains(displayObjects[k].getBoundingBox()) != ContainmentType.Disjoint))
                    {
                        instanceVertices.AddRange(displayObjects[k].getVertices());
                        useInstanceBuffer = true;
                    }
                }
                else
                {
                    if (!useViewFrustumOcclusion || (useViewFrustumOcclusion && frustum.Contains(displayObjects[k].getBoundingBox()) != ContainmentType.Disjoint))
                    {
                        graphics.GraphicsDevice.VertexDeclaration = declaration;
                        displayObjects[k].draw(drawer);
                    }

                }
            }
            if (useInstanceBuffer)
            {
                drawer.World = Matrix.Identity;
                drawer.Texture = displayObjects[0].texture;
                graphics.GraphicsDevice.VertexDeclaration = declaration;
                graphics.GraphicsDevice.Vertices[0].SetSource(null, 0, 0);
                instanceBuffer.SetData<VertexPositionNormalTexture>(instanceVertices.ToArray());
                graphics.GraphicsDevice.Vertices[0].SetSource(instanceBuffer, 0, VertexPositionNormalTexture.SizeInBytes);
                drawer.Begin();
                foreach (EffectPass pass in drawer.CurrentTechnique.Passes)
                {
                    pass.Begin();

                    graphics.GraphicsDevice.DrawPrimitives(PrimitiveType.TriangleList, 0, instanceVertices.Count / 3);

                    pass.End();
                }
                drawer.End();
            }

        }


        /// <summary>
        /// Draws entities using a generally faster (though more GPU intensive) shader instancing method.  Requires SM2.0.
        /// All DisplayObjects should have their vertices initialized by ensuring that they are constructed with initShape = true.
        /// </summary>
        /// <param name="viewMatrix">View matrix to use.</param>
        /// <param name="projectionMatrix">Projection matrix to use.</param>
        void drawShaderInstanced(ref Matrix viewMatrix, ref Matrix projectionMatrix)
        {
            int numBatches = 0;
            int index = 0;
            //Matrix[] worldMatrices = new Matrix[MAX_INSTANCES];

            //Matrix worldMatrix;
            Matrix rotMat;
            Vector3 translation;
            viewParameter.SetValue(viewMatrix);
            viewPositionParameter.SetValue(viewMatrix.Translation);
            drawer.View = viewMatrix;
            drawer.Projection = projectionMatrix;

            DisplayObject d;
            
            for (int k = 0; k < displayObjects.Count; k++)
            {
                d = displayObjects[k];
                if (!(d is DisplayTerrain))
                {

                    rotMat = d.getRotation();
                    rotationMatrices[index] = rotMat;
                    translation = d.getPosition();
                    translations[index] = translation;

                    textureIndices[index] = d.textureIndex;


                    if (index == MAX_INSTANCES - 1 || index == nonTerrainDisplayObjectsCount - 1 || index == nonTerrainDisplayObjectsCount - 1 - (MAX_INSTANCES) * numBatches)
                    {
                        rotationsParameter.SetValue(rotationMatrices);
                        translationsParameter.SetValue(translations);
                        textureIndicesParameter.SetValue(textureIndices);
                        graphics.GraphicsDevice.VertexDeclaration = instancedDeclaration;
                        graphics.GraphicsDevice.Vertices[0].SetSource(batchBuffers[numBatches], 0, InstancedVertex.SizeInBytes);
                        instancedDrawer.Begin();
                        foreach (EffectPass pass in instancedDrawer.CurrentTechnique.Passes)
                        {
                            pass.Begin();

                            graphics.GraphicsDevice.DrawPrimitives(PrimitiveType.TriangleList, 0, batches[numBatches].Count / 3);

                            pass.End();
                        }
                        instancedDrawer.End();
                        index = 0;
                        numBatches++;

                    }
                    else
                        index++;



                }
                else
                {
                    graphics.GraphicsDevice.VertexDeclaration = declaration;
                    d.draw(drawer);
                }
            }

        }

        
    }


    /// <remarks>
    /// Special type of vertex used in shader instanced drawing.
    /// </remarks>
    public struct InstancedVertex
    {
        /// <summary>
        /// Position of the vertex.
        /// </summary>
        public Vector3 Position;
        /// <summary>
        /// Normal of the vertex.
        /// </summary>
        public Vector3 Normal;
        /// <summary>
        /// Texture coordinates of the vertex.
        /// </summary>
        public Vector2 TextureCoordnates;
        /// <summary>
        /// Index of the instance to which this vertex belongs.
        /// </summary>
        public float index;
        /// <summary>
        /// Size in bytes of the instanced vertex type.
        /// </summary>
        public const int SizeInBytes = 36;
        /// <summary>
        /// Constructs an instanced vertex.
        /// </summary>
        /// <param name="pos">Position of the vertex.</param>
        /// <param name="norm">Normal of the vertex.</param>
        /// <param name="textureCoords">Texture coordinates at the vertex.</param>
        /// <param name="instanceIndex">Index of the instance this vertex belongs to.</param>
        public InstancedVertex(Vector3 pos, Vector3 norm, Vector2 textureCoords, float instanceIndex)
        {
            Position = pos;
            Normal = norm;
            TextureCoordnates = textureCoords;
            index = instanceIndex;
        }

        /// <summary>
        /// Defines the elements used by the vertex type; its position, normal, texture coordinates, and index.
        /// </summary>
        public static VertexElement[] VertexElements = new VertexElement[] {
                        new VertexElement(0, 0, VertexElementFormat.Vector3, VertexElementMethod.Default, VertexElementUsage.Position, 0),
                        new VertexElement(0, sizeof(float) * 3, VertexElementFormat.Vector3, VertexElementMethod.Default, VertexElementUsage.TextureCoordinate, 0),
                        new VertexElement(0, sizeof(float) * 6, VertexElementFormat.Vector2, VertexElementMethod.Default, VertexElementUsage.TextureCoordinate, 1),
                        new VertexElement(0, sizeof(float) * 8, VertexElementFormat.Single, VertexElementMethod.Default, VertexElementUsage.TextureCoordinate, 2)};




    }

    

}
