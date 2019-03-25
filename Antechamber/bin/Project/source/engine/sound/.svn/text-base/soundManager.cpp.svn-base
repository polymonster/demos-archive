#include "soundManager.h"
#include "d3dapp.h"

CSound *g_sound_mgr_p;

CSound::CSound()
{
	// initialise fmod, 44000 Hz, 64 channels
	if( FSOUND_Init(44000,64,0) == FALSE )	
	{
		//error handling
	}

	numberOfSoundEffects = 0;
	stream_in_use = false;

	m_old_sfx_volume = g_sfx_volume;

	music = NULL;
	dialogue = NULL;
}

CSound::~CSound()
{
	cleanUp();
}

void CSound::cleanUp()
{
	for(int i = 0; i < numberOfSoundEffects; i++)
	{
		//clean up here
	}

	FSOUND_Close();
};

int  CSound::loadAndPlay(char *filename)
{
	FSOUND_STREAM  *m = FSOUND_Stream_Open( filename , FSOUND_2D|FSOUND_LOOP_OFF, 0 , 0 );
	FSOUND_Stream_Play(0,m);

	return -1;
}

int CSound::loadAndPlay(char *filename,int volume,FSOUND_STREAM **stream)
{
	m_old_sfx_volume = g_sfx_volume;
	g_sfx_volume = m_old_sfx_volume - 18;
	FSOUND_SetVolume (m_music_id, g_music_volume - 25);

	dialogue = FSOUND_Stream_Open( filename , FSOUND_2D|FSOUND_LOOP_OFF, 0 , 0 );	
	stream = &dialogue;

	int id = FSOUND_Stream_Play(FSOUND_FREE,dialogue);
	FSOUND_SetVolume (id, volume);

	return FSOUND_Stream_GetLengthMs(dialogue);
}

void CSound::playMusic(char *filename,int volume,FSOUND_STREAM **stream)
{
	int r = rand()%5 + 1;

	char number[32];
	strcpy(number,"\0");

	itoa(r,number,10);

	char finalName[64];

	strcpy(finalName,filename);
	strcat(finalName,number);
	strcat(finalName,".mp3");

	music = FSOUND_Stream_Open( finalName , FSOUND_2D|FSOUND_LOOP_OFF, 0 , 0 );	
	stream = &dialogue;

	m_music_id = FSOUND_Stream_Play(FSOUND_FREE,music);	
	FSOUND_SetVolume (m_music_id, volume);

	m_music_len = FSOUND_Stream_GetLengthMs(music);

	stream = &music;
}

int CSound::load(char *filename, bool loop)
{
	soundEffect[numberOfSoundEffects] = FSOUND_Sample_Load(FSOUND_FREE, filename, FSOUND_2D|FSOUND_LOOP_OFF, 0, 0);

	if(!soundEffect[numberOfSoundEffects])
	{
		//error handling
	}

	m_looping[numberOfSoundEffects].channelID = -1;
	m_looping[numberOfSoundEffects].callers = 0;

	return numberOfSoundEffects++;
}

void CSound::play(int id,int volume_boost)
{
	int channelID = FSOUND_PlaySound(FSOUND_FREE,soundEffect[id]);

	FSOUND_SetVolume(channelID, g_sfx_volume + volume_boost);
}

void CSound::play(int id)
{
	int channelID = FSOUND_PlaySound(FSOUND_FREE,soundEffect[id]);

	FSOUND_SetVolume(channelID, g_sfx_volume);
}

int CSound::playLoop(int id)
{
	if(m_looping[id].callers == 0)
	{
		m_looping[id].channelID = FSOUND_PlaySound(FSOUND_FREE,soundEffect[id]);

		FSOUND_SetVolume(m_looping[id].channelID, g_sfx_volume + 15);
		FSOUND_SetLoopMode(m_looping[id].channelID, FSOUND_LOOP_NORMAL);

		m_looping[id].callers++;

		return m_looping[id].channelID;
	}

	m_looping[id].callers++;

	return m_looping[id].channelID;
}

int CSound::playLoop(int id, int volume_boost)
{
	if(m_looping[id].callers == 0)
	{
		m_looping[id].channelID = FSOUND_PlaySound(FSOUND_FREE,soundEffect[id]);

		FSOUND_SetVolume(m_looping[id].channelID, g_sfx_volume + volume_boost);
		FSOUND_SetLoopMode(m_looping[id].channelID, FSOUND_LOOP_NORMAL);

		m_looping[id].callers++;

		return m_looping[id].channelID;
	}

	m_looping[id].callers++;

	return m_looping[id].channelID;
}

void CSound::stopLoop(int id)
{
	m_looping[id].callers--;

	if(m_looping[id].callers == 0)
	{
		FSOUND_SetLoopMode(m_looping[id].channelID, FSOUND_LOOP_OFF);
	}
}

void CSound::stopStream(FSOUND_STREAM *stream)
{ 
	g_sfx_volume = m_old_sfx_volume;

	FSOUND_SetVolume (m_music_id, g_music_volume);

	FSOUND_Stream_Stop(dialogue);
}

void CSound::stopStream()
{ 
	FSOUND_Stream_Stop(dialogue);
}

void CSound::checkMusicForEnd()
{
	m_music_len -= g_scene_p->m_game_time * 1000;

	if(m_music_len <= 0)
	{
		playMusic("sound/music/musics",g_music_volume,&music);
	}
}

