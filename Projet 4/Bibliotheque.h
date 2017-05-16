//
//  Bibliotheque.h
//  Projet 4
//
//  Created by Paul-Henri Zimmerlin on 04/05/2017.
//  Copyright © 2017 Paul-Henri Zimmerlin. All rights reserved.
//

#ifndef Bibliotheque_h
#define Bibliotheque_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAILLE_MAX 30
#define LIGNE 200
#define NOMBRE_DOEUVRES 6


//Création et affichage de tableaux 1D dynamiques

typedef struct oeuvre
{
    char* titre;
    char* auteur;
    char* chemin_dacces;
    int annee;
}oeuvre;


char* creerTab1D(int taille)
{
    char* tab = malloc(taille * sizeof(char));
    
    for (int i = 0; i < taille; ++i)
    {
        tab[i] = '*';
    }
    return tab;
}

void afficherTab1D(char* tab, int taille)
{
    for (int i = 0; i < taille; ++i)
    {
        printf("%c ", tab[i]);
    }
    printf("\n");
}

//Création et affichage de tableaux 2D dynamiques

char** creerTab2D(int taillex, int tailley)
{
    char** tab = malloc(taillex * sizeof(char*));
    
    for (int i = 0; i < taillex; ++i)
    {
        tab[i] = creerTab1D(tailley);
    }
    return tab;
}

void afficherTab2D(char** tab, int taillex, int tailley)
{
    for (int i = 0; i < taillex; ++i)
    {
        afficherTab1D(tab[i], tailley);
    }
}

//Insertion sécurisée de caractères dans la grille

void insererCharDansTab2D_v1(char** tab, int taillex, int tailley, int posx, int posy, char c)
{
    if (posx < taillex && posx >= 0 && posy >= 0 && posy < tailley)
    {
        tab[posx][posy] = c;
    }
}

//Insertion sécurisée de mots dans la grille

void insererMotDansTab2D_v1(char** tab, int taillex, int tailley, int posx, int posy, int horizontal, char* mot)
{
    for(int i = 0; mot[i] != '\0'; i++)
    {
        insererCharDansTab2D_v1(tab, taillex, tailley, posx, posy, mot[i]);
        if (horizontal == 0) //mot écrit en horizontal
        {
            posx++;
        }
        else //mot écrit en vertical
        {
            posy++;
        }
    }
}


void insererCharDansTab2D_v2(char*** tab, int* taillex, int* tailley, int posx, int posy, char c)
{
    // Verifier que les positions sont positives
    if( posx < 0 || posy < 0 ) {
        printf("Impossible d'insérer un caractere a un index négatif [%d,%d]\n", posx, posy);
        return;
    }
    
    // Verifier si la position en Y depasse la taille du tableau sur l'axe Y
    if( posy >= *tailley ) {
        // Pour chaque ligne à agrandir
        for (int i = 0; i < *taillex; ++i)
        {
            // On realloue la ligne avec sa nouvelle taille
            (*tab)[i] = realloc((*tab)[i], (posy + 1) * sizeof(char));
            
            // Pour chaque nouvelle case de la ligne
            for (int j = *tailley; j < posy + 1; j++) {
                // On l'initialise
                (*tab)[i][j] = '*';
            }
        }
        // On met a jour la nouvelle taille de l'axe Y
        *tailley = posy + 1;
    }
    
    // Verifier si la position en X depasse la taille du tableau sur l'axe X
    if(posx >= *taillex) {
        // On realloue le tableau afin d'avoir le nouveau nombre de ligne nécessaire
        *tab = realloc(*tab, (posx + 1) * sizeof(char*));
        // Pour chaque nouvelle ligne à ajouter
        for (int i = *taillex; i < posx + 1; i++)
        {
            // On alloue la ligne et on la stock dans le tableau
            (*tab)[i] = malloc(*tailley * sizeof(char));
            
            // Pour chaque case de la nouvelle ligne
            for (int j = 0; j < *tailley; ++j)
            {
                // On l'initialise
                (*tab)[i][j] = '*';
            }
        }
        // On met a jour la nouvelle taille de l'axe X
        *taillex = posx + 1;
    }
    // Maintenant que les positions existent dans le tableau, on met à jour la valeur de la case
    (*tab)[posx][posy] = c;
}



