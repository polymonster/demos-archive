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
    public class Source
    {
        public String name;
        public float[] data;
    }

    public class Sampler
    {
        public float[] times;
        public float[] transforms;
        public String sampler_name;

        public Sampler()
        {
            sampler_name = "not_set";
        }
    }

    public class Channel
    {
        public Sampler source;
        public String target;

        public Channel()
        {
            source = new Sampler();
            target = "not_set";
        }
    }

    public class Animation
    {
        public List<Channel> channels;
        public List<Source> sources;
        public List<Sampler> samplers;

        public Animation()
        {
           channels = new List<Channel>();
           sources = new List<Source>();
           samplers = new List<Sampler>();
        }

        public Source findSourceByName(String name)
        {
            foreach (Source s in sources)
            {
                if(s.name == (name.Remove(0, 1)))
                {
                    return s;
                }
            }

            return null;
        }

        public Sampler findSamplerByName(String name)
        {
            foreach (Sampler s in samplers)
            {
                if (s.sampler_name == (name.Remove(0, 1)))
                {
                    return s;
                }
            }

            return null;
        }
    }

    public partial class converter
    {
        void loadAnimations(XmlNode animation_node)
        {
            Animation animation = new Animation();

            foreach (XmlNode node in animation_node.ChildNodes)
            {
                switch (node.Name)
                {
                    case "source":
                    {
                        Source s = new Source();

                        foreach (XmlAttribute attrib in node.Attributes)
                        {
                            switch (attrib.Name)
                            {
                                case "id":

                                    s.name = attrib.Value;

                                    break;
                            }
                        }

                        foreach (XmlNode sub_node in node.ChildNodes)
                        {
                            switch (sub_node.Name)
                            {
                                case "float_array":

                                    int count = 0;

                                    foreach (XmlAttribute attrib in sub_node.Attributes)
                                    {
                                        switch (attrib.Name)
                                        {
                                            case "count":

                                                count = System.Convert.ToInt32(attrib.Value);

                                                break;
                                        }
                                    }

                                    s.data = new float[count];

                                    String[] data_s = sub_node.InnerText.Split(' ');

                                    try
                                    {
                                        for (int i = 0; i < count; i++)
                                            s.data[i] = System.Convert.ToSingle(data_s[i]);
                                    }
                                    catch (System.Exception ex)
                                    {
                                        for (int i = 0; i < count; i++)
                                            s.data[i] = 0.0f;
                                    }


                                    break;
                            }
                        }

                        animation.sources.Add(s);
                    }
                       
                        break;

                    case "sampler":
                    {
                        Sampler sampler = new Sampler();

                        foreach (XmlAttribute attrib in node.Attributes)
                        {
                            switch (attrib.Name)
                            {
                                case "id":

                                    sampler.sampler_name = attrib.Value;

                                    break;
                            }
                        }

                        foreach (XmlNode sub_node in node.ChildNodes)
                        {
                            switch (sub_node.Name)
                            {
                                case "input":

                                    String semantic = String.Empty;
                                    String source = String.Empty;

                                    foreach (XmlAttribute attrib in sub_node.Attributes)
                                    {
                                        switch (attrib.Name)
                                        {
                                            case "semantic":

                                                semantic = attrib.Value;

                                                break;

                                            case "source":

                                                source = attrib.Value;

                                                break;
                                        }
                                    }

                                    Source datasource = animation.findSourceByName(source);

                                    switch(semantic)
                                    {
                                        case "INPUT":

                                            sampler.times = datasource.data;

                                            break;

                                        case "OUTPUT":

                                            sampler.transforms = datasource.data;

                                            break;
                                    }

                                    animation.samplers.Add(sampler);

                                    break;
                            }
                        }
                    }
                        break;

                    case "channel":
                    {
                        Channel c = new Channel();

                        String target = String.Empty;
                        Sampler sampler_source = null;

                        foreach (XmlAttribute attrib in node.Attributes)
                        {
                            switch (attrib.Name)
                            {
                                case "source":

                                    String sampler_name = attrib.Value;

                                    sampler_source = animation.findSamplerByName(sampler_name);

                                    break;

                                case "target":

                                    target = attrib.Value.Split('/')[0];

                                    break;
                            }
                        }

                        c.source = sampler_source;
                        c.target = target;

                        animation.channels.Add(c);

                        break;
                    }
                }
            }

            animations.Add(animation);
        }
    }
}