/** \~french
 * \file Step.h
 * \brief Représentation d'une mesure
 * \author Victor Veillerette
 * \version 1
 * \date Janvier 2017
 */

#ifndef __HEAD_STEP__
#define __HEAD_STEP__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "System.h"

/**
 * \enum Note_Flags
 * \brief Attributs d'une note.
 */
typedef enum {
	NOTE_DEFAULT 		= 0,		/*!< Aucun */
	NOTE_SHARP		= 0x01,		/*!< Note diésée */
	NOTE_DOUBLESHARP	= 0x02,		/*!< Note double-diésée */
	NOTE_FLAT		= 0x04,		/*!< Note bémolisée */
	NOTE_DOUBLEFLAT		= 0x08,		/*!< Note double-bémolisée */
	NOTE_POINTED		= 0x10,		/*!< Note pointée */
	NOTE_DOUBLEPOINTED	= 0x20,		/*!< Note double-pointée */
	NOTE_NATURAL		= 0x40,		/*!< Note avec un bécare */
	NOTE_LINKED		= 0x80		/*!< Note prolongée */
	} Note_Flags;


typedef enum {
	STEP_DEFAULT		= 0
	/*** ***/
	} Step_Flags;


/**
 * \enum Note_Duration
 * \brief Durée d'une note/silence
 */
typedef enum {
	RONDE 		= 1,	/*!< Ronde, 64 x base */
	BLANCHE		= 2,	/*!< Blanche, 32 x base */
	NOIRE		= 4,	/*!< Noire, 16 x base */
	CROCHE		= 8,	/*!< Croche, 8 x base */
	DOUBLECROCHE	= 16, 	/*!< Double, 4 x base */
	TRIPLECROCHE	= 32,	/*!< Triple, 2 x base */
	QUADRUPLECROCHE	= 64	/*!< Quadruple, 1 x base */
	} Note_Duration;

/**
 * \enum Cle
 * \brief Les différents Clés.
 */
typedef enum {
	CLE_SOL 	= 1,	/*!< Clé de Sol Seconde */
	CLE_FA 		= 2,	/*!< Clé de Fa Quatrième */
	CLE_UT3,
	CLE_UT4
	} Cle;

/**
 * \struct Note
 * \brief Représentation d'une note/silence.
 */
typedef struct Note Note;
struct Note
{
	char note; 			/*!< Hauteur d'une note, A0=21, B1=22 ...*/
	char rest; 			/*!< Silence = 1, Note = 0 */
	Note_Flags flags : 24;		/*!< Attributs d'une note */
	Note_Duration duration : 8;	/*!< Durée d'une note/silence */
};

/**
 * \struct ToNote
 * \brief Permet d'enchainer les notes avec une liste chainée.
 */
typedef struct ToNote ToNote;
struct ToNote
{
	Note *note;		/*!< Note courante */
	ToNote *next;		/*!< Prochaine note */
};

/**
 * \struct Step
 * \brief Représentation d'une mesure.
 */
typedef struct Step Step;
struct Step
{
	ToNote *notes;			/*!< Liste chainées des notes */
	int num;			/*!< Numérateur de la fraction de mesure */
	Cle cle : 16;			/*!< Clé de la mesure */
	Note_Duration den : 8;		/*!< Dénominateur de la fraction de mesure, temps de base */
	Step_Flags flags : 16;		/*!< Attributs de la mesure */
	signed char sign;		/*!< Armure de la mesure */
};

/**
 * \brief Alloue une nouvelle note
 *
 * \param note Hauteur de la note (ignoré si rest = 1)
 * \param flags Attributs de la note
 * \param duration Durée de la note
 * \param rest 0 si note, 1 si silence
 *
 * \return La note allouée, NULL sinon.
 */
Note *Note_Alloc(char note, Note_Flags flags, Note_Duration duration, char rest);


/**
 * \brief Libère en mémoire une note
 *
 * \param note La note à libérer
 */
void Note_Free(Note **note);


/**
 * \brief Retourne la durée réelle d'une note, en prenant en compte la mesure et ses attributs.
 *
 * \param note La note
 * \param step La mesure où se trouve la note
 *
 * \return La durée réelle de la note
 */
int Note_RealDuration(Note *note, Step *step);


/**
 * \brief Convertie le nom d'une note en sa hauteur 
 *
 * \param note La note
 *
 * \return La hauteur de la note, ou 0 si la note n'est pas reconnue
 */
char ConvertStringToID(const char *note);


