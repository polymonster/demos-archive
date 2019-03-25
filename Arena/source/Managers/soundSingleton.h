#ifndef _SOUND_SINGLETON_H
#define _SOUND_SINGLETON_H

#include "globals.h"

#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"

#pragma comment(lib,"fmod/fmod.lib")


class CSound{

public:

	static void create();
	static CSound* getInstance(){return m_instance;};
	static void destroy();
	void kill();

	void startAtmosphere();
	void endAtmosphere();
	void startMenuMusic();
	
	void play(int id);
	void gunshot(int id);
	void click();


private:

	CSound();
	~CSound(){};

	static CSound *m_instance;
	FSOUND_STREAM* gun[3];
	FSOUND_STREAM* reload;
	FSOUND_STREAM* noAmmo;
	FSOUND_STREAM* atmosphere;

	FSOUND_STREAM* clicks;

	FSOUND_STREAM* walk;
	FSOUND_STREAM* walk2;
	FSOUND_STREAM* jump;

	FSOUND_STREAM* pain[3];
	FSOUND_STREAM* death[3];
	
	FSOUND_STREAM* run[3];


	int m_gunChannel;
	
};


#endif 