void insererMotDansTab2D_v2(char*** tab, int* taillex, int* tailley, int posx, int posy, int horizontal, char* mot)
{
    for(int i = 0; mot[i] != '\0'; i++)
    {
        insererCharDansTab2D_v2(tab, taillex, tailley, posx, posy, mot[i]);
        if (horizontal == 0) //mot écrit en horizontal
        {
            posx++;
        }
        else //mot écrit en vertical
        {
            posy++;
        }
    }
}

char accederTab2D(char** tab, int taillex, int tailley, int posx, int posy)
{
    if (posx < taillex && posx >= 0 && posy >= 0 && posy < tailley)
    {
        return tab[posx][posy];
    }
    else
    {
        return '*'; //évite les seg fault
    }
}

//Dictionnaire (lexique)
//Chargement du lexique

int calculerTailleLexique(char *filename)
{
    int nb_mots = 0;
    FILE* fichier = NULL;
    char* chaine = malloc(TAILLE_MAX * sizeof(char));
    
    fichier = fopen(filename, "r");
    if (fichier != NULL)
    {
        while(fgets(chaine, TAILLE_MAX, fichier) != NULL)
        {
            nb_mots++;
        }
        fclose(fichier);
    }
    else
    {
        printf("Error : Le fichier se nommant %s est introuvable\n", filename);
        return -1;
    }
    return nb_mots;
}

char** chargerLexique(char* filename, int nb_mots)
{
    FILE* fichier = NULL;
    char** tab = NULL;
    char* chaine = NULL;
    
    fichier = fopen(filename, "r");
    if(fichier != NULL)
    {
        tab = malloc(nb_mots * sizeof(char*));
        for (int i = 0; i < nb_mots; ++i)
        {
            chaine = malloc(TAILLE_MAX * sizeof(char));
            fgets(chaine, TAILLE_MAX, fichier);
            chaine = strtok(chaine, "\n");
            tab[i] = chaine;
        }
        fclose(fichier);
    }
    else
    {
        printf("Error : Le fichier se nommant %s est introuvable\n", filename);
    }
    return tab;
}

//Normalisation
//Normalisation de mot

int compterCaracteresChaine(char* chaine)
{
    int caractere = 0;
    
    while(chaine[caractere] != '\0')
    {
        caractere++;
    }
    return caractere;
}

void decalerCaracteresDroite(char* chaine, int index)
{
    int nb_char = compterCaracteresChaine(chaine);
    
    chaine = realloc(chaine, nb_char + 1);
    for (int i = nb_char + 1; i > index ; --i)
    {
        chaine[i] = chaine[i - 1];
    }
}

void decalerCaracteresGauche(char* chaine, int index)
{
    int nb_char = compterCaracteresChaine(chaine);
    
    for (int i = index; i < nb_char; ++i)
    {
        chaine[i] = chaine[i + 1];
    }
}

void supprimerCaracteresSymboles(char* chaine)
{
    int i = 0;
    
    while(chaine[i] != '\0')
    {
        if ((chaine[i] >= (char) 33 && chaine[i] <= (char) 45)
            || (chaine[i] == (char) 47)
            || (chaine[i] >= (char) 58 && chaine[i] <= (char) 64)
            || (chaine[i] >= (char) 91 && chaine[i] <= (char) 96)
            || (chaine[i] >= (char) 123 && chaine[i] <= (char) 126))
        {
            decalerCaracteresGauche(chaine, i);
        }
        else
        {
            i++;
        }
    }
}

void supprimerCaracteresNumeriques(char* chaine)
{
    int i = 0;
    
    while(chaine[i] != '\0')
    {
        if (chaine[i] >= (char) 48 && chaine[i] <= (char) 57)
        {
            decalerCaracteresGauche(chaine, i);
        }
        else
        {
            i++;
        }
    }
}