/**
 * \brief Crée une nouvelle note en analysant une chaîne de caractère.
 *	  Remplie automatiquement les attributs et toutes les autres informations.
 *
 * \param note La note
 * \param duration Durée de la note
 *
 * \return La note créee, ou NULL si échec.
 */
Note *Note_CreateNoteFromString(const char *note, Note_Duration duration);


/**
 * \brief Alloue une liste chainée de note à un élement.
 *
 * \param note La hauteur de note
 * \param flags Attributs de note
 * \param duration Durée de note
 * \param rest Note ou Silence
 *
 * \return La ToNote allouée ou NULL si échec.
 */
ToNote *ToNote_Alloc(char note, Note_Flags flags, int duration, char rest);


/**
 * \brief Libère UN seul élément de la liste chainée.
 *
 * \param tonote L'élément à libérer.
 */
void ToNote_Free(ToNote **tonote);


/**
 * \brief Libère TOUS les éléments de la liste chainée.
 *
 * \param tonote La liste chainée à libérer.
 */
void ToNote_FreeAll(ToNote **tonote);


/**
 * \brief Alloue une nouvelle mesure sans notes ni silence.
 *	  Attention, utiliser Step_Init avant de faire quoi que ce soit avec cette mesure.
 *
 * \param num Numérateur de la mesure
 * \param den Dénominateur de la mesure
 * \param cle Clé de la mesure
 * \param sign Armure de la mesure
 * \param flags Attributs de la mesure
 *
 * \return La mesure allouée, NULL si échec.
 */
Step *Step_Alloc(int num, Note_Duration den, Cle cle, char sign, Step_Flags flags);


/**
 * \brief Libère entièrement une mesure et ses notes.
 *
 * \param step La mesure à libérer.
 */
void Step_Free(Step **step);


/**
 * \brief Change une note en silence, ou inversement dans une liste
 *
 * \param notes La liste des notes
 * \param id Numéro de la note à modifier
 * \param newStatus Nouvel état de note/silence
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int ToNote_ChangeRestStatus(ToNote *notes, int id, char newStatus);


/**
 * \brief Change une note en silence, ou inversement dans une mesure
 *
 * \param step La mesure
 * \param id Numéro de la note à modifier
 * \param newStatus Nouvel état de note/silence
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int Step_ChangeRestStatus(Step *step, int id, char newStatus);


/**
 * \brief Divise un silence en deux silences de durées égales dans une liste de notes.
 *
 * \param tonote La liste des notes
 * \param id Numéro de la note à modifier
 * \param newDuration Nouvelle durée
 * \param stepNum Numérateur de la mesure
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int ToNote_DiviseRest(ToNote *tonote, int id, int newDuration, int stepNum);


/**
 * \brief Divise un silence en deux silences de durées égales dans une mesure.
 *
 * \param step La mesure
 * \param id Numéro de la note à modifier
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int Step_DiviseRest(Step *step, int id);


/**
 * \brief Divise une note ou un silence en deux silences de durées égales dans une mesure. Si la note est une ronde, emplie par la durée d'un temps
 *
 * \param step La mesure
 * \param id Numéro de la note à modifier
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int Step_Divise(Step *step, int id);

/**
 * \brief Initialise une mesure. Indispensable avant d'ajouter des notes.
 *	  Rajoute un ou plusieurs silences pour remplir la mesure avec la bonne durée globale.
 *
 * \param step La mesure à initialiser.
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int Step_Init(Step *step);


/**
 * \brief Change les informations d'une mesure.
 *
 * \param step La mesure à modifier
 * \param num Nouveau numérateur
 * \param den Nouveau dénominateur
 * \param cle Nouvelle clé
 * \param flags Nouveaux attributs
 * \param sign Nouvelle armure
 * 
 * \return 1 en cas de succès, 0 sinon
 */
int Step_Change(Step *step, int num, Note_Duration den, Cle cle, Step_Flags flags, char sign);
	

/**
 * \brief Affiche sous forme texte une note en console.
 *
 * \param note La note à afficher
 */					
void Note_ConsolePrintf(Note *note);


/**
 * \brief Affiche sous forme texte une mesure et toutes ses notes en console.
 *
 * \param step La mesure à afficher
 */
void Step_ConsoleFastPrintf(Step *step);


/**
 * \brief Affiche sous forme texte une liste de notes en console.
 *
 * \param notes La liste de notes
 */
void ToNote_ConsolePrintf(ToNote *notes);


