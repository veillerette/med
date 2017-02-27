#ifndef __HEAD_FLUID__
#define __HEAD_FLUID__

#include "System.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "Staff.h"
#include "System.h"
#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <math.h>
#include <dlfcn.h>

#ifndef M_PI
#define M_PI 3.141592654
#endif

#define BASE_DO 264

#define SOUNDFONT_FILE "media/gu.sf2"
#define LIBFLUID_PATH "media/libfluidsynth.so"

typedef struct FluidSettings FluidSettings;
struct FluidSettings
{
	void *      	settings;
	void *         	synth;
	void *  	adriver;
	int             soundfont;
};

typedef struct PlayingNotes PlayingNotes;
struct PlayingNotes
{
	Note **tab;
	int n;
	int m;
};

typedef struct FluidAudioConfig FluidAudioConfig;
struct FluidAudioConfig
{
	int globalVolume;
	int maxVolume;
	Score *score;			/*!< Partition */
	int id_step;			/*!< Position du curseur de lecture */
	int playing;			/*!< Boolean, 1 = joue, 0 = ne joue pas */
	int need_refresh;		/*!< Demande de rafraichissement de l'écran */
	FluidSettings *settings;
	SDL_Thread **threads;
	double tabFreq[12];
	
	
	PlayingNotes *tabplaying;
	void *libfluid;
};

extern void (*fluid_noteon)();
extern void *(*new_fluid_settings)();
extern void *(*new_fluid_synth)();
extern void (*fluid_settings_setstr)();
extern void *(*new_fluid_audio_driver)();
extern int (*fluid_synth_sfload)();
extern void (*fluid_synth_noteoff)();
extern void (*delete_fluid_audio_driver)();
extern void (*delete_fluid_synth)();
extern void (*delete_fluid_settings)();
extern void (*fluid_synth_program_change)();

int Audio_PlayStep(Step *step);

int Audio_PlayStaffThread(void *data);

int Audio_KillThreads(void);



int Playing_isNote(Note *note);

int Playing_RemoveNote(Note *note);

int Playing_AddNote(Note *note);

void Audio_StopPlayingNotes(void);

/*****************************************************
		MAIN FUNCTIONS
******************************************************/

int Audio_Init(FluidAudioConfig *ac);

void Audio_Quit(void);

void Audio_AssignateScore(Score *score);

void Audio_GoToStep(int id_step);

int Audio_isPlaying(void);

int Audio_isInit(void);

int Audio_Play(void);

void Audio_Pause(void);

void Audio_SetTempo(int newTempo);

int Audio_GetTempo(void);

void Audio_SetVolume(int newVolume);

int Audio_GetVolume(void);

double Audio_GetFracVolume(void);

int Audio_ChangeFunction(double (*f)(int, double, int));

FluidAudioConfig *AudioConfig_Init(void);




double sinusoide(int x, double freq, int hardwareFreq);
double fcarre(int x, double freq, int hardwareFreq);
double carreHarmo(int x, double freq, int hardwareFreq);
double mixSinCarre(int x, double freq, int hardwareFreq);





extern FluidAudioConfig *main_audio;


#endif