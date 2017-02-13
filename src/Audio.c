#include "../include/Audio.h"


AudioConfig *main_audio = NULL;


/*****************************************************
		ALLOCATIONS && SETTERS
******************************************************/




Wave *Wave_Alloc(double (*f)(int,double, int), double freq, int volume)
{
	Wave *temp = (Wave *)malloc(sizeof(Wave));
	memtest(temp);

	temp->f = f;
	temp->freq = freq;
	temp->volume = volume;

	return temp;
}

void Wave_Free(Wave **wave)
{
	if(*wave != NULL)
	{
		free(*wave);
		*wave = NULL;
	}
}

Channel *Channel_Alloc(void)
{
	Channel *temp = (Channel *)malloc(sizeof(Channel));
	memtest(temp);

	temp->waves = (Wave **)malloc(sizeof(Wave *) * 2);
	memtest(temp->waves);

	temp->n = 0;
	temp->m = 2;
	temp->on = 0;

	temp->note_playing = NULL;

	return temp;
}

void Channel_Free(Channel **chan)
{
	if(*chan != NULL)
	{
		int i;
		for(i = 0; i < (*chan)->n; i++)
			Wave_Free(&((*chan)->waves[i]));

		free((*chan)->waves);

		free(*chan);
		*chan = NULL;
	}
}

int Channel_Add(Channel *chan, Wave *wave)
{
	if((NULL == chan) || (NULL == wave))
		return 0;

	if(chan->n == chan->m)
	{
		chan->waves = (Wave **)realloc(chan->waves, sizeof(Wave *) * (chan->m * 2));
		memtest(chan->waves);

		chan->m *= 2;
	}

	chan->waves[chan->n] = wave;
	chan->n++;

	return 1;
}

int Channel_Delete(Channel *chan, int id)
{
	int i;
	if((NULL == chan) || (id < 0) || (id >= chan->n))
		return 0;

	Wave_Free(&(chan->waves[id]));

	for(i = id; i < chan->n-1; i++)
		chan->waves[i] = chan->waves[id + 1];
	chan->n--;
	return 1;
}

int Channel_ReRoll(Channel *chan)
{
	int i;
	if((NULL == chan) || (NULL == chan->waves))
		return 0;

	for(i = 0; i < chan->n; i++)
		Wave_Free(&(chan->waves[i]));

	chan->n = 0;
	return 1;
}

void Channel_Enable(Channel *chan)
{
	if(chan != NULL)
		chan->on = 1;
}

void Channel_Disable(Channel *chan)
{
	if(chan != NULL)
		chan->on = 0;
}

void Mixer_Init(Mixer *mixer)
{
	if(mixer != NULL)
	{
		mixer->channels = (Channel **)malloc(sizeof(Channel *) * (2));
		memtest(mixer->channels);

		mixer->n = 0;
		mixer->m = 2;
	}
}

int Mixer_Add(Mixer *mixer, Channel *chan)
{
	if((NULL == mixer) || (NULL == chan))
		return 0;

	if(mixer->n == mixer->m)
	{
		mixer->channels = (Channel **)realloc(mixer->channels,
						sizeof(Channel *) * mixer->m * 2);
		memtest(mixer->channels);

		mixer->m *= 2;
	}

	mixer->channels[mixer->n] = chan;
	mixer->n++;

	return 1;
}

void Mixer_Quit(Mixer *mixer)
{
	int i;
	if(NULL == mixer)
		return;
	for(i = 0; i < mixer->n; i++)
		Channel_Free(&(mixer->channels[i]));
	free(mixer->channels);
	mixer->n = 0;
	mixer->m = 0;
}

AudioConfig *AudioConfig_DevInit(int askFreq, Uint16 format, Uint8 channels, Uint16 samples,
					void (*callback)(void *, Uint8 *, int), void *userdata,
					int globalVolume)
{
	SDL_AudioSpec spec;
	AudioConfig *ac = (AudioConfig *)malloc(sizeof(AudioConfig));
	memtest(ac);

	ac->x = 0;
	ac->globalVolume = globalVolume;
	ac->score = NULL;
	ac->id_step = 0;
	ac->playing = 0;
	ac->threads = NULL;
	ac->need_refresh = 0;
	ac->maxVolume = 32000;
	Mixer_Init(&(ac->mixer));

	spec.freq = askFreq;
	spec.format = format;
	spec.channels = channels;
	spec.samples = samples;
	spec.callback = callback;
	spec.userdata = userdata;

	if(SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		colorprintf(RED, "Error with SDL_Init(SDL_INIT_AUDIO)\n");
		exit(EXIT_FAILURE);
	}

	if(SDL_OpenAudio(&spec, &(ac->hardware)) < 0)
	{
		colorprintf(RED, "Error while oppening Audio (SDL_OpenAudio())\n");
		exit(EXIT_FAILURE);
	}

	return ac;
}

void AudioConfig_Free(AudioConfig **ac)
{
	if(*ac != NULL)
	{
		Mixer_Quit(&((*ac)->mixer));
		free(*ac);
		*ac = NULL;
	}
}

