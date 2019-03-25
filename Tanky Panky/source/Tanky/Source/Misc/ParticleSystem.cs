/*******************************************************
 * ParticleSystem.cs        * TankyPanky
 * Alex Bleasdale           * 18/01/09
 *******************************************************/

using System;
using System.IO;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Net;
using Microsoft.Xna.Framework.Storage;

namespace Tanky
{

	public class EffectSystem
	{
        // particle effect variables
        public enum fx_e
        {
            EFFECT_SKIP_LINE = 0,
            EFFECT_COLOR,
            EFFECT_SPEED,
            EFFECT_ACCELERATION,
            EFFECT_DURATION,
            EFFECT_FALLOFF,
            EFFECT_MIN_SIZE,
            EFFECT_MAX_SIZE,
            EFFECT_EXPANSION_RATE,
            EFFECT_GRAVITY,
            EFFECT_RANDOM_DRIFT,
            EFFECT_RANDOM_FORCE,
            EFFECT_DRAG,
            EFFECT_DRAG_DIR,
            EFFECT_MIN_PARTICLES,
            EFFECT_MAX_PARTICLES,
            EFFECT_BILLBOARD_MODE,
            EFFECT_HEMISPHERE_SPREAD,
            EFFECT_EMITTER_MODE,
            EFFECT_EMITTER_LIFE,
            EFFECT_EMITTER_FREQUENCY,
            EFFECT_SPAWN_AREA,
            EFFECT_PARTICLE_POOL_SIZE,

            NUM_EFFECT_VARS
        };

        public Timer m_particle_timer;

        // rendering bits
        public BasicEffect m_particle_routine_effect;
        public VertexDeclaration m_vertex_declaration;

        // list for particle effects
        private List<ParticleEffect> m_effect_list;

        // list for particle instances
        private List<ParticleObject> m_particle_object_list;

        // quad instance
        public Quad m_normal_quad;

        // textures
        public enum textures_e
        {
            TEX_SMOKE_1 = 0,
            TEX_FLAME_1,
            TEX_MUZZLE_FLASH_1,
            TEX_MUZZLE_FLASH_2,
            TEX_SNOW_1,
            TEX_RAIN_1
        };

        private Texture2D[] m_texture = new Texture2D[8];

        public Texture2D GetTexture(textures_e tex)
        {
            return m_texture[(int)tex];
        }

        // load effects, initialize
        public EffectSystem()
        {
            m_particle_timer = new Timer();

            m_effect_list = new List<ParticleEffect>();
            m_particle_object_list = new List<ParticleObject>();

            // preload effect items
            m_particle_object_list.Add(new ParticleObject("effects/smoke2.txt"));
            m_particle_object_list.Add(new ParticleObject("effects/smoke1.txt"));
            m_particle_object_list.Add(new ParticleObject("effects/explosion1.txt"));
            m_particle_object_list.Add(new ParticleObject("effects/tankmuzzle.txt"));
            m_particle_object_list.Add(new ParticleObject("effects/smokeemitter1.txt"));
            m_particle_object_list.Add(new ParticleObject("effects/bigexplosion.txt"));
            m_particle_object_list.Add(new ParticleObject("effects/snow1.txt"));
            m_particle_object_list.Add(new ParticleObject("effects/bigsmoke.txt"));
            m_particle_object_list.Add(new ParticleObject("effects/rain1.txt"));

            m_normal_quad = new Quad( Vector2.One );

            m_particle_routine_effect = new BasicEffect(g.graphics.GraphicsDevice, null);
            m_vertex_declaration = new VertexDeclaration(g.graphics.GraphicsDevice, VertexPositionNormalTexture.VertexElements);

            m_texture[(int)textures_e.TEX_SMOKE_1] = g.content.Load<Texture2D>("Particles\\Smoke");
            m_texture[(int)textures_e.TEX_FLAME_1] = g.content.Load<Texture2D>("Particles\\Flame1");
            m_texture[(int)textures_e.TEX_MUZZLE_FLASH_1] = g.content.Load<Texture2D>("Particles\\Muzzle1");
            m_texture[(int)textures_e.TEX_MUZZLE_FLASH_2] = g.content.Load<Texture2D>("Particles\\Muzzle2");
            m_texture[(int)textures_e.TEX_SNOW_1] = g.content.Load<Texture2D>("Particles\\Snow1");
            m_texture[(int)textures_e.TEX_RAIN_1] = g.content.Load<Texture2D>("Particles\\Rain1");
        }

