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
    public class globals
    {
        public enum buttonTypes
        {
            BUTTON_ONETAP = 0,
            BUTTON_TOGGLE,
            BUTTON_LIST,
            BUTTON_IMG_SELECT
        };

        public enum states
        {
            GAME_STATE_MAIN_MENU = 1,
            GAME_STATE_INFO,
            GAME_STATE_LANG,
            GAME_STATE_QUIT,
            GAME_STATE_TANK_SELECT,
            GAME_STATE_GAME_SETUP,
            GAME_STATE_IN_GAME,
            GAME_STATE_FINISH
        };

        public const int MAX_DEVICES = 6;

        // integer 2d vector
        public struct Vector2i
        {
            public int X;
            public int Y;
        };

        public enum deviceTypes
        {
            DEVICE_TYPE_KEYBOARD = 0,
            DEVICE_TYPE_360PAD
        };

        public static int winner = 0;
        public static SpriteFont font;
        public static int[] gameTimes = { 0, 2, 3, 5, 10 };
        public static int[] gamePoints = { 0, 5, 10, 25, 50 };
        public static Level level;
        public static CollisionDetection collisions;
        public static SpriteBatch spriteBatch;
        public static int numberOfcollisions;
        public static Vector2[] recheck = new Vector2[2];
        public static bool secondRecursion;
        public static textOutput text;
        public static states gameState;
        public static states savedState;
        public static sound soundManager;
    }

    public class Main : Microsoft.Xna.Framework.Game
    {
        Texture2D logo;

        GraphicsDeviceManager graphics;
        
        Tank[] tank;
        MainMenu mainMenu;
        MainMenu finishMenu;
        TankSelectMenu tanksMenu;
        SetupMenu setupMenu;
        Texture2D mouseCursor;
        Texture2D infoScreen;
        Texture2D[] playerWin = new Texture2D[4];
        public MenuButton goButton;
        public MenuButton backButton;
        public MenuButton infoButton;

        public Main()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            tank = new Tank[4];
            for (int i = 0; i < 4; i++)
            {
                tank[i] = new Tank();
            }

            goButton = new MenuButton(730, 600-40, globals.buttonTypes.BUTTON_ONETAP, true);
            goButton.showButton(false);

            backButton = new MenuButton( 70, 600-40, globals.buttonTypes.BUTTON_ONETAP, true);
            backButton.showButton(false);

            infoButton = new MenuButton(730, 40, globals.buttonTypes.BUTTON_ONETAP, true);

            globals.collisions = new CollisionDetection();

            globals.level = new Level();

            globals.text = new textOutput();

            globals.soundManager = new sound();
            mainMenu = new MainMenu(402, 412, 402, 540);
            finishMenu = new MainMenu(730, 560, 60, 560);
            tanksMenu = new TankSelectMenu();
            setupMenu = new SetupMenu();

            graphics.PreferredBackBufferHeight = 600;

            graphics.PreferredBackBufferWidth = 800;

            //graphics.IsFullScreen = true;

            graphics.ApplyChanges();

            globals.gameState = globals.states.GAME_STATE_MAIN_MENU;
        }

        protected override void Initialize()
        {
            

            base.Initialize();
        }

        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            globals.spriteBatch = new SpriteBatch(GraphicsDevice);

            mouseCursor = Content.Load<Texture2D>("mouse");
            infoScreen = Content.Load<Texture2D>("infobg");
            playerWin[0] = Content.Load<Texture2D>("player1");
            playerWin[1] = Content.Load<Texture2D>("player2");
            playerWin[2] = Content.Load<Texture2D>("player3");
            playerWin[3] = Content.Load<Texture2D>("player4");

            tank[0].loadTexture(Content, "tank1_base", "tank1_turret");
            tank[1].loadTexture(Content, "tank2_base", "tank1_turret");
            tank[2].loadTexture(Content, "tank3_base", "tank1_turret");
            tank[3].loadTexture(Content, "tank4_base", "tank1_turret");

            mainMenu.loadTexture(Content, "mainbg");
            mainMenu.startButton.loadSprites(Content, "startbutton", "startbuttonm");
            mainMenu.quitButton.loadSprites(Content, "quitbutton", "quitbuttonm");
            finishMenu.loadTexture(Content, "finishbg");
            finishMenu.startButton.loadSprites(Content, "startbutton", "startbuttonm");
            finishMenu.quitButton.loadSprites(Content, "quitbutton", "quitbuttonm");

            tanksMenu.loadTexture(Content);
            tanksMenu.inputDevice[0].loadTexture(Content, "icokeyboard2");
            tanksMenu.inputDevice[1].loadTexture(Content, "icokeyboard1");
            tanksMenu.inputDevice[2].loadTexture(Content, "ico360pad");
            tanksMenu.inputDevice[3].loadTexture(Content, "ico360pad");
            tanksMenu.inputDevice[4].loadTexture(Content, "ico360pad");
            tanksMenu.inputDevice[5].loadTexture(Content, "ico360pad");

            infoButton.loadSprites(Content, "infobutton", "infobuttonovr");
            goButton.loadSprites(Content, "gobutton", "gobuttonovr");
            backButton.loadSprites(Content, "backbutton", "backbuttonovr");

            setupMenu.loadTexture(Content);
            setupMenu.gameButtonLast.loadSprites(Content,"gamemode_lms", "gamemode_lmsovr");
            setupMenu.gameButtonPoints.loadSprites(Content, "gamemode_sdm", "gamemode_sdmovr");
            setupMenu.gameButtonTime.loadSprites(Content, "gamemode_tdm", "gamemode_tdmovr");
            setupMenu.gameButtonMap.loadSprites(Content, "mapselect_title", "mapselect_title");

            globals.text.loadTextures(Content);

            globals.level.loadTextures(Content);

            globals.collisions.loadTexture(Content);

        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if ((GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed) ||
                (Keyboard.GetState().IsKeyDown(Keys.Escape)) )
                this.Exit();

            infoButton.update();
            goButton.update();
            backButton.update();

            globals.soundManager.update();

            switch (globals.gameState)
            {
                case globals.states.GAME_STATE_MAIN_MENU:

                    goButton.showButton(false);
                    backButton.showButton(false);
                    infoButton.showButton(true);
                    mainMenu.startButton.set_pos(402-61, 412-50);

                    mainMenu.update();

                    if (mainMenu.startButton.m_selected)
                    {
                        // reset some game variables
                        mainMenu.startButton.m_selected = false;
                        globals.gameState = globals.states.GAME_STATE_TANK_SELECT;
                        for (int i = 0; i < globals.MAX_DEVICES; i++)
                            tanksMenu.inputDevice[i].m_pos.Y = 0;
                        setupMenu.gameButtonLast.m_selected = false;
                        setupMenu.gameButtonPoints.m_selected = false;
                        setupMenu.gameButtonTime.m_selected = false;
                    }

                    if (mainMenu.quitButton.m_selected)
                    {
                        this.Exit();
                    }

                break;

                case globals.states.GAME_STATE_TANK_SELECT:

                    infoButton.showButton(true);
                    backButton.showButton(true);

                    tanksMenu.update();

                    int assigned = 0;
                    for (int i = 0; i < 5; i++)
                        if (tanksMenu.selected[i])
                            assigned++;
                    goButton.showButton( assigned > 1 );

                    if (goButton.m_selected)
                    {
                        goButton.m_selected = false;
                        globals.gameState = globals.states.GAME_STATE_GAME_SETUP;
                    }

                    if (backButton.m_selected)
                    {
                        backButton.m_selected = false;
                        globals.gameState = globals.states.GAME_STATE_MAIN_MENU;
                    }

                break;

                case globals.states.GAME_STATE_GAME_SETUP:

                    setupMenu.update();
                    setupMenu.gameButtonMap.set_pos(400-40, 510-40);

                    infoButton.showButton(true);
                    backButton.showButton(true);
                    goButton.showButton(false);

                    mainMenu.startButton.update();
                    mainMenu.startButton.set_pos(730-66, 560-30);
                    if (setupMenu.gameButtonLast.m_selected || setupMenu.gameButtonPoints.m_list_value > 0 || setupMenu.gameButtonTime.m_list_value > 0)
                        mainMenu.startButton.showButton(true);
                    else
                        mainMenu.startButton.showButton(false);

                    if (mainMenu.startButton.m_selected)
                    {
                        mainMenu.startButton.m_selected = false;
                        globals.gameState = globals.states.GAME_STATE_IN_GAME;

                        //initialise a new game with all this
                        //register tanks and control schemes, reset scores
                        for (int i = 0; i < 4; i++)
                        {
                            bool found = false;
                            for (int j = 0; j < 6; j++)
                            {
                                if (tanksMenu.inputDevice[j].m_activeDevice && 4 - tanksMenu.inputDevice[j].m_pos.Y == i)
                                {
                                    found = true;
                                    tank[i].init(tanksMenu.inputDevice[j].m_schemeID, i);
                                }
                            }
                            if (!found)
                                tank[i].init(-1, i);
                        }

                        //reset the level
                        globals.level.init
                        (
                            globals.gameTimes[setupMenu.gameButtonTime.m_list_value],
                            globals.gamePoints[setupMenu.gameButtonPoints.m_list_value],
                            setupMenu.gameButtonLast.m_selected,
                            setupMenu.gameButtonMap.m_list_value
                        );

                        //lets go
                        globals.level.startGame(ref tank);

                        globals.soundManager.playMusic("music");
                    }

                    if (backButton.m_selected)
                    {
                        backButton.m_selected = false;
                        globals.gameState = globals.states.GAME_STATE_TANK_SELECT;
                    }

                break;

                case globals.states.GAME_STATE_IN_GAME:

                    

                    globals.level.update(gameTime);
                    backButton.showButton(false);
                    goButton.showButton(false);
                    infoButton.showButton(false);

                    for (int i = 0; i < 4; i++)
                    {
                        if (tank[i].isRegistered())
                        {
                            tank[i].update(gameTime);

                            globals.level.checkCollision(ref tank[i]);

                            for (int j = 0; j < 4; j++)
                            {
                                if (i != j)
                                {
                                    if (tank[i].isAlive() && tank[j].isAlive() && tank[j].isRegistered())
                                    {
                                        globals.collisions.tankVsTank(ref tank[i], ref tank[j]);
                                    }
                                }
                            }

                        }
                    }

                    globals.level.awardPoints(ref tank);

                    if (!globals.level.isRunning())
                    {

                        globals.gameState = globals.states.GAME_STATE_FINISH;
                        globals.soundManager.stop("music");
                    }

                    break;

                case globals.states.GAME_STATE_FINISH:
                    goButton.showButton(false);
                    backButton.showButton(false);
                    infoButton.showButton(false);

                    

                    finishMenu.update();
                    finishMenu.startButton.showButton(true);
                    finishMenu.quitButton.showButton(true);

                    setupMenu.gameButtonMap.update();

                    if (finishMenu.startButton.m_selected)
                    {
                        finishMenu.startButton.m_selected = false;
                        globals.gameState = globals.states.GAME_STATE_IN_GAME;

                        //initialise a new game with all this
                        //register tanks and control schemes, reset scores
                        for (int i = 0; i < 4; i++)
                        {
                            bool found = false;
                            for (int j = 0; j < 6; j++)
                            {
                                if (tanksMenu.inputDevice[j].m_activeDevice && 4 - tanksMenu.inputDevice[j].m_pos.Y == i)
                                {
                                    found = true;
                                    tank[i].init(tanksMenu.inputDevice[j].m_schemeID, i);
                                }
                            }
                            if (!found)
                                tank[i].init(-1, i);
                        }

                        //reset the level
                        globals.level.init
                        (
                            globals.gameTimes[setupMenu.gameButtonTime.m_list_value],
                            globals.gamePoints[setupMenu.gameButtonPoints.m_list_value],
                            setupMenu.gameButtonLast.m_selected,
                            setupMenu.gameButtonMap.m_list_value
                        );

                        //lets go
                        globals.level.startGame(ref tank);

                        globals.soundManager.play("music");
                    }

                    if (finishMenu.quitButton.m_selected)
                    {
                        finishMenu.quitButton.m_selected = false;
                        globals.gameState = globals.states.GAME_STATE_MAIN_MENU;
                    }
                    break;

                case globals.states.GAME_STATE_INFO:
                    infoButton.showButton(false);
                    if (backButton.m_selected)
                    {
                        backButton.m_selected = false;
                        globals.gameState = globals.savedState;
                    }
                    break;

            }


            if (infoButton.m_selected)
            {
                infoButton.m_selected = false;
                globals.savedState = globals.gameState;
                globals.gameState = globals.states.GAME_STATE_INFO;
            }

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            graphics.GraphicsDevice.Clear(Color.Black);

            switch (globals.gameState)
            {
                case globals.states.GAME_STATE_MAIN_MENU:

                    mainMenu.draw();

                break;

                case globals.states.GAME_STATE_TANK_SELECT:

                    tanksMenu.draw();

                break;

                case globals.states.GAME_STATE_GAME_SETUP:

                    setupMenu.draw();
                    mainMenu.startButton.draw();

                break;

                case globals.states.GAME_STATE_INFO:
                    globals.spriteBatch.Begin(SpriteBlendMode.AlphaBlend);
                    Vector2 pos; pos.X = 0; pos.Y = 0;
                    globals.spriteBatch.Draw(infoScreen, pos, Color.White);
                    globals.spriteBatch.End();
                    backButton.showButton(true);
                    break;

                case globals.states.GAME_STATE_IN_GAME:

                    globals.level.drawBottom();

                    for (int i = 0; i < 4; i++)
                    {
                        if (tank[i].isRegistered())
                        {
                            tank[i].drawScore();
                        }
                    }

                    globals.level.drawTop();

                    for (int i = 0; i < 4; i++)
                    {
                        if (tank[i].isRegistered())
                        {
                            tank[i].draw();
                        }
                    }

                    globals.level.drawTopMost();

                break;
            }

            if (globals.gameState != globals.states.GAME_STATE_IN_GAME)
            {
                Vector2 mousePos;
                mousePos.X = Mouse.GetState().X;
                mousePos.Y = Mouse.GetState().Y;

                goButton.draw();
                backButton.draw();
                infoButton.draw();

                if (globals.gameState == globals.states.GAME_STATE_FINISH)
                {
                    finishMenu.draw();
                    setupMenu.gameButtonMap.set_pos(250, 420);
                    setupMenu.gameButtonMap.draw();

                    globals.spriteBatch.Begin(SpriteBlendMode.AlphaBlend);

                    Vector2 logoPos;
                    logoPos.X = 550;
                    logoPos.Y = 250;
                    globals.spriteBatch.Draw(playerWin[globals.winner],logoPos,Color.White);
                }
                else
                {
                    globals.spriteBatch.Begin(SpriteBlendMode.AlphaBlend);
                }

                //if (globals.gameState != globals.states.GAME_STATE_MAIN_MENU)
                //{
                //    Vector2 logoPos;
                //    logoPos.X = 20;
                //    logoPos.Y = 20;

                //    globals.spriteBatch.Draw
                //    (
                //        mainMenu.logo,
                //        logoPos,
                //        null,
                //        Color.White,
                //        0,
                //        Vector2.Zero,
                //        0.5f,
                //        SpriteEffects.None,
                //        0
                //    );
                //}

                // mouse pointer?
                globals.spriteBatch.Draw(mouseCursor, mousePos, Color.White);

                globals.spriteBatch.End();

            }

            base.Draw(gameTime);

        }
    }
}
