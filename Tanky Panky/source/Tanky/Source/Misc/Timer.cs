using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Tanky
{
    public class Timer
    {

        public int Elapsed { get; private set; }


        public Timer()
        {
            Reset();
        }

        public void Update()
        {
            Elapsed += g.game_time.ElapsedGameTime.Milliseconds;
        }

        public float GetTimePerFrame()
        {
            return g.game_time.ElapsedGameTime.Milliseconds * 0.001f;
        }

        public void Reset()
        {
            Elapsed = 0;
        }

    }
}
