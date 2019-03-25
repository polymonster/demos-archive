using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Net;
using Microsoft.Xna.Framework.Storage;

namespace tankGame
{
    public class CollisionDetection 
    {
        Texture2D sprite;

        public void loadTexture(ContentManager content)
        {
            //sprite = content.Load<Texture2D>("block");
        }

        public Vector2 closestPointOnLine(Vector2 point, Vector2 lineStart, Vector2 lineEnd, bool[] corners)
        {
            // Create a vector from point to line start.
            Vector2 a = point - lineStart;

            // Create a unit vector of the line
            Vector2 b = lineEnd - lineStart;
            b.Normalize();

            // Use the distance formula to find the magnitude
            float d = Vector2.Distance(lineStart, lineEnd);

            // using dot project to get our ratio along the line
            float t = Vector2.Dot(a, b);

            //if the ratio is less than 0 the the line start is closest,
            //if we are greater than the 

            if (!corners[0])
            {
                if (t <= 0)
                    return lineEnd;
            }
            else
            {
                if (t <= 0)
                    return lineStart;
            }

            if (!corners[1])
            {
                if (t >= d)
                    return lineStart;
            }
            else
            {
                if (t >= d)
                    return lineEnd;
            }

            //if we get this far find the point on the line using t the ratio along the line

            //vector of length t and direction of vB
            Vector2 unitAlongLine = b * t;

            //finally add this unit to the start of our line
            Vector2 closestPoint = lineStart + unitAlongLine;

            return closestPoint;
        }

        public bool tankVsLevel(ref Tank tank, Vector2 aabbPos, Vector2 aabbDimensions, bool[] check)
        {
            //bail if we dont check any
            for (int i = 0; i < 4; i++)
            {
                if (check[i])
                {
                    i = 4;
                }
                else
                {
                    if (i == 3) return false;
                }
            }

            Vector2[] lineEnd;
            Vector2[] lineStart;
            Vector2[] closest;

            lineEnd = new Vector2[4];
            lineStart = new Vector2[4];
            closest = new Vector2[4];

            bool[] corners = new bool[8];

            for (int i = 0; i < 4; i++)
            {
                int previous = i - 1;
                int next = i + 1;
                int index = i * 2;

                if(previous < 0) previous = 3;
                if(next > 3) next = 0;

                if (check[i] && check[previous])
                {
                    corners[index] = true;
                }

                if (check[i] && check[next])
                {
                    corners[index + 1] = true;
                }
            }

            lineStart[0].X = aabbPos.X; lineEnd[0].X = aabbPos.X + aabbDimensions.X;
            lineStart[0].Y = aabbPos.Y; lineEnd[0].Y = aabbPos.Y;

            lineStart[1].X = lineEnd[0].X; lineEnd[1].X = lineEnd[0].X;
            lineStart[1].Y = lineEnd[0].Y; lineEnd[1].Y = lineEnd[0].Y + aabbDimensions.Y;

            lineStart[2].X = lineEnd[1].X; lineEnd[2].X = lineEnd[1].X - aabbDimensions.X;
            lineStart[2].Y = lineEnd[1].Y; lineEnd[2].Y = lineEnd[1].Y;

            lineStart[3].X = aabbPos.X; lineEnd[3].X = aabbPos.X;
            lineStart[3].Y = aabbPos.Y; lineEnd[3].Y = aabbPos.Y + aabbDimensions.Y;

            bool[] duplicated;
            duplicated = new bool[4];

            for (int i = 0; i < 4; i++)
            {
                //check the closest point on a line
                if (check[i])
                {
                    duplicated[i] = false;
                    bool[] checkCorners = new bool[2];
                    checkCorners[0] = corners[i * 2];
                    checkCorners[1] = corners[(i * 2) + 1];
                    closest[i] = closestPointOnLine(tank.getPos(), lineStart[i], lineEnd[i], checkCorners);

                    for (int j = 0; j < 4; j++)
                    {
                        if (closest[i] == closest[j])
                        {
                            if (i != j)
                            {
                                duplicated[i] = true;
                                duplicated[j] = true;
                            }
                        }
                    }
                }

            }

            int bestClosest = 0;
            float d1 = 10000000000000000000000.0f;

            for (int i = 0; i < 4; i++)
            {
                if (check[i])
                {
                    d1 = Vector2.Distance(tank.getPos(), closest[0]);
                    i = 4;
                }
            }

            for (int i = 1; i < 4; i++)
            {
                if (check[i])
                {
                    float d2 = Vector2.Distance(tank.getPos(), closest[i]);

                    if (d2 < d1)
                    {
                        d1 = d2;
                        bestClosest = i;
                    }
                }
            }

            Vector2 unitVector = tank.getPos() - closest[bestClosest];
            unitVector.Normalize();

            Vector2 offset = Vector2.Zero;
 
            if (d1 < tank.getRadius())
            {
                    offset = unitVector * (tank.getRadius() - d1);

                    Vector2 slidingPlane;
                    slidingPlane = lineStart[bestClosest] - lineEnd[bestClosest];

                    bool forceOffset = false;
                    if (duplicated[bestClosest]) forceOffset = true;
                    else if (globals.secondRecursion) forceOffset = true;

                    tank.offsetPos(offset, slidingPlane, forceOffset);

                    return true;

            }

            return false;
        }