int Audio_RegulariseWave(Sint16 *stream, int len)
{
	Sint16 max = 0.;
	Sint16 min = 0.;
	int i;
	for(i = 0; i < len; i++)
		if(stream[i] > max)
			max = stream[i];
		else if(stream[i] < min)
			min = stream[i];

	for(i = 0; i < len; i++)
	{
		if(stream[i] > 0)
			stream[i] = stream[i] * 8000 / max;
		else if(stream[i] < 0)
			stream[i] = stream[i] * 8000 / min;
	}
	return 1;
}


void MAIN_CALLBACK(void *userdata, Uint8 *stream, int len)
{
	int i;
	AudioConfig *ac = NULL;
	if(NULL == main_audio)
		return;
	ac = main_audio;

	for(i = 0; i < len; i += 2)
	{
		*((Sint16 *) &stream[i]) = (Sint16)Mixer_Calc(ac, &(ac->mixer));
		/* Audio_RegulariseWave((Sint16 *)stream, len / 2); */
		ac->x++;
	}

	userdata = userdata; /* to avoid Wunused-variable */
}

AudioConfig *AudioConfig_Init(void)
{
	return AudioConfig_DevInit(44100, AUDIO_S16, 2, 512, MAIN_CALLBACK, NULL, 8000);
}

void AudioConfig_SetVolume(AudioConfig *ac, int newVolume)
{
	if(NULL == ac)
		return;

	ac->globalVolume = newVolume;
}


/*****************************************************
		CALCS & GENERAL FUNCTIONS
******************************************************/

double GetFreqFromId(int id)
{
	return (33.0) * pow(2.0, id / 12 - 2) * pow(1.059463, id % 12) ;
}

int GetRealId(Note *note)
{
	return note->note 	+ ((note->flags & NOTE_SHARP) != 0)
				+ 2 * ((note->flags & NOTE_DOUBLESHARP) != 0)
				- ((note->flags & NOTE_FLAT) != 0)
				- 2 * ((note->flags & NOTE_DOUBLEFLAT) != 0);
}

double Note_GetFreq(Note *note)
{
	if(NULL == note)
		return 0.;

	return GetFreqFromId(GetRealId(note));
}

double Wave_Calc(AudioConfig *ac, Wave *wave)
{
	if((NULL == ac) || (NULL == wave))
		return 0.;

	return wave->f(ac->x, wave->freq, ac->hardware.freq) * wave->volume;
}

double Channel_Calc(AudioConfig *ac, Channel *chan)
{
	int i;
	double sum = 0.;

	if((NULL == ac) || (NULL == chan))
		return 0.;

	for(i = 0; i < chan->n; i++)
		sum += Wave_Calc(ac, chan->waves[i]);

	if(chan->on)
		return sum * ac->globalVolume * 1.0 / (chan->n * 100.0);
	else
		return ac->globalVolume + 10.;
}

double Mixer_Calc(AudioConfig *ac, Mixer *mixer)
{
	int i;
	double sum = 0.;
	double temp = 0.;
	int n = 0;
	if((NULL == ac) || (NULL == mixer))
		return 0.;

	for(i = 0; i < mixer->n; i++)
	{
		temp =  Channel_Calc(ac, mixer->channels[i]);
		if(temp != ac->globalVolume + 10.)
		{
			sum += temp;
			n++;
		}
	}

	return sum / mixer->n;

}

Channel *Channel_CreateOne(double freq, double (*f)(int, double, int))
{
	Channel *chan = Channel_Alloc();
	Channel_Add(chan, Wave_Alloc(f, freq, 100));
	return chan;
}

void Channel_ChangeOne(Channel *chan, double freq)
{
	int i;
	for(i = 0; i < chan->n; i++)
		chan->waves[i]->freq = freq;
}

Channel *Channel_CreateSimplyOctave(double freq, double (*f)(int, double, int))
{
	int i;
	Channel *chan = Channel_Alloc();

	for(i = 0; i < 3; i++)
		Channel_Add(chan, Wave_Alloc(f, freq*pow(2, i), 100*pow(0.5, i)));

	return chan;
}

void Channel_ChangeOctave(Channel *chan, double freq)
{
	int i;
	for(i = 0; i < chan->n; i++)
	{
		chan->waves[i]->freq = freq * pow(2, i);
		chan->waves[i]->volume = 100 * pow(0.5, i);
	}
}

int *Integer_Alloc(int n)
{
	int *temp = (int *)malloc(sizeof(int));
	memtest(temp);
	*temp = n;
	return temp;
}

int Tempo_GetMsTempo(void)
{
	if(main_audio != NULL)
	{
		return (int)(60.0 / main_audio->score->tempo * 1000);
	}
	return 0;
}

/*****************************************************
		WAVE FUNCTIONS
******************************************************/

double sinusoide(int x, double freq, int hardwareFreq)
{
	return sin(1.0 * M_PI * freq * x / hardwareFreq);
}

double carre(int x, double freq, int hardwareFreq)
{
	return (sinusoide(x, freq, hardwareFreq) > 0)?1.0:-1.0 ;
}

double mixSinCarre(int x, double freq, int hardwareFreq)
{
	return carre(x, freq, hardwareFreq) + sinusoide(x, freq, hardwareFreq);
}