        // spawn an effect using file config
        public void CreateEffect(Vector3 pos, Vector3 dir, String configfile, Texture2D tex)
        {
            m_effect_list.Add(new ParticleEffect(pos, dir, configfile,tex));
        }

        public ParticleEffect CreateEmitter(Vector3 pos, Vector3 dir, String configfile, Texture2D tex)
        {
            ParticleEffect p = new ParticleEffect(pos, dir, configfile, tex);
            m_effect_list.Add(p);
            return p;
        }

        public void CreateFlash(Vector3 pos, Vector3 dir, String configfile, Texture2D tex1, Texture2D tex2)
        {
            m_effect_list.Add(new ParticleEffect(pos, dir, configfile, tex1, tex2));
        }

        // remove an effect from the list (should then be garbage collected)
        public void RemoveEffect(ParticleEffect effect)
        {
            m_effect_list.Remove(effect);
        }

        public void Update()
        {
            if (m_effect_list.Count > 0)
            {
                ParticleEffect[] effects = m_effect_list.ToArray();
                for (int i = m_effect_list.Count-1; i >= 0; i--)
                {
                    if (!effects[i].update())
                    {
                        m_effect_list.RemoveAt(i);
                    }
                }
            }
        }

        public void Draw()
        {
            if (m_effect_list.Count > 0)
            {
                ParticleEffect[] effects = m_effect_list.ToArray();
                for (int i = m_effect_list.Count - 1; i >= 0; i--)
                {
                    effects[i].draw();
                }
            }
        }

        public ParticleObject GetParticleObject(String config)
        {
            foreach (ParticleObject pobj in m_particle_object_list)
            {
                string name = config.ToString();
                if( pobj.m_filename.Equals( config ) )
                {
                    return pobj;
                }
            }

            // not found!
            return null;
        }
        
	}

    /*
     * This stores all the data for a NEW ParticleEffect (see below).
     * Objects are pre-loaded at EffectSystem initialization.
     */
    public class ParticleObject
    {
        public String m_filename;

        // variables
        public Color m_col;

        //float m_stagger;
        //float m_delay;
        public float m_duration;
        public float m_falloff;
        public float m_min_size;
        public float m_max_size;
        public float m_expansion;
        public float m_gravity;
        public float m_velocity;
        public float m_acceleration;

        public float m_random_drift;
        public float m_random_force;
        public float m_drag_speed;
        public Vector3 m_drag_dir;

        public int m_min_particles;
        public int m_max_particles;
        public int m_particle_pool_size;

        public bool m_billboarded;
        public bool m_is_emitter;

        public float m_emitter_life;
        public float m_emitter_frequency;

        public Vector2 m_spawn_dims;

        public float m_spread;