        public bool projectileVsLevel(ref Projectile projectile, Vector2 aabbPos, Vector2 aabbDimensions)
        {
            Vector2[] lineEnd;
            Vector2[] lineStart;
            Vector2[] closest;

            lineEnd = new Vector2[4];
            lineStart = new Vector2[4];
            closest = new Vector2[4];

            lineStart[0].X = aabbPos.X; lineEnd[0].X = aabbPos.X + aabbDimensions.X;
            lineStart[0].Y = aabbPos.Y; lineEnd[0].Y = aabbPos.Y;

            lineStart[1].X = lineEnd[0].X; lineEnd[1].X = lineEnd[0].X;
            lineStart[1].Y = lineEnd[0].Y; lineEnd[1].Y = lineEnd[0].Y + aabbDimensions.Y;

            lineStart[2].X = lineEnd[1].X; lineEnd[2].X = lineEnd[1].X - aabbDimensions.X;
            lineStart[2].Y = lineEnd[1].Y; lineEnd[2].Y = lineEnd[1].Y;

            lineStart[3].X = aabbPos.X; lineEnd[3].X = aabbPos.X;
            lineStart[3].Y = aabbPos.Y; lineEnd[3].Y = aabbPos.Y + aabbDimensions.Y;

            for (int i = 0; i < 4; i++)
            {
                //check the closest point on a line
                bool[] corners = new bool[2];
                corners[0] = true;
                corners[1] = true;
                closest[i] = closestPointOnLine(projectile.getPos(), lineStart[i], lineEnd[i], corners);
            }

            int bestClosest = 0;

            float d1 = Vector2.Distance(projectile.getPos(), closest[0]);

            for (int i = 1; i < 4; i++)
            {
                float d2 = Vector2.Distance(projectile.getPos(), closest[i]);

                if (d2 < d1)
                {
                    d1 = d2;
                    bestClosest = i;
                }
            }

            if (d1 < projectile.getRadius())
            {
                return true;
            }

            return false;
        }

        public bool tankVsProjectile(ref Tank tank, ref Projectile projectile)
        {
            float d = Vector2.Distance(tank.getPos(), projectile.getPos());

            if (d < tank.getRadius())
            {
                return true;
            }

            return false;
        }

        public void tankVsTank(ref Tank collider, ref Tank collidee)
        {
            float d = Vector2.Distance(collider.getPos(), collidee.getPos());

            if (d < (collider.getRadius() * 2))
            {
                Vector2 unitVector = collider.getPos() - collidee.getPos();
                unitVector.Normalize();

                Vector2 offset = unitVector * ((collider.getRadius() * 2) - d);

                collider.offsetPos(offset, Vector2.Zero, true);
            }
        }

        public void tankVsLine(ref Tank tank, Vector2 lineStart, Vector2 lineEnd)
        {
            bool[] corners = new bool[2];
            corners[0] = true;
            corners[1] = true;

            Vector2 closestPoint;

            closestPoint = globals.collisions.closestPointOnLine(tank.getPos(), lineStart, lineEnd, corners);

            float d = Vector2.Distance(tank.getPos(),closestPoint);

            if(d < tank.getRadius())
            {
                Vector2 unitVector = tank.getPos() - closestPoint;
                unitVector.Normalize();

                Vector2 offset = unitVector * (tank.getRadius() - d);

                Vector2 slidingPlane = lineStart - lineEnd;

                tank.offsetPos(offset, slidingPlane, true);
            }

        }

        public void draw()
        {
            globals.spriteBatch.Begin(SpriteBlendMode.AlphaBlend);

            Vector2 pos;
            pos.X = 200;
            pos.Y = 200;

            globals.spriteBatch.Draw
            (
                    sprite,
                    pos,
                    null,
                    Color.White,
                    0,
                    Vector2.Zero,
                    1,
                    SpriteEffects.None,
                    0
            );

            globals.spriteBatch.End();
        }
    }
}
