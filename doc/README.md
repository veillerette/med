# med
MusicEDitor
Objectifs


<<<<<-------------------------------------------->>>>>
<<<<<-------------------------------------------->>>>>

Mi-Octobre (15-17) 2016


-> Notes

	- Hauteurs
	
	- Durées
	
	- Flags (altérations ...)
	
	- Silences
	

-> Mesures

	- Ajout d'une note
	
	- Suppression d'une ou plusieurs note(s)
	
	- Vidage
	
	- Fonctions de vérifications
	
	- Gestion des clés, armures
	
	- Division temporelle
	
	- Flags des mesures
	

-> Portées

	- Gestion des portées simples

	- Gestion des multi-portées (struct Score)
	

-> Fonctions de tests au fur et à mesure de l'avancé (module "Test")



<<<<<-------------------------------------------->>>>>
<<<<<-------------------------------------------->>>>>



Octobre 2016



-> Mesures

	- Transpositions
	
	- Gestion de plusieurs voies par mesure (tableaux de ToNote*)


-> Portées
	
	- Ajout d'une note sur plusieurs mesures (Liaisons)


-> Fonctions de tests




<<<<<-------------------------------------------->>>>>
<<<<<-------------------------------------------->>>>>



Début Novembre 2016



-> SDL (video)

	- Gestion de la Fenêtre
	
		- 3 Structures principales (menu, palette, corps)
		
		- SDL_Thread...

	- Quelques surcouches qui facilitent la vie
	
		- Boutons
		
		- Menu auto
		

	- Affichage (Généré par SDL)
	
		- Lignes de portées
		
		- Clés
		
		- Armures
		
		- Indication de mesure
		
		- Ronde, Blanche, Noire, Croche, Double-Croche, Triple-Croche, Quadruple-Croche
		
		- Silences associés
		
		- Séparations de mesure
		
		- Points
		

-> Exportation / Importation fichier perso.

-> Explorateur fichiers


<<<<<-------------------------------------------->>>>>
<<<<<-------------------------------------------->>>>>



Novembre 2016


-> SDL (video)

	- Affichage (Généré par SDL)
	
		- Liaisons
		
		- Articulations
		
		- Autres signes ...
		
		- Curseur
		

	- Evenements sur la partition :
	
		- Ajout/Suppr note (Souris + SUPPR/DEL)
		
		- Selection(CTRL, voire CTRL+E comme FL ?)


	
-> SDL (audio)

	- Génération
	
		- Génération fréquence libre sinusoide
		
		- Enchainement fréquences
		
		- Génération fréquence libre carrée
		
		- Mixage de deux sons générés

	- Personnalisation des fréquences



<<<<<-------------------------------------------->>>>>
<<<<<-------------------------------------------->>>>>


	
