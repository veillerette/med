#ifndef __HEAD_AUDIO__
#define __HEAD_AUDIO__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Staff.h"
#include "System.h"
#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>

#ifndef M_PI
#define M_PI 3.141592654
#endif

typedef struct ScoreStaff ScoreStaff;
struct ScoreStaff
{
	Score *score;
	int id_staff;
};

typedef struct Wave Wave;
struct Wave
{
	double (*f)(int x, double freq, int hardwareFreq);
	double freq;
	int volume; /* 0 to 100 */
};

typedef struct Channel Channel;
struct Channel
{
	Wave **waves;
	int n;
	int m;
	int on;
	
	Note *note_playing;
};

typedef struct Mixer Mixer;
struct Mixer
{
	Channel **channels;
	int n;
	int m;
};

typedef struct AudioConfig AudioConfig;
struct AudioConfig
{
	SDL_AudioSpec hardware;
	unsigned long x;
	int globalVolume;
	
	Mixer mixer;
	Score *score;
	int id_step;
	int playing;
	
	SDL_Thread **threads;
	int need_refresh;
	
	int tempo;
};


/*****************************************************
		DEV FUNCTIONS
******************************************************/

ScoreStaff *ScoreStaff_Alloc(Score *score, int id_staff);

void ScoreStaff_Free(ScoreStaff **ss);

Wave *Wave_Alloc(double (*f)(int,double, int), double freq, int volume);

void Wave_Free(Wave **wave);

Channel *Channel_Alloc(void);

void Channel_Free(Channel **chan);

int Channel_Add(Channel *chan, Wave *wave);

int Channel_Delete(Channel *chan, int id);

int Channel_ReRoll(Channel *chan);

void Channel_Enable(Channel *chan);

void Channel_Disable(Channel *chan);

void Mixer_Init(Mixer *mixer);

int Mixer_Add(Mixer *mixer, Channel *chan);

void Mixer_Quit(Mixer *mixer);

AudioConfig *AudioConfig_DevInit(int askFreq, Uint16 format, Uint8 channels, Uint16 samples,
					void (*callback)(void *, Uint8 *, int), void *userdata,
					int globalVolume);

void AudioConfig_Free(AudioConfig **ac);

void MAIN_CALLBACK(void *userdata, Uint8 *stream, int len);

AudioConfig *AudioConfig_Init(void);

void AudioConfig_SetVolume(AudioConfig *ac, int newVolume);

double Note_GetFreq(Note *note);

double Wave_Calc(AudioConfig *ac, Wave *wave);

double Channel_Calc(AudioConfig *ac, Channel *chan);

double Mixer_Calc(AudioConfig *ac, Mixer *mixer);

Channel *Channel_CreateOne(double freq, double (*f)(int, double, int));

void Channel_ChangeOne(Channel *chan, double freq);

Channel *Channel_CreateSimplyOctave(double freq, double (*f)(int, double, int));

void Channel_ChangeOctave(Channel *chan, double freq);

int *Integer_Alloc(int n);

int Tempo_GetMsTempo(void);

/*****************************************************
		WAVE FUNCTIONS
******************************************************/

double sinusoide(int x, double freq, int hardwareFreq);

double carre(int x, double freq, int hardwareFreq);

double carreHarmo(int x, double freq, int hardwareFreq);


/*****************************************************
		PLAYING FUNCTIONS
******************************************************/

int Audio_PlayStep(Step *step, Channel *chan);

int Audio_PlayStaffThread(void *data);

int Audio_KillThreads(void);


/*****************************************************
		MAIN FUNCTIONS
******************************************************/

void Audio_Init(AudioConfig *ac);

void Audio_Quit(void);

void Audio_AssignateScore(Score *score);

void Audio_GoToStep(int id_step);

int Audio_isPlaying(void);

int Audio_isInit(void);

void Audio_Play(void);

void Audio_Pause(void);

void Audio_SetTempo(int newTempo);

int Audio_GetTempo(void);

/*****************************************************
******************************************************/

extern AudioConfig *main_audio;

#endif

