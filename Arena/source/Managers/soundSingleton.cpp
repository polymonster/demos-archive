#include "soundSingleton.h"

CSound* CSound::m_instance = NULL; 

void CSound::create(){

	if(m_instance == NULL)
	{
		m_instance = new CSound;
	}
}

void CSound::destroy(){

	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;

	}
}

void CSound::kill(){
	
	for(int i = 0; i < 3; i++)
	{
		FSOUND_Stream_Stop( gun[i] );
		FSOUND_Stream_Close( gun[i] );
	}

	for(i = 0; i < 3; i++)
	{
		FSOUND_Stream_Stop( run[i] );
		FSOUND_Stream_Close( run[i] );
	}

	FSOUND_Stream_Stop( reload );
	FSOUND_Stream_Close( reload );

	FSOUND_Stream_Stop( noAmmo );
	FSOUND_Stream_Close( noAmmo );

	FSOUND_Stream_Stop( atmosphere );
	FSOUND_Stream_Close( atmosphere );

	FSOUND_Stream_Stop( clicks );
	FSOUND_Stream_Close( clicks );

	FSOUND_Stream_Stop( walk );
	FSOUND_Stream_Close( walk );

	FSOUND_Stream_Stop( walk2 );
	FSOUND_Stream_Close( walk2 );

	FSOUND_Stream_Stop( jump );
	FSOUND_Stream_Close( jump );

	for(i = 0; i < 3; i++)
	{
		FSOUND_Stream_Stop( pain[i] );
		FSOUND_Stream_Close( pain[i] );
		FSOUND_Stream_Stop( death[i] );
		FSOUND_Stream_Close( death[i] );
	}


};


CSound::CSound()
{
	// initialise fmod, 44000 Hz, 64 channels
	if( FSOUND_Init(44000,64,0) == FALSE )	{
		cout << "[ERROR] Could not initialise fmod\n";
	}

	atmosphere = FSOUND_Stream_Open( "data/sound/threat.mp3" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );

	//FSOUND_Stream_Play(3,atmosphere);
	//FSOUND_SetVolume (3, 70);

	// attempt to open the mp3 file as a stream - use the following option:
	// FSOUND_2D - Tells software (not hardware) based sample not to be included in 3d processing.
	// FSOUND_LOOP_NORMAL - To enable repeat playing
	gun[0] = FSOUND_Stream_Open( "data/sound/machinegun.wav" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );
	gun[1] = FSOUND_Stream_Open( "data/sound/shotgun.wav" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );
	gun[2] = FSOUND_Stream_Open( "data/sound/plasma.wav" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );

	reload = FSOUND_Stream_Open( "data/sound/reload.wav" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );
	noAmmo = FSOUND_Stream_Open( "data/sound/noammo.wav" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );

	clicks = FSOUND_Stream_Open( "data/sound/click.mp3" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );

	walk = FSOUND_Stream_Open( "data/sound/boot1.wav" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );
	walk2 = FSOUND_Stream_Open( "data/sound/boot2.wav" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );

	jump = FSOUND_Stream_Open( "data/sound/jump.wav" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );

	pain[0] = FSOUND_Stream_Open( "data/sound/pain25_1.wav" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );
	pain[1] = FSOUND_Stream_Open( "data/sound/pain50_1.wav" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );
	pain[2] = FSOUND_Stream_Open( "data/sound/pain75_1.wav" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );

	death[0] = FSOUND_Stream_Open( "data/sound/death1.wav" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );
	death[1] = FSOUND_Stream_Open( "data/sound/death2.wav" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );
	death[2] = FSOUND_Stream_Open( "data/sound/death3.wav" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );
	
	run[0] = FSOUND_Stream_Open( "data/sound/speedfinal1.mp3" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );
	run[1] = FSOUND_Stream_Open( "data/sound/speedfinal2.mp3" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );
	run[2] = FSOUND_Stream_Open( "data/sound/speedfinal3.mp3" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );

	FSOUND_Stream_SetLoopCount(gun[0],0);
	FSOUND_Stream_SetLoopCount(gun[1],0);
	FSOUND_Stream_SetLoopCount(gun[2],0);

	FSOUND_Stream_SetLoopCount(run[0],0);
	FSOUND_Stream_SetLoopCount(run[1],0);
	FSOUND_Stream_SetLoopCount(run[2],0);

	FSOUND_Stream_SetLoopCount(clicks,0);
	FSOUND_Stream_SetLoopCount(walk,0);
	FSOUND_Stream_SetLoopCount(walk2,0);
	FSOUND_Stream_SetLoopCount(jump,0);
	FSOUND_Stream_SetLoopCount(reload,0);
	FSOUND_Stream_SetLoopCount(noAmmo,0);

	for(int i = 0; i < 3; i++)	
	{
		FSOUND_Stream_SetLoopCount(death[i],0);
		FSOUND_Stream_SetLoopCount(pain[i],0);
	}

}

void CSound::play(int id){

 if(id == 0) FSOUND_Stream_Play(1,noAmmo);
 if(id == 1) FSOUND_Stream_Play(1,reload);

 if(id == 2) 
 {
	 FSOUND_SetVolume (2, 30);
	 FSOUND_Stream_Play(4,walk);

 }

 if(id == 3) FSOUND_Stream_Play(5,jump);
 if(id == 6) FSOUND_Stream_Play(12,run[0]);
 if(id == 7) FSOUND_Stream_Play(13,run[1]);
 if(id == 8) FSOUND_Stream_Play(14,run[2]);

 if(id == 9) 
 {
	 FSOUND_SetVolume (9, 20);
	 FSOUND_Stream_Play(6,walk2);

 }

 if(id == 10) FSOUND_Stream_Play(10,pain[0]);
 if(id == 11) FSOUND_Stream_Play(11,pain[1]);
 if(id == 12) FSOUND_Stream_Play(12,pain[2]);

 if(id == 20) FSOUND_Stream_Play(11,death[0]);
 if(id == 21) FSOUND_Stream_Play(11,death[1]);
 if(id == 22) FSOUND_Stream_Play(11,death[2]);
}

void CSound::click(){

	//FSOUND_Stream_Stop(clicks);
	FSOUND_Stream_Play(0,clicks);
}

void CSound::gunshot(int id){

	FSOUND_Stream_Stop( gun[id] );
	FSOUND_Stream_Play(0,gun[id]);
}

void  CSound::startAtmosphere(){

	FSOUND_Stream_Stop( atmosphere );
	atmosphere = FSOUND_Stream_Open( "data/sound/grind.mp3" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );
	FSOUND_Stream_Play(3,atmosphere);
	FSOUND_SetVolume (3, 150);
}

void  CSound::startMenuMusic(){

	FSOUND_Stream_Stop( atmosphere );
	atmosphere = FSOUND_Stream_Open( "data/sound/threat.mp3" , FSOUND_2D|FSOUND_LOOP_NORMAL, 0 , 0 );
	FSOUND_Stream_Play(3,atmosphere);
	FSOUND_SetVolume (3, 150);
}

void  CSound::endAtmosphere(){

	FSOUND_Stream_Stop( atmosphere );
	//FSOUND_Stream_Close( atmosphere );
}