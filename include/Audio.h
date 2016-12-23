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

typedef enum {
	AUDIO_NONE,
	AUDIO_INIT,
	AUDIO_QUIT,
	AUDIO_ERR
	} Audio_Status;

typedef struct AudioConfig AudioConfig;
struct AudioConfig
{
	double (*f)(int x, double freq);
	double freq;
	int volume;
};

typedef struct AudioMain AudioMain;
struct AudioMain
{
	SDL_AudioSpec *hardware;
	Audio_Status status;
	AudioConfig config;
	unsigned int progress;
};

void Audio_SetConfig(double (*f)(int x, double freq), double freq, int volume);

int Audio_Quit(void);

void Audio_Play(void);

void Audio_Pause(void);

int Audio_Init(void);

void PlaySeconds(unsigned int n, int freq);

double GetFreqFromId(int id);

void TEST(void);

int ThreadAudioStaff(void *data);

int PlayStaff(Staff *staff);

int PlayStep(Step *step);

extern AudioMain *main_audio;

#endif