/**
 * \brief Affiche sous forme texte une mesure et toutes ses notes en console.
 *
 * \param step La mesure à afficher
 */
void Step_ConsolePrintf(Step *step);


/**
 * \brief Régularise une liste de notes, c'est-à-dire, fusionne les silences fusionnables.
 *
 * \param notes La liste de notes
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int ToNote_Regularise(ToNote *notes);


/**
 * \brief Régularise une mesure, c'est-à-dire, fusionne les silences fusionnables.
 *
 * \param step La mesure
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int Step_Regularise(Step *step);


/** 
 * \brief Renvoie la puissance de 2 strictement inférieure à n.
 *
 * \param n n 
 *
 * \return Le résultat.
 */
int find2min(int n);


/**
 * \brief Ajoute une note à une mesure, à une certaine place.
 *	  Si id dépasse le nombre de notes, ne fait rien
 *	  S'il n'y a pas assez de silences à l'endroit, la fonction supprimera les notes suivantes
 *	  au besoin.
 *	  Si la note dépasse de la mesure, la fonction renvoie la durée restante à ajouter sur 
 *	  la mesure suivante. Résultat qui sera utilisé par Staff_AddNote().
 *
 * \param step La mesure
 * \param id Position où ajouter la nouvelle note
 * \param note Hauteur de la nouvelle note
 * \param flags Attributs de la nouvelle note
 * \param duration Durée de la nouvelle note
 *
 * \return -1 en cas d'erreur, 0 en cas de succès, > 0 s'il reste de la durée de la note.
 */
int Step_AddNote(Step *step, int id, char note, Note_Flags flags, Note_Duration duration);


/**
 * \brief Supprime une portion de note entre begin et end compris.
 *
 * \param step La mesure
 * \param begin Position où démarrer la suppression
 * \param end Position où termienr la suppression.
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int Step_DelLocal(Step *step, int begin, int end);


/**
 * \brief Vérifie si les notes d'une mesure respectes bien la durée globales autorisée.
 *
 * \param step La mesure à vérifier
 *
 * \return 1 s'il y a respect, 0 sinon.
 */
int Step_Verif(Step *step);


/**
 * \brief Récupère une clé à partir d'une chaîne de caractère
 *
 * \param str La chaîne.
 *
 * \return La clé
 */
Cle Cle_GetFromString(const char *str);


/**
 * \brief Récupère le nom de la clé sous forme d'une chaîne
 *
 * \param cle La clé
 *
 * \return La chaîne, NULL si échec.
 */
char *Cle_GetFromId(Cle cle);


/**
 * \brief Transpose toute une liste de notes.
 *
 * \param tonote La liste
 * \param value Nombre de demi-tons (entier) à transposer.
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int ToNote_Transpose(ToNote *tonote, char value);


/**
 * \brief Transpose toute les notes d'une mesure
 *
 * \param step La mesure
 * \param value Nombre de demi-tons (entier) à transposer.
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int Step_Transpose(Step *step, char value);


/**
 * \brief Retourne la durée minimal d'une note/silence dans une mesure
 *	  Est utilisé notamment pour l'affichage.
 *
 * \param step La mesure
 *
 * \return La durée minimale calculée
 */
Note_Duration Step_GetMinDuration(Step *step);


/**
 * \brief Récupère une note extraite d'une mesure
 *
 * \param step Mesure
 * \param id_note Position de la note à récupérer
 *
 * \return La note, NULL si introuvable
 */
Note *Step_GetNote(Step *step, int id_note);


/**
 * \brief Vérifie la cohérence de l'armure d'une mesure
 *
 * \param step La mesure
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int Step_VerifArmFlags(Step *step);


/** 
 * \brief Renvoie la durée totale des notes présentes avant la position id_note
 *
 * \param step La mesure
 * \param id_note position
 *
 * \return Durée totale avant id_note, 0 en cas d'erreur
 */
int Step_DurationBefore(Step *step, int id_note);


/**
 * \brief Renvoie la ToNote située après une certaine durée.
 *
 * \param step La mesure
 * \param duration Durée totale
 *
 * \return La note, NULL si introuvable.
 */
ToNote *Step_GetToNoteAfterDuration(Step *step, int duration);

/**
 * \brief Renvoie la note située après une certaine durée.
 *
 * \param step La mesure
 * \param duration Durée totale
 *
 * \return La note, NULL si introuvable.
 */
Note *Step_GetNoteAfterDuration(Step *step, int duration);


#endif




