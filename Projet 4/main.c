//
//  main.c
//  Projet 4
//
//  Created by Paul-Henri Zimmerlin on 04/05/2017.
//  Copyright © 2017 Paul-Henri Zimmerlin. All rights reserved.
//

#include <stdio.h>
#include "Bibliotheque.h"
#define FLAG_HORIZONTAL 1
#define FLAG_VERTICAL 0

typedef struct {
    int x_debut;
    int y_debut;
    int x_fin;
    int y_fin;
} Mot;

int insererMotGrille(char **tab, int *taillex, int *tailley, char *mot){
    for(int i = 0; i < *taillex; i++){
        for(int j = 0; j < *tailley; j++){
            if(accederTab2D(tab, *taillex, *tailley, i, j) != '*'){
                int dir = detectOrientation(tab, taillex, tailley, i, j);
            }
        }
    }
    return 1;
}

Mot DetectPossibilites(char **tab, int taillex, int tailley, char *mot, int posx, int posy, int orientation){
    Mot newmot;
    int i = posx,j = posy;
    if(orientation == FLAG_HORIZONTAL){
        newmot.x_debut = posx;
        newmot.x_fin = posx;
        while(tab[i][j] != '*'){
            newmot.y_debut = j;
            j--;
        }
        j = posy;
        while(tab[i][j] != '*'){
            newmot.y_fin = j;
            j++;
        }
    }
    if(orientation == FLAG_VERTICAL){
        newmot.y_debut = posy;
        newmot.y_fin = posy;
        while(tab[i][j] != '*'){
            newmot.x_debut = i;
            i--;
        }
        j = posy;
        while(tab[i][j] != '*'){
            newmot.x_fin = i;
            i++;
        }
    }
    return newmot;
}

int detectOrientation(char **tab, int taillex, int tailley, int posx, int posy){
    if((accederTab2D(tab, taillex, tailley, posx - 1, posy) != '*'|| accederTab2D(tab, taillex, tailley, posx + 1, posy) != '*') && (accederTab2D(tab, taillex, tailley, posx, posy - 1) == '*'|| accederTab2D(tab, taillex, tailley, posx, posy + 1) == '*')) return FLAG_VERTICAL;
    if((accederTab2D(tab, taillex, tailley, posx, posy - 1) != '*'|| accederTab2D(tab, taillex, tailley, posx, posy + 1) != '*') && (accederTab2D(tab, taillex, tailley, posx - 1, posy) == '*'|| accederTab2D(tab, taillex, tailley, posx + 1, posy) == '*')) return FLAG_HORIZONTAL;
    return -1;
}
void MotsCroises(){
    /**int longueur, largeur;
     printf("Saisir la largeur ");
     scanf("%d", &largeur);
     printf("\n");
     printf("Saisir la longueur ");
     scanf("%d", &longueur);
     printf("\n");*/
    int taillex = 0;
    int tailley = 0;
    char **tab = creerTab2D(taillex, tailley);
    afficherTab2D(tab, taillex, tailley);
    char **lexique = chargerLexique("/Users/Paul-Henri/ownCloud/Cours EFREI/Semestre 2/Algorithmique/Projet 4/Projet 4/dico.txt", calculerTailleLexique("/Users/Paul-Henri/ownCloud/Cours EFREI/Semestre 2/Algorithmique/Projet 4/Projet 4/dico.txt"));
    insererMotDansTab2D(&tab, &taillex, &tailley, 0, 5, 0, lexique[0]);
    insererMotDansTab2D(&tab, &taillex, &tailley, 1, 4, 1, lexique[0]);
    afficherTab2D(tab, taillex, tailley);
    printf("%d", detectOrientation(tab, taillex, tailley, 1, 4));
}


int main(int argc, const char * argv[]) {
    MotsCroises();
    return 0;
}
