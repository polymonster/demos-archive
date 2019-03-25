using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.Specialized;
using Microsoft.Xna.Framework;
using System.Collections;
using Tanky.Particles;
using Tanky.Particles.Modifiers.Generic;
using Microsoft.Xna.Framework.Graphics;

namespace Tanky.Particles.Tools
{
    class ParticleMisc
    {
        public static double DegreesToRadians(double input)
        {
            return input * 0.017453293F;
        }
        public static float DistanceBetween(Vector2 Point1, Vector2 Point2)
        {
            float XDelta = Point2.X - Point1.X;
            float YDelta = Point2.Y - Point1.Y;
            return (float)Math.Sqrt((XDelta * XDelta) + (YDelta * YDelta));
        }
        public static float DistanceBetween(Vector3 Point1, Vector3 Point2)
        {
            float XDelta = Point2.X - Point1.X;
            float YDelta = Point2.Y - Point1.Y;
            float ZDelta = Point2.Z - Point1.Z;
            return (float)Math.Sqrt((XDelta * XDelta) + (YDelta * YDelta) + (ZDelta * ZDelta));
        }

        public static bool DistanceCheckGreater(Vector2 Point1, Vector2 Point2, float Distance) 
        {
            float XDelta = Point2.X - Point1.X;
            float YDelta = Point2.Y - Point1.Y;

            return ((XDelta * XDelta) + (YDelta * YDelta)) > (Distance * Distance);
        }
        public static bool DistanceCheckGreater(Vector3 Point1, Vector3 Point2, float Distance)
        {
            float XDelta = Point2.X - Point1.X;
            float YDelta = Point2.Y - Point1.Y;
            float ZDelta = Point2.Z - Point1.Z;

            return ((XDelta * XDelta) + (YDelta * YDelta) + (ZDelta * ZDelta)) > (Distance * Distance);
        }
        public static bool DistanceCheckLessThan(Vector2 Point1, Vector2 Point2, float Distance) 
        {
            float XDelta = Point2.X - Point1.X;
            float YDelta = Point2.Y - Point1.Y;

            return ((XDelta * XDelta) + (YDelta * YDelta)) < (Distance * Distance);
        }
        public static bool DistanceCheckLessThan(Vector3 Point1, Vector3 Point2, float Distance)
        {
            float XDelta = Point2.X - Point1.X;
            float YDelta = Point2.Y - Point1.Y;
            float ZDelta = Point2.Z - Point1.Z;

            return ((XDelta * XDelta) + (YDelta * YDelta) + (ZDelta * ZDelta)) < (Distance * Distance);
        }

        public static float InvSquare(float theInput)
        {
            return Convert.ToSingle(1 / (Math.Pow(theInput, 2)));
        }

    }

    public class SinTable
    {
        public float[] Table;
        public SinTable()
        {
            Table = new float[360];
            for (int i = 0; i < 360; i++)
            {
                Table[i] = Convert.ToSingle(Math.Sin(ParticleMisc.DegreesToRadians(i)));
            }
        }

    }
    public class CosTable
    {
        public float[] Table;

        public CosTable()
        {
            Table = new float[360];
            for (int i = 0; i < 360; i++)
            {
                Table[i] = Convert.ToSingle(Math.Cos(ParticleMisc.DegreesToRadians(i)));
            }
        }
    }
    public class TanTable
    {
        public float[] Table;

        public TanTable()
        {
            Table = new float[360];
            for (int i = 0; i < 360; i++)
            {
                Table[i] = Convert.ToSingle(Math.Tan(ParticleMisc.DegreesToRadians(i)));
            }
        }
    }

    public class AngleGenerator
    {
        public SinTable Sin;
        public CosTable Cos;
        public TanTable Tan;

        public AngleGenerator()
        {
            Sin = new SinTable();
            Cos = new CosTable();
            Tan = new TanTable();
        }