void supprimerSymbolesDiacritiques(char* chaine)
{
    for(int i = 0; chaine[i] != '\0'; i++)
    {
        if (chaine[i] == (char) 135)
        {
            chaine[i] = 'c';
        }
        else if (chaine[i] == (char) 129 || chaine[i] == (char) 150 || chaine[i] == (char) 151 || chaine[i] == (char) 163)
        {
            chaine[i] = 'u';
        }
        else if ((chaine[i] >= (char) 129 && chaine[i] <= (char) 134) || chaine[i] == (char) 160)
        {
            chaine[i] = 'a';
        }
        else if ((chaine[i] >= (char) 136 && chaine[i] <= (char) 138) || chaine[i] == (char) 130)
        {
            chaine[i] = 'e';
        }
        else if ((chaine[i] >= (char) 139 && chaine[i] <= (char) 141) || chaine[i] == (char) 161)
        {
            chaine[i] = 'i';
        }
        else if ((chaine[i] >= (char) 147 && chaine[i] <= (char) 149) || chaine[i] == (char) 162)
        {
            chaine[i] = 'o';
        }
    }
}

void remplacerAeOe(char* chaine)
{
    for(int i = 0; chaine[i] != '\0'; i++)
    {
        if (chaine[i] == (char) 145)
        {
            chaine[i] = 'a';
            decalerCaracteresDroite(chaine, i + 1);
            chaine[i + 1] = 'e';
        }
        if (chaine[i] == (char) 156)
        {
            chaine[i] = 'o';
            decalerCaracteresDroite(chaine, i + 1);
            chaine[i + 1] = 'e';
        }
    }
}

void convertirEnMinuscules(char* chaine)
{
    for(int i = 0; chaine[i] != '\0'; i++)
    {
        if (chaine[i] >= (char) 65 && chaine[i] <= (char) 90)
        {
            chaine[i] = (char) chaine[i] + (char) 32;
        }
    }
}

void convertirEnMajuscules(char* chaine)
{
    for(int i = 0; chaine[i] != '\0'; i++)
    {
        if (chaine[i] >= (char) 97 && chaine[i] <= (char) 122)
        {
            chaine[i] = chaine[i] - (char) 32;
        }
    }
}

//Normalisation de lexique

int comparerChaines(char* chaine1, char* chaine2)
{
    int i = 0;
    
    for(; chaine1[i] != '\0' && chaine2[i] != '\0'; i++)
    {
        if (chaine1[i] < chaine2[i])
        {
            return -1; //chaine 1 avant chaine 2
        }
        else if (chaine1[i] > chaine2[i])
        {
            return 1; //chaine 2 avant chaine 1
        }
    }
    if(chaine1[i] != '\0')
    {
        return 1; //chaine 2 avant chaine 1
    }
    if(chaine1[i] == '\0' && chaine2[i] == '\0')
    {
        return 0; //deux mots pareils
    }
    return -1;
    
}

void afficherResultatComparaisonChaine(char* s1, char* s2)
{
    int resultat = comparerChaines(s1, s2);
    if (resultat == -1)
    {
        printf("\"%s\" est avant \"%s\" selon l'ordre lexicographique\n", s1, s2);
    }
    else if (resultat == 0)
    {
        printf("\"%s\" et \"%s\" sont égales", s1, s2);
    }
    else if (resultat == 1)
    {
        printf("\"%s\" est avant \"%s\" selon l'ordre lexicographique\n", s2, s1);
    }
}

void echangerCases(char** tab, int index)
{
    char* s = tab[index];
    tab[index] = tab[index+1];
    tab[index+1] = s;
}

void trierParOrdreLexicographique(char** tab, int nb_mots) //tri à bulles
{
    int tab_trie = 0;
    
    while(tab_trie == 0)
    {
        tab_trie = 1;
        for (int i = 0; i < nb_mots - 1; ++i)
        {
            if (comparerChaines(tab[i], tab[i + 1]) == 1)
            {
                echangerCases(tab, i);
                tab_trie = 0;
            }
        }
        nb_mots--;
    }
}

void decalerMotsGauche(char** tab, int index, int nb_mots)
{
    free(tab[index]);
    for (int i = index; i < nb_mots; ++i)
    {
        tab[i] = tab[i + 1];
    }
}

int dedoublonnerLexique(char** tab, int nb_mots)
{
    for (int i = 0; i < nb_mots - 1; ++i)
    {
        if (comparerChaines(tab[i], tab[i + 1]) == 0)
        {
            decalerMotsGauche(tab, i, nb_mots);
            nb_mots--;
        }
    }
    return nb_mots;
}

