using System;
using System.IO;
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
    public class Level
    {
        int max_projectiles;
        int max_tank_tracks;
        int max_particles;

        Projectile[] projectiles;
        TankTracks[] tankTracks;
        Particles[] particles;
        Texture2D projectileSprite;


        Texture2D blockSprite;
        Texture2D[] backgroundSprite = new Texture2D[5];
        Texture2D trackSprite;
        Texture2D smokeSprite;
        Texture2D explosionSprite;

        String[] mapString = { "map1.txt", "map2.txt", "map3.txt", "map4.txt", "map5.txt" };
        int[,] m_grid;
        Vector2 m_tileSize;
        Vector2 m_gridDimensions;
        Vector2[] m_spawnPoints;

        Vector2[] m_tankPositions;
        bool[] m_registeredPlayer;

        bool lastManStanding;
        bool useTimer;
        float timer;
        float minutes;
        int m_activeLevelId;

        int killLimit;

        int[] pointsToAward;

        bool gameRunning;

        public bool isRunning() { return gameRunning; }

        public void init(int timeLimit,int setKillLimit, bool lastMan, int levelId)
        {
            m_activeLevelId = levelId;
            max_projectiles = 20;
            max_tank_tracks = 200;
            max_particles = 100;

            lastManStanding = lastMan;
            useTimer = true;
            if(timeLimit <= 0) useTimer = false;

            timer = 0;
            minutes = timeLimit;
            killLimit = setKillLimit;

            projectiles = new Projectile[max_projectiles];

            for (int i = 0; i < max_projectiles; i++)
            {
                projectiles[i] = new Projectile(Vector2.Zero, Vector2.Zero, 0);
            }

            tankTracks = new TankTracks[max_tank_tracks]; 

            for (int i = 0; i < max_tank_tracks; i++)
            {
                tankTracks[i] = new TankTracks();
            }

            particles = new Particles[max_particles];

            for (int i = 0; i < max_particles; i++)
            {
                particles[i] = new Particles();
            }

            pointsToAward = new int[4];

            m_spawnPoints = new Vector2[4];
            
            m_gridDimensions.X = 25;
            m_gridDimensions.Y = 20;

            m_grid = new int[(int) m_gridDimensions.X, (int) m_gridDimensions.Y];
            m_tileSize.X = 32;
            m_tileSize.Y = 30;

            FileStream fin = new FileStream(mapString[levelId], FileMode.Open, FileAccess.Read, FileShare.Read);
            StreamReader stream = new StreamReader(fin);

            int m_spawnCounter = 0;

            for (int i = 0; i < m_gridDimensions.Y; i++)
            {
                for (int j = 0; j < m_gridDimensions.X; j++)
                {
                    int tile = 0;

                    do
                    {
                        tile = (int)stream.Read();

                    } while (tile == 10 || tile == 13);

                    
                    switch(tile)
                    {
                        case 50:

                            //Spawn Point
                            m_grid[j, i] = 0;

                            m_spawnPoints[m_spawnCounter].X = j;
                            m_spawnPoints[m_spawnCounter].Y = i;

                            m_spawnCounter++;

                        break;

                        case 49:

                            //Block
                            m_grid[j, i] = 1;

                        break;

                        default:

                            //Nothing
                            m_grid[j, i] = 0;

                        break;
                    }     
                }
            }
        }

        public void loadTextures(ContentManager content)
        {
            projectileSprite = content.Load<Texture2D>("shell_1");
            blockSprite = content.Load<Texture2D>("bloc_tile");
            backgroundSprite[0] = content.Load<Texture2D>("background");
            backgroundSprite[1] = content.Load<Texture2D>("background2");
            backgroundSprite[2] = content.Load<Texture2D>("background3");
            backgroundSprite[3] = content.Load<Texture2D>("background4");
            backgroundSprite[4] = content.Load<Texture2D>("background5");
            trackSprite = content.Load<Texture2D>("tank_tracks");
            smokeSprite = content.Load<Texture2D>("smoke");
            explosionSprite = content.Load<Texture2D>("explosion");
        }

        public void startGame(ref Tank[] player)
        {
            m_registeredPlayer = new bool[4];
            m_tankPositions = new Vector2[4];

            for(int i = 0; i < 4; i++)
            {
                if(player[i].isRegistered())
                {
                   Vector2 spawnPos;

                   spawnPos.X = ((m_spawnPoints[i].X * m_tileSize.X) + (m_tileSize.X * 0.5f));
                   spawnPos.Y = ((m_spawnPoints[i].Y * m_tileSize.Y) + (m_tileSize.Y * 0.5f));

                    player[i].spawn(spawnPos);

                    m_registeredPlayer[i] = true;

                    player[i].dontRespawn(!lastManStanding);
                }
                
            }



            gameRunning = true;
        }

        public double magnitude(Vector2 v)
        {
            return Math.Sqrt((v.X * v.X) + (v.Y * v.Y));
        }

        public Vector2 getSpawnPos()
        {
            float[] d = new float[4];

            for (int i = 0; i < 4; i++)
            {
                Vector2 spawnPos;
                spawnPos.X = ((m_spawnPoints[i].X * m_tileSize.X) + (m_tileSize.X * 0.5f));
                spawnPos.Y = ((m_spawnPoints[i].Y * m_tileSize.Y) + (m_tileSize.Y * 0.5f));

                
                float d2;
                int counter = 0;

                for (int j = 0; j < 4; j++)
                {
                    if (m_registeredPlayer[j])
                    {
                        if(counter == 0)
                        {
                            d[i] = (float)magnitude(spawnPos - m_tankPositions[j]);
                        }
                        else
                        {
                            d2 = (float)magnitude(spawnPos - m_tankPositions[j]);
                            if (d2 < d[i]) d[i] = d2;
                        }

                        counter++;
                    }
                }
            }

            float furthest = d[0];
            int furthestID = 0;

            for (int i = 1; i < 4; i++)
            {
                if (d[i] > furthest)
                {
                    furthest = d[i];
                    furthestID = i;
                }
            }

            Vector2 bestSpawnPos;
            bestSpawnPos.X = ((m_spawnPoints[furthestID].X * m_tileSize.X) + (m_tileSize.X * 0.5f));
            bestSpawnPos.Y = ((m_spawnPoints[furthestID].Y * m_tileSize.Y) + (m_tileSize.Y * 0.5f));

            return bestSpawnPos;
        }

        public void addNewProjectile(Vector2 position, Vector2 dir, float rotation, float speed, int ownerID)
        {
            for (int i = 0; i < max_projectiles; i++)
            {
                if (!projectiles[i].isActive())
                {

                    for (int j = 0; j < max_particles; j++)
                    {
                        if (!particles[j].isActive())
                        {
                            particles[j].spawn(position, 0.25f, 0.6f,0);
                            j = max_particles;
                        }
                    }

                    globals.soundManager.play("shell");
                    projectiles[i].spawn(position, dir, rotation, speed, ownerID);
                    i = max_projectiles;
                }
            }
        }

        public void awardPoints(ref Tank[] tank)
        {

            if (minutes < 0)
            {
                int highscore = 0;
                for (int i = 0; i < 4; i++)
                    highscore = highscore < tank[i].getScore() ? tank[i].getScore() : highscore;
                for (int i = 0; i < 4; i++)
                    if (tank[i].getScore() == highscore)
                        globals.winner = i;
                gameRunning = false;
            }

            int aliveCount = 0;
            for(int i = 0; i < 4; i++)
            {
                if (m_registeredPlayer[i])
                {
                    tank[i].addPoints(pointsToAward[tank[i].getID()]);
                    pointsToAward[tank[i].getID()] = 0;

                    if (lastManStanding)
                    {
                        if (tank[i].isAlive()) aliveCount++;
                    }
                    else
                    {
                        if (killLimit > 0)
                        {
                            if (tank[i].getScore() >= killLimit)
                            {
                                globals.winner = i;
                                gameRunning = false;
                            }
                        }
                    }
                }
            }

            if (lastManStanding && aliveCount <= 1)
            {
                for (int i = 0; i < 4; i++)
                    if (tank[i].isAlive() && tank[i].isRegistered())
                        globals.winner = i;
                 gameRunning = false;
            }
        }

        public void checkCollision(ref Tank tank)
        {
            Vector2 trackPos = Vector2.Zero;
            float trackRot = 0;

            trackPos.X = 200;
            trackPos.Y = 200;

            if (tank.spawnTracks(ref trackPos, ref trackRot))
            {
                for (int i = 0; i < max_tank_tracks; i++)
                {
                    if (!tankTracks[i].isActive())
                    {
                        tankTracks[i].spawn(trackPos, trackRot);
                        i = max_tank_tracks;
                    }
                }
            }

            tankVsLevel(ref tank);
            tankVsProjectiles(ref tank);
        }

        public bool isCollidable(int i, int j)
        {
            if (m_grid[i, j] == 1) return true;

            return false;
        }

        public void projectileVsLevel(ref Projectile checkProj)
        {
            for (int i = (int)m_gridDimensions.X - 1; i >= 0; i--)
            {
                for (int j = (int)m_gridDimensions.Y - 1; j >= 0; j--)
                {
                    if (isCollidable(i, j))
                    {
                        Vector2 aabbPos;
                        Vector2 boundries;

                        aabbPos.X = i * m_tileSize.X;
                        aabbPos.Y = j * m_tileSize.Y;

                        boundries.X = m_tileSize.X;
                        boundries.Y = m_tileSize.Y;

                        if(globals.collisions.projectileVsLevel(ref checkProj, aabbPos, boundries))
                        {
                            globals.soundManager.play("shot");

                            checkProj.kill();

                            for (int k = 0; k < max_particles; k++)
                            {
                                if (!particles[k].isActive())
                                {
                                    particles[k].spawn(checkProj.getPos(), 0.5f, 1, 1);
                                    k = max_particles;
                                }
                            }
                        }
                    }
 
                }
            }

        }

        public void tankVsLevel(ref Tank tank)
        {
            globals.numberOfcollisions = 0;
            globals.secondRecursion = false;

            for (int i = (int)m_gridDimensions.X - 1; i >= 0 ; i--)
            {
                for (int j = (int)m_gridDimensions.Y - 1; j >= 0; j--)
                {
                    if (isCollidable(i,j))
                    {
                        Vector2 aabbPos;
                        Vector2 boundries;

                        aabbPos.X = i * m_tileSize.X;
                        aabbPos.Y = j * m_tileSize.Y;

                        boundries.X = m_tileSize.X;
                        boundries.Y = m_tileSize.Y;

                        bool [] check = new bool[4];
                        int [] index = new int[4];

                        for(int k = 0; k < 4; k++)
                            check[k] = true;

                        //check if top is shared
                        index[0] = j - 1;

                        if (index[0] < 0)
                        {
                            check[0] = false;
                        }
                        else
                        {
                            if (isCollidable(i, index[0])) check[0] = false;
                        }

                        //check right
                        index[1] = i + 1;

                        if (index[1] < m_gridDimensions.X)
                        {
                            if (isCollidable(index[1], j)) check[1] = false;
                        }

                        //check bottom
                        index[2] = j + 1;

                        if (index[2] < m_gridDimensions.Y)
                        {
                            if (isCollidable(i, index[2])) check[2] = false;
                        }

                        //check left
                        index[3] = i - 1;

                        if (index[3] < 0)
                        {
                            check[3] = false;
                        }
                        else
                        {
                            if (isCollidable(index[3], j)) check[3] = false;
                        }

                        if (globals.collisions.tankVsLevel(ref tank, aabbPos, boundries, check))
                        {
                            globals.recheck[globals.numberOfcollisions].X = i;
                            globals.recheck[globals.numberOfcollisions].Y = j;
                            globals.numberOfcollisions++;
                        }
                    }

                    
                        if (globals.numberOfcollisions > 1)
                        {
                            globals.secondRecursion = true;

                            bool[] check = new bool[4];
                            int[] index = new int[4];

                            for (int k = 0; k < 4; k++)
                                check[k] = true;

                            Vector2 aabbPos;
                            Vector2 boundries;

                            aabbPos.X = globals.recheck[1].X * m_tileSize.X;
                            aabbPos.Y = globals.recheck[1].Y * m_tileSize.Y;

                            boundries.X = m_tileSize.X;
                            boundries.Y = m_tileSize.Y;

                            globals.collisions.tankVsLevel(ref tank, aabbPos, boundries, check);
                        }
                }
            }

            globals.collisions.tankVsLine(ref tank, new Vector2(0, 0), new Vector2(0, 600));
            globals.collisions.tankVsLine(ref tank, new Vector2(0, 600), new Vector2(800, 600));
            globals.collisions.tankVsLine(ref tank, new Vector2(800, 600), new Vector2(800, 0));
            globals.collisions.tankVsLine(ref tank, new Vector2(800, 0), new Vector2(0, 0));

            int id = tank.getID();
            m_tankPositions[id] = tank.getPos();

        }

        public void tankVsProjectiles(ref Tank tank)
        {
            if(tank.isAlive())
            {
                for(int i = 0; i < max_projectiles; i++)
                {
                    if (projectiles[i].isActive())
                    {
                        if(globals.collisions.tankVsProjectile(ref tank, ref projectiles[i]))
                        {
                            globals.soundManager.play("explode2");
                            globals.soundManager.play("explode");

                            pointsToAward[projectiles[i].getOwner()]++;

                            for (int j = 0; j < max_particles; j++)
                            {
                                if (!particles[j].isActive())
                                {
                                    particles[j].spawn(tank.getPos(), 2, 3,1);
                                    j = max_particles;
                                }
                            }

                            projectiles[i].kill();
                            tank.kill();
                        }
                    }
                }
            }
        }

        public void updateProjectiles()
        {
            for (int i = 0; i < max_projectiles; i++)
            {
                if (projectiles[i].isActive()) projectiles[i].update();
            }
        }

        public void updateTracks(GameTime gameTime)
        {
            for (int i = 0; i < max_tank_tracks; i++)
            {
                if (tankTracks[i].isActive()) tankTracks[i].update(gameTime);
            }
        }

        public void updateParticles(GameTime gameTime)
        {
            for (int i = 0; i < max_particles; i++)
            {
                if (particles[i].isActive()) particles[i].update(gameTime);
            }
        }

        public void update(GameTime gameTime)
        {
            updateProjectiles();

            updateTracks(gameTime);

            updateParticles(gameTime);

            if (useTimer)
            {
                timer -= (float)gameTime.ElapsedGameTime.TotalSeconds;

                if (timer < 0)
                {
                    timer = 59;
                    minutes--;
                }
            }

            for (int i = 0; i < max_projectiles; i++)
            {
                if(projectiles[i].isActive()) projectileVsLevel(ref projectiles[i]);
            }
        }

        public void drawProjectiles()
        {
            for (int i = 0; i < max_projectiles; i++)
            {
                if (projectiles[i].isActive()) projectiles[i].draw(projectileSprite);
            }
        }

        public void drawTile(int i, int j)
        {
            Vector2 pos;
            pos.X = (float)i * m_tileSize.X;
            pos.Y = (float)j * m_tileSize.Y;

            globals.spriteBatch.Draw
             (
                     blockSprite,
                     pos,
                     null,
                     Color.White,
                     0,
                     Vector2.Zero,
                     1.0f,
                     SpriteEffects.None,
                     0
             );
        }

        public void drawBottom()
        {
            globals.spriteBatch.Begin(SpriteBlendMode.AlphaBlend);

              globals.spriteBatch.Draw
              (
                      backgroundSprite[m_activeLevelId],
                      Vector2.Zero,
                      null,
                      Color.White,
                      0,
                      Vector2.Zero,
                      1.0f,
                      SpriteEffects.None,
                      0
              );


            for (int i = 0; i < m_gridDimensions.X; i++)
            {
                for (int j = 0; j < m_gridDimensions.Y; j++)
                {
                    if(isCollidable(i,j)) drawTile(i, j);
                }
            }

            globals.spriteBatch.End();
        }

        public void drawTop()
        {
            globals.spriteBatch.Begin(SpriteBlendMode.AlphaBlend);

            if (useTimer)
            {
                int m = (int)minutes;
                int s = (int)timer;

                String mins = m.ToString();
                String secs = s.ToString();

                if (minutes < 10)
                {
                    mins = "0" + m.ToString();
                }

                if (timer < 10)
                {
                    secs = "0" + s.ToString();
                }

                globals.text.outputString(mins, 1, new Vector2(354, 62), new Color(0, 0, 0, 90), 1.1f);
                globals.text.outputString(secs, 1, new Vector2(454, 62), new Color(0, 0, 0, 90), 1.1f);

                globals.text.outputString(mins, 1, new Vector2(350, 60), new Color(255, 255, 255, 200),1.0f);
                globals.text.outputString(secs, 1, new Vector2(450, 60), new Color(255, 255, 255, 200),1.0f);
            }

            drawProjectiles();

            for (int i = 0; i < max_tank_tracks; i++)
            {
                tankTracks[i].draw(trackSprite);
            }

            globals.spriteBatch.End();
        }

        public void drawTopMost()
        {
            globals.spriteBatch.Begin(SpriteBlendMode.AlphaBlend);

            for (int i = 0; i < max_particles; i++)
            {
                particles[i].draw(smokeSprite,explosionSprite);
            }

            globals.spriteBatch.End();
        }
    }
}
