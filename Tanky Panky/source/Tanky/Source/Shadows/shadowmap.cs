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

namespace Tanky
{
    public class Shadowmap
    {
        RenderTarget2D m_shadow_map_target;
        public Texture2D m_shadow_map;

        DepthStencilBuffer m_shadow_stencil_buffer;
        DepthStencilBuffer m_old_stencil_buffer;

        //ModelTP m_model;
        //CullMode m_cull_mode;

        public void initialise()
        {
            m_shadow_map_target = new RenderTarget2D
            (g.graphics.GraphicsDevice,
            2048,
            2048,
            1,
            SurfaceFormat.Single);

            m_shadow_stencil_buffer = new DepthStencilBuffer
            (g.graphics.GraphicsDevice,
            2048,
            2048,
            g.graphics.GraphicsDevice.DepthStencilBuffer.Format);

        }

        public Shadowmap()
        {
        }

        public void shadowmap_begin()
        {
           g.graphics.GraphicsDevice.SetRenderTarget(0, m_shadow_map_target);

           m_old_stencil_buffer = g.graphics.GraphicsDevice.DepthStencilBuffer;
           g.graphics.GraphicsDevice.DepthStencilBuffer = m_shadow_stencil_buffer;

           g.graphics.GraphicsDevice.Clear(Color.White);
        }

        public void shadowmap_end()
        {
            g.graphics.GraphicsDevice.SetRenderTarget(0, null);

            g.graphics.GraphicsDevice.DepthStencilBuffer = m_old_stencil_buffer;

            g.graphics.GraphicsDevice.Clear(Color.CornflowerBlue);

            m_shadow_map = m_shadow_map_target.GetTexture();
            
        }

    }
}
