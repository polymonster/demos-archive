#ifndef _AUDIO_H_
#define _AUDIO_H_

// Required for forward reference in AudioDevice class
class SoundSample;

// class to handle interface to the audio device
class AudioDevice
{
public:
	AudioDevice(const int Which);
	~AudioDevice();

	int Open(const int which);
	void Close(void);	
	void HandleAudio(void);
	void SetSoundSample(SoundSample * const Sample);	
	int GetFragmentSize(void) const;

protected:
	int GetFreeFragments(void) const;
	
	SoundSample * m_Current;
	int m_AudioFD;
	int m_FragmentSize;
	unsigned char * m_Silence;
};



// class to handle audio sample
class SoundSample 
{  
public:
	SoundSample(char * Filename, AudioDevice * const pAD);
	~SoundSample();

	void Load(char * Filename);
	void Reset();
	void Play(void);
	unsigned char * GetNextFragment();

protected:

	AudioDevice * m_pAD;
	unsigned char * m_Stream;
	long m_Location;
	int m_FragSize;
	long m_NumFragments;
	char m_SampleName[32];
};


#endif