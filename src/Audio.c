#include "../include/Audio.h"

AudioMain *main_audio = NULL;

void MYCALLBACK(void *userdata, Uint8 *stream, int len)
{
	int i = 0;
	double freq = main_audio->config.freq;
	int volume = main_audio->config.volume;

	if((NULL == main_audio) || (main_audio->status != AUDIO_INIT) 
		|| (NULL == main_audio->hardware))
		return;
	for(i = 0; i < len; i++)
	{
		*((Sint16 *) &stream[i]) = (Sint16)(volume * 
				main_audio->config.f(main_audio->progress, freq));
		printf("%d\n", *((Sint16 *) &stream[i]));
		main_audio->progress = (main_audio->progress+1) % main_audio->hardware->freq;
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

void Audio_SetConfig(double (*f)(int x, double freq), int freq, int volume)
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
	return Audio_DevInit(44100, AUDIO_S8, 2, 1024, MYCALLBACK, NULL);
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
	int f = 300;
	int i = 0;
	Uint32 begin = 0;
	Audio_Init();
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















