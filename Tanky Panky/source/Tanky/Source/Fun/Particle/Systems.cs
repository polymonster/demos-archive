using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tanky.Particles;
using Tanky.Particles.Tools;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;
using Tanky.Particles.Modifiers.Generic;
using System.Diagnostics;
using Tanky.Particles.Structures;

namespace Tanky.Source.Fun.Particle
{
    public class BaseSystem3D
    {
        protected BaseParticle3D[] m_particle_list;
        protected AngleGenerator m_angle_gen;
        protected MersenneTwister m_RNG;
        protected Vector3 m_system_position;
        protected Texture2D m_texture;
        protected ModifierCollection m_modifiers;
        protected Stopwatch m_timer;
        protected GraphicsDevice m_device;

        public BaseSystem3D(GraphicsDevice theDevice)
        {
            m_angle_gen = new AngleGenerator();
            m_RNG = new MersenneTwister(DateTime.Now.Millisecond);
            m_timer = new Stopwatch();
            m_device = theDevice;
        }
        public virtual void Update()
        {

        }
        public virtual void Draw()
        {

        }
        public virtual void SetPosition(Vector3 theNewPos)
        {
            m_system_position = theNewPos;
        }

        public virtual int AddModifier(BaseModifier theMod)
        {
            return m_modifiers.Add(theMod);
        }
        public virtual void ChangeModifier(int Index, BaseModifier theMod)
        {
            if (m_modifiers.Count > Index)
            {
                m_modifiers[Index] = theMod;
            }
            else
            {
                throw new SystemException();
            }
        }


        /////////////////////////////////////////////////
        //Quad Rendering Stuff
        ////////////////////////////////////////////////
        
        protected VertexDeclaration m_vertex_decleration;
        protected VertexPositionTexture[] m_vertices;
        protected short[] m_indices;
        protected BasicEffect m_basic_effect;

        private void InitQuads()
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