void ecrireLexique(char* filename, int nb_mots)
{
    FILE* fichier = NULL;
    char mot[TAILLE_MAX];
    
    fichier = fopen(filename, "w");
    if (fichier != NULL)
    {
        printf("Ecrivez votre lexique de %d mots\n (Attention %d caracteres maximun pour chaque mot) :\n", nb_mots, TAILLE_MAX);
        for (int i = 0; i < nb_mots; ++i)
        {
            scanf("%s", mot);
            fprintf(fichier, "%s\n", mot);
        }
        fclose(fichier);
    } else {
        printf("Impossible d'ouvrir le fichier %s\n", filename);
    }
}

//Analyse d'oeuvres
//Dénombrements

int compterLignesFichier(char *filename)
{
    FILE* fichier = NULL;
    int c;
    int c2 = '\n';
    int nLignes = -1;
    
    fichier = fopen(filename, "r");
    if (fichier != NULL)
    {
        nLignes = 0;
        while((c=fgetc(fichier)) != EOF)
        {
            if(c == '\n')
            {
                nLignes++;
            }
            c2 = c;
        }
        if(c2 != '\n')
        {
            nLignes++;
        }
        fclose(fichier);
    }
    else
    {
        printf("Error : Le fichier se nommant %s est introuvable\n", filename);
    }
    return nLignes;
}

int compterPhrasesFichier(char *filename)
{
    FILE* fichier = NULL;
    int c;
    int nPhrases = -1;
    
    fichier = fopen(filename, "r");
    if (fichier != NULL)
    {
        nPhrases = 0;
        while((c=fgetc(fichier)) != EOF)
        {
            if((c == '.') || (c == '!') || (c == '?'))
            {
                nPhrases++;
            }
        }
        fclose(fichier);
    }
    else
    {
        printf("Error : Le fichier se nommant %s est introuvable\n", filename);
    }
    return nPhrases;
}

int compterCaracteresFichier(char *filename)
{
    FILE* fichier = NULL;
    int c;
    int nChar = -1;
    
    fichier = fopen(filename, "r");
    if (fichier != NULL)
    {
        nChar = 0;
        while((c=fgetc(fichier)) != EOF)
        {
            if((c >= (char) 65 && c <= (char) 90)
               || (c >= (char) 97 && c <= (char) 122))
            {
                nChar++;
            }
        }
        fclose(fichier);
    }
    else
    {
        printf("Error : Le fichier se nommant %s est introuvable\n", filename);
    }
    return nChar;
}

int compterMotsFichier(char *filename)
{
    FILE* fichier = NULL;
    int c;
    int c2 = ' ';
    int nMots = -1;
    
    fichier = fopen(filename, "r");
    if (fichier != NULL)
    {
        nMots = 0;
        while((c=fgetc(fichier)) != EOF)
        {
            if(c == ' ')
            {
                nMots++;
            }
            c2 = c;
        }
        if( c2 != ' ') {
            nMots++;
        }
        fclose(fichier);
    }
    else
    {
        printf("Error : Le fichier se nommant %s est introuvable\n", filename);
    }
    return nMots;
}

void afficherRapportAnalyse(char* filename)
{
    FILE* fichier = NULL;
    int count;
    
    fichier = fopen(filename, "r");
    if(fichier != NULL)
    {
        count = compterCaracteresFichier(filename);
        printf("Le nombre de caracteres est de %d.\n", count);
        count = compterLignesFichier(filename);
        printf("Le nombre de lignes est de %d.\n", count);
        count = compterPhrasesFichier(filename);
        printf("Le nombre de phrases est de %d.\n", count);
        count = compterMotsFichier(filename);
        printf("Le nombre de mots est de %d.\n", count);
    }
    fclose(fichier);
    
}

//Constitution de tables
//Alphabet

