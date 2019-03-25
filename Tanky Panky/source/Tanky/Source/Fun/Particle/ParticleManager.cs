using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Graphics;

namespace Tanky.Source.Fun.Particle
{
    public class ParticleManager
    {
        public Texture2D tex; 
        public ParticleManager()
        {
            tex = g.content.Load<Texture2D>("Particles\\Smoke");
        }
    }
}
