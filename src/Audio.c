#include "../include/Audio.h"

AudioMain *main_audio = NULL;

void MYCALLBACK(void *userdata, Uint8 *stream, int len)
{
	int i = 0;
	double freq = (double)(int)main_audio->config.freq;
	int volume = main_audio->config.volume;

	if((NULL == main_audio) || (main_audio->status != AUDIO_INIT) 
		|| (NULL == main_audio->hardware))
		return;
	for(i = 0; i < len; i+=2)
	{
		*((Sint16 *) &stream[i]) = (Sint16)(volume * 
				main_audio->config.f(main_audio->progress, freq));
		main_audio->progress = (main_audio->progress+1);
	}
	
	userdata = userdata; /* to avoid Wunused-variable */
}


SDL_AudioSpec *AudioSpec_Alloc(void)
{
	SDL_AudioSpec *temp = (SDL_AudioSpec *)malloc(sizeof(SDL_AudioSpec));
	memtest(temp);
	
	temp->freq = 0;
	temp->format = 0;
	temp->channels = 0;
	temp->silence = 0;
	temp->samples = 0;
	temp->size = 0;
	temp->callback = NULL;
	temp->userdata = NULL;
	
	return temp;
}

AudioMain *AudioMain_Alloc(void)
{
	AudioMain *temp = (AudioMain *)malloc(sizeof(AudioMain));
	memtest(temp);
	
	temp->hardware = AudioSpec_Alloc();
	temp->status = AUDIO_NONE;
	temp->config.f = NULL;
	temp->config.freq = 0;
	temp->progress = 0;
	
	return temp;
}

void Audio_SetConfig(double (*f)(int x, double freq), double freq, int volume)
{
	if(NULL == main_audio)
		return;
	main_audio->config.f = f;
	main_audio->config.freq = freq;
	main_audio->config.volume = volume;
}

void AudioMain_Free(AudioMain **am)
{
	if(*am != NULL)
	{
		if((*am)->hardware != NULL)
			free((*am)->hardware);
		free(*am);
		*am = NULL;
	}
}

int Audio_DevInit(int freq, Uint16 format, Uint8 channels, Uint16 samples, 
			void (*callback)(void*, Uint8 *, int), void *userdata)
{
	SDL_AudioSpec temp;
	
	if(main_audio != NULL && main_audio->status != AUDIO_NONE)
		return 0;
	
	if(NULL == main_audio)
	{
		main_audio = AudioMain_Alloc();
	}
	
	temp.freq = freq;
	temp.format = format;
	temp.channels = channels;
	temp.samples = samples;
	temp.callback = callback;
	temp.userdata = userdata;
	
	if(SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		colorprintf(RED, "Error with SDL_Init(SDL_INIT_AUDIO)\n");
		exit(EXIT_FAILURE);
	}
	
	if(SDL_OpenAudio(&temp, main_audio->hardware) < 0)
	{
		colorprintf(RED, "Error with SDL_OpenAudio()\n");
		exit(EXIT_FAILURE);
	}
	
	main_audio->status = AUDIO_INIT;
	return 1;
}

int Audio_Init(void)
{
	return Audio_DevInit(44100, AUDIO_S16, 2, 512, MYCALLBACK, NULL);
}

int Audio_Quit(void)
{
	if(NULL == main_audio)
		return 0;
	main_audio->status = AUDIO_QUIT;
	SDL_CloseAudio();
	AudioMain_Free(&main_audio);
	return 1;
}

void Audio_Play(void)
{
	SDL_PauseAudio(0);
}

void Audio_Pause(void)
{
	main_audio->progress = 0;
	SDL_PauseAudio(1);
}

double sinusoide(int x, double freq)
{
	return sin(M_PI * freq * x / main_audio->hardware->freq);
}

double carre(int x, double freq)
{
	return (sinusoide(x, freq) > 0)?1.0:-1.0;
}

double other(int x, double freq)
{
	return (double)(x%((int)freq));
}

void PlaySeconds(unsigned int n, int freq)
{
	Uint32 begin = 0;
	
	begin = SDL_GetTicks();
	Audio_SetConfig(sinusoide, freq, 254);
	
	Audio_Play();
	while(SDL_GetTicks() - begin < n*1000)
		SDL_Delay(1);
	Audio_Pause();
}

void TEST(void)
{
	double f = 300.;
	int i = 0;
	int j = 0;
	Uint32 begin = 0;
	Audio_Init();
	
	for(j = 21; j < 108; j++)
		printf("%d => %g Hz\n", j, GetFreqFromId(j));
	exit(1);
	
	Audio_SetConfig(sinusoide, f, 100);
	Audio_Play();
	while(i < 6)
	{	
		Audio_SetConfig(sinusoide, f, 100);
		begin = SDL_GetTicks();
		Audio_Play();
		while(SDL_GetTicks() - begin < 200)
			SDL_Delay(1);
		Audio_Pause();
		begin = SDL_GetTicks();
		f+=50;
		i++;
		
		while(SDL_GetTicks() - begin < 50)
			SDL_Delay(1);
	}
	Audio_Quit();
}

double GetFreqFromId(int id)
{
	int oct = id/12 ;
	int add = id%12;
	return (33.0) * pow(2.0, oct - 2) * pow(1.059463, add) ;
}

int GetRealId(Note *note)
{
	return note->note 	+ ((note->flags & NOTE_SHARP) != 0) 
				+ 2 * ((note->flags & NOTE_DOUBLESHARP) != 0)
				- ((note->flags & NOTE_FLAT) != 0)
				- 2 * ((note->flags & NOTE_DOUBLEFLAT) != 0);
}

int Test_PlayStep(Step *step)
{
	Uint32 begin = 0;
	ToNote *note = NULL;
	if((NULL == step) || (NULL == step->notes))
		return 0;
	
	Audio_Init();
	
	note = step->notes;
	do
	{
		printf("playing note %f...\n", GetFreqFromId(GetRealId(note->note)));
		if(!note->note->rest)
		{
			Audio_SetConfig(carre, 
					GetFreqFromId(GetRealId(note->note)), 30000);
			Audio_Play();
		}
		else
			Audio_Pause();
		begin = SDL_GetTicks();
		while(SDL_GetTicks() - begin < 
				(1000 * ((64.0/note->note->duration) / 16.0)))
			SDL_Delay(1);

		note = note->next;
	}
	while(note != NULL);
	
	
	Audio_Quit();
	return 1;
}















