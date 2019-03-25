using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Net;
using Microsoft.Xna.Framework.Storage;


namespace Tanky
{
    //model rendering class
    public class ModelTP
    {
        Model m_model;
        Texture2D[] m_texture;
        public String m_name;

        public bool check_list()
        {
            foreach (ModelEntry entry in g.model_list)
            {
                if (entry.name == m_name)
                {
                    m_texture = new Texture2D[entry.texture_count];

                    for (int i = 0; i < entry.texture_count; i++)
                    {
                         m_texture[i] = entry.texture[i];
                    }

                    return true;
                }
            }

            return false;
        }

        public void load(String filename)
        {
            m_name = filename;

            m_model = g.content.Load<Model>(filename);

            remap(m_model, g.model_shader);
        }

        public void remap(Model model, Effect effect)
        {
            //check if a new texture needs to be loaded (false if its already loaded in the array)
            if (!check_list())
            {
                //create a new model texture entry
                ModelEntry new_model = new ModelEntry();

                //ensure the mesh id, is syncronised with the texture id (i.e mesh 1 uses texture 1)
                new_model.texture = new Texture2D[model.Meshes.Count];
                m_texture = new Texture2D[model.Meshes.Count];
                new_model.texture_count = model.Meshes.Count;

                int counter = 0;

                //loop though the mesh
                foreach (ModelMesh mesh in model.Meshes)
                {
                    //use basic effect to obtain texture information
                    foreach (BasicEffect basic in mesh.Effects)
                    {
                        //set the corresponind model id, to use the same texture id
                        new_model.texture[counter] = basic.Texture;
                        new_model.name = m_name;

                        m_texture[counter] = new_model.texture[counter];
                    }

                    counter++;
                }

                //add the model to the global model list (prevent duplicating this data when loaded again)
                g.model_list.Add(new_model);
            }

            //loop thorugh all the meshes within the model
            foreach (ModelMesh mesh in model.Meshes)
            {
                //loop through each mesh part
                foreach (ModelMeshPart part in mesh.MeshParts)
                {
                    //set the poarts effect to the custom HLSL shader
                    part.Effect = effect;
                }
            }
        }

        public void get_vertex_data(ref Vector3[] data, int start_element, int end_element)
        {
            foreach (ModelMesh mesh in m_model.Meshes)
            {
                mesh.VertexBuffer.GetData<Vector3>(data, start_element, end_element);

                return;
            }
        }

        public void get_vertex_data(ref Vector3[] data)
        {
            foreach (ModelMesh mesh in m_model.Meshes)
            {
                mesh.VertexBuffer.GetData<Vector3>(data);

                return;
            }
        }

        public void draw(Matrix projection, Matrix view, Matrix world)
        {
            int counter = 0;

            //loop through each sub mesh
            foreach (ModelMesh mesh in m_model.Meshes)
            {
                foreach (Effect effect in mesh.Effects)
                {
                    effect.CurrentTechnique = effect.Techniques["MainRender"];

                    g.model_shader.Parameters["view_projection_matrix"].SetValue
                        (world * view * projection);

                    g.model_shader.Parameters["shadow_projection_matrix"].SetValue
                        (world * g.camera.m_shadow_view * g.camera.m_shadow_projection);

                    g.model_shader.Parameters["shadow_catser_position"].SetValue
                        (g.camera.m_shadow_position);

                    g.model_shader.Parameters["world_matrix"].SetValue
                            (world);

                    g.model_shader.Parameters["x_diffuse_texture"].SetValue(m_texture[counter]);

                    int lit = 1;

                    switch (m_name)
                    {
                        case "Models\\tree":
                        case "Models\\tree_2":
                        case "Models\\desert_bush":
                        case "Models\\forest_bush":

                            lit = 0;

                        break;

                        default:

                            lit = 1;

                        break;
                    }

                    g.model_shader.Parameters["lit"].SetValue(lit);

                    g.model_shader.Parameters["x_shadow_map"].SetValue(g.shadow_map.m_shadow_map);

                    //finally render the mesh
                    mesh.Draw();
                }

                counter++;
            }
        }

        public void shadow_map_render(Matrix projection, Matrix view, Matrix world)
        {
            int counter = 0;

            //loop through each sub mesh
            foreach (ModelMesh mesh in m_model.Meshes)
            {
                foreach (Effect effect in mesh.Effects)
                {
                    effect.CurrentTechnique = effect.Techniques["ShadowRender"];

                    g.model_shader.Parameters["view_projection_matrix"].SetValue
                        (world * view * projection);

                    g.model_shader.Parameters["shadow_catser_position"].SetValue
                        (g.camera.m_shadow_position);

                    g.model_shader.Parameters["x_diffuse_texture"].SetValue(m_texture[counter]);

                    g.model_shader.Parameters["world_matrix"].SetValue
                            (world);

                    //finally render the mesh
                    mesh.Draw();
                }

                counter++;
            }
        }

		public Model GetModel()
        {
            return m_model;
        }

