#include "../include/Fluid.h"

FluidAudioConfig *main_audio = NULL;

FluidSettings *FluidSettings_Alloc(void)
{
	FluidSettings *temp = (FluidSettings *)malloc(sizeof(FluidSettings));
	memtest(temp);
	
	temp->settings = new_fluid_settings();
	temp->synth = new_fluid_synth(temp->settings);
	fluid_settings_setstr(temp->settings, "audio.driver", "alsa");
	fluid_settings_setstr(temp->settings, "synth.reverb.active", "yes");
	temp->adriver = new_fluid_audio_driver(temp->settings, temp->synth);
	temp->soundfont = fluid_synth_sfload(temp->synth, SOUNDFONT_FILE, 1);

	return temp;
}

void FluidSettings_Free(FluidSettings **fs)
{
	if(*fs != NULL)
	{
		delete_fluid_audio_driver((*fs)->adriver);
		delete_fluid_synth((*fs)->synth);
		delete_fluid_settings((*fs)->settings);
		free(*fs);
		*fs = NULL;
	}
}

PlayingNotes *PlayingNotes_Init(void)
{
	PlayingNotes *temp = (PlayingNotes *)malloc(sizeof(PlayingNotes));
	memtest(temp);
	
	temp->tab = (Note **)malloc(sizeof(Note *) * 5);
	memtest(temp->tab);
	temp->n = 0;
	temp->m = 5;
	
	return temp;
}

FluidAudioConfig *FluidAudio_Alloc(void)
{
	FluidAudioConfig *temp = (FluidAudioConfig *)malloc(sizeof(FluidAudioConfig));
	memtest(temp);
	
	temp->globalVolume = 50;
	temp->maxVolume = 100;
	temp->score = NULL;
	temp->id_step = 0;
	temp->playing = 0;
	temp->need_refresh = 0;
	temp->settings = FluidSettings_Alloc();
	temp->threads = NULL;
	temp->tabplaying = PlayingNotes_Init();
	
	return temp;
}



int Playing_isNote(Note *note)
{
	PlayingNotes *pn = NULL;
	int i;
	
	if(NULL == main_audio)
		return 0;
	
	pn = main_audio->tabplaying;
	
	for(i = 0; i < pn->n; i++)
		if(pn->tab[i] == note)
			return 1;
	return 0;
}

int Playing_RemoveNote(Note *note)
{
	PlayingNotes *pn = NULL;
	int i,j;
	
	if(NULL == main_audio)
		return 0;
	
	if(!Playing_isNote(note))
		return 1;
	
	pn = main_audio->tabplaying;
	
	for(i = 0; i < pn->n; i++)
	{
		if(pn->tab[i] == note)
		{
			for(j = i; j < pn->n - 1; j++)
				pn->tab[j] = pn->tab[j+1];
		}
	}
	pn->n--;
	return 1;
}

int Playing_AddNote(Note *note)
{
	PlayingNotes *pn = NULL; 
	if(NULL == main_audio)
		return 0;
	
	pn = main_audio->tabplaying;
	
	if(Playing_isNote(note))
		return 1;
	
	if(pn->n == pn->m)
	{
		pn->m *= 2;
		pn->tab = realloc(pn->tab, sizeof(Note *) * pn->m);
		memtest(pn->tab);
	}
	
	pn->tab[pn->n] = note;
	pn->n++;
	return 1;
}


FluidAudioConfig *AudioConfig_Init(void)
{
	return FluidAudio_Alloc();
}

void FluidAudio_Free(FluidAudioConfig **fac)
{
	if(*fac != NULL)
	{
		if((*fac)->settings != NULL)
		{
			FluidSettings_Free(&((*fac)->settings));
		}
		free(*fac);
		*fac = NULL;
	}
}

int Tempo_GetMsTempo(void)
{
	if(main_audio != NULL)
	{
		return (int)(60.0 / main_audio->score->tempo * 1000);
	}
	return 0;
}

int Audio_KillThreads(void)
{
	if(main_audio != NULL && main_audio->score != NULL)
	{
		int i;
		for(i = 0; i < main_audio->score->n; i++)
		{
			if(main_audio->threads[i] != NULL)
			{
				SDL_KillThread(main_audio->threads[i]);
				main_audio->threads[i] = NULL;
			}
		}
	}
	return 1;
}

int GetRealId(Note *note)
{
	if(note->flags & NOTE_NATURAL)
		return note->note;
	return note->note 	+ ((note->flags & NOTE_SHARP) != 0)
				+ 2 * ((note->flags & NOTE_DOUBLESHARP) != 0)
				- ((note->flags & NOTE_FLAT) != 0)
				- 2 * ((note->flags & NOTE_DOUBLEFLAT) != 0);
}

