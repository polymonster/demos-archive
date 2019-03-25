using System;
using System.IO;
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
    [Serializable]
    public struct TerrainVertex
    {
        Vector3 vertex_pos;
        Vector4 vertex_weight;
        Vector3 vertex_normal;
        Vector2 vertex_texture_coords;

        public static int SizeInBytes { get { return sizeof(float) * 12; } }

        public static readonly VertexElement[] VertexElements = new VertexElement[]
        {
           new VertexElement(0, 0, VertexElementFormat.Vector3,
            VertexElementMethod.Default, VertexElementUsage.Position, 0),

           new VertexElement(0, sizeof(float) * 3, VertexElementFormat.Vector4,
            VertexElementMethod.Default, VertexElementUsage.Color, 0),

           new VertexElement(0, sizeof(float) * 7, VertexElementFormat.Vector3,
            VertexElementMethod.Default, VertexElementUsage.Normal, 0),

           new VertexElement(0, sizeof(float) * 10, VertexElementFormat.Vector2,
            VertexElementMethod.Default, VertexElementUsage.TextureCoordinate, 0),
        };

        public TerrainVertex(Vector3 pos, Vector4 weight, Vector3 normal, Vector2 texture_coords)
        {
            vertex_pos = pos;
            vertex_weight = weight;
            vertex_normal = normal;
            vertex_texture_coords = texture_coords;
        }

        //Public methods for accessing the components of the custom vertex.
        public Vector3 pos { get { return vertex_pos; } set { vertex_pos = value; } }
        public Vector4 weight { get { return vertex_weight; } set { vertex_weight = value; } }
        public Vector3 normal { get { return vertex_normal; } set { vertex_normal = value; } }
        public Vector2 texture_coords { get { return vertex_texture_coords; } set { vertex_texture_coords = value; } }
    }

    class Terrain
    {
        //-------------------------------------------------------------------------------------------

        public TerrainVertex[] vertices;
        VertexDeclaration m_vertex_declaration;
        IndexBuffer m_index_buffer;
        VertexBuffer m_vertex_buffer;

        Texture2D[] m_texture = new Texture2D[4];

        int[] m_indices;

        int m_width;
        int m_height;

        private void SetUpVertices(String filename)
        {
            FileStream fin = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.Read);
            StreamReader stream = new StreamReader(fin);

            switch (filename)
            {
                case "Maps\\level_1_terrain.txt":

                    m_texture[0] = g.content.Load<Texture2D>("Textures\\Terrain\\gras_02");
                    m_texture[1] = g.content.Load<Texture2D>("Textures\\Terrain\\soil_02");
                    m_texture[2] = g.content.Load<Texture2D>("Textures\\Terrain\\ground_03");
                    m_texture[3] = g.content.Load<Texture2D>("Textures\\Terrain\\stone_02");

                break;

                case "Maps\\level_2_terrain.txt":

                    m_texture[0] = g.content.Load<Texture2D>("Textures\\Terrain\\ground_03");
                    m_texture[1] = g.content.Load<Texture2D>("Textures\\Terrain\\sand_01");
                    m_texture[2] = g.content.Load<Texture2D>("Textures\\Terrain\\sand_03");
                    m_texture[3] = g.content.Load<Texture2D>("Textures\\Terrain\\soil_02");

                break;

                case "Maps\\level_3_terrain.txt":

                    m_texture[0] = g.content.Load<Texture2D>("Textures\\Terrain\\snow_01");
                    m_texture[1] = g.content.Load<Texture2D>("Textures\\Terrain\\snow_02");
                    m_texture[2] = g.content.Load<Texture2D>("Textures\\Terrain\\snow_03");
                    m_texture[3] = g.content.Load<Texture2D>("Textures\\Terrain\\gras_01");

                break;

            }

            while (!stream.EndOfStream)
            {
                String line = stream.ReadLine();
                String[] splitted = line.Split(' ');


                if (splitted[0] == "terrain")
                {
                    m_width = System.Convert.ToInt32(splitted[1]);
                    m_height = System.Convert.ToInt32(splitted[2]);

                    m_indices = new int[m_width * m_height * 3];
                    vertices = new TerrainVertex[m_width * m_height];

                   for (int i = 0; i < m_width * m_height; i++)
                   {
                        String read = stream.ReadLine();
                        String[] data = read.Split(' ');

                        vertices[i].pos =
                            new Vector3(System.Convert.ToSingle(data[0]),
                             System.Convert.ToSingle(data[1]),
                             System.Convert.ToSingle(data[2]));

                        Vector3 tempNorm;
                        tempNorm.X = System.Convert.ToSingle(data[3]);
                        tempNorm.Y = System.Convert.ToSingle(data[4]);
                        tempNorm.Z = System.Convert.ToSingle(data[5]);
                        tempNorm.Normalize();

                       vertices[i].normal = tempNorm;

                       vertices[i].weight =
                           new Vector4(System.Convert.ToSingle(data[6]),
                               System.Convert.ToSingle(data[7]),
                               System.Convert.ToSingle(data[8]),
                               System.Convert.ToSingle(data[9]));

                        vertices[i].texture_coords =
                         new Vector2(System.Convert.ToSingle(data[10]) * 0.25f,
                             System.Convert.ToSingle(data[11]) * 0.25f);
                   }

                }
            }

            int index_count = 0;

            for (int i = 0; i < (m_width - 1); i++)
            {
                for (int j = 0; j < (m_height - 1); j++)
                {
                    //index 1 is the first vertex to render
                    //in a primitive
                    int index1 = (i * m_width) + j;
                    //index 2 is the vertex next to index 1
                    int index2 = ((i + 1) * m_width) + j;

                    //if we are at the start of a row we need to
                    //render a degenerate polygon
                    if (j == 0) m_indices[index_count++] = index1;

                    //add the first vertex of the primitive
                    m_indices[index_count++] = index1;
                    //add the next vertex in the strip
                    m_indices[index_count++] = index2;

                    if (j == m_width - 2)
                    {
                        //if we are at the end of a row
                        //we need to render another vertex
                        //to make a degenerate polygon
                        m_indices[index_count] = index2;
                        index_count++;
                    }
                }
            }

            m_vertex_buffer = new VertexBuffer(g.graphics.GraphicsDevice, vertices.Length * TerrainVertex.SizeInBytes, BufferUsage.WriteOnly);
            m_vertex_buffer.SetData(vertices);

            m_index_buffer = new IndexBuffer(g.graphics.GraphicsDevice, typeof(int), m_indices.Length, BufferUsage.WriteOnly);
            m_index_buffer.SetData(m_indices);

            m_vertex_declaration = new VertexDeclaration(g.graphics.GraphicsDevice,
            TerrainVertex.VertexElements);
        }

        public void load(String filename)
        {
            SetUpVertices(filename);

            get_heights();
        }

        public void draw()
        {
               g.graphics.GraphicsDevice.RenderState.CullMode = CullMode.CullClockwiseFace;

               g.terrain_shader.Parameters["world_matrix"].SetValue
                        (Matrix.CreateScale(0.1f, 0.1f, 0.1f));

               g.terrain_shader.Parameters["view_projection_matrix"].SetValue
                   (Matrix.CreateScale(0.1f,0.1f,0.1f) * g.camera.m_view_matrix * g.camera.m_projection_matrix);

               g.terrain_shader.Parameters["shadow_projection_matrix"].SetValue
                    (Matrix.CreateScale(0.1f,0.1f,0.1f) * g.camera.m_shadow_view * g.camera.m_shadow_projection);

               g.terrain_shader.Parameters["shadow_catser_position"].SetValue
               (g.camera.m_shadow_position);


               for (int i = 0; i < 4; i++)
               {
                   int j = i + 1;
                   String shader_location = "x_texture_" + j.ToString();
                   g.terrain_shader.Parameters[shader_location].SetValue(m_texture[i]);
               }

               g.terrain_shader.Parameters["x_shadow_map"].SetValue(g.shadow_map.m_shadow_map);

               g.terrain_shader.Begin();

               g.graphics.GraphicsDevice.VertexDeclaration = m_vertex_declaration;

               foreach (EffectPass pass in g.terrain_shader.CurrentTechnique.Passes)
               {
                   pass.Begin();

                   //bind the index buffer of the terrain
                   g.graphics.GraphicsDevice.Indices = m_index_buffer;
                   //set the vertex buffer source data (terrain data loaded from file)
                   g.graphics.GraphicsDevice.Vertices[0].SetSource(m_vertex_buffer, 0, TerrainVertex.SizeInBytes);
                   //draw a traingle strip size of width * height 
                   g.graphics.GraphicsDevice.DrawIndexedPrimitives
                       (PrimitiveType.TriangleStrip, 0, 0, m_width * m_height, 0, (m_indices.Length / 3) * 2);

                   pass.End();
               }

               g.terrain_shader.End();
               g.graphics.GraphicsDevice.RenderState.CullMode = CullMode.CullCounterClockwiseFace;

        }

        public Vector2 GetTerrainDimensions()
        {
            return new Vector2(m_width, m_height);
        }
        public float[,] get_heights()
        {
            float[,] temp = new float[m_width,m_height];
            int CurrentIndex;

            for (int i = 0; i < m_width; i++)
            {
                for (int j = 0; j < m_height; j++)
                {
                    CurrentIndex = (i * m_width) + j;
                    temp[i, j] = ((TerrainVertex)vertices.GetValue(CurrentIndex)).pos.Y * g.scaleFactor;
                }
            }

            return temp;
        }
    }
}