unsigned char* genererAlphabet(char* filename, int* char_diff) //int* dans le main à 0;
{
    FILE* fichier = NULL;
    unsigned char* alphabet = NULL;
    int c;
    int* compt_alphabet = malloc(256 * sizeof(int));
    // Init nb de char a  0
    for (int i = 0; i < 256; ++i)
    {
        compt_alphabet[i] = 0;
    }
    
    fichier = fopen(filename, "r");
    if (fichier != NULL)
    {
        // Parcourir chaque caractere du fichier
        while((c=fgetc(fichier)) != EOF)
        {
            // printf("[%c]\n", (char) c);
            if(c >= 0 && c <= 255)
            {
                compt_alphabet[c]++;
            }
            
        }
        for (int i = 0; i < 256; ++i)
        {
            if(compt_alphabet[i] != 0)
            {
                (*char_diff)++;
            }
        }
        // printf("nombre de caractere diff : %d\n", *char_diff);
        alphabet = malloc((*char_diff) * sizeof(unsigned char));
        for (int i = 0, j = 0; i < 256; ++i)
        {
            if (compt_alphabet[i] != 0)
            {
                alphabet[j] = (char) i;
                j++;
            }
        }
        fclose(fichier);
    }
    else
    {
        printf("Error : Le fichier se nommant %s est introuvable\n", filename);
    }
    return alphabet;
}

int* genererSousAlphabetCaracteresNumeriques(unsigned char* alphabet, int char_diff, int* nb_num) // int* nb_num a 0
{
    int* caractere_numerique = NULL;
    for (int i = 0; i < char_diff; ++i)
    {
        if(alphabet[i] >= 48 && alphabet[i] <= 57)
        {
            (*nb_num)++;
        }
    }
    caractere_numerique = malloc(
                                 *nb_num * sizeof(int));
    for (int i = 0, j = 0; i < char_diff; ++i)
    {
        if(alphabet[i] >= (char) 48 && alphabet[i] <= (char) 57)
        {
            caractere_numerique[j] = alphabet[i] - 48;
            j++;
        }
    }
    return caractere_numerique;
}

char* genererSousAlphabetCaracteresAlphabetiques(unsigned char* alphabet, int char_diff, int* nb_alpha)
{
    char* caractere_alphabetiques = NULL;
    for (int i = 0; i < char_diff; ++i)
    {
        if((alphabet[i] >= 65 && alphabet[i] <= 90) || (alphabet[i] >= 97 && alphabet[i] <= 122))
        {
            (*nb_alpha)++;
        }
    }
    caractere_alphabetiques = malloc(*nb_alpha * sizeof(char));
    for (int i = 0, j = 0; i < char_diff; ++i)
    {
        if((alphabet[i] >= 65 && alphabet[i] <= 90) || (alphabet[i] >= 97 && alphabet[i] <= 122))
        {
            caractere_alphabetiques[j] = alphabet[i];
            j++;
        }
    }
    return caractere_alphabetiques;
}

char* genererSousAlphabetCaracteresPonctuation(unsigned char* alphabet, int char_diff, int* nb_ponct)
{
    char* caractere_ponctuation = NULL;
    for (int i = 0; i < char_diff; ++i)
    {
        if((alphabet[i] >= 33 && alphabet[i] <= 47) || (alphabet[i] >= 58 && alphabet[i] <= 64) || (alphabet[i] >= 92 && alphabet[i] <= 96) || (alphabet[i] >= 123 && alphabet[i] <= 126))
        {
            (*nb_ponct)++;
        }
    }
    caractere_ponctuation = malloc(*nb_ponct * sizeof(char));
    for (int i = 0, j = 0; i < char_diff; ++i)
    {
        if((alphabet[i] >= 33 && alphabet[i] <= 47) || (alphabet[i] >= 58 && alphabet[i] <= 64) || (alphabet[i] >= 92 && alphabet[i] <= 96) || (alphabet[i] >= 123 && alphabet[i] <= 126))
        {
            caractere_ponctuation[j] = alphabet[i];
            j++;
        }
    }
    return caractere_ponctuation;
}

//Lexique