int Audio_PlayStep(Step *step)
{
	Uint32 begin = 0;
	ToNote *note = NULL;
	if((NULL == step) || (NULL == step->notes))
		return 0;


	note = step->notes;
	do
	{
		if(!note->note->rest)
		{
			fluid_synth_noteon(main_audio->settings->synth, 0,
				 GetRealId(note->note), 80);
		}
		
		Playing_AddNote(note->note);
		main_audio->need_refresh = 1;
		
		begin = SDL_GetTicks();
		while(SDL_GetTicks() - begin < ((Tempo_GetMsTempo()-30) * 
				((Note_RealDuration(note->note, step)) / 16.0)))
			SDL_Delay(1);

		
		begin = SDL_GetTicks();
		while(SDL_GetTicks() - begin < 30 * ((64.0 / note->note->duration) / 16.0))
			SDL_Delay(1);
		fluid_synth_noteoff(main_audio->settings->synth, 0, GetRealId(note->note));
		Playing_RemoveNote(note->note);
		main_audio->need_refresh = 1;

		note = note->next;
	}
	while(note != NULL);

	return 1;
}

int Audio_PlayStaffThread(void *data)
{
	int id_staff = *(int *)data;
	Score *score = main_audio->score;
	Staff *staff = score->lst[id_staff];
	int i;
	for(i = main_audio->id_step; i < staff->n; i++)
		Audio_PlayStep(*(staff->steps + i));
	main_audio->playing = 0;
	main_audio->need_refresh = 1;
	return 1;
}

int *Integer_Alloc(int n)
{
	int *temp = (int *)malloc(sizeof(int));
	memtest(temp);
	*temp = n;
	return temp;
}

int Audio_Play(void)
{
	int i;
	if((NULL == main_audio) || (NULL == main_audio->score))
		return 0;
	for(i = 0; i < main_audio->score->n; i++)
	{
		if(NULL == main_audio->threads[i])
		{
			main_audio->threads[i] = SDL_CreateThread(Audio_PlayStaffThread, Integer_Alloc(i));
		}
		else
			printf("(error) thread n°%d is already playing !\n", i);
	}
	main_audio->playing = 1;
	return 1;
}

void Audio_StopPlayingNotes(void)
{
	int i;
	for(i = 0; i < main_audio->tabplaying->n; i++)
	{
		fluid_synth_noteoff(main_audio->settings->synth, 0, main_audio->tabplaying->tab[i]->note);
	}
	main_audio->tabplaying->n = 0;	
}

void Audio_Pause(void)
{
	main_audio->playing = 0;
	Audio_StopPlayingNotes();
	Audio_KillThreads();
}







int Audio_Init(FluidAudioConfig *config)
{
	if(main_audio != NULL)
		return 0;
	main_audio = config;
	return 1;
}

void Audio_Quit(void)
{
	FluidAudio_Free(&(main_audio));
}

void Audio_AssignateScore(Score *score)
{
	main_audio->score = score;
	if(main_audio->threads != NULL)
		free(main_audio->threads);
	main_audio->threads = (SDL_Thread **)calloc(score->n, sizeof(SDL_Thread *));
	memtest(main_audio->threads);
}

void Audio_GoToStep(int id_step)
{
	Audio_StopPlayingNotes();
	main_audio->id_step = id_step;
}

int Audio_isPlaying(void)
{
	return main_audio->playing;
}

int Audio_isInit(void)
{
	return main_audio != NULL;
}

void Audio_SetTempo(int newTempo)
{
	if(main_audio != NULL)
	{
		if(main_audio->score != NULL)
		{
			main_audio->score->tempo = newTempo;
		}
	}
}

int Audio_GetTempo(void)
{
	if(main_audio != NULL)
	{
		if(main_audio->score != NULL)
		{
			return main_audio->score->tempo;
		}
	}
	return 0;
}

void Audio_SetVolume(int newVolume)
{
	if(main_audio != NULL)
	{
		if(newVolume < 0)
			main_audio->globalVolume = 0;
		else if(newVolume > main_audio->maxVolume)
			main_audio->globalVolume = main_audio->maxVolume;
		else
			main_audio->globalVolume = newVolume;
	}
}

int Audio_GetVolume(void)
{
	if(main_audio != NULL)
	{
		return main_audio->globalVolume;
	}
	return 0;
}

double Audio_GetFracVolume(void)
{
	if(main_audio != NULL)
	{
		return main_audio->globalVolume * 1.0 / main_audio->maxVolume;
	}
	return 0.0;
}

int Audio_ChangeFunction(double (*f)(int, double, int))
{
	if(f != NULL)
		return 1;
	return 1;
}



double sinusoide(int x, double freq, int hardwareFreq)
{
	return sin(1.0 * M_PI * freq * x / hardwareFreq);
}

double fcarre(int x, double freq, int hardwareFreq)
{
	return (sinusoide(x, freq, hardwareFreq) > 0)?1.0:-1.0 ;
}

double mixSinCarre(int x, double freq, int hardwareFreq)
{
	return fcarre(x, freq, hardwareFreq) + sinusoide(x, freq, hardwareFreq);
}

double carreHarmo(int x, double freq, int hardwareFreq)
{
	int i;
	double sum = 0.;

	for(i = 0; i < 4; i++)
		sum += fcarre(x, freq*pow(2.0, i), hardwareFreq);

	return sum;
}

