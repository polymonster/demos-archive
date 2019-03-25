using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BEPUphysics;
using Microsoft.Xna.Framework;

namespace Tanky
{
    class KeepOut
    {
        public Tornado tornado;
 
        public KeepOut()
        {
            Entity forceFieldShape = new Box(Vector3.Zero, 30, 60, 30);
            //Get the tornado moving.  Note the use of 'internal' linear velocity; since this object isn't handled by the space it won't have its buffered velocity set.
            forceFieldShape.internalLinearVelocity = new Vector3(5, 0, 0);

            tornado = new Tornado(forceFieldShape, new Vector3(0, 1, 0), 60, false, 100, 40, 1000, 100, 30, 1000, 40, 30);
        }
    }
}