double carreHarmo(int x, double freq, int hardwareFreq)
{
	int i;
	double sum = 0.;

	for(i = 0; i < 4; i++)
		sum += carre(x, freq*pow(2.0, i), hardwareFreq);

	return sum;
}


/*****************************************************
		PLAYING FUNCTIONS
******************************************************/

int Audio_PlayStep(Step *step, Channel *chan)
{
	Uint32 begin = 0;
	ToNote *note = NULL;
	if((NULL == step) || (NULL == step->notes))
		return 0;
	if((NULL == chan))
		return 0;

	#ifndef DEBUG
	printf("playing step on channel %p\n", chan);
	#endif

	note = step->notes;
	do
	{
		if(!note->note->rest)
		{
			Channel_Enable(chan);
			Channel_ChangeOne(chan, Note_GetFreq(note->note));
			chan->note_playing = note->note;
			main_audio->need_refresh = 1;
		}
		else
		{
			Channel_Disable(chan);
			chan->note_playing = note->note;
			main_audio->need_refresh = 1;
		}

		begin = SDL_GetTicks();
		while(SDL_GetTicks() - begin < ((Tempo_GetMsTempo()-30) * ((64.0 / note->note->duration) / 16.0)))
			SDL_Delay(1);

		Channel_Disable(chan);
		begin = SDL_GetTicks();
		while(SDL_GetTicks() - begin < 30 * ((64.0 / note->note->duration) / 16.0))
			SDL_Delay(1);

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
		Audio_PlayStep(*(staff->steps + i), main_audio->mixer.channels[id_staff]);
	Channel_Disable(main_audio->mixer.channels[id_staff]);
	main_audio->threads[id_staff] = NULL;
	colorprintf(GREEN, "End of Playing staff n°%d\n", id_staff);
	main_audio->playing = 0;
	main_audio->need_refresh = 1;
	return 1;
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
				printf("Kill a thread for staff n°%d\n", i);
				SDL_KillThread(main_audio->threads[i]);
				main_audio->threads[i] = NULL;
			}
		}
	}
	return 1;
}


/*****************************************************
		MAIN FUNCTIONS
******************************************************/

void Audio_Init(AudioConfig *ac)
{
	if(NULL == main_audio && ac != NULL)
	{
		main_audio = ac;
	}
}

void Audio_Quit(void)
{
	if(main_audio != NULL)
	{
		AudioConfig_Free(&main_audio);
	}
}

void Audio_AssignateScore(Score *score)
{
	if(main_audio != NULL)
	{
		int i;
		main_audio->score = score;
		main_audio->id_step = 0;
		Mixer_Quit(&(main_audio->mixer));
		Mixer_Init(&(main_audio->mixer));
		for(i = 0; i < score->lst[0]->n; i++)
			Mixer_Add(&(main_audio->mixer), Channel_CreateOne(0, mixSinCarre));
		if(main_audio->threads != NULL)
			free(main_audio->threads);
		main_audio->threads = (SDL_Thread **)calloc(score->n, sizeof(SDL_Thread *));
		memtest(main_audio->threads);

		printf("end assignateScore(), mixer->n=%d score=%p, id_step=%d\n", main_audio->mixer.n, main_audio->score, main_audio->id_step);
	}
}

void Audio_GoToStep(int id_step)
{
	if(main_audio != NULL && main_audio->score != NULL)
	{
		main_audio->id_step = id_step;
	}
}

int Audio_isPlaying(void)
{
	if(NULL == main_audio)
		return 0;
	return main_audio->playing;
}

int Audio_isInit(void)
{
	return main_audio != NULL;
}

void Audio_Play(void)
{
	int i;
	if((NULL == main_audio) || (NULL == main_audio->score) || (NULL == main_audio->threads)
	|| (NULL == main_audio->mixer.channels) || (0 == main_audio->mixer.n))
	{
		colorprintf(RED, "Unknown error with Audio Playing\n");
		exit(EXIT_FAILURE);
	}
	main_audio->playing = 1;
	SDL_PauseAudio(0);
	colorprintf(GREEN, "Playing audio !\n");
	for(i = 0; i < main_audio->score->n; i++)
	{

		if(NULL == main_audio->threads[i])
		{
			printf("Init a new thread for staff n°%d\n", i);
			main_audio->threads[i] = SDL_CreateThread(Audio_PlayStaffThread, Integer_Alloc(i));
		}
		else
			printf("(error) thread n°%d is already playing !\n", i);
	}
}

void Audio_Pause(void)
{
	if(main_audio->playing)
	{
		main_audio->playing = 0;
		SDL_PauseAudio(1);
		colorprintf(GREEN, "Pausing Audio !\n");
		main_audio->x = 0;
		Audio_KillThreads();
	}
}

void Audio_SetTempo(int newTempo)
{
	if(main_audio != NULL)
	{
		if(newTempo > 30 && newTempo < 250)
			main_audio->score->tempo = newTempo;
	}
}

int Audio_GetTempo(void)
{
	if(main_audio != NULL)
		return main_audio->score->tempo;
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
	return 0.;
}