        public ParticleObject(String config)
        {
            m_filename = config;
            FileStream fin = new FileStream(config, FileMode.Open, FileAccess.Read, FileShare.Read);
            StreamReader stream = new StreamReader(fin);

            while (!stream.EndOfStream)
            {
                String line = stream.ReadLine();
                String[] data = line.Split(' ');

                EffectSystem.fx_e fx = EffectSystem.fx_e.EFFECT_SKIP_LINE;
                for (int i = 0; i < (int)EffectSystem.fx_e.NUM_EFFECT_VARS; i++)
                {
                    if (data[0] == g.effect_strings[i])
                    {
                        fx = (EffectSystem.fx_e)i;
                        break;
                    }
                }
                if (fx == EffectSystem.fx_e.EFFECT_SKIP_LINE)
                    continue;

                switch (fx)
                {
                    case EffectSystem.fx_e.EFFECT_COLOR:
                        m_col.R = System.Convert.ToByte(data[1]);
                        m_col.G = System.Convert.ToByte(data[2]);
                        m_col.B = System.Convert.ToByte(data[3]);
                        m_col.A = System.Convert.ToByte(data[4]);
                        break;
                    case EffectSystem.fx_e.EFFECT_ACCELERATION:
                        m_acceleration = System.Convert.ToSingle(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_BILLBOARD_MODE:
                        m_billboarded = System.Convert.ToBoolean(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_DRAG:
                        m_drag_speed = System.Convert.ToSingle(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_DRAG_DIR:
                        m_drag_dir.X = System.Convert.ToSingle(data[1]);
                        m_drag_dir.Y = System.Convert.ToSingle(data[2]);
                        m_drag_dir.Z = System.Convert.ToSingle(data[3]);
                        break;
                    case EffectSystem.fx_e.EFFECT_DURATION:
                        m_duration = System.Convert.ToSingle(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_EXPANSION_RATE:
                        m_expansion = System.Convert.ToSingle(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_FALLOFF:
                        m_falloff = System.Convert.ToSingle(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_GRAVITY:
                        m_gravity = System.Convert.ToSingle(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_MAX_PARTICLES:
                        m_max_particles = System.Convert.ToInt32(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_MAX_SIZE:
                        m_max_size = System.Convert.ToSingle(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_MIN_PARTICLES:
                        m_min_particles = System.Convert.ToInt32(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_MIN_SIZE:
                        m_min_size = System.Convert.ToSingle(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_RANDOM_DRIFT:
                        m_random_drift = System.Convert.ToSingle(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_RANDOM_FORCE:
                        m_random_force = System.Convert.ToSingle(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_SPEED:
                        m_velocity = System.Convert.ToSingle(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_HEMISPHERE_SPREAD:
                        m_spread = System.Convert.ToSingle(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_EMITTER_MODE:
                        m_is_emitter = System.Convert.ToBoolean(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_EMITTER_LIFE:
                        m_emitter_life = System.Convert.ToSingle(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_EMITTER_FREQUENCY:
                        m_emitter_frequency = System.Convert.ToSingle(data[1]);
                        break;
                    case EffectSystem.fx_e.EFFECT_SPAWN_AREA:
                        m_spawn_dims.X = System.Convert.ToSingle(data[1]);
                        m_spawn_dims.Y = System.Convert.ToSingle(data[2]);
                        break;
                    case EffectSystem.fx_e.EFFECT_PARTICLE_POOL_SIZE:
                        m_particle_pool_size = System.Convert.ToInt32(data[1]);
                        break;
                }
            }

            // normalize drag direction
            if (m_drag_dir != Vector3.Zero)
                m_drag_dir.Normalize();
        }
    }

    /*
     * This is an effect to be spawned on the fly by EffectSystem. A number of
     * particles (or 'quads') can be created at once.
     * Not to be accessed directly. call CreateEffect from EffectSystem!!
     */
    public class ParticleEffect
    {
        protected float m_timer = 0.0f;

        private List<Particle> m_particle_list;
        public Texture2D m_texture1;
        public Texture2D m_texture2;

        public bool m_active;
        Random m_rand;

        private Color m_col;
        private float m_duration;
        private float m_gravity;
        private float m_velocity;
        public float m_random_force;
        public float m_emitter_life;
        public float m_emitter_frequency;

        private ParticleObject m_obj;

        public Vector3 m_pos;
        public Vector3 m_dir;

        public ParticleEffect(Vector3 pos, Vector3 dir, String configfile, Texture2D tex)
        {
            m_particle_list = new List<Particle>();

            m_obj = g.effect_sys.GetParticleObject(configfile);
            if (m_obj == null)
            {
                return;
            }

            m_col = m_obj.m_col;
            m_duration = m_obj.m_duration;
            m_gravity = m_obj.m_gravity;
            m_velocity = m_obj.m_velocity;
            m_random_force = m_obj.m_random_force;
            m_emitter_life = m_obj.m_emitter_life;
            m_emitter_frequency = m_obj.m_emitter_frequency;

            m_texture1 = tex;

            m_rand = new Random((int)DateTime.Now.Ticks % Int32.MaxValue);
            m_active = true;

            m_pos = pos;
            m_dir = dir;
            m_dir.Normalize();

            SpawnParticles( true );
        }

        public ParticleEffect(Vector3 pos, Vector3 dir, String configfile, Texture2D tex1, Texture2D tex2)
        {
            m_particle_list = new List<Particle>();

            m_obj = g.effect_sys.GetParticleObject(configfile);
            if (m_obj == null)
            {
                return;
            }

            m_col = m_obj.m_col;
            m_duration = m_obj.m_duration;
            m_gravity = m_obj.m_gravity;
            m_velocity = m_obj.m_velocity;
            m_random_force = m_obj.m_random_force;
            m_emitter_life = m_obj.m_emitter_life;
            m_emitter_frequency = m_obj.m_emitter_frequency;

            m_texture1 = tex1;
            m_texture2 = tex2;

            m_rand = new Random((int)DateTime.Now.Ticks % Int32.MaxValue);
            m_active = true;

            m_pos = pos;
            m_dir = dir;
            m_dir.Normalize();

            SpawnParticles( true );
        }

        private void FillParticleData(ref Vector3 pos, ref Vector3 dir, ref float scale)
        {
            // randomize size of particle between min and max
            float size;
            int min_size = (int)((float)m_obj.m_min_size * 1000);
            int max_size = (int)((float)m_obj.m_max_size * 1000);
            int isize = m_rand.Next(min_size, max_size);
            size = (float)isize * 0.001f;


            Vector3 p_dir = m_dir;
            if (m_obj.m_spread != 0.0f)
            {
                int rand_x = m_rand.Next(0, 1000);
                int rand_z = m_rand.Next(0, 1000);
                float new_x = (float)rand_x * 0.001f;
                float new_z = (float)rand_z * 0.001f;

                p_dir = new Vector3(new_x, m_obj.m_spread, new_z);
                p_dir.Normalize();
            }

            Vector2 ext = m_obj.m_spawn_dims * 0.5f;
            Vector3 random_pos = m_pos;
            if (m_obj.m_spawn_dims != Vector2.Zero)
            {
                random_pos = new Vector3(m_pos.X + (float)m_rand.Next((int)-ext.X, (int)ext.X),
                                        m_pos.Y,
                                        m_pos.Z + (float)m_rand.Next((int)-ext.Y, (int)ext.Y));
            }

            dir = p_dir;
            pos = random_pos;
            scale = size;
        }

        private void SpawnParticles( bool first_spawn )
        {
            int count = 0;
            int num_particles = m_rand.Next(m_obj.m_min_particles, m_obj.m_max_particles);

            if (first_spawn)
            {
                for (int i = 0; i < num_particles; i++)
                {

                    Vector3 random_pos = Vector3.Zero;
                    Vector3 p_dir = Vector3.Zero;
                    float size = 0.0f;
                    FillParticleData(ref random_pos, ref p_dir, ref size);

                    m_particle_list.Add(new Particle(random_pos, p_dir, size, m_obj.m_duration, g.effect_sys.m_normal_quad));
                }

                // spawn the remaining particles as dormant
                if( m_obj.m_is_emitter )
                {
                    for (int i = 0; i < m_obj.m_particle_pool_size - num_particles; i++)
                    {
                        m_particle_list.Add(new Particle(Vector3.Zero, Vector3.Zero, 0.0f, 0.0f, g.effect_sys.m_normal_quad));
                    }
                }
            }

            foreach (Particle p in m_particle_list)
            {
                if (count >= num_particles)
                    break;

                if (!first_spawn)
                {
                    // relocate only dead particles
                    if (p.m_life > 0.0f)
                        continue;
                    else
                    {
                        Vector3 pos = Vector3.Zero;
                        Vector3 dir = Vector3.Zero;
                        float scale = 0.0f;
                        FillParticleData(ref pos, ref dir, ref scale);

                        p.m_pos = pos;
                        p.m_dir = dir;
                        p.m_scale = scale * p.m_quad_i.m_scale;
                        p.m_life = m_obj.m_duration;

                        count++;
                    }
                }

                p.m_gravity = m_gravity;
                p.m_random_force = m_random_force;
                p.m_velocity = m_velocity;
                p.m_alpha = m_col.A;

                float[] random_drift = new float[3];

                for (int i = 0; i < 3; i++)
                {
                    int drift = (int)m_obj.m_random_drift * 100;
                    int idrift = m_rand.Next(-drift, drift);
                    random_drift[i] = (float)idrift * 0.01f;
                }
                p.m_drift.X = random_drift[0];
                p.m_drift.Y = random_drift[1];
                p.m_drift.Z = random_drift[2];

                if (m_obj.m_billboarded)
                {
                }
                else
                {
                    p.m_dir = m_dir;
                }

                if (first_spawn)
                {
                    count++;
                    if (count >= num_particles)
                        break;
                }
            }
        }

        public bool update()
        {
            if (m_active)
            {
                float seconds = g.effect_sys.m_particle_timer.GetTimePerFrame();
                m_timer += seconds;

                // if this effect doesnt die after one emission, we need to clear old particles!
                if (m_obj.m_is_emitter)
                {
                    m_emitter_life -= seconds;
                    m_emitter_frequency -= seconds;
                    if (m_emitter_frequency < 0.0f)
                    {
                        m_emitter_frequency = m_obj.m_emitter_frequency;
                        SpawnParticles( false );
                    }

                    //// kill dead particles
                    //Particle[] particles = m_particle_list.ToArray();
                    //for (int i = m_particle_list.Count-1; i >= 0; i--)
                    //{
                    //    if (!particles[i].update())
                    //        m_particle_list.RemoveAt(i);
                    //}

                }
                else
                {
                    // not an emitter, so just use duration as effect lifespan
                    m_duration -= seconds;
                }

                // do stuff!!
                foreach (Particle p in m_particle_list)
                {
                    // reduce particle lifespan
                    p.update();

                    // apply psuedo forces
                    Vector3 new_pos = Vector3.Zero;

                    new_pos.Y -= p.m_gravity;
                    new_pos += m_obj.m_drag_dir * m_obj.m_drag_speed;
                    new_pos += p.m_drift;
                    new_pos += p.m_dir * (p.m_velocity + p.m_random_force);
                    new_pos *= seconds * 0.05f;

                    p.m_pos += new_pos;

                    if (m_timer >= 0.1f)
                    {
                        float alpha = (float)p.m_alpha;
                        alpha -= m_obj.m_falloff;
                        if (alpha < 0.0f)
                            alpha = 0.0f;
                        p.m_alpha = (byte)alpha;

                        p.m_scale += m_obj.m_expansion;
                        if (p.m_scale <= 0.0f)
                            p.m_scale = 0.0f;
                    }

                    // simulate some hacky physics
                    if (m_timer >= 0.1f)
                    {
                        p.m_gravity *= 1.04f;
                        p.m_random_force *= 0.40f;
                        p.m_velocity *= m_obj.m_acceleration;
                    }

                    if (p.m_velocity < 0.0f)
                        p.m_velocity = 0.0f;

                }

                if (m_timer >= 0.1f)
                    m_timer = 0.0f;

                if ((!m_obj.m_is_emitter && m_duration <= 0.0f) || (m_obj.m_is_emitter && m_emitter_life <= 0.0f))
                {
                    m_active = false;
                }
            }
            else
            {
                m_particle_list.Clear();
            }

            return m_active;
        }

        public void draw()
        {
            // set particle render states
            g.graphics.GraphicsDevice.RenderState.AlphaTestEnable = false;
            g.graphics.GraphicsDevice.RenderState.AlphaBlendEnable = true;
            g.graphics.GraphicsDevice.RenderState.DepthBufferWriteEnable = false;
            g.graphics.GraphicsDevice.RenderState.SourceBlend = Blend.SourceAlpha;
            g.graphics.GraphicsDevice.RenderState.DestinationBlend = Blend.InverseSourceAlpha;
            g.graphics.GraphicsDevice.RenderState.BlendFunction = BlendFunction.Add;
            CullMode old_cull_mode = g.graphics.GraphicsDevice.RenderState.CullMode;

            // set view matrices
            g.effect_sys.m_particle_routine_effect.Projection = g.camera.m_projection_matrix;
            g.effect_sys.m_particle_routine_effect.View = g.camera.m_view_matrix;

            g.graphics.GraphicsDevice.VertexDeclaration = g.effect_sys.m_vertex_declaration;

            // set alpha and texture for all particles
            g.effect_sys.m_particle_routine_effect.Texture = m_texture1;
            g.effect_sys.m_particle_routine_effect.TextureEnabled = true;

            // set an orientation matrix
            Vector2 rot = Vector2.Zero;
            if (!m_obj.m_billboarded)
            {
                // match the rotation of the particle effect
                g.graphics.GraphicsDevice.RenderState.CullMode = CullMode.None;
                rot.X = (float)-Math.Atan2((double)m_dir.Y, Math.Sqrt(((double)m_dir.X * (double)m_dir.X) + ((double)m_dir.Z * (double)m_dir.Z)));
                rot.Y = (float)Math.Atan2((double)m_dir.X, (double)m_dir.Z);
            }
            else
            {
                // billboard!
            }

            Matrix orientation = Matrix.CreateRotationX(rot.X);
            orientation *= Matrix.CreateRotationY(rot.Y);

            // particle specific draw stuff
            foreach (Particle p in m_particle_list)
            {
                if (p.m_life <= 0.0f)
                    continue;

                int passes = 1;
                if (m_texture2 != null)
                    passes = 3;

                g.effect_sys.m_particle_routine_effect.Alpha = (float)(p.m_alpha / 255.0f);

                g.effect_sys.m_particle_routine_effect.Begin();

                for (int i = 0; i < passes; i++)
                {
                    // calculate flash particle matrices
                    // a flash has 3 particle renders per 'registered' particle, using 2 different textures as specified by the effect
                    // a normal (non-muzzle effect) particle has just one
                    Matrix mrot = Matrix.Identity;
                    switch (i)
                    {
                        case 0:
                            // first particle pass - match orientation matrix
                            mrot = orientation;
                            break;
                        case 1:
                            // second particle pass - rotate 90 degrees through Y
                            g.effect_sys.m_particle_routine_effect.Texture = m_texture2;
                            mrot = Matrix.CreateRotationY(MathHelper.ToRadians(90)) * orientation;
                            break;
                        case 2:
                            // third particle pass - rotate 90 degrees through X, then Y
                            mrot = Matrix.CreateRotationX(MathHelper.ToRadians(90)) * Matrix.CreateRotationY(MathHelper.ToRadians(90)) * orientation;
                            break;
                    }

                    // move particle 2+ forward slightly
                    Vector3 d = Vector3.Zero;
                    if (i > 0)
                        d = m_dir * 1.2f;

                    // set final world matrix transformation
                    g.effect_sys.m_particle_routine_effect.World = Matrix.CreateScale(p.m_scale) * mrot * Matrix.CreateTranslation(p.m_pos + d);

                    // render particle!
                    foreach (EffectPass pass in g.effect_sys.m_particle_routine_effect.CurrentTechnique.Passes)
                    {
                        pass.Begin();

                        g.graphics.GraphicsDevice.DrawUserIndexedPrimitives<VertexPositionNormalTexture>
                        (
                            PrimitiveType.TriangleStrip,
                            p.m_quad_i.m_verts,
                            0, 4,
                            p.m_quad_i.m_index,
                            0, 2
                        );

                        pass.End();
                    }
                }

                g.effect_sys.m_particle_routine_effect.End();


            }

            // reset to common render states
            g.graphics.GraphicsDevice.RenderState.SourceBlend = Blend.SourceAlpha;
            g.graphics.GraphicsDevice.RenderState.DestinationBlend = Blend.InverseSourceAlpha;
            g.graphics.GraphicsDevice.RenderState.DepthBufferWriteEnable = true;
            g.graphics.GraphicsDevice.RenderState.AlphaTestEnable = true;
            g.graphics.GraphicsDevice.RenderState.AlphaBlendEnable = false;
            g.graphics.GraphicsDevice.RenderState.CullMode = old_cull_mode;

        }

    }

    // Textured quad primitive
    public class Quad
    {
        public float m_scale;

        // vertices
        public VertexPositionNormalTexture[] m_verts;
        public int[] m_index;

        // constructor
        public Quad(Vector2 dims)
        {
            m_scale = 1.0f;
            m_verts = new VertexPositionNormalTexture[4];
            m_index = new int[4];

            m_verts[0].Position = new Vector3(-dims.X, -dims.Y, 0.0f);
            m_verts[1].Position = new Vector3(-dims.X, dims.Y, 0.0f);
            m_verts[2].Position = new Vector3(dims.X, -dims.Y, 0.0f);
            m_verts[3].Position = new Vector3(dims.X, dims.Y, 0.0f);

            m_verts[0].TextureCoordinate = new Vector2(0.0f, 0.0f);
            m_verts[1].TextureCoordinate = new Vector2(1.0f, 0.0f);
            m_verts[2].TextureCoordinate = new Vector2(0.0f, 1.0f);
            m_verts[3].TextureCoordinate = new Vector2(1.0f, 1.0f);

            // Set index buffer
            m_index[0] = 0;
            m_index[1] = 1;
            m_index[2] = 2;
            m_index[3] = 3;
        }

    }

    // Single particle made from a textured quad
    public class Particle
    {
        public Quad m_quad_i;
        public float m_scale;
        public Vector3 m_drift;
        public Vector3 m_pos;
        public Vector3 m_dir;
        public float m_gravity;
        public float m_life;
        public float m_random_force;
        public float m_velocity;
        public byte m_alpha;

        public Particle(Vector3 pos, Vector3 dir, float scale, float life, Quad quad)
        {
            m_quad_i = quad;
            m_scale = scale * quad.m_scale;
            m_pos = pos;
            m_dir = dir;
            m_life = life;
        }

        // active flag management
        public bool update()
        {
            if (m_life <= 0.0f)
                return false;
            m_life -= g.effect_sys.m_particle_timer.GetTimePerFrame();
            return true;
        }
    }
}
