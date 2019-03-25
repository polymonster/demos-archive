using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Tanky.Particles
{
    public class BaseParticle3D
    {
        public Vector3 Position;
        public Vector3 OldPosition;
        public Vector3 Velocity;
        public float CurrentLife;
        public bool Alive;
        public Color Colour;

        public BaseParticle3D()
        {
            Position = new Vector3(0);
            OldPosition = new Vector3(0);
            Alive = false;
            Velocity = Vector3.Zero;

        }
        public BaseParticle3D(Vector3 theInitialPosition, Vector3 theInitialVelocity)
        {
            Position = theInitialPosition;
            Velocity = theInitialVelocity;
            CurrentLife = 0;
            Alive = false;
            Colour = Color.White;
        }
        public void Draw()
        {

        }

    }
}
