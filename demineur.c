/************************************************************************/
/* Auteur : LECLER Quentin									    		*/
/* Groupe : 2102														*/
/* Application : demineur.c												*/
/* Description : réalise le jeu du démineur								*/
/* Date de dernière mise à jour : 19/02/10								*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAXLIG		15
#define MAXCOL		15
#define MAXBMB		30
#define MINE		'*'
#define INIT		'.'
#define DESAMORCE	'D'
#define VIDE		' '

/************************************************************************/

int genererNombre(int min, int max);
void saisirCoordonnees(int *xUser, int *yUser, int tailleLig, int tailleCol);
char afficherMenu();

void initialiserGrille(char *grille, char init, int tailleLig, int tailleCol);
void afficherGrille(char *grille, int tailleLig, int tailleCol);
void genererMines(char *grille, char mine, char init, int nombreMine, \
	int tailleLig, int tailleCol);
void ajouterValeurs(char *grille, char mine, char vide, \
	int tailleLig, int tailleCol);
int devoilerMines(char *grille, char mine, int xUser, int yUser, \
	int tailleLig, int tailleCol);
void decouvrirCases(char *grilleJeu, char *grilleMines, char mine, \
	char desamorce, char vide, int xUser, int yUser, int tailleLig, int tailleCol);

/************************************************************************/

int main(int argc, char *argv[])
{
	char grilleJeu[MAXLIG][MAXCOL]; /* grille de jeu */
	char grilleMines[MAXLIG][MAXCOL]; /* grille de mines */
	char recommencer; /* flag pour recommencer une nouvelle partie */
	char choix; /* choix de l'utilisateur dans le menu */
	int xUser; /* coordonnée X saisie par l'utilisateur */
	int yUser; /* coordonnée Y saisie par l'utilisateur */
	int minesDesamorcee; /* compteur de mines désamorçées */

	recommencer = 'O';
	minesDesamorcee = 0;

	srand((unsigned int) time(NULL));

	printf("Bienvenue dans le jeu du demineur !\n");
	printf("___________________________________\n");

	/*********************/
	/* boucle principale */
	/*********************/

	do
	{
		/******************************/
		/* initialisation des grilles */
		/******************************/

		initialiserGrille(&grilleJeu[0][0], INIT, MAXLIG, MAXCOL);
		initialiserGrille(&grilleMines[0][0], INIT, MAXLIG, MAXCOL);

		/**********************************/
		/* génération aléatoire des mines */ 
		/**********************************/

		genererMines(&grilleMines[0][0], MINE, INIT, MAXBMB, MAXLIG, MAXCOL);

		/************************************************/
		/* ajout du nombre de mines entourant les cases */
		/************************************************/

		ajouterValeurs(&grilleMines[0][0], MINE, VIDE, MAXLIG, MAXCOL);

		/*******************************/
		/* affichage du menu principal */
		/*******************************/

		do
		{
			choix = afficherMenu();

			switch(choix)
			{
			case 'D':
				printf("\n");
				afficherGrille(&grilleJeu[0][0], MAXLIG, MAXCOL);
				printf("\n");
				saisirCoordonnees(&xUser, &yUser, MAXLIG, MAXCOL);
				printf("\n");

				/**************************************************/
				/* si la case est une mine la partie est terminée */ 
				/**************************************************/

				if(grilleMines[xUser-1][yUser-1] == MINE)
				{
					choix = 'Q';
					printf("La case est une mine ! Vous avez perdu.\n");
				}

				else
				{
					decouvrirCases(&grilleJeu[0][0], &grilleMines[0][0], MINE, \
						DESAMORCE, VIDE, xUser, yUser, MAXLIG, MAXCOL);
					afficherGrille(&grilleJeu[0][0], MAXLIG, MAXCOL);
				}

				break;

			case 'P':
				printf("\n");
				afficherGrille(&grilleJeu[0][0], MAXLIG, MAXCOL);
				printf("\n");
				saisirCoordonnees(&xUser, &yUser, MAXLIG, MAXCOL);
				printf("\n");

				/*************************************************/
				/* si la case est une mine, elle sera désamorçée */
				/*************************************************/

				if(grilleMines[xUser-1][yUser-1] == MINE)
				{
					grilleJeu[xUser-1][yUser-1] = DESAMORCE;
					grilleMines[xUser-1][yUser-1] = DESAMORCE;
					printf("La bombe a ete desamorcee.\n");
					minesDesamorcee++;
				}

				/********************************/
				/* sinon la partie est terminée */
				/********************************/

				else
				{
					printf("La case n'est pas une mine ! Vous avez perdu.\n");
					choix = 'Q';
				}
				break;
			}

		/**************************************************/
		/* tant qu'on a pas perdu et que toutes les mines */
		/* n'ont pas été désamorçées 					  */
		/**************************************************/

		} while((choix != 'Q') && (minesDesamorcee != MAXBMB));

		/****************************************************/
		/* on demande à l'utilisateur s'il veut recommencer */
		/****************************************************/

		do
		{
			fflush(stdin); /* clear du buffer d'entrée */
			printf("\nVoulez-vous recommencer (o/ n) ? ");
			recommencer = toupper(getchar());
		} while((recommencer != 'O') && (recommencer != 'N'));
	} while(recommencer == 'O');

	fflush(stdin); /* clear du buffer d'entrée */
	printf("\nAppuyez sur une touche pour continuer... "); \
	getchar();

	return 0;
}

