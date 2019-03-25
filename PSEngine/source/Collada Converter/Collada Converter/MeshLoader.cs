using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.IO;

namespace Collada_Converter
{
    public class Vector3f
    {
        public float x;
        public float y;
        public float z;
    }

    public class VertexElementArray
    {
        public List<Vector3f> values;
        public String name;
        public String semantic;

        public VertexElementArray()
        {
            values = new List<Vector3f>();
            name = "not_set";
            semantic = "not_set";
        }
    }

    public class VertexElementOutputArray
    {
        public String semantic_name;
        public List<Vector3f> values;

        public VertexElementOutputArray()
        {
            values = new List<Vector3f>();
        }
    }

    public class Mesh
    {
        public List<VertexElementArray> elements;
        public List<VertexElementOutputArray> out_elements;
        public String m_name;
        public List<int> m_indices;
        public String material_name;

        public Mesh(String name)
        {
            elements = new List<VertexElementArray>();
            out_elements = new List<VertexElementOutputArray>();
            m_name = name;
            m_indices = new List<int>();
        }
    }

    public partial class converter
    {
        VertexElementArray findVertexElementArray(String name)
        {
            foreach (VertexElementArray ve in meshes[current_mesh].elements)
            {
                if (ve.name == (name.Remove(0, 1)))
                {
                    return ve;
                }
            }

            return null;
        }

        bool makeIndexable(XmlNode node, out VertexElementArray array_to_index, out int offset, out String semantic_name)
        {
            semantic_name = String.Empty;

            String source_name = String.Empty;

            offset = 0;

            foreach (XmlAttribute attrib in node.Attributes)
            {
                switch (attrib.Name)
                {
                    case "semantic":
                        semantic_name = attrib.Value;
                        break;

                    case "source":
                        source_name = attrib.Value;
                        break;

                    case "offset":
                        offset = System.Convert.ToInt32(attrib.Value);
                        break;
                }
            }

            array_to_index = findVertexElementArray(source_name);

            if (array_to_index != null) return true;

            return false;
        }

        void buildIndexedData(XmlNode triangles_node)
        {
            List<VertexElementArray> element_list = new List<VertexElementArray>();
            List<int> offset_list = new List<int>();
            List<String> semantic_list = new List<String>();

            String index_array_s = null;
            String[] index_array_ss = null;
            int[] index_array = null;

            int stride = 0;
            int index_count = 0;

            foreach (XmlNode node in triangles_node.ChildNodes)
            {
                switch (node.Name)
                {
                    case "input":

                        VertexElementArray ve = null;
                        int offset;
                        String semantic;

                        if (makeIndexable(node, out ve, out offset, out semantic))
                        {
                            element_list.Add(ve);
                            offset_list.Add(offset);
                            semantic_list.Add(semantic);
                        }

                        stride = Math.Max((offset + 1), stride);

                        break;

                    case "p":

                        //build indices
                        index_array_s = node.InnerText;
                        index_array_ss = index_array_s.Split(' ');
                        index_count = index_array_ss.Length;

                        index_array = new int[index_count];

                        for (int i = 0; i < index_count; i++)
                        {
                            index_array[i] = System.Convert.ToInt32(index_array_ss[i]);
                        }

                        break;
                }
            }

            for (int i = 0; i < index_count; i += stride)
            {
                int index = index_array[i];
                meshes[current_mesh].m_indices.Add(index);
            }

            for (int i = 0; i < element_list.Count; i++)
            {
                VertexElementOutputArray output_array = new VertexElementOutputArray();
                VertexElementArray current_element = element_list[i];
                int offset = offset_list[i];

                output_array.semantic_name = semantic_list[i];

                for (int j = offset; j < index_count; j += stride)
                {
                    int index = index_array[j];
                    output_array.values.Add(current_element.values[index]);
                }

                meshes[current_mesh].out_elements.Add(output_array);
            }
        }

        void loadVertices(XmlNode element_node)
        {
            String element_name = String.Empty;

            foreach (XmlAttribute attrib in element_node.Attributes)
            {
                switch (attrib.Name)
                {
                    case "id":
                        element_name = attrib.Value;
                        break;
                }
            }

            foreach (XmlNode node in element_node.ChildNodes)
            {
                switch (node.Name)
                {
                    case "input":
                        {
                            foreach (XmlAttribute attrib in node.Attributes)
                            {
                                switch (attrib.Name)
                                {
                                    case "source":

                                        VertexElementArray ve = new VertexElementArray();
                                        String ve_name = attrib.Value;
                                        ve.name = element_name;

                                        VertexElementArray match = findVertexElementArray(ve_name);
                                        if (match != null)
                                        {
                                            ve.values = match.values;
                                            meshes[current_mesh].elements.Add(ve);
                                        }

                                        break;
                                }
                            }


                        }
                        break;
                }
            }
        }

        void loadVertexElement(XmlNode element_node)
        {
            String element_name = String.Empty;

            foreach (XmlAttribute attrib in element_node.Attributes)
            {
                switch (attrib.Name)
                {
                    case "id":
                        element_name = attrib.Value;
                        break;
                }
            }

            foreach (XmlNode node in element_node.ChildNodes)
            {
                switch (node.Name)
                {
                    case "float_array":
                        {
                            VertexElementArray element_array = new VertexElementArray();
                            element_array.name = element_name;

                            int count = 0;

                            foreach (XmlAttribute attrib in node.Attributes)
                            {
                                switch (attrib.Name)
                                {
                                    case "count":

                                        count = System.Convert.ToInt32(attrib.Value);

                                        break;
                                }
                            }

                            String values = node.InnerText;

                            String[] split_vals = new String[count];
                            split_vals = values.Split(' ');

                            for (int i = 0; i < count; i += 3)
                            {
                                Vector3f v = new Vector3f();

                                v.x = System.Convert.ToSingle(split_vals[i]);
                                v.y = System.Convert.ToSingle(split_vals[i + 1]);
                                v.z = System.Convert.ToSingle(split_vals[i + 2]);

                                element_array.values.Add(v);
                            }

                            meshes[current_mesh].elements.Add(element_array);
                        }
                        break;
                }
            }
        }

        void loadMesh(XmlNode mesh_node, String name)
        {
            meshes.Add(new Mesh(name));

            foreach (XmlNode node in mesh_node.ChildNodes)
            {
                switch (node.Name)
                {
                    case "source":

                        loadVertexElement(node);

                        break;

                    case "vertices":

                        loadVertices(node);

                        break;

                    case "triangles":

                        meshes[current_mesh].material_name = null;

                        foreach (XmlAttribute attrib in node.Attributes)
                        {
                            switch (attrib.Name)
                            {
                                case "material":

                                    meshes[current_mesh].material_name = attrib.Value;

                                    break;
                            }
                        }

                        buildIndexedData(node);

                        break;
                }
            }

            current_mesh++;
        }

        void loadGeometry(XmlNode geom_node)
        {
            String mesh_name = String.Empty;

            foreach (XmlAttribute attrib in geom_node.Attributes)
            {
                switch (attrib.Name)
                {
                    case "name":

                        mesh_name = attrib.Value;

                        break;
                }
            }


            foreach (XmlNode node in geom_node.ChildNodes)
            {
                switch (node.Name)
                {
                    case "mesh":

                        loadMesh(node, mesh_name);

                        break;
                }
            }
        }
    }
}
