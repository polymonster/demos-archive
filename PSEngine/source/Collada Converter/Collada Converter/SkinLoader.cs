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
    class SkinElement
    {
        public String name;
        public String semantic;

        public List<String> values_s;
        public List<float> values_f;

        public SkinElement()
        {
            name = "not_set";
            semantic = "not_set";

            values_s = new List<String>();
            values_f = new List<float>();
        }
    }

    class SkinnedJoint
    {
        public float[] inverse_bind_matrix;
        public String name;

        public SkinnedJoint()
        {
            inverse_bind_matrix = new float[16];
            name = "not_set";
        }
    }

    class SkinnedVertex
    {
       public List<uint> skinned_joints;
       public List<float> weights;

       public SkinnedVertex()
       {
           skinned_joints = new List<uint>();
           weights = new List<float>();
       }
    }

    class Skin
    {
        public float[] bind_shape_matrix;
        public String mesh_name;

        public List<SkinnedJoint> skinned_joints;
        public List<SkinnedVertex> skinned_vertices;

        public List<SkinElement> skin_elements;

        public Skin()
        {
            bind_shape_matrix = new float[16];
            mesh_name = "not_set";

            skinned_joints = new List<SkinnedJoint>();
            skin_elements = new List<SkinElement>();
            skinned_vertices = new List<SkinnedVertex>();
        }

        public void makeSkinnedJoints(String joints_id, String bind_poses_id)
        {
            SkinElement joints = new SkinElement();
            SkinElement bind_poses = new SkinElement();

            foreach (SkinElement s in skin_elements)
            {
                if (s.name == joints_id.Remove(0, 1))
                {
                    joints = s;
                }

                if (s.name == bind_poses_id.Remove(0, 1))
                {
                    bind_poses = s;
                }
            }

           for(int i = 0; i < joints.values_s.Count; i++)
           {
               SkinnedJoint new_sj = new SkinnedJoint();

               new_sj.name = joints.values_s[i];

               for(int j = 0; j < 16; j++)
               {
                   int index = (i * 16) + j;
                   new_sj.inverse_bind_matrix[j] = bind_poses.values_f[index];
               }

               skinned_joints.Add(new_sj);
           }
        }

        public void makeSkinnedVerts(XmlNode node, String joints_id, String weights_id)
        {
            SkinElement joints = new SkinElement();
            SkinElement weights = new SkinElement();

            foreach (SkinElement s in skin_elements)
            {
                if (s.name == joints_id.Remove(0, 1))
                {
                    joints = s;
                }

                if (s.name == weights_id.Remove(0, 1))
                {
                    weights = s;
                }
            }

            int[] number_of_bones = null;
            int[] vertex_details = null;

            foreach (XmlNode sub_node in node.ChildNodes)
            {
                String semantic = String.Empty;
                String target = String.Empty;

                switch (sub_node.Name)
                {
                    case "vcount":

                        String[] number_of_bones_s = sub_node.InnerText.Split(' ');

                        number_of_bones = new int[number_of_bones_s.Length];
                        for (int i = 0; i < number_of_bones_s.Length; i++)
                        {
                            number_of_bones[i] = System.Convert.ToInt32(number_of_bones_s[i]);
                        }

                        break;

                    case "v":

                        String[] details = sub_node.InnerText.Split(' ');

                        vertex_details = new int[details.Length];
                        for (int i = 0; i < details.Length; i++)
                        {
                            vertex_details[i] = System.Convert.ToInt32(details[i]);
                        }

                        break;
                }
            }

            int current_detail_loc = 0;

            for(int i = 0; i < number_of_bones.Length; i++)
            {
                int count = number_of_bones[i];

                SkinnedVertex sv = new SkinnedVertex();

                for(int j = 0; j < count; j++)
                {
                    sv.skinned_joints.Add((uint) vertex_details[current_detail_loc++]);
                    sv.weights.Add(weights.values_f[ vertex_details[current_detail_loc++] ]);
                }

                skinned_vertices.Add(sv);
            }
        }
    }

    public partial class converter
    {
        String joints = String.Empty;
        String weights = String.Empty;

        void makeSkinnedVerts(XmlNode node, Skin new_skin)
        {
            foreach (XmlNode sub_node in node.ChildNodes)
            {
                String semantic = String.Empty;
                String target = String.Empty;

                switch (sub_node.Name)
                {
                    case "input":

                        foreach (XmlAttribute attrib in sub_node.Attributes)
                        {
                            switch (attrib.Name)
                            {
                                case "semantic":

                                    semantic = attrib.Value;

                                    break;

                                case "source":

                                    target = attrib.Value;

                                    break;
                            }
                        }

                        break;
                }

                if (semantic == "JOINT") joints = target;
                if (semantic == "WEIGHT") weights = target;
            }

            new_skin.makeSkinnedVerts(node, joints, weights);
        }

        void makeSkinnedJoints(XmlNode node, Skin new_skin)
        {
            String joints = String.Empty;
            String matrix = String.Empty;

            foreach (XmlNode sub_node in node.ChildNodes)
            {
                String semantic = String.Empty;
                String target = String.Empty;

                switch (sub_node.Name)
                {
                    case "input":

                        foreach (XmlAttribute attrib in sub_node.Attributes)
                        {
                            switch (attrib.Name)
                            {
                                case "semantic":

                                    semantic = attrib.Value;

                                    break;

                                case "source":

                                    target = attrib.Value;

                                    break;
                            }
                        }

                        break;
                }

                if(semantic == "JOINT") joints = target;
                if(semantic == "INV_BIND_MATRIX") matrix = target;
            }

            new_skin.makeSkinnedJoints(joints, matrix);
        }

        SkinElement addSkinSource(XmlNode node)
        {
            SkinElement new_skin_source = new SkinElement();

            foreach (XmlAttribute attrib in node.Attributes)
            {
                switch (attrib.Name)
                {
                    case "id":

                        new_skin_source.name = attrib.Value;

                        break;
                }
            }

            foreach (XmlNode sub_node in node.ChildNodes)
            {
                switch (sub_node.Name)
                {
                    case "Name_array":

                        String[] names = sub_node.InnerText.Split(' ');

                        new_skin_source.semantic = "NAMES";

                        for (int i = 0; i < names.Length; i++)
                            new_skin_source.values_s.Add(names[i]); 
      

                        break;

                    case "float_array":

                        String[] values_s = sub_node.InnerText.Split(' ');

                        new_skin_source.semantic = "FLOATS";

                        for (int i = 0; i < values_s.Length; i++)
                            new_skin_source.values_f.Add(System.Convert.ToSingle( values_s[i]) ); 

                        break;
                }
            }

            return new_skin_source;
        }

        void loadSkin(XmlNode node)
        {
            Skin new_skin = new Skin();

            foreach (XmlAttribute attrib in node.Attributes)
            {
                switch (attrib.Name)
                {
                    case "source":

                        new_skin.mesh_name = attrib.Value.Split('-')[1];

                        break;
                }
            }

            foreach (XmlNode sub_node in node.ChildNodes)
            {
                switch (sub_node.Name)
                {
                    case "bind_shape_matrix":

                        String[] matrix_s = sub_node.InnerText.Split(' ');

                        for (int i = 0; i < matrix_s.Length; i++)
                        {
                            new_skin.bind_shape_matrix[i] = System.Convert.ToSingle(matrix_s[i]);
                        }

                        break;

                    case "source":

                        new_skin.skin_elements.Add( addSkinSource(sub_node) );

                        break;

                    case "joints":

                        makeSkinnedJoints(sub_node, new_skin);

                        break;

                    case "vertex_weights":

                        makeSkinnedVerts(sub_node, new_skin);

                        break;
                }
            }

            skins.Add(new_skin);
        }

        void loadController(XmlNode node)
        {
            foreach (XmlNode sub_node in node.ChildNodes)
            {
                switch (sub_node.Name)
                {
                    case "skin":

                        loadSkin(sub_node);

                        break;
                }
            }
        }

    }
}