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
    public partial class converter
    {
        List<Mesh> meshes = new List<Mesh>();
        List<Skin> skins = new List<Skin>();
        List<Animation> animations = new List<Animation>();
        List<Material> materials = new List<Material>();

        Skeleton skeleton = new Skeleton();

        int current_mesh = 0;

        void output(String dest)
        {
            StreamWriter sw = new
            StreamWriter(dest, false);

            //begin outputting
            sw.Write("psmesh v0.1\n");
            sw.Write("- some kind of header -\n");

            //materials
            foreach (Material m in materials)
            {
                sw.Write("\nmaterial_begin\n");

                sw.Write("name " + m.name + "\n");
                sw.WriteLine("TEXTURE_NAME " + m.texture_name + "\n");
                sw.WriteLine("AMBIENT " + m.ambient.ToString() + "\n");
                sw.WriteLine("DIFFUSE " + m.diffuse.ToString() + "\n");
                sw.WriteLine("SPECULAR " + m.specular.ToString() + "\n");
                sw.WriteLine("SHININESS " + m.shininess.ToString() + "\n");
                sw.WriteLine("TRANSPARENCY " + m.transparency.ToString()  + "\n");

                sw.Write("\nmaterial_end\n");
            }

            //meshes
            foreach (Mesh m in meshes)
            {
                sw.Write("\nmesh_begin\n");

                sw.Write("num_elements " + m.out_elements.Count.ToString() + "\n");
                sw.Write("name " + m.m_name + "\n");

                int mat_id = getMaterialByName(m.material_name);
                sw.Write("material_id " + mat_id.ToString() + "\n");

                sw.WriteLine("\nINDICES " + m.m_indices.Count + "\n");
                for (int i = 0; i < m.m_indices.Count; i++)
                {
                    sw.Write(m.m_indices[i].ToString() + "\n");
                }

                foreach (VertexElementOutputArray veo in m.out_elements)
                {
                    sw.Write("\n" + veo.semantic_name + " " + veo.values.Count + "\n");

                    foreach (Vector3f ea in veo.values)
                    {
                        sw.Write(ea.x.ToString() + " ");
                        sw.Write(ea.y.ToString() + " ");
                        sw.Write(ea.z.ToString() + "\n");
                    }
                }

                sw.Write("\nmesh_end\n");
            }

            //skeleton
            sw.Write("\nskeleton_begin\n");

            foreach (Joint j in skeleton.joints)
            {
                sw.Write("\njoint\n");

                sw.Write("name " + j.name + "\n");
                sw.Write("target_name " + j.id + "\n");
                sw.Write("parent_name " + j.parent_name + "\n");

                sw.Write("matrix ");
                for (int i = 0; i < 16; i++)
                {
                    sw.Write(j.matrix[i].ToString() + " ");
                }
            }

            sw.Write("\n\nskeleton_end\n");

            //animation
            foreach (Animation a in animations)
            {
                sw.Write("\nanimation_begin\n");

                foreach (Channel c in a.channels)
                {
                    sw.Write("\nchannel_target " + c.target + "\n");

                    sw.Write("count " + c.source.times.Length.ToString() + "\n");
                    sw.Write("times ");
                    for (int i = 0; i < c.source.times.Length; i++)
                    {
                        sw.Write(c.source.times[i].ToString() + " ");
                    }

                    sw.Write("\ntransforms ");
                    for (int i = 0; i < c.source.transforms.Length; i++)
                    {
                        sw.Write(c.source.transforms[i].ToString() + " ");
                    }
                }

                sw.Write("\n\nanimation_end\n");
            }


            //skinning
            foreach (Skin s in skins)
            {
                sw.Write("\nskinning_info_begin\n");

                sw.Write("\nskin_target " + s.mesh_name + "\n");

                sw.Write("bind_shape_matrix ");
                for (int i = 0; i < s.bind_shape_matrix.Length; i++)
                {
                    sw.Write(s.bind_shape_matrix[i].ToString() + " ");
                }

                sw.Write("\nnum_skinned_joints " + s.skinned_joints.Count.ToString() + "\n");
                foreach (SkinnedJoint sj in s.skinned_joints)
                {
                    sw.Write("name " + sj.name + "\n");

                    for(int i = 0; i < sj.inverse_bind_matrix.Length; i++)
                    {
                        sw.Write(sj.inverse_bind_matrix[i].ToString() + " ");
                    }

                    sw.Write("\n");
                }

                sw.Write("\nnum_skinned_verts " + s.skinned_vertices.Count.ToString() + "\n");
                foreach (SkinnedVertex sv in s.skinned_vertices)
                {
                    sw.Write("joint_count " + sv.skinned_joints.Count + "\n");
                    for (int i = 0; i < sv.skinned_joints.Count; i++)
                    {
                        sw.Write(sv.skinned_joints[i].ToString() + " ");
                        sw.Write(sv.weights[i].ToString() + "\n");
                    }
                }

                sw.Write("\n\nskinning_info_end\n");
            }

            sw.Close();
        }

        void loadGeometriesLibrary(XmlDocument file)
        {
            XmlNodeList geometry = file.GetElementsByTagName("library_geometries");

            for (int i = 0; i < geometry.Count; i++)
            {
                XmlNode geom_library = geometry.Item(i);

                foreach (XmlNode node in geom_library.ChildNodes)
                {
                    switch (node.Name)
                    {
                        case "geometry":

                            loadGeometry(node);

                            break;
                    }
                }
            }
        }

        void loadBonesLibrary(XmlDocument file)
        {
            XmlNodeList bones = file.GetElementsByTagName("library_visual_scenes");

            for (int i = 0; i < bones.Count; i++)
            {
                XmlNode scene_library = bones.Item(i);

                foreach (XmlNode node in scene_library.ChildNodes)
                {
                    switch (node.Name)
                    {
                        case "visual_scene":

                            foreach (XmlNode sub_node in node.ChildNodes)
                            {
                                switch (sub_node.Name)
                                {
                                    case "node":

                                        loadJoints(sub_node, "root");

                                        break;
                                }
                            }

                            break;
                    }
                }
            }
        }

        void loadSkinningLibrary(XmlDocument file)
        {
            XmlNodeList skins = file.GetElementsByTagName("library_controllers");

            for (int i = 0; i < skins.Count; i++)
            {
                XmlNode controller = skins.Item(i);

                foreach (XmlNode node in controller.ChildNodes)
                {
                    switch (node.Name)
                    {
                        case "controller":

                            loadController(node);

                            break;
                    }
                }
            }
        }

        void loadAnimationLibrary(XmlDocument file)
        {
            XmlNodeList anims = file.GetElementsByTagName("library_animations");

            for (int i = 0; i < anims.Count; i++)
            {
                XmlNode animation = anims.Item(i);

                foreach (XmlNode node in animation.ChildNodes)
                {
                    switch (node.Name)
                    {
                        case "animation":

                            loadAnimations(node);

                            break;
                    }
                }
            }
        }

        void loadMaterialsLibrary(XmlDocument file)
        {
            XmlNodeList effects = file.GetElementsByTagName("library_effects");

            for (int i = 0; i < effects.Count; i++)
            {
                XmlNode mat = effects.Item(i);

                foreach (XmlNode node in mat.ChildNodes)
                {
                    switch (node.Name)
                    {
                        case "effect":

                            loadMaterial(node);

                            break;
                    }
                }
            }
        }

        public void colladaToPsm(List<ParseData> parse_data, String dest)
        {
            XmlDocument file = new XmlDocument();

            foreach (ParseData pd in parse_data)
            {
                file.Load(pd.filename);

                //get geom
                if (pd.resposibility == Responsibility.ALL)
                {
                    loadGeometriesLibrary(file);
                }

                //get bones
                if (pd.resposibility == Responsibility.ALL)
                {
                    loadBonesLibrary(file);
                }

                //get anims
                if (pd.resposibility == Responsibility.ALL || pd.resposibility == Responsibility.ANIMATION)
                {
                    loadAnimationLibrary(file);
                }

                //get skinning
                if (pd.resposibility == Responsibility.ALL)
                {
                    loadSkinningLibrary(file);
                }

                //get mats
                if (pd.resposibility == Responsibility.ALL)
                {
                    loadMaterialsLibrary(file);
                }
            }

            output(dest);
        }

        public converter()
        {

        }
    }
}
