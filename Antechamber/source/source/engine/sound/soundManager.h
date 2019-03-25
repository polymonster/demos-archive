#ifndef _SOUND_MANAGER_H
#define _SOUND_MANAGER_H

#include "fmod.h"
#include "fmod_errors.h"

#pragma comment(lib,"fmod/fmod.lib")

#define MAX_SOUNDS 25

typedef struct
{
	int callers;
	int channelID;

}s_loop_data;

signed char aaaa(FSOUND_STREAM *stream, void *buff, int len, int param);

class CSound
{

public:

	CSound();
	~CSound();

	//sounds
	int load(char *filename,bool loop);
	void play(int id);
	void play(int id,int volume_boost);
	void stop(int id);

	//loops
	int playLoop(int id);
	int playLoop(int id, int volume_boost);
	void stopLoop(int id);

	//streams
	int loadAndPlay(char *filename);
	int loadAndPlay(char *filename,int volume,FSOUND_STREAM **stream);
	void stopStream(FSOUND_STREAM *stream);
	void playMusic(char *filename,int volume,FSOUND_STREAM **stream);
	void stopStream();
	void checkMusicForEnd();

	void cleanUp();

private:

	FSOUND_STREAM  *music;
	FSOUND_STREAM  *dialogue;
	FSOUND_SAMPLE  *soundEffect[MAX_SOUNDS];

	FSOUND_STREAMCALLBACK music_cb;
	FSOUND_STREAMCALLBACK dialogue_cb;

	signed char music_callback(FSOUND_STREAM *stream, void *buff, int len, int param);
	signed char dialogue_callback(FSOUND_STREAM *stream, void *buff, int len, int param);

	s_loop_data		m_looping[MAX_SOUNDS];

	int numberOfSoundEffects;

	bool stream_in_use;
	int m_old_sfx_volume, m_old_music_volume;

	int m_music_len;
	int m_music_start;
	int m_music_id;
};

extern CSound *g_sound_mgr_p;


#endif 
