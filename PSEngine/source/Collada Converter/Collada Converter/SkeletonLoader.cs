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
    public class Joint
    {
        public float[] matrix;
        public String parent_name;
        public String name;
        public String id;

        public Joint()
        {
            matrix = new float[16];
            parent_name = String.Empty;
            name = String.Empty;
        }

    }

    public class Skeleton
    {
        public List<Joint> joints;

        public Skeleton()
        {
            joints = new List<Joint>();
        }
    }

    public partial class converter
    {
        void loadJoints(XmlNode node, String parent_name)
        {
            String joint_name = "root";
            String id = "null";

            String[] matrix_s;
            float[] matrix = new float[16];

            foreach (XmlAttribute attrib in node.Attributes)
            {
                switch (attrib.Name)
                {
                    case "sid":

                        joint_name = attrib.Value;

                        break;

                    case "id":

                        id = attrib.Value;

                        break;

                    case "type":

                        if (attrib.Value == "JOINT")
                        {
                            foreach (XmlNode sub_node in node.ChildNodes)
                            {
                                switch (sub_node.Name)
                                {
                                    case "matrix":

                                        matrix_s = sub_node.InnerText.Split(' ');

                                        for (int i = 0; i < 16; i++)
                                            matrix[i] = System.Convert.ToSingle(matrix_s[i]);

                                        Joint j = new Joint();

                                        j.name = joint_name;
                                        j.id = id;
                                        j.parent_name = parent_name;
                                        j.matrix = matrix;

                                        skeleton.joints.Add(j);

                                        break;

                                    case "node":

                                        loadJoints(sub_node, joint_name);

                                        break;
                                }
                            }
                        }
                        break;
                }
            }
        }
    }
}