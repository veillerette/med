/** \~french
 * \file Audio.h
 * \brief Moteur sonore du projet. Génération signaux audio.
 * \author Victor Veillerette
 * \version 1
 * \data Janvier 2017
 */

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


/**
 * \struct Représente un signal audio à travers une fonction qui le modélise.
 * 	   La fonction f permet de détermiser la valeur de l'onde à un point et une fréquence donnés.
 *
 */
typedef struct Wave Wave;
struct Wave
{
	double (*f)(int x, double freq, int hardwareFreq);	/*!< Fonction du signal, en général sin/carre */
	double freq;						/*!< Fréquence */
	int volume; /* 0 to 100 */				/*!< Volume du signal */
};

/**
 * \struct Channel
 * \brief Représente une voix d'une partition (portée).
 *	  Permet qu'une note soit constitué de plusieurs sons de différentes hauteurs (harmoniques)
 *	  Il est à noté qu'un channel est indépendant et chacun sera accédé par un sous-thread différent.
 */
typedef struct Channel Channel;
struct Channel
{
	Wave **waves;	/*!< Différents harmoniques */
	int n;		/*!< Nombre */
	int m;		/*!< Place disponible */
	int on;		/*!< Une note est-telle en cours d'être joué sur ce channel ? */
	
	Note *note_playing;	/*!< Si on = 1, renvoie vers la note jouée à ce moment */
};

/**
 * \struct Mixer
 * \brief Représente l'ensemble des voix. C'est-à-dire l'ensemble des portées d'une partition.
 *
 */
typedef struct Mixer Mixer;
struct Mixer
{
	Channel **channels;	/*!< Tableau dynamique des voix */
	int n;			/*!< Nombre */
	int m;			/*!< Place allouée */
};

/**
 * \struct AudioConfig
 * \brief Structure de configuration du module Audio (Variable Globale main_audio).
 *
 */
typedef struct AudioConfig AudioConfig;
struct AudioConfig
{
	SDL_AudioSpec hardware;		/*!< Spécifications hardware disponibles */
	unsigned long x;		/*!< Incrémenteur dans le temps*/
	int globalVolume;		/*!< Volume maximal */
	
	Mixer mixer;			/*!< Ensemble des voix */
	Score *score;			/*!< Partition */
	int id_step;			/*!< Position du curseur de lecture */
	int playing;			/*!< Boolean, 1 = joue, 0 = ne joue pas */
					
	SDL_Thread **threads;		/*!< Les différents threads des voix */
	int need_refresh;		/*!< Demande de rafraichissement de l'écran */
};


/*****************************************************
		DEV FUNCTIONS
******************************************************/


/**
 * \brief Alloue un nouveau signal.
 *
 * \param f Fonction modélisant le signal à tout instant.
 * \param freq Fréquence du signal.
 * \param volume Amplitude/Volume du signal
 *
 * \return Le signal alloué avec les bons paramètres.
 */
Wave *Wave_Alloc(double (*f)(int,double, int), double freq, int volume);


/**
 * \brief Libére un signal et sa mémoire associée.
 * \param wave Le signal à libérer.
 */
void Wave_Free(Wave **wave);


/**
 * \brief Alloue un nouveau channel vide.
 * \return Le nouveau channel.
 */
Channel *Channel_Alloc(void);


/**
 * \brief Libère un channel et sa mémoire associée.
 * \param chan Le channel à libérer.
 */
void Channel_Free(Channel **chan);


/**
 * \brief Ajoute un nouveau signal à un channel.
 *
 * \param chan Le channel
 * \param wave Le signal à rajouter.
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int Channel_Add(Channel *chan, Wave *wave);


/**
 * \brief Supprime un signal d'un channel.
 * \param chan Le Channel.
 * \param id Numéro de signal.
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int Channel_Delete(Channel *chan, int id);


/**
 * \brief Supprime et libère tous les sons d'un channel.
 * \param chan Le channel.
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int Channel_ReRoll(Channel *chan);


/**
 * \brief Réactive le son d'un channel.
 * \param chan Le channel.
 */
void Channel_Enable(Channel *chan);


/**
 * \brief Rends un channel muet.
 * \param chann Le channel.
 */
void Channel_Disable(Channel *chan);


/**
 * \brief Initialise un mixeur avec aucun voix.
 * \param mixer Le mixer.
 */
void Mixer_Init(Mixer *mixer);


/**
 * \brief Ajoute une voix à un mixer audio.
 *
 * \param mixer Le mixer.
 * \param chan Le channel à rajouter.
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int Mixer_Add(Mixer *mixer, Channel *chan);


/**
 * \brief Quitte et libère la mémoire lié à un Mixer.
 * \param mixer Le mixer à libérer.
 */
void Mixer_Quit(Mixer *mixer);


/**
 * \brief Initialise une nouvelle configuration audio.
 *	  Quitte le programmme avec un exit en cas d'échec.
 *	  En cas de succès, les propriétés réelles possibles du driver et de la carte son sont stockés
 *	  dans le champs hardware de la structure renvoyée.
 *
 * \param askFreq Fréquence hardware demandée
 * \param format  Format Audio (Signé, Non Signé, 8 bits, 16 bits...).
 * \param samples Nombre de channels (1 = mono, 2 = stéréo).
 * \param callback Fonction de remplissage du buffer une fois qu'il est vide.
 * \param userdata Donnée utilisateur (unused).
 * \param globalVolume Volume maximal global.
 *
 * \return La configuration audio obtenue.
 */