/**int main()
 {
 // ETAPE 1 :
 printf("Projet du semestre 2 -- jeux de mots --- réalisation de fonctions diverses\n\n");
 printf("Voici les fonctions réalisées dans le cadre de ce projet :\n");
 printf("\n\n\n\n\n");
 
 int taillex_tab0 = 2;
 char* tab0 = creerTab1D(taillex_tab0);
 printf("Création d'un tableau 1D de deux cases :\n");
 afficherTab1D(tab0, taillex_tab0);
 printf("\n");
 int taillex_tab1 = 10;
 int tailley_tab1 = 10;
 char** tab1 = creerTab2D(taillex_tab1, tailley_tab1);
 printf("Création d'un tableau 2D de 10*10 cases :\n");
 afficherTab2D(tab1, taillex_tab1, tailley_tab1);
 printf("\n");
 insererCharDansTab2D_v1(tab1, taillex_tab1, tailley_tab1, 2, 4, 'y');
 printf("Insertion du caractere y à la place [2][4] dans le tableau 2D :\n");
 afficherTab2D(tab1, taillex_tab1, tailley_tab1);
 printf("\n");
 insererMotDansTab2D_v1(tab1, taillex_tab1, tailley_tab1, 4, 2, 1, "projet");
 printf("Insertion du mot projet à partir de la case [4][2] horizontalement dans le tableau 2D :\n");
 afficherTab2D(tab1, taillex_tab1, tailley_tab1);
 printf("\n");
 printf("Insertion du caractere x en dehors du tableau initial à la case [11][15] : \n");
 insererCharDansTab2D_v2(&tab1, &taillex_tab1, &tailley_tab1, 11, 10, 'x');
 afficherTab2D(tab1, taillex_tab1, tailley_tab1);
 printf("Insertion du mot elliptique, \nverticalement en dehors du tableau initial à la case [2][11] : \n");
 insererMotDansTab2D_v2(&tab1, &taillex_tab1, &tailley_tab1, 14, 11, 0, "elliptique");
 afficherTab2D(tab1, taillex_tab1, tailley_tab1);
 
 char c;
 c = accederTab2D(tab1, 10, 10, 2, 4);
 printf("valeur du caractere à la position [2][4] : %c\n", c);
 c = accederTab2D(tab1, 10, 10, 45, 4);
 printf("valeur du caractere à la position [45][4] : %c\n", c);
 
 printf("\n\n\n\n\n");
 
 int taille_lexique;
 char* oeuvre1 = "francais.dict";
 taille_lexique = calculerTailleLexique(oeuvre1);
 printf("Il y a %d mots dans le lexique : [%s]\n", taille_lexique, oeuvre1);
 
 printf("\n\n\n\n\n");
 
 char *s = malloc(7 * sizeof(char));
 s[0] = 'h';
 s[1] = '{';
 s[2] = 'h';
 s[3] = 'U';
 s[4] = 'h';
 s[5] = '{';
 s[6] = '1';
 
 printf("chaine de caractere : %s\n", s);
 int nb_char = compterCaracteresChaine(s);
 printf("nombre de caractere : %d\n", nb_char);
 supprimerCaracteresNumeriques(s);
 printf("sans les caracteres numériques : %s\n", s);
 supprimerCaracteresSymboles(s);
 printf("sans les symboles : %s\n", s);
 convertirEnMinuscules(s);
 printf("en minuscule : %s\n", s);
 convertirEnMajuscules(s);
 printf("en majuscule : %s\n", s);
 decalerCaracteresDroite(s, 2);
 printf("s avec un décalage à droite à partir de l'index 2 : %s\n", s);
 decalerCaracteresGauche(s, 3);
 printf("s avec un décalage à gauche à partir de l'index 3 : %s\n", s);
 
 printf("\n\n\n\n\n");
 
 char* s1 = "Hugo";
 char* s2 = "Benjamin";
 char* s3 = "Hugo";
 printf("Prenons les 3 chaines suivantes  : 1 : \"%s\", 2 : \"%s\", 3 : \"%s\"\n", s1, s2, s3);
 afficherResultatComparaisonChaine(s1, s2);
 afficherResultatComparaisonChaine(s2, s3);
 afficherResultatComparaisonChaine(s1, s3);
 
 
 printf("\n\n\n\n\n");
 
 char* oeuvre_a_tester = "Lexique1.txt";
 int nb_mots = calculerTailleLexique(oeuvre_a_tester);
 printf("nombre de mot dans [%s] : %d\n", oeuvre_a_tester, nb_mots);
 char** lexique = chargerLexique(oeuvre_a_tester, nb_mots);
 printf("Lexique : \n");
 for (int i = 0; i < nb_mots; ++i)
 {
 printf("%d) %s\n", i, lexique[i]);
 }
 trierParOrdreLexicographique(lexique, nb_mots);
 nb_mots = dedoublonnerLexique(lexique, nb_mots);
 printf("\n\nNombre de mot dans [%s] dédoublonné : %d\n", oeuvre_a_tester, nb_mots);
 printf("Lexique dédoublonné: \n");
 for (int i = 0; i < nb_mots; ++i)
 {
 printf("%d) %s\n", i, lexique[i]);
 }
 
 printf("\n\n\n\n\n");
 
 int mots_dans_lexique;
 printf("Combien voulez-vous de mots dans votre nouveau lexique ?\n");
 scanf("%d", &mots_dans_lexique);
 ecrireLexique("new_lexique", mots_dans_lexique);
 printf("Votre nouveau lexique a été créé");
 
 printf("\n\n\n\n\n");
 
 oeuvre* tableau_doeuvres[NOMBRE_DOEUVRES];
 
 for(int i = 0; i < NOMBRE_DOEUVRES; i++)
 {
 tableau_doeuvres[i] = malloc(sizeof(oeuvre));
 }
 
 tableau_doeuvres[0]->titre = "Discours de la méthode";
 tableau_doeuvres[0]->chemin_dacces = "/Users/hugocharpentier/Documents/Cours/Algo/Projet3C/Discours de la méthode.txt";
 tableau_doeuvres[0]->auteur = "Rene Descartes";
 tableau_doeuvres[0]->annee = 1673;
 
 tableau_doeuvres[1]->titre = "Du contrat social";
 tableau_doeuvres[1]->chemin_dacces = "/Users/hugocharpentier/Documents/Cours/Algo/Projet3C/Du contrat social.txt";
 tableau_doeuvres[1]->auteur = "Jean Jacques Rousseau";
 tableau_doeuvres[1]->annee = 1762;
 
 tableau_doeuvres[2]->titre = "Essais";
 tableau_doeuvres[2]->chemin_dacces = "/Users/hugocharpentier/Documents/Cours/Algo/Projet3C/Essais.txt";
 tableau_doeuvres[2]->auteur = "Michel de Montaigne";
 tableau_doeuvres[2]->annee = 1580;
 
 tableau_doeuvres[3]->titre = "Germinal";
 tableau_doeuvres[3]->chemin_dacces = "/Users/hugocharpentier/Documents/Cours/Algo/Projet3C/Germinal.txt";
 tableau_doeuvres[3]->auteur = "Emile Zola";
 tableau_doeuvres[3]->annee = 1885;
 
 tableau_doeuvres[4]->titre = "L'art de la guerre";
 tableau_doeuvres[4]->chemin_dacces = "/Users/hugocharpentier/Documents/Cours/Algo/Projet3C/L'art de la guerre.txt";
 tableau_doeuvres[4]->auteur = "Sun Tzu";
 tableau_doeuvres[4]->annee = 1521;
 
 tableau_doeuvres[5]->titre = "Pantagruel";
 tableau_doeuvres[5]->chemin_dacces = "/Users/hugocharpentier/Documents/Cours/Algo/Projet3C/Pantagruel.txt";
 tableau_doeuvres[5]->auteur = "Francois Rabelais";
 tableau_doeuvres[5]->annee = 1532;
 
 for (int i = 0; i < NOMBRE_DOEUVRES; ++i)
 {
 printf("L'oeuvre [%s] écrit par %s en %d à comme caractéristiques :\n(Chemin d'accès : %s)\n", tableau_doeuvres[i]->titre, tableau_doeuvres[i]->auteur, tableau_doeuvres[i]->annee, tableau_doeuvres[i]->chemin_dacces);
 afficherRapportAnalyse(tableau_doeuvres[i]->chemin_dacces);
 printf("\n");
 }
 
 printf("\n\n\n\n\n");
 
 int char_diff = 0;
 unsigned char* tab_alphabet = NULL;
 oeuvre_a_tester = "texte_aleatoire.txt";
 printf("Générons l'alphabet de l'oeuvre [%s]\n", oeuvre_a_tester);
 tab_alphabet = genererAlphabet(oeuvre_a_tester, &char_diff);
 printf("Il y a %d caractere(s) différents dans %s, les voici :\n", char_diff, oeuvre_a_tester);
 for (int i = 0; i < char_diff; ++i)
 {
 printf("'%c', ", tab_alphabet[i]);
 }
 printf("\n");
 int nb_num = 0;
 int* sousAlphabetNum = NULL;
 sousAlphabetNum = genererSousAlphabetCaracteresNumeriques(tab_alphabet, char_diff, &nb_num);
 printf("Générons le sous alphabet numérique de l'oeuvre [%s]\n", oeuvre_a_tester);
 printf("Il y a %d caractere(s) numérique différents dans %s, les voici :\n", nb_num, oeuvre_a_tester);
 for (int i = 0; i < nb_num; ++i)
 {
 printf("'%d', ", sousAlphabetNum[i]);
 }
 printf("\n");
 int nb_alpha = 0;
 char* sousAlphabetAlpha = NULL;
 sousAlphabetAlpha = genererSousAlphabetCaracteresAlphabetiques(tab_alphabet, char_diff, &nb_alpha);
 printf("Générons le sous alphabet alphabétique de l'oeuvre [%s]\n", oeuvre_a_tester);
 printf("Il y a %d caractere(s) alphabétique différents dans %s, les voici :\n", nb_alpha, oeuvre_a_tester);
 for (int i = 0; i < nb_alpha; ++i)
 {
 printf("'%c', ", sousAlphabetAlpha[i]);
 }
 printf("\n");
 int nb_ponct = 0;
 char* sousAlphabetPonct = NULL;
 sousAlphabetPonct = genererSousAlphabetCaracteresPonctuation(tab_alphabet, char_diff, &nb_ponct);
 printf("Générons le sous alphabet de ponctuation de l'oeuvre [%s]\n", oeuvre_a_tester);
 printf("Il y a %d caractere(s) de ponctuation différents dans %s, les voici :\n", nb_ponct, oeuvre_a_tester);
 for (int i = 0; i < nb_ponct; ++i)
 {
 printf("'%c', ", sousAlphabetPonct[i]);
 }
 printf("\n");
 
 printf("\n\n\n\n\n");
 
 return 0;
 }*/







