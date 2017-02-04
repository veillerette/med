/** \~french
 * \file ABC.h
 * \brief Ouverture, Sauvegarde de fichiers au format ABC.
 * \author Victor Veillerette
 * \version 1
 * \data Janvier 2017
 */

#ifndef __HEAD_ABC__
#define __HEAD_ABC__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Staff.h"

/*********************************
	   DEV FUNCTIONS
**********************************/

/**
 * \brief Scan la prochaine entête d'un fichier ABC.
 */
int ABC_ScanHead(FILE *f, char *field, char *text);

/**
 * \brief Supprime le Slash d'une chaîne de caractères
 */
void ABC_replaceSlash(char *str);

/**
 * \brief Trouve l'armure à partir d'une tonalité sous forme texte.
 */
int ABC_FindKeyFromStr(char *text);

/**
 * \brief Vérifie si le caractère note est bien le nom d'une note
 */
int ABC_isNote(char note);

/**
 * \brief Parse toutes les entêtes d'un fichier ABC
 */
int ABC_ParseHeader(Score *score, FILE *f);

/**
 * \brief Récupère le prochain caractère d'un fichier sans faire avancer le curseur de lecture
 */
int nextcar(FILE *f);

/**
 * \brief Récupère la prochaine line d'un fichier avec \n en faisant avancer le curseur
    	  Bugs connus avec les retours chariots de Windows et Mac \r
 */
char *File_GetLine(FILE *f);

/**
 * \brief Vérifie si une line est de la forme d'une entête ABC
 */
int ABC_IsAnHeader(const char *line);

/**
 * \brief Vérifie si une ligne d'entête a une signification en ABC et contient les bons champs.
 */
int ABC_IsAGoodHeader(const char *line);

/**
 * \brief Supprime d'une ligne de notes ABC les notes simultanées.
 */
int ABC_MultiSimplify(char *buf);

/**
 * \brief Transforme et Simplifie une ligne ABC en une nouvelle ligne compréhensible par le parseur
 */
char *ABC_TransformLine(const char *line, int isHeader);

/**
 * \brief Ajoute une chaine à un tableau de chaine déjà alloué
 */
int ABC_AppendStr(char **dest, char *source);

/**
 * \brief Simplifie un fichier ABC sourcePath et sauvegarde le résultat dans destPath
 */
int File_SimplifyABC(const char *destPath, const char *sourcePath);

/**
 * \brief Parse un fichier ABC simplifié par SimplifyABC, et renvoie la partition résultante
 */
extern Score *ABC_ParseFile(const char *path);


/**
 * \brief Ecrite une mesure au format ABC
 *
 * \param f Le fichier destinataire
 * \param step La mesure à écrire
 * \param base_l La base de temps d'une note pour ce fichier
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int ABC_WriteStep(FILE *f, Step *step, Note_Duration base_l);


/**
 * \brief Ecrit les entêtes du fichier ABC
 *
 * \param f Le fichier destinataire
 * \param score La partition
 * \param base_l La base de temps d'une note pour ce fichier
 *
 * \return 1 en cas de succès, 0 sinon.
 */
int ABC_WriteHeaderScore(FILE *f, Score *score, Note_Duration basel);


/**
 * \brief Ecrit une partition entière dans un fichier ABC
 * 
 * \param f Le fichier destinataire
 * \param score La partition à écrire
 *
 * \return 1 en cas de succès, 0 sinon
 */
int ABC_WriteDevScore(FILE *f, Score *score);


/*********************************
	  MAIN FUNCTIONS
**********************************/

/**
 * \brief Ouvre et Parse un fichier ABC et renvoie la partition associée.
 *	  Remplie la variable globale abc_error en cas de problème.
 *
 * \param path Path du fichier à ouvrir
 *
 * \return La partition si succès, NULL si échec
 */
extern Score *ABC_OpenABC(const char *path);


/**
 * \brief Sauvegarde une partition dans un fichier au format ABC.
 *
 * \param path Path du fichier
 * \param score La partition à sauvegarde
 *
 * \return 1 en cas de succès, 0 sinon.
 */
extern int ABC_WriteScore(const char *path, Score *score);


extern char abc_error[50];

#endif
