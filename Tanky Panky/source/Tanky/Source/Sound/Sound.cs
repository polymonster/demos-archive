using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Media;

namespace Tanky
{
    public class Sound
    {
        public SoundEffect Shot;
        public SoundEffect Bugle;
        public SoundEffect MetalHit;
        public SoundEffect WoodHit;
        public Sound()
        {
            Shot = g.content.Load<SoundEffect>("Sound\\Explosion");
            Bugle = g.content.Load<SoundEffect>("Sound\\Bugle");
            MetalHit = g.content.Load<SoundEffect>("Sound\\MetalHit");
            WoodHit = g.content.Load<SoundEffect>("Sound\\WoodHit");
        }
       
    }
}
