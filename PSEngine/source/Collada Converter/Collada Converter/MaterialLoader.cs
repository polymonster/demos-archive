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
    public class ColourFloat
    {
        public float r, g, b, a;

        public ColourFloat()
        {
            r = g = b = a = 1.0f;
        }

        public override String ToString()
        {
            String return_string = 
                r.ToString() + " " +
                g.ToString() + " " +
                b.ToString() + " " +
                a.ToString();

            return return_string;
        }
    }

    public class Material
    {
        public String name;
        public String texture_name;

        public ColourFloat ambient;
        public ColourFloat diffuse;
        public ColourFloat specular;

        public float transparency;
        public float shininess;

        public Material()
        {
            ambient = new ColourFloat();
            diffuse = new ColourFloat();
            specular = new ColourFloat();
            transparency = 1.0f;
            shininess = 1.0f;
            name = "none";
            texture_name = "none";
        }
    }

    public partial class converter
    {
        int getMaterialByName(String name)
        {
            int count = 0;
            foreach(Material m in materials)
            {
                if (name.Contains(m.name))
                {
                    return count;
                }

                count++;
            }

            return -1;
        }
        ColourFloat loadColour(XmlNode node)
        {
            ColourFloat return_colour = new ColourFloat();

            foreach(XmlNode sub_node in node.ChildNodes)
            {
                if (sub_node.Name == "color")
                {
                    String[] colour_split_s = sub_node.InnerText.Split(' ');

                    int count = colour_split_s.Length;
                    float[] colour = new float[count];

                    for(int i = 0; i < count; i++)
                    {
                        colour[i] = System.Convert.ToSingle(colour_split_s[i]);
                    }

                    return_colour.r = colour[0];
                    return_colour.g = colour[1];
                    return_colour.b = colour[2];
                    return_colour.a = colour[3];
                }

                return return_colour;
            }

            return null;
        }

        float loadFloat(XmlNode node)
        {
            float return_float = -1.0f;

            foreach (XmlNode sub_node in node.ChildNodes)
            {
                if (sub_node.Name == "float")
                {
                    String float_s = sub_node.InnerText;
                    return_float = System.Convert.ToSingle(float_s);
                }

                return return_float;
            }

            return return_float;
        }

        String loadTexture(XmlNode node)
        {
            String return_filename;

            foreach (XmlNode sub_node in node.ChildNodes)
            {
                if (sub_node.Name == "texture")
                {
                    foreach (XmlAttribute attrib in sub_node.Attributes)
                    {
                        switch (attrib.Name)
                        {
                            case "texture":

                                String convoluted_name = attrib.Value;
                                String[] convoluted_name_split = convoluted_name.Split('-');

                                String filename_with_underscore_ext = convoluted_name_split[0];

                                int strpos = filename_with_underscore_ext.Length -1;
                                while (filename_with_underscore_ext[strpos] != '_')
                                {
                                    strpos--;
                                }

                                return_filename = filename_with_underscore_ext.Remove(strpos, 1);
                                return_filename = return_filename.Insert(strpos, ".");

                                return return_filename;
                        }
                    }
                }
            }

            return null;
        }

        void loadBlinnParameters(XmlNode node, Material mtl)
        {
            foreach (XmlNode sub_node in node)
            {
                ColourFloat cf = new ColourFloat();
                float f;
                String texture_name = String.Empty;

                switch (sub_node.Name)
                {
                    case "blinn":
                    case "phong":

                        foreach (XmlNode param in sub_node)
                        {
                            switch (param.Name)
                            {
                                case "ambient":

                                    cf = loadColour(param);

                                    if(cf != null)
                                    {
                                        mtl.ambient = cf;
                                    }

                                    break;

                                case "diffuse":

                                    cf = loadColour(param);

                                    if (cf != null)
                                    {
                                        mtl.diffuse = cf;
                                    }

                                    texture_name = loadTexture(param);

                                    if (texture_name != null)
                                    {
                                        mtl.texture_name = texture_name;
                                    }

                                    break;

                                case "specular":

                                    cf = loadColour(param);

                                    if (cf != null)
                                    {
                                        mtl.specular = cf;
                                    }

                                    break;

                                case "transparency":

                                    f = loadFloat(param);

                                    if (f != -1.0f)
                                    {
                                        mtl.transparency = f;
                                    }

                                    break;

                                case "shininess":

                                    f = loadFloat(param);

                                    if (f != -1.0f)
                                    {
                                        mtl.shininess = f;
                                    }

                                    break;
                            }
                        }

                   break;
                }
            }
        }

        void loadCommonMaterial(XmlNode node, Material mtl)
        {
            foreach (XmlNode sub_node in node)
            {
                switch (sub_node.Name)
                {
                    case "technique":

                        loadBlinnParameters(sub_node, mtl);

                        break;
                }
            }
        }

        void loadMaterial(XmlNode node)
        {
            Material new_mtl = new Material();

            foreach (XmlAttribute attrib in node.Attributes)
            {
                switch (attrib.Name)
                {
                    case "id":

                        new_mtl.name = attrib.Value;

                        break;
                }
            }

            foreach (XmlNode sub_node in node)
            {
                switch (sub_node.Name)
                {
                    case "profile_COMMON":

                        loadCommonMaterial(sub_node, new_mtl);

                        break;
                }
            }

            materials.Add(new_mtl);
        }
    }
}