/************************************************************************/
/* INPUT : nombre minimum à générer (min)								*/
/*		   nombre maximum à générer (max)								*/
/* PROCESS : génération aléatoire d'un nombre compris entre	min et max 	*/
/* OUTPUT : nombre aléatoire compris entre min et max					*/
/************************************************************************/

int genererNombre(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

/************************************************************************/
/* INPUT : coordonnée X saisie par l'utilisateur (*xUser)				*/
/*		   coordonnée Y saisie par l'utilisateur (*yUser)				*/
/*		   nombre de lignes de la grille (tailleLig)					*/
/*		   nombre de colonnes de la grille (tailleCol)					*/
/* PROCESS : saisie des coordonnées de la case de la grille			 	*/
/* OUTPUT : /															*/
/************************************************************************/

void saisirCoordonnees(int *xUser, int *yUser, int tailleLig, int tailleCol)
{
	do
	{
		printf("Veuillez saisir la coordonnee en X (%d ~ %d) : ", 1, tailleLig);
		scanf("%d", xUser);
	} while((*xUser < 1) || (*xUser > tailleLig));

	do
	{
		printf("Veuillez saisir la coordonnee en Y (%d ~ %d) : ", 1, tailleCol);
		scanf("%d", yUser);
	} while((*yUser < 1) || (*yUser > tailleCol));
}

/************************************************************************/
/* INPUT : /															*/
/* PROCESS : demande à l'utilisateur de faire un choix dans le menu		*/
/* OUTPUT : choix de l'utilisateur										*/
/************************************************************************/

char afficherMenu()
{
	char choix;

	printf("\n(D)ecouvrir une case.\n");
	printf("(P)lacer un drapeau.\n");
	printf("(Q)uitter.\n\n");

	fflush(stdin); /* clear du buffer d'entrée */
	printf("Votre choix ? ");
	choix = getchar();

	return toupper(choix);
}

/************************************************************************/
/* INPUT : grille (*grille)												*/
/*		   valeur d'initialisation (init)								*/
/*		   nombre de lignes de la grille (tailleLig)					*/
/*		   nombre de colonnes de la grille (tailleCol)					*/
/* PROCESS : initialisation de la grille de jeu						 	*/
/* OUTPUT : /															*/
/************************************************************************/

void initialiserGrille(char *grille, char init, int tailleLig, int tailleCol)
{
	int compteurLig;
	int compteurCol;

	for(compteurLig = 0; compteurLig < tailleLig; compteurLig++)
	{
		for(compteurCol = 0; compteurCol < tailleCol; \
			compteurCol++, grille++)
		{
			*grille = init;
		}
	}
}

/************************************************************************/
/* INPUT : grille (*grille)												*/
/*		   nombre de lignes de la grille (tailleLig)					*/
/*		   nombre de colonnes de la grille (tailleCol)					*/
/* PROCESS : affichage de la grille de jeu							 	*/
/* OUTPUT : /															*/
/************************************************************************/

void afficherGrille(char *grille, int tailleLig, int tailleCol)
{
	int compteurLig;
	int compteurCol;

	for(compteurCol = 0; compteurCol < tailleCol+2; compteurCol++)
	{
		printf("%% ");
	}

	printf("\n");

	for(compteurLig = 0; compteurLig < tailleLig; compteurLig++)
	{
		printf("%% ");
		for(compteurCol = 0; compteurCol < tailleCol; \
			compteurCol++, grille++)
		{
			printf("%c ", *grille);
		}
		printf("%%\n");
	}

	for(compteurCol = 0; compteurCol < tailleCol+2; compteurCol++)
	{
		printf("%% ");
	}

	printf("\n");
}

/************************************************************************/
/* INPUT : grille de mines (*grille)									*/
/*		   valeur donnée à une mine (mine)								*/
/*		   valeur d'initialisation (init)								*/
/*		   nombre de mines à ajouter dans la grille (nombreMines)		*/
/*		   nombre de lignes de la grille (tailleLig)					*/
/*		   nombre de colonnes de la grille (tailleCol)					*/
/* PROCESS : ajout aléatoire de mines dans la grille				 	*/
/* OUTPUT : /															*/
/************************************************************************/

void genererMines(char *grille, char mine, char init, int nombreMines, \
	int tailleLig, int tailleCol)
{
	int compteurMines;
	int ligne;
	int colonne;

	for(compteurMines = 0; compteurMines < nombreMines; \
		compteurMines++)
	{
		do
		{
			ligne = genererNombre(1, tailleLig);
			colonne = genererNombre(1, tailleCol);
		} while(*(grille + ((ligne) * colonne)) != init);
		/* on place la mine dans la grille */
		*(grille + ((ligne) * colonne)) = mine;
	}
}

/************************************************************************/
/* INPUT : grille de mines (*grille)									*/
/*		   valeur donnée à une mine (mine)								*/
/*         valeur donnée à une case sans mine(s) voisine (vide)			*/
/*		   nombre de lignes de la grille (tailleLig)					*/
/*		   nombre de colonnes de la grille (tailleCol)					*/
/* PROCESS : ajout du nombre de mines entourant chacune des cases		*/
/* OUTPUT : /															*/
/************************************************************************/

void ajouterValeurs(char *grille, char mine, char vide, \
	int tailleLig, int tailleCol)
{
	int compteurLig;
	int compteurCol;

	for(compteurLig = 1; compteurLig <= tailleLig; compteurLig++)
	{
		for(compteurCol = 1; compteurCol <= tailleCol; \
			compteurCol++, grille++)
		{
			if(*grille != mine)
			{
				*grille = '0' + devoilerMines(grille, mine, compteurLig, \
					compteurCol, tailleLig, tailleCol);

				if(*grille == '0')
				{
					*grille = vide;
				}
			}
		}
	}
}

/************************************************************************/
/* INPUT : grille de mines (*grille)									*/
/*		   valeur d'initialisation donnée à une mine (mine)				*/
/*		   coordonnée X saisie par l'utilisateur (xUser)				*/
/*		   coordonnée Y saisie par l'utilisateur (yUser)				*/
/*		   nombre de lignes de la grille (tailleLig)					*/
/*		   nombre de colonnes de la grille (tailleCol)					*/
/* PROCESS : dévoile le nombre de mines autour d'une case			 	*/
/* OUTPUT : le nombre de mines autour de la case						*/
/************************************************************************/

int devoilerMines(char *grille, char mine, int xUser, int yUser, \
	int tailleLig, int tailleCol)
{
	int compteurMines;

	compteurMines = 0;

	if(xUser > 1)
	{
		/********************************************/
		/* présence d'une mine au dessus de la case */
		/********************************************/

		if(*(grille-tailleCol) == mine)
		{
			compteurMines++;
		}
	}

	if(xUser < tailleLig)
	{
		/*********************************************/
		/* présence d'une mine en dessous de la case */
		/*********************************************/

		if(*(grille+tailleCol) == mine)
		{
			compteurMines++;
		}
	}

	if(yUser > 1)
	{
		/*******************************************/
		/* présence d'une mine à gauche de la case */
		/*******************************************/

		if(*(grille-1) == mine)
		{
			compteurMines++;
		}
	}

	if(yUser < tailleCol)
	{
		/*******************************************/
		/* présence d'une mine à droite de la case */
		/*******************************************/

		if(*(grille+1) == mine)
		{
			compteurMines++;
		}
	}

	if((xUser > 1) && (yUser > 1))
	{
		/******************************************/
		/* présence d'une mine au dessus à gauche */
		/******************************************/

		if(*(grille-tailleCol-1) == mine)
		{
			compteurMines++;
		}
	}

	if((xUser > 1) && (yUser < tailleCol))
	{
		/******************************************/
		/* présence d'une mine au dessus à droite */
		/******************************************/

		if(*(grille-tailleCol+1) == mine)
		{
			compteurMines++;
		}
	}

	if((xUser < tailleLig) && (yUser > 1))
	{
		/*******************************************/
		/* présence d'une mine en dessous à gauche */
		/*******************************************/

		if(*(grille+tailleCol-1) == mine)
		{
			compteurMines++;
		}
	}

	if((xUser < tailleLig) && (yUser < tailleCol))
	{
		/*******************************************/
		/* présence d'une mine en dessous à droite */
		/*******************************************/

		if(*(grille+tailleCol+1) == mine)
		{
			compteurMines++;
		}
	}

	return compteurMines;
}

/************************************************************************/
/* INPUT : grille de jeu (*grilleJeu)									*/
/*		   grile de mines (*grilleMines)								*/
/*         valeur donnée à une mine (mine)								*/
/*         valeur donnée à une mine désamorçée (desamorce)				*/
/*         valeur donnée à une case sans bombe(s) voisine (vide)		*/
/*		   coordonnée X saisie par l'utilisateur (xUser)				*/
/*		   coordonnée Y saisie par l'utilisateur (yUser)				*/
/*		   nombre de lignes de la grille (tailleLig)					*/
/*		   nombre de colonnes de la grille (tailleCol)					*/
/* PROCESS : découvre le nombre de mines autour des cases			 	*/
/* OUTPUT : le nombre de mines autour de la case						*/
/************************************************************************/

void decouvrirCases(char *grilleJeu, char *grilleMines, char mine, \
	char desamorce, char vide, int xUser, int yUser, int tailleLig, int tailleCol)
{
	int ligne;
	int colonne;

	/*****************************************/
	/* on vérifie qu'on reste dans la grille */
	/*****************************************/

	if((xUser >= 1) && (xUser <= tailleLig) && (yUser >= 1) && (yUser <= tailleCol))
	{
		/*********************************************/
		/* on vérifie que la case n'est pas une mine */
		/*********************************************/

		if(*(grilleMines + xUser * tailleLig - (tailleCol - yUser) - 1) != mine && \
			*(grilleMines + xUser * tailleLig - (tailleCol - yUser) - 1) != desamorce)
		{
			/****************************************************/
			/* si la case est voisine à au moins une mine, 		*/
			/* elle indiquera combien de bombe(s) l'entoure(nt) */
			/****************************************************/

			if(*(grilleMines + xUser * tailleLig - (tailleCol - yUser) - 1) != vide)
			{
				*(grilleJeu + xUser * tailleLig - (tailleCol - yUser) - 1) = \
					*(grilleMines + xUser * tailleLig - (tailleCol - yUser) - 1);
			}

			/*****************************************************************/
			/* si la case n'a pas de bombe voisine elle dévoile ses voisines */
			/*****************************************************************/

			else
			{
				*(grilleJeu + xUser * tailleLig - (tailleCol - yUser) - 1) = vide;
				for(ligne = xUser-1; ligne <= xUser+1; ligne++)
				{
					for(colonne = yUser-1; colonne <= yUser-1; colonne++)
					{
						if((ligne != xUser) || (colonne != yUser))
						{
							decouvrirCases(grilleJeu, grilleMines, mine, \
								desamorce, vide, ligne, colonne, tailleLig, tailleCol);
						}
					}
				}
			}
		}
	}
}