        public Vector2 AngleToVector(uint angle)
        {
            if (0 <= angle && angle < 360)
            {
                return new Vector2(Sin.Table[angle], Cos.Table[angle]);
            }
            return default(Vector2);
        }
        public Vector3 AngleToVector3D(uint angle)
        {
            if (0 <= angle && angle < 360)
            {
                return new Vector3(Sin.Table[angle], Cos.Table[angle],Tan.Table[angle]);
            }
            return default(Vector3);
        }
    }
    class QuadRenderer
    {
        //private VertexDeclaration m_vertex_dec;
        //private VertexPositionNormalTexture[] m_vertex_buffer;

        //GraphicsDevice GDev;

        public QuadRenderer()
        {

        }
    }

}
namespace Tanky.Particles.Structures
{
    public class ParticleCollection : CollectionBase
    {

        public int Add()
        {
            return List.Add(new BaseParticle3D());
        }
        public int Add(BaseParticle3D Item)
        {
            return List.Add(Item);

        }
        public void Insert()
        {

        }
        public void Remove()
        {

        }
        public bool Contains(BaseParticle3D Item)
        {
            return List.Contains(Item);
        }
        public int IndexOf(BaseParticle3D Item)
        {
            return List.IndexOf(Item);
        }
        public void CopyTo(BaseParticle3D[] array, int index)
        {
            List.CopyTo(array, index);
        }
        public BaseParticle3D this[int index]
        {
            get
            {
                return (BaseParticle3D)List[index];
            }
            set
            {
                List[index] = value;
            }
        }
    }

    public class ModifierCollection : CollectionBase
    {

        public int Add()
        {
            return List.Add(new BaseModifier());
        }
        public int Add(BaseModifier Item)
        {
            return List.Add(Item);

        }
        public void Insert()
        {

        }
        public void Remove()
        {

        }
        public bool Contains(BaseModifier Item)
        {
            return List.Contains(Item);
        }
        public int IndexOf(BaseModifier Item)
        {
            return List.IndexOf(Item);
        }
        public void CopyTo(BaseModifier[] array, int index)
        {
            List.CopyTo(array, index);
        }
        public BaseModifier this[int index]
        {
            get
            {
                return (BaseModifier)List[index];
            }
            set
            {
                List[index] = value;
            }
        }
    }

    class QuickMatrix
    {
        //transformation matrices
        private Matrix _WorldMatrix;
        private Matrix _ViewMatrix;
        private Matrix _ProjectionMatrix;

        public Matrix myWorldMatrix
        {
            get
            {
                return _WorldMatrix;
            }
            set
            {
                _WorldMatrix = value;
            }
        }
        public Matrix myViewMatrix
        {
            get
            {
                return _ViewMatrix;
            }
            set
            {
                _ViewMatrix = value;
            }
        }
        public Matrix myProjectionMatrix
        {
            get
            {
                return _ProjectionMatrix;
            }
            set
            {
                _ProjectionMatrix = value;
            }
        }

        public Matrix myWorldViewProjMatrix
        {
            get
            {
                return _WorldMatrix * _ViewMatrix * _ProjectionMatrix;

            }
        }

        public QuickMatrix(GraphicsDevice GDev)
        {
            _WorldMatrix = Matrix.Identity;
            _ViewMatrix = Matrix.CreateLookAt(new Vector3(0, 0, -1), Vector3.Zero, Vector3.Up);
            _ProjectionMatrix = Matrix.CreatePerspectiveFieldOfView(MathHelper.ToRadians(90), GDev.Viewport.Width / GDev.Viewport.Height, 0.1f, 1000.0f);
        }

        public QuickMatrix(Matrix theWorldMatrix, Matrix theViewMatrix, Matrix theProjectionMatrix)
        {
            _WorldMatrix = theWorldMatrix;
            _ViewMatrix = theViewMatrix;
            _ProjectionMatrix = theProjectionMatrix;
        }
    }

    class QuadRenderer
    {
        GraphicsDevice m_device;

