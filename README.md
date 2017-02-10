# MusicalEditor

### Présentation

MusicalEDitor est un éditeur de partition musicale simple écrit en C et utilisant la librairie SDL 1.2


### Calendrier



#### 2 Octobre 2016
* Création des structures :
    * Note : Représente une hauteur, des flags (altérations, pointée, liée...), une durée. Elle permet de représenter un silence également. (rest = 1)
    * ToNote : Liste chainée représentant une suite de note ou de silences (une voix)
    * Step : Représente une mesure (clé, armure, clé, la division temporelle, des flags pour les signes spéciaux)
* Fonctions d'Allocations / Free associées
* Ajout d'une note à une mesure
* Suppression d'une ou plusieurs note/silence d'une mesure
* Regularisation d'une mesure (suite à un ajout/suppression)



#### 4 Octobre 2016
* Conversion d'une note donné en string (ex: "C4") en son identifiant midi (champ char note;)
    * Format (Note anglaise) : "F2" = "Es2", "B3" = "Cf4"
    * Notes en minuscule possibles
    * Altérations : "s" (sharp) pour Dieze, "ss" pour Double-Dieze "f" (flat) pour Bémol, "ff" pour Double-Bémol
* Fonction de vérification du nombre de temps d'une mesure
* Gestion des notes pointées



#### 5 Octobre 2016
* Fonctions de tests d'allocation / free des Notes, Steps et ToNote
* Créations des structures :
    * Staff : Représente une portée composée de plusieurs Steps. La portée possède un nom (nom d'instrument ou nom de piste)
    * Score (non complète * pas de fonctions) : Représente un ensemble de Staff, représente donc l'ensemble d'une partition
* Fonctions d'allocation/free/Initialisation d'un Staff
* Ajout/Suppression de Steps à un Staff
* Transposition entière d'un Staff


#### 8 Octobre 2016
* Correction de la répartition des silences lors de l'ajout d'une note (On obtient maintenant "Croche * Demi-Soupir * Soupir * Demi-Pause" au lieu de "Croche * Demi-Pause * Soupir * Demi-Soupir")
* Fonctions d'allocation/free/initialisation de la structure Score
* Création de la structure :
    * Sign : Représente les signs qui ne sont pas liés à une note/mesure/portée. Par exemple : Titre, Tempo, Auteur...
* Fonctions d'allocation/free.. de la structure Sign.
* Ajout de fonctions de test ainsi que des tailles des structures/enum (avec l'argument -test)

#### 9 Octobre 2016
* Creation des modules :
    * Window : Sert à gérer la fenêtre graphique, l'affichage, les événements (souris, clavier...) en SDL
    * Images :Permet de générer les images des notes, portées et de tous les signes graphiques en SDL
* Création des structures :
    * WindowData : Représente les données de la fenêtre principale. (Surface, dimensions...)
    * Color : Représente une couleur(rgba)
* Allocation/Libération des structures ci-dessus.
* Initiation/Quittage du moteur graphique
* Ouverture de la fenêtre, gestion des trois structures d'affichages
* Fonction d'attente de la souris, Nettoyage de l'écran

#### 10-20 Octobre 2016
* Step
    * Conversion d'un string en une note (gestion des Flats/Sharps) 
* Staff
    * Ajout d'une note/silence à n'importe quelle mesure
* Générations des Images
    * Notes Simples (tête noire/blanche/ronde + queue + crochets) + Pause/Demi-Pause
    * Lignes de portées
* Tests d'affichages
    * Portée
    * Note simple (ronde, blanche, noire, croche*quadruple)
    * Silence (pause, demi-pause seulement)
    * Lignes supplémentaires
    * Affichage d'un Score
* Fenêtre
    * Agrandissement / Réduction de la taille de la partition
    * Déplacement horizontal de la partition
* Makefile
    * Amélioration du makefile (modules en .o)


#### Fin Novembre 2016
* Menu
    * Gestion d'un menu horizontal (création, ajout, sélection)
    * Mise en avant du menu au premier plan
*

#### Début Décembre 2016
* Interface
    * Création de la toolbar et de deux modes : 'A'jout et 'E'dition
    * Sélection d'une durée de note à rajouter dans la toolbar
    * Ajout d'une note à la portée avec le clic de la souris
* Bugs
    * Corrections de nombreuses fuites mémoires
    * Correction des erreurs des plans


#### Mi-Décembre 2016
* Audio
    * Premiers test audio
    * Création d'ondes sinusoïdales, carrés, triangulaires et en dents de scie

### Objectifs

### Mi-Octobre 2016


* Notes
    * Hauteurs
    * Durées
    * Flags (altérations ...)
    * Silences
* Mesures
    * Ajout d'une note
    * Suppression d'une ou plusieurs note(s)
    * Vidage
    * Fonctions de vérifications
    * Gestion des clés, armures
    * Division temporelle
    * Flags des mesures
* Portées
    * Gestion des portées simples
    * Gestion des multi-portées (struct Score)
* Fonctions de tests au fur et à mesure de l'avancé (module "Test")

#### Octobre 2016

* Mesures
    * Transpositions
    * Gestion de plusieurs voies par mesure (tableaux de ToNote*)
* Portées
    * Ajout d'une note sur plusieurs mesures (Liaisons)
* Fonctions de tests


#### Début Novembre 2016


* Gestion de la Fenêtre
    * 3 Structures principales (menu, palette, corps)
    * SDL_Thread...
* Quelques surcouches qui facilitent la vie
    * Boutons
    * Menu auto
* Affichage (Généré par SDL)
    * Lignes de portées
    * Clés
    * Armures
    * Indication de mesure
    * Ronde, Blanche, Noire, Croche, Double-Croche, Triple-Croche, Quadruple-Croche
    * Silences associés
    * Séparations de mesure
    * Points
* Exportation / Importation fichier perso.
* Explorateur fichiers


#### Novembre 2016

* Affichage (Généré par SDL)
    * Liaisons
    * Articulations
    * Autres signes ...
    * Curseur
* Evenements sur la partition :
    * Ajout/Suppr note (Souris + SUPPR/DEL)
    * Selection(CTRL, voire CTRL+E comme FL ?)
    * Génération
    * Génération fréquence libre sinusoidales
* Enchainement fréquences
    * Génération fréquence libre carrée
    * Mixage de deux sons générés
    * Personnalisation des fréquences


    