// Insère un caractère dans le tableau, et agrandi le tableau au besoin
void insererCharDansTab2D(char ***tab, int *taillex, int *tailley, int posx, int posy, char c){
    if(posx >= 0 && posy >= 0){ // On vérifie que la position est bien positive
        
        if (posx + 1 > *taillex) { // On traite le cas où il faut rajouter des lignes
            *tab = realloc(*tab, (posx + 1) * sizeof(char **)); // On rajoute les cases manquantes
            
            for(int i = *taillex; i < posx + 1; i++){ // On alloue et on initialise la ligne
                (*tab)[i] = creerTab1D(*tailley);
            }
            
            *taillex = posx + 1; // On change la définition de la taille du tableau
        }
        if (posy + 1 > *tailley){ // On traite le cas où il faut rajouter des colonnes
            for(int ligne = 0; ligne < *taillex; ligne++){ // On parcours les lignes
                (*tab)[ligne] = realloc((*tab)[ligne], (posy + 1) * sizeof(char *)); // On rajoute les cases manquantes sur chaque ligne
                for(int colonne = *tailley; colonne < (posy + 1); colonne++){ // On initialise les cases nouvellement créées
                    (*tab)[ligne][colonne] = '*';
                }
            }
            *tailley = posy + 1; // On change la définition de la taille du tableau
        }
        
        (*tab)[posx][posy] = c; // On insère le caractère
    }
    else if(DEBUG) printf("\nLa position n'est pas correcte (inférieure à 0)\n"); // On prévient qu'on ne traitera pas le cas où la cible est au dessus, ou à gauche du tableau
}

// Insère un mot dans le tableau 2D
void insererMotDansTab2D(char ***tab, int *taillex, int *tailley, int posx, int posy, int horizontal, char *mot){
    while(*mot){ // Tant qu'on ne rencontre pas le caractère nul
        insererCharDansTab2D(tab, taillex, tailley, posx, posy, *mot); // On réutilise la sous-fonction pour le caractère courant
        mot++;
        if (horizontal) posy++; // On fait un saut de ligne
        else posx++; // On passe à la colonne suivante
        
    }
}












#endif /* Bibliotheque_h */
