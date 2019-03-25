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
    public class sound
    {
        AudioEngine m_audioEngine;
        WaveBank m_waveBank;
        SoundBank m_soundBank;
        Cue music;

        public sound()
        {
            m_audioEngine = new AudioEngine("Content\\Sound\\Win\\XACT.xgs");
            m_waveBank = new WaveBank(m_audioEngine, "Content\\Sound\\Win\\Wave Bank.xwb");
            m_soundBank = new SoundBank(m_audioEngine, "Content\\Sound\\Win\\Sound Bank.xsb");
        }

        public void update()
        {
            m_audioEngine.Update();

        }

        public void play(String cueName)
        {
            m_soundBank.GetCue(cueName).Play();
        }

        public void playMusic(String cueName)
        {
            music = m_soundBank.GetCue(cueName);
            music.Play();
        }

        public void stop(String cueName)
        {
            music.Stop(AudioStopOptions.AsAuthored);
            //m_soundBank.GetCue(cueName).Stop(AudioStopOptions.Immediate);

            // doesnt seem to work
            //Cue cue = 
            //cue.Stop(AudioStopOptions.AsAuthored);
        }
    }
}