            m_basic_effect = new BasicEffect(m_device, null);
            m_basic_effect.VertexColorEnabled = true;
        }

    }
    public class PlugabbleSystem3D
    {
        protected BaseParticle3D[] _ParticleList;
        protected UInt32 _DeadParticles;
        protected Stack<UInt32> _DeadParticleList;

        protected AngleGenerator _AngleGen;
        protected MersenneTwister _RNG;

        protected VertexDeclaration m_vertex_dec;
        protected VertexPositionNormalTexture[] m_vertex_buffer;

        protected SpriteBatch _SpriteBatch;
        protected Vector3 _SystemPosition;
        protected Vector2 _ScreenDimensions;
        protected Texture2D _Texture;
        protected Stopwatch _Timer;
        protected Color _ParticleColour;
        public Color Colour
        {
            set
            {
                _ParticleColour = value;
            }
        }
        public Vector3 Position
        {
            set
            {
                _SystemPosition = value;
            }
            get
            {
                return _SystemPosition;
            }
        }

        protected ModifierCollection _Modifiers;

        protected UInt32 _EmissionRate;
        protected UInt32 _MaxParticles;
        protected UInt32 _MaxParticleLife;

        public PlugabbleSystem3D(GraphicsDevice GDev, UInt32 theEmissionRate, UInt32 theMaxParticles, ref Texture2D theTexture)
        {
            _ParticleList = new BaseParticle3D[theMaxParticles];

            _AngleGen = new AngleGenerator();
            _RNG = new MersenneTwister(DateTime.Now.Millisecond);
            _SpriteBatch = new SpriteBatch(GDev);
            _SystemPosition = Vector3.Zero;
            _Texture = theTexture;

            _ScreenDimensions.X = GDev.Viewport.Width;
            _ScreenDimensions.Y = GDev.Viewport.Height;

            _Timer = new Stopwatch();

            _EmissionRate = theEmissionRate;
            _MaxParticles = theMaxParticles;

            //max particle life (- 10%)
            _MaxParticleLife = Convert.ToUInt32((theMaxParticles / theEmissionRate) - ((theMaxParticles / theEmissionRate) / 10));

            _DeadParticles = theMaxParticles;
            _DeadParticleList = new Stack<uint>();

            _ParticleColour = Color.White;

            _Modifiers = new ModifierCollection();


            //set particles
            for (UInt32 i = 0; i < theMaxParticles; i++)
            {
                _ParticleList[i] = new BaseParticle3D();
                _DeadParticleList.Push(i);
            }

            _Timer.Start();

            Spawn(_EmissionRate);


        }

        protected void Spawn(UInt32 theNumber)
        {
            UInt32 number = theNumber;
            if (_DeadParticleList.Count < theNumber)
            {
                number = Convert.ToUInt32(_DeadParticleList.Count);
            }
            for (int i = 0; i < number; i++)
            {
                UInt32 Number = _DeadParticleList.Pop();
                _ParticleList[Number].Alive = true;
                uint RanAngle = _RNG.NextUInt32(0, 360);

                _ParticleList[Number].CurrentLife = Convert.ToSingle(_RNG.NextUInt32(0, Convert.ToUInt32(_MaxParticleLife)));
                _ParticleList[Number].Position = _SystemPosition;
                _ParticleList[Number].Velocity = _AngleGen.AngleToVector3D(RanAngle);
                _ParticleList[Number].Alive = true;
                _ParticleList[Number].Colour = _ParticleColour;

                _DeadParticles--;
            }
        }


        public void Update()
        {
            bool SecondLapse = false;

            if (_Timer.ElapsedMilliseconds > 1000)
            {
                SecondLapse = true;
                Spawn(_EmissionRate);
            }
            for (UInt32 i = 0; i < _ParticleList.Length; i++)
            {
                //if particle alive
                if (_ParticleList[i].Alive)
                {
                    if (SecondLapse)
                    {
                        _ParticleList[i].CurrentLife -= 1;
                    }
                    // if particle is dead or out of bounds ..kill
                    if (_ParticleList[i].CurrentLife <= 0 || _ParticleList[i].Position.X > _ScreenDimensions.X || _ParticleList[i].Position.Y > _ScreenDimensions.Y || _ParticleList[i].Position.X < 0 || _ParticleList[i].Position.Y < 0)
                    {
                        _ParticleList[i].Alive = false;
                        _DeadParticles++;
                        _DeadParticleList.Push(i);
                    }
                    else
                    {
                        _ParticleList[i].Position.X += _ParticleList[i].Velocity.X;
                        _ParticleList[i].Position.Y += _ParticleList[i].Velocity.Y;
                        _ParticleList[i].Position.Z += _ParticleList[i].Velocity.Z;
                        foreach (BaseModifier Mod in _Modifiers)
                        {
                            Mod.ApplyModifier(ref _ParticleList[i]);
                        }
                    }
                    //if its time to then spawn more particles
                    if (SecondLapse)
                    {
                        Spawn(_EmissionRate);
                    }
                }
            }
        }
        public void Draw()
        {
            _SpriteBatch.Begin();
            foreach (BaseParticle3D P in _ParticleList)
            {
                if (P.Alive)
                {
                    P.Draw();

                    _SpriteBatch.Draw(_Texture, new Rectangle(Convert.ToInt32(P.Position.X), Convert.ToInt32(P.Position.Y), 50, 50), P.Colour);
                }
            }
            _SpriteBatch.End();
        }

        public int AddModifier(BaseModifier theMod)
        {
            return _Modifiers.Add(theMod);
        }
        public void ChangeModifier(int Index, BaseModifier theMod)
        {
            if (_Modifiers.Count >= Index)
            {
                _Modifiers[Index] = theMod;
            }
            else
            {
                throw new SystemException();
            }
        }

        private void RenderQuad()
        {

        }
    }

    public class BurstSystem : BaseSystem3D
    {

        /// <summary>
        /// Postion of the system
        /// </summary>
        public Vector3 Position
        {
            get
            {
                return m_system_position;
            }
            set
            {
                m_system_position = value;
            }
        }

        /// <summary>
        /// Is ready to be fired
        /// </summary>
        public bool IsReady { get; private set; }

        /// <summary>
        /// has finished its run
        /// </summary>
        public bool IsFinished { get; private set; }

        /// <summary>
        /// has been set off
        /// </summary>
        private bool m_has_started;

        /// <summary>
        /// the amount of time between group spawns
        /// smaller is smoother
        /// bigger is faster
        /// </summary>
        private uint m_time_step;
        private uint m_current_step;
        private uint m_total_steps;
        private uint m_particles_per_step;

        protected uint m_dead_particles;
        protected Stack<UInt32> m_dead_particle_list;

        public BurstSystem(uint numberParticles, uint length, uint theTimestep,GraphicsDevice theDevice):base(theDevice)
        {
            m_total_steps = (length / theTimestep);
            m_current_step = 0;

            //add a buffer for the particles to deal
            //with rounding errors in calculations
            uint particles = Convert.ToUInt32(numberParticles + ((numberParticles / 100) * 5));


            m_particle_list = new BaseParticle3D[particles];
            m_dead_particle_list = new Stack<uint>((int)particles);

            m_time_step = theTimestep;

            m_particles_per_step = (uint)(numberParticles / (length / theTimestep));

        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="particlePerStep">the amount of particles spawned per increment</param>
        /// <param name="length"> the total length the system will run for (in milliseconds)</param>
        /// <param name="theTimestep">the increment at which particle spawns occur (in milliseconds)</param>
        //public BurstSystem(uint particlePerStep, uint length,uint theTimestep)
        //{
        //    m_total_steps = Convert.ToUInt32(length / theTimestep);
        //    m_current_step = 0;

        //    //add a buffer for the particles to deal
        //    //with rounding errors in calculations
        //    uint particles = (length / theTimestep) * particlePerStep;
        //    particles += ((particles / 100) * 5);

        //    m_particle_list = new BaseParticle3D[particles];
        //    m_dead_particle_list = new Stack<uint>((int)particles);

        //    m_time_step = theTimestep;

        //    m_particles_per_step = (uint)particlePerStep;
        //}

        public void Start()
        {

        }
        /// <summary>
        /// resets the system and stops it
        /// </summary>
        public void Reset()
        {
            foreach (BaseParticle3D p in m_particle_list)
            {
                p.Alive = false;
            }
            m_current_step = 0;
            IsReady = true;
            IsFinished = true;
            m_has_started = false;
        }

        public override void Draw()
        {

        }
        public override void Update()
        {
            if (m_has_started)
            {
                m_current_step++;
                if (m_timer.ElapsedMilliseconds > m_time_step)
                {
                    Spawn(m_particles_per_step);
                }
                for (UInt32 i = 0; i < m_particle_list.Length; i++)
                {
                    if (m_particle_list[i].Alive)
                    {
                        //update life
                        m_particle_list[i].CurrentLife += m_time_step;

                        //update positions

                        m_particle_list[i].Position.X += m_particle_list[i].Velocity.X;
                        m_particle_list[i].Position.Y += m_particle_list[i].Velocity.Y;
                        m_particle_list[i].Position.Z += m_particle_list[i].Velocity.Z;
                        foreach (BaseModifier M in m_modifiers)
                        {
                            M.ApplyModifier(ref m_particle_list[i]);
                        }
                    }
                    else
                    {
                        m_dead_particle_list.Push(i);
                    }
                }
                if (m_current_step == m_total_steps)
                {
                    Reset();
                }
            }
        }

        protected void Spawn(UInt32 theNumber)
        {
            UInt32 number = theNumber;
            //if there are not enough dead particles
            if (m_dead_particle_list.Count < theNumber)
            {
                //just spawn however many are left
                number = Convert.ToUInt32(m_dead_particle_list.Count);
            }

            for (int i = 0; i < number; i++)
            {
                UInt32 Number = m_dead_particle_list.Pop();
                m_particle_list[Number].Alive = true;
                uint RanAngle = m_RNG.NextUInt32(0, 360);

                m_particle_list[Number].Position = m_system_position;
                m_particle_list[Number].Alive = false;
                m_particle_list[Number].Colour = Color.White;

                m_dead_particles--;
            }
        }


        
    }

    #region Shader System commented out for the moment
    /// <summary>
    /// Not Converted yet Do Not Use
    /// </summary>
    //private class ShaderSystem 
    //{
    //    protected BaseParticle3D[] _ParticleList;
    //    protected UInt32 _DeadParticles;
    //    protected Stack<UInt32> _DeadParticleList;
    //    protected AngleGenerator _AngleGen;
    //    protected MersenneTwister _RNG;
    //    protected SpriteBatch _SpriteBatch;
    //    protected Vector3 _SystemPosition;
    //    protected Vector2 _ScreenDimensions;
    //    protected Texture2D _Texture;
    //    protected Stopwatch _Timer;
    //    protected Color _ParticleColour;
    //    public Color Colour
    //    {
    //        set
    //        {
    //            _ParticleColour = value;
    //        }
    //    }
    //    public Vector3 Position
    //    {
    //        set
    //        {
    //            _SystemPosition = new Vector3(-(value.X),value.Y,value.Z);
    //        }
    //        get
    //        {
    //            return _SystemPosition;
    //        }
    //    }

    //    protected ModifierCollection _Modifiers;


    //    protected UInt32 _EmissionRate;
    //    protected UInt32 _MaxParticles;
    //    protected UInt32 _MaxParticleLife;
    //    protected Effect _Effect;
    //    protected VertexDeclaration _EffectVertexDeclaration;
    //    protected DynamicVertexBuffer _VertexBuffer;
    //    protected VertexPositionColor[] _Vertices;
    //    protected GraphicsDevice _GDev;

    //    protected VertexDeclaration _VDec;


    //    public ShaderSystem(GraphicsDevice GDev, UInt32 theEmissionRate, UInt32 theMaxParticles, ref Effect theEffect)
    //    {
    //        _ParticleList = new BaseParticle3D[theMaxParticles];
    //        _AngleGen = new AngleGenerator();
    //        _RNG = new MersenneTwister(DateTime.Now.Millisecond);
    //        _SystemPosition = Vector3.Zero;
    //        _Effect = theEffect;

    //        _ScreenDimensions.X = GDev.Viewport.Width;
    //        _ScreenDimensions.Y = GDev.Viewport.Height;

    //        _EffectVertexDeclaration = new VertexDeclaration(GDev, VertexPositionColor.VertexElements);

    //        _Timer = new Stopwatch();

    //        _GDev = GDev;

    //        _EmissionRate = theEmissionRate;
    //        _MaxParticles = theMaxParticles;

    //        //max particle life (- 10%)
    //        _MaxParticleLife = Convert.ToUInt32((theMaxParticles / theEmissionRate) - ((theMaxParticles / theEmissionRate) / 10));

    //        _DeadParticles = theMaxParticles;
    //        _DeadParticleList = new Stack<uint>();

    //        _ParticleColour = Color.White;

    //        _Modifiers = new ModifierCollection();

    //        _Vertices = new VertexPositionColor[theMaxParticles];

    //        _VDec = new VertexDeclaration(GDev, VertexPositionColor.VertexElements);

    //        //set particles
    //        for (UInt32 i = 0; i < theMaxParticles; i++)
    //        {
    //            _ParticleList[i] = new BaseParticle3D();
    //            _DeadParticleList.Push(i);
    //        }

    //        _Timer.Start();

    //        _VertexBuffer = new DynamicVertexBuffer(_GDev, (int)(VertexPositionColor.SizeInBytes * _MaxParticles), BufferUsage.WriteOnly);

    //        Spawn(_EmissionRate);

    //        _GDev.VertexDeclaration = _VDec;
    //    }

    //    protected void Spawn(UInt32 theNumber)
    //    {
    //        UInt32 number = theNumber;
    //        if (_DeadParticleList.Count < theNumber)
    //        {
    //            number = Convert.ToUInt32(_DeadParticleList.Count);
    //        }
    //        for (int i = 0; i < number; i++)
    //        {
    //            UInt32 Number = _DeadParticleList.Pop();
    //            _ParticleList[Number].Alive = true;
    //            uint RanAngle = _RNG.NextUInt32(0, 360);

    //            _ParticleList[Number].CurrentLife = Convert.ToSingle(_RNG.NextUInt32(0, Convert.ToUInt32(_MaxParticleLife)));
    //            _ParticleList[Number].Position = _SystemPosition;
    //            _ParticleList[Number].Velocity = _AngleGen.AngleToVector3D(RanAngle);
    //            _ParticleList[Number].Alive = true;
    //            _ParticleList[Number].Colour = _ParticleColour;

    //            _DeadParticles--;
    //        }
    //    }

    //    public void Draw()
    //    {
    //        SetVertices();
    //        foreach (BaseParticle3D P in _ParticleList)
    //        {
    //            //
    //            _Effect.Begin();
    //            foreach (EffectPass pass in _Effect.CurrentTechnique.Passes)
    //            {
    //                pass.Begin();

    //                _GDev.DrawUserPrimitives(PrimitiveType.PointList, _Vertices, 0, _ParticleList.Length - _DeadParticleList.Count);

    //                pass.End();
    //            }
    //            _Effect.End();
    //        }
    //   }
    //    public void Update()
    //    {
    //        bool SecondLapse = false;

    //        if (_Timer.ElapsedMilliseconds > 1000)
    //        {
    //            SecondLapse = true;
    //            Spawn(_EmissionRate);
    //        }
    //        for (UInt32 i = 0; i < _ParticleList.Length; i++)
    //        {
    //            //if particle alive
    //            if (_ParticleList[i].Alive)
    //            {
    //                if (SecondLapse)
    //                {
    //                    _ParticleList[i].CurrentLife -= 1;
    //                }
    //                // if particle is dead or out of bounds ..kill
    //                if (_ParticleList[i].CurrentLife <= 0 || _ParticleList[i].Position.X > _ScreenDimensions.X || _ParticleList[i].Position.Y > _ScreenDimensions.Y || _ParticleList[i].Position.X < 0 || _ParticleList[i].Position.Y < 0)
    //                {
    //                    _ParticleList[i].Alive = false;
    //                    _DeadParticles++;
    //                    _DeadParticleList.Push(i);
    //                }
    //                else
    //                {
    //                    _ParticleList[i].Position.X += _ParticleList[i].Velocity.X;
    //                    _ParticleList[i].Position.Y += _ParticleList[i].Velocity.Y;
    //                    foreach (BaseModifier Mod in _Modifiers)
    //                    {
    //                        Mod.ApplyModifier(ref _ParticleList[i]);
    //                    }
    //                }
    //                //if its time to then spawn more particles
    //                if (SecondLapse)
    //                {
    //                    Spawn(_EmissionRate);
    //                }
    //            }
    //        }
            
    //    }
    //    public void SetVertices()
    //    {
    //        int LiveParticles = (int)(_ParticleList.Length - _DeadParticleList.Count);
 
    //        _Vertices = new VertexPositionColor[LiveParticles];

    //        UInt32 AddedCount = 0;

    //        for(int i =0 ; i < _ParticleList.Length; i++)
    //        {
    //            if (_ParticleList[i].Alive == true)
    //            {
    //                _Vertices[AddedCount].Position = new Vector3(-1.0f + 2.0f * _ParticleList[i].Position.X / _ScreenDimensions.X,
    //                                                            -(-1.0f + 2.0f * _ParticleList[i].Position.Y / _ScreenDimensions.Y)
    //                                                            , 0);
    //                _Vertices[AddedCount].Color = _ParticleList[i].Colour;
                                                                
    //                AddedCount ++;   
    //            }
               
    //        }
    //        _VertexBuffer.Dispose();
    //        _VertexBuffer = new DynamicVertexBuffer(_GDev, VertexPositionColor.SizeInBytes * _Vertices.Length, BufferUsage.None);
    //        _VertexBuffer.SetData<VertexPositionColor>(_Vertices);

    //    }

    //    public int AddModifier(BaseModifier theMod)
    //    {
    //        return _Modifiers.Add(theMod);
    //    }
    //    public void ChangeModifier(int Index, BaseModifier theMod)
    //    {
    //        if (_Modifiers.Count >= Index)
    //        {
    //            _Modifiers[Index] = theMod;
    //        }
    //        else
    //        {
    //            throw new SystemException();
    //        }
    //    }

    //}
    #endregion
}