        /// <summary>
        /// makes a box that is physically simulated
        /// </summary>
        /// <param name="mass">the weight of the box</param>
        /// <returns></returns>
        public BEPUphysics.Box GetBepuBox(float mass,float scale)
        {
            BEPUphysics.Box ReturnBox;
            float width;
            float height;
            float length;

            ModelMesh mesh = m_model.Meshes[0];

            //create empty array of vertices
            TerrainVertex[] temp = new TerrainVertex[mesh.VertexBuffer.SizeInBytes / TerrainVertex.SizeInBytes];
            //fill with vertex data
            mesh.VertexBuffer.GetData<TerrainVertex>(temp);

            //create empty array of Vector3
            List<Vector3> vertices = new List<Vector3>();
            //make a list of the positional coords
            for (int i = 0; i < temp.Length; i++)
            {
                vertices.Add(temp[i].pos);
            }
            //create a bounding box
            BoundingBox tempbox = BoundingBox.CreateFromPoints(vertices.ToArray());

            //get the width,height and length
            width = tempbox.Max.X - tempbox.Min.X * 0.7f;
            height = tempbox.Max.Y - tempbox.Min.Y * 0.7f;
            length = tempbox.Max.Z - tempbox.Min.Z * 0.7f;

            ReturnBox = new BEPUphysics.Box(new Vector3(0.0f, 0.0f, 0f), width * scale, height * scale, length * scale, mass);

            return ReturnBox;
        }

        /// <summary>
        /// makes a box that is physically simulated
        /// </summary>
        /// <param name="mass">the weight of the box</param>
        /// <returns></returns>
        public BEPUphysics.Box GetBepuBox(float mass)
        {
            BEPUphysics.Box ReturnBox;
            float width;
            float height;
            float length;

            ModelMesh mesh = m_model.Meshes[0];
            
            //create empty array of vertices
            TerrainVertex[] temp = new TerrainVertex[mesh.VertexBuffer.SizeInBytes / TerrainVertex.SizeInBytes];
            //fill with vertex data
            mesh.VertexBuffer.GetData<TerrainVertex>(temp);

            //create empty array of Vector3
            List<Vector3> vertices = new List<Vector3>();
            //make a list of the positional coords
            for (int i = 0; i < temp.Length; i++)
            {
                vertices.Add(temp[i].pos);
            }
            //create a bounding box
            BoundingBox tempbox = BoundingBox.CreateFromPoints(vertices.ToArray());

            //get the width,height and length
            width = tempbox.Max.X - tempbox.Min.X * 0.7f;
            height = tempbox.Max.Y - tempbox.Min.Y * 0.7f;
            length = tempbox.Max.Z - tempbox.Min.Z * 0.7f;

                /*Vector3 min = Vector3.Zero;
                Vector3 max = Vector3.Zero;

                Vector3[] vertices = new Vector3[1024];

                get_vertex_data(ref vertices);

                for (int i = 0; i < vertices.Length; i++)
                {
                    if (i == 0)
                    {
                        min = vertices[i];
                        max = vertices[i];
                    }
                    else
                    {
                        //mins
                        if (vertices[i].X < min.X) min.X = vertices[i].X;
                        if (vertices[i].Y < min.Y) min.Y = vertices[i].Z;
                        if (vertices[i].Z < min.Z) min.Z = vertices[i].Z;

                        //max's
                        if (vertices[i].X > max.X) max.X = vertices[i].X;
                        if (vertices[i].Y > max.Y) max.Y = vertices[i].Y;
                        if (vertices[i].Z > max.Z) max.Z = vertices[i].Z;
                    }
                }

                width = (max.X - min.X);
                height = (max.Y - min.Y);
                length = (max.Z - min.Z);*/

            ReturnBox = new BEPUphysics.Box(new Vector3( 0.0f, 0.0f, -10.0f), width, height, length,mass);

            return ReturnBox;
        }

        /// <summary>
        /// Makes a nondynamic box
        /// </summary>
        /// <returns></returns>
        public BEPUphysics.Box GetBepuBox()
        {
            BEPUphysics.Box ReturnBox;
            float width;
            float height;
            float length;

            ModelMesh mesh = m_model.Meshes[0];

            //create empty array of vertices
            TerrainVertex[] temp = new TerrainVertex[mesh.VertexBuffer.SizeInBytes / TerrainVertex.SizeInBytes];
            //fill with vertex data
            mesh.VertexBuffer.GetData<TerrainVertex>(temp);

            //create empty array of Vector3
            List<Vector3> vertices = new List<Vector3>();
            //make a list of the positional coords
            for (int i = 0; i < temp.Length; i++)
            {
                vertices.Add(temp[i].pos);
            }
            //create a bounding box;
            BoundingBox tempbox = BoundingBox.CreateFromPoints(vertices.ToArray());

            //get the width,height and length
            width = tempbox.Max.X - tempbox.Min.X;
            height = tempbox.Max.Y - tempbox.Min.Y;             
            length = tempbox.Max.Z - tempbox.Min.Z;

            Vector3 boxCentre;
            boxCentre.X = tempbox.Max.X - (width * 0.5f);
            boxCentre.Y = tempbox.Max.Y - (height * 0.5f);
            boxCentre.Z = tempbox.Max.Z - (length * 0.5f);

            ReturnBox = new BEPUphysics.Box(boxCentre, width, height, length);

            return ReturnBox;
        }
    }
}