        VertexDeclaration m_vertex_decleration;
        VertexPositionTexture[] m_vertices;
        short[] m_indices;

        public QuadRenderer(GraphicsDevice thedevice)
        {
            m_device = thedevice;
            Init();
        }

        private void Init()
        {
            m_vertex_decleration = new VertexDeclaration(m_device, VertexPositionTexture.VertexElements);
            m_vertices = new VertexPositionTexture[]
                            {
                                new VertexPositionTexture(Vector3.Zero,new Vector2(1,1)),
                                new VertexPositionTexture(Vector3.Zero,new Vector2(0,1)),
                                new VertexPositionTexture(Vector3.Zero,new Vector2(0,0)),
                                new VertexPositionTexture(Vector3.Zero,new Vector2(1,0))
                            };
            m_indices = new short[] { 0, 1, 2, 2, 3, 0 };
        }

        public void DrawQuad(Vector3 thePos,float theSize)
        {
            m_device.VertexDeclaration = m_vertex_decleration;

            m_vertices[0].Position.X = thePos.X - theSize / 2.0f;
            m_vertices[0].Position.Y = thePos.Y - theSize / 2.0f;
            m_vertices[0].Position.Z = thePos.Z;

            m_vertices[1].Position.X = thePos.X + theSize / 2.0f;
            m_vertices[1].Position.Y = thePos.Y - theSize / 2.0f;
            m_vertices[1].Position.Z = thePos.Z;

            m_vertices[2].Position.X = thePos.X + theSize / 2.0f;
            m_vertices[2].Position.Y = thePos.Y + theSize / 2.0f;
            m_vertices[2].Position.Z = thePos.Z;

            m_vertices[3].Position.X = thePos.X - theSize / 2.0f;
            m_vertices[3].Position.Y = thePos.Y + theSize / 2.0f;
            m_vertices[3].Position.Z = thePos.Z;

            //m_device.DrawIndexedPrimitives(PrimitiveType.TriangleList,)
        }
        
    }
}
namespace Tanky.Particles.Modifiers
{
    namespace Generic
    {
        public class BaseModifier
        {
            public virtual bool ApplyModifier(ref BaseParticle3D theParticle)
            {
                return true;
            }
        }

        public class ColourMod : BaseModifier
        {
            private Color _Colour;
            public ColourMod(Color theColour)
            {
                _Colour = theColour;
            }
            public override bool ApplyModifier(ref BaseParticle3D theParticle)
            {
                theParticle.Colour = _Colour;
                return true;
            }
        }
    }
    namespace Forces
    {
        public class GenericGravity : BaseModifier
        {
            private Vector3 m_down;
            private float m_terminal_velocity;
            private float m_pull;

            public GenericGravity(Vector3 theDown,float theTerminalVelocity, float thePull)
            {
                m_down = theDown;

                m_terminal_velocity = theTerminalVelocity;
                m_pull = thePull;
            }

            public override bool ApplyModifier(ref BaseParticle3D theParticle)
            {
                if((theParticle.Velocity.Y + m_pull) > m_terminal_velocity)
                {
                    theParticle.Velocity.Y = m_terminal_velocity;
                }
                else
                {
                    theParticle.Velocity.Y += m_pull;
                }
                return true;
            }
        }
        public class DirectionalForce : BaseModifier
        {
            private Vector3 m_velocity;
            private Vector3 m_position;

            public DirectionalForce(Vector3 theVelocity,ref Vector3 thePosition)
            {
                m_velocity = theVelocity;
                m_position = thePosition;
            }

            public override bool ApplyModifier(ref BaseParticle3D theParticle)
            {
                float ratio = Tools.ParticleMisc.InvSquare(Tools.ParticleMisc.DistanceBetween(theParticle.Position,m_position));
                theParticle.Velocity.Y += m_velocity.Y * ratio;
                theParticle.Velocity.X += m_velocity.X * ratio;
                theParticle.Velocity.X += m_velocity.Y * ratio;
                return true;
            }
        }
    }
}