AudioConfig *AudioConfig_DevInit(int askFreq, Uint16 format, Uint8 channels, Uint16 samples,
					void (*callback)(void *, Uint8 *, int), void *userdata,
					int globalVolume);

void AudioConfig_Free(AudioConfig **ac);


/**
 * \brief Fonction de remplissage du buffer audio lorsque celui-ci est vide.
 *	  Fonction à ne pas appeler directement, elle doit être uniquement passé à SDL qui va l'utiliser
 *	  pour remplir le buffer au moment qu'il juge opportun.
 *
 * \param userdata (unused)
 * \param stream   Buffer à remplir
 * \param len	   Taille du buffer à remplir
 */
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


/**
 * \brief Alloue un entier en mémoire et renvoie son pointeur.
 * \param n Un entier
 * \return Pointeur vers la nouvelle case mémoire.
 */
int *Integer_Alloc(int n);


/**
 * \brief Renvoie le temps en ms associé au tempo pour une Noire.
 * \return Le temps en ms.
 */
int Tempo_GetMsTempo(void);

/*****************************************************
		WAVE FUNCTIONS
******************************************************/
/**
 * \brief Signal sinusoidal simple.
 *
 * \param x t
 * \param freq Fréquence
 * \param hardwareFreq Fréquence du driver hardware (normalement 44100).
 *
 * \return Valeur f(x) associée
 */
double sinusoide(int x, double freq, int hardwareFreq);


/**
 * \brief Signal carré simple.
 *
 * \param x t
 * \param freq Fréquence
 * \param hardwareFreq Fréquence du driver hardware (normalement 44100).
 *
 * \return Valeur f(x) associée
 */
double carre(int x, double freq, int hardwareFreq);


/**
 * \brief Signal carré avec des harmoniques dans les aigues.
 *
 * \param x t
 * \param freq Fréquence
 * \param hardwareFreq Fréquence du driver hardware (normalement 44100).
 *
 * \return Valeur f(x) associée
 */
double carreHarmo(int x, double freq, int hardwareFreq);


/*****************************************************
		PLAYING FUNCTIONS
******************************************************/

/**
 * \brief Fonction qui permet de jouer une seule mesure dans un channel.
 *	  A ne pas utiliser directement, cette fonction est appelée par des sous-threads.
 *
 * \param step La mesure à jouer
 * \param chan Le channel de destination
 *
 * \return 1 en cas de succès, 0 en cas d'erreur.
 */
int Audio_PlayStep(Step *step, Channel *chan);


/**
 * \brief Fonction qui joue l'intégralité d'une portée dans un channel.
 *	  A ne pas utiliser directement, cette fonction est appelée par des sous-threads.
 *
 * \param data Identifiant de la portée (Int *).
 *
 * \return 1 en cas de succès, 0 en cas d'erreur.
 */
int Audio_PlayStaffThread(void *data);


/**
 * \brief Fonction qui termine immédiatement tous les threads de lecture Audio.
 *	  Cette fonction est notamment appelée dans la fonction Audio_Pause().
 *
 * \return 1 en cas de succès, 0 en cas d'erreur.
 */
int Audio_KillThreads(void);


/*****************************************************
		MAIN FUNCTIONS
******************************************************/

/**
 * \brief Initialise le module Audio avec une configuration Audio établie.
 *	  La SDL doit avoir été initilisé avant l'appel à cette fonction.
 *
 * \param ac Configuration audio.
 */
void Audio_Init(AudioConfig *ac);


/**
 * \brief Quitte et libère le module Audio.
 *	  Ne quitte pas la SDL.
 */
void Audio_Quit(void);


/**
 * \brief Associe une partition au module Audio.
 *	  Indispensable avant le lancement de la lecture.
 *
 * \param score La partition à associer.
 */
void Audio_AssignateScore(Score *score);


/**
 * \brief Déplace le curseur de lecture à une autre mesure.
 *
 * \param id_step L'identifiant de la nouvelle mesure.
 */
void Audio_GoToStep(int id_step);


/**
 * \brief Est-ce que la lecture est lancée.
 *
 * \return 1 si oui, 0 sinon.
 */
int Audio_isPlaying(void);


/**
 * \brief Est-ce que le module Audio est initialisé.
 *
 * \return 1 si oui, 0 sinon.
 */
int Audio_isInit(void);


/**
 * \brief Relance la lecture Audio.
 */
void Audio_Play(void);


/**
 * \brief Met en pause la lecture Audio.
 */
void Audio_Pause(void);


/**
 * \brief Modifie le tempo.
 *
 * \param newTempo Le nouveau tempo
 */
void Audio_SetTempo(int newTempo);


/**
 * \brief Renvoie le tempo en battements par minute.
 *
 * \return tempo
 */
int Audio_GetTempo(void);

/*****************************************************
******************************************************/

/*
	Variable globale de configuration des paramètres audio.
	A ne pas modifier directement, faire appels aux fonctions MAIN FUNCTIONS,
	comme Audio_Init(), Audio_Quit() ...
*/
extern AudioConfig *main_audio;

#endif

