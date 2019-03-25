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
    public class Prop
    {
        Vector3 m_position;
        public ModelTP m_model;

        //CullMode m_cull_mode;

        Matrix m_world_matrix;
      
        BEPUphysics.Entity m_physics_model;

        String m_model_name;
        public int m_prop_id;

        public BEPUphysics.Entity PhysicsModel
        {
            get
            {
                return m_physics_model;
            }
        }

        public bool HasPhysicsModel()
        {
            return m_physics_model != null;
        }
        

        public void load(String model_name, Vector3 position)
        {
            m_model = new ModelTP();
            m_model.load(model_name);

            m_model_name = model_name;

            m_position = position;

            physics_load(model_name);
        }

        public void update()
        {

        }

        public void draw(Matrix projection, Matrix view)
        {
            if (m_physics_model == null) 
            {
                g.graphics.GraphicsDevice.RenderState.CullMode = CullMode.None;

                m_world_matrix = Matrix.CreateScale(3.0f) * Matrix.CreateTranslation(m_position) * Matrix.CreateScale(0.1f);

                m_model.draw(projection, view, m_world_matrix);

                g.graphics.GraphicsDevice.RenderState.CullMode = CullMode.CullCounterClockwiseFace;

                return;
            }

            Vector3 translation = m_physics_model.centerPosition;

            if (m_model_name == "Models\\tree" || m_model_name == "Models\\tree_2")
            {
                g.graphics.GraphicsDevice.RenderState.CullMode = CullMode.None;
                translation.X -= 2.0f;
                translation.Z -= 1.5f;
            }

            m_world_matrix = Matrix.CreateScale(3.0f) * Matrix.CreateScale(0.1f) * m_physics_model.orientationMatrix *
                Matrix.CreateTranslation(translation);

            m_model.draw(projection, view, m_world_matrix);

            g.graphics.GraphicsDevice.RenderState.CullMode = CullMode.CullCounterClockwiseFace;
        }

        public void shadow_map_render(Matrix projection, Matrix view)
        {
            if (m_physics_model == null)
            {
                g.graphics.GraphicsDevice.RenderState.CullMode = CullMode.None;

                m_world_matrix = Matrix.CreateScale(3.0f) * Matrix.CreateTranslation(m_position) * Matrix.CreateScale(0.1f);

                m_model.draw(projection, view, m_world_matrix);

                g.graphics.GraphicsDevice.RenderState.CullMode = CullMode.CullCounterClockwiseFace;

                return;
            }

            Vector3 translation = m_physics_model.centerPosition;

            if (m_model_name == "Models\\tree" || m_model_name == "Models\\tree_2")
            {
                g.graphics.GraphicsDevice.RenderState.CullMode = CullMode.None;
                translation.X -= 2.0f;
                translation.Z -= 1.5f;
            }

            m_world_matrix = Matrix.CreateScale(3.0f) * Matrix.CreateScale(0.1f) * m_physics_model.orientationMatrix *
                    Matrix.CreateTranslation(translation);

            m_model.shadow_map_render(projection, view, m_world_matrix);
        }

        public void physics_load(string model_name)
        {
            float scale = g.scaleFactor;

            //scale the position by global scale
            Vector3 tempPos = Vector3.Multiply(m_position, scale);

            switch(model_name)
            {
                case "Models\\barrel_1":
                case "Models\\barrel_2":
                case "Models\\barrel_3":
                case "Models\\barrel_4":
                {
                    BEPUphysics.Box temp = m_model.GetBepuBox();

                    Vector3 posi = ((temp.centerPosition * 3) + m_position) * 0.1f;

                    m_physics_model = new BEPUphysics.Cylinder(posi, (temp.height * 3) * 0.1f, temp.halfWidth * 0.1f * 3,10);
                    break;
                }

                case "Models\\container_1":
                case "Models\\container_2":
                {
                    BEPUphysics.Box temp = m_model.GetBepuBox();

                    m_physics_model = new BEPUphysics.Box(tempPos, temp.width * scale * 3, temp.height * scale * 3, temp.length * scale * 3, 1000);
                    break;
                }

                case "Models\\tree":
                case "Models\\tree_2":
                {
                    BEPUphysics.Box temp = m_model.GetBepuBox();

                    Vector3 pos = tempPos;

                    m_physics_model = new BEPUphysics.Cylinder(pos, temp.height * scale * 3 + 15, 3);

                    m_physics_model.makeNonDynamic();
                    break;
                }

                case "Models\\woodbox_1":
                case "Models\\woodbox_2":
                case "Models\\woodbox_3":
                case "Models\\woodbox_4":
                {
                    BEPUphysics.Box temp = m_model.GetBepuBox();
                    m_physics_model = new BEPUphysics.Box(tempPos, temp.width * scale * 3, temp.height * scale * 3, temp.length * scale * 3, 10);
                    break;
                }

                case "Models\\pallet":
                {
                    BEPUphysics.Box temp = m_model.GetBepuBox();
                    m_physics_model = new BEPUphysics.Box(tempPos, temp.width * scale * 3, (temp.height + 3) * scale * 3, temp.length * scale * 3, 10);
                    break;
                }

                default:
                {
                    m_physics_model = null;
                    break;
                }
                
            }
            if (m_physics_model != null)
            {
                //Add tag
                PropTag tempTag;
                tempTag.ModelName = model_name;
                m_physics_model.tag = tempTag;
            }
        }

        public bool IsDynamic()
        {
            bool dynamic = true;
            if (m_physics_model == null)
            {
                dynamic = false;
            }
            return dynamic;
        }

        public virtual PropPackage SnapShotOut()
        {
            PropPackage returnPackage = new PropPackage();

            EntityPackage vOut = new EntityPackage();

            vOut.correctiveAngularVelocity = m_physics_model.correctiveAngularVelocity;
            vOut.correctiveLinearVelocity = m_physics_model.correctiveLinearVelocity;
            vOut.angularMomentum = m_physics_model.angularMomentum;
            vOut.angularVelocity = m_physics_model.angularVelocity;
            vOut.centerPosition = m_physics_model.centerPosition;
            vOut.internalAngularMomentum = m_physics_model.internalAngularMomentum;
            vOut.internalAngularVelocity = m_physics_model.internalAngularVelocity;
            vOut.internalCenterPosition = m_physics_model.internalCenterPosition;
            vOut.internalLinearMomentum = m_physics_model.internalLinearMomentum;
            vOut.internalLinearVelocity = m_physics_model.internalLinearVelocity;
            vOut.internalOrientationMatrix = m_physics_model.internalOrientationMatrix;
            vOut.internalOrientationQuaternion = m_physics_model.internalOrientationQuaternion;
            vOut.linearMomentum = m_physics_model.linearMomentum;
            vOut.linearVelocity = m_physics_model.linearVelocity;
            vOut.localSpaceInertiaTensor = m_physics_model.localSpaceInertiaTensor;
            vOut.localSpaceInertiaTensorInverse = m_physics_model.localSpaceInertiaTensorInverse;
            vOut.orientationMatrix = m_physics_model.orientationMatrix;
            vOut.orientationQuaternion = m_physics_model.orientationQuaternion;

            

            returnPackage.entityPackage = vOut;
            returnPackage.modelName = m_model_name;
            returnPackage.propId = m_prop_id;

            return returnPackage;
        }
        public virtual void SnapShotIn(PropPackage aPackage)
        {
            if (aPackage != null && aPackage.propId == m_prop_id && aPackage.modelName == m_model_name)
            {
                m_physics_model.correctiveAngularVelocity = aPackage.entityPackage.correctiveAngularVelocity;
                m_physics_model.correctiveLinearVelocity = aPackage.entityPackage.correctiveLinearVelocity;
                m_physics_model.angularMomentum = aPackage.entityPackage.internalAngularMomentum;
                m_physics_model.angularVelocity = aPackage.entityPackage.internalAngularVelocity;
                m_physics_model.internalAngularMomentum = aPackage.entityPackage.internalAngularMomentum;
                m_physics_model.internalAngularVelocity = aPackage.entityPackage.internalAngularVelocity;
                m_physics_model.internalCenterPosition = aPackage.entityPackage.internalCenterPosition;
                m_physics_model.internalLinearMomentum = aPackage.entityPackage.internalLinearMomentum;
                m_physics_model.internalLinearVelocity = aPackage.entityPackage.internalLinearVelocity;
                m_physics_model.internalOrientationMatrix = aPackage.entityPackage.internalOrientationMatrix;
                m_physics_model.internalOrientationQuaternion = aPackage.entityPackage.internalOrientationQuaternion;
                m_physics_model.linearMomentum = aPackage.entityPackage.internalLinearMomentum;
                m_physics_model.linearVelocity = aPackage.entityPackage.internalLinearVelocity;
                m_physics_model.localSpaceInertiaTensor = aPackage.entityPackage.localSpaceInertiaTensor;
                m_physics_model.localSpaceInertiaTensorInverse = aPackage.entityPackage.localSpaceInertiaTensorInverse;
                m_physics_model.orientationMatrix = aPackage.entityPackage.orientationMatrix;
                m_physics_model.orientationQuaternion = aPackage.entityPackage.orientationQuaternion;
 
            }

        }
    }
}
