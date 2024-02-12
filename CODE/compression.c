#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RI 3.999999
// Fonction de permutation logistique 1
void permutationLogistique1(unsigned char* message, int taille) {
    
    // Tableau de permutation logistique 1
    int permutation[16] = {8, 0, 2, 7, 10, 3, 12, 15, 5, 14, 6, 11, 4, 9, 13, 1};

    unsigned char messagePermute[17]; // Tableau temporaire pour stocker le message permuté

    // Permutation logistique 1
    for (int i = 0; i < 16; i++) {
        messagePermute[i] = message[permutation[i]];
    }
    messagePermute[16] = '\0';

    // Copie du message permuté dans le tableau d'origine
    strncpy(message, messagePermute, 16);
}

void pre_traitement(unsigned int cle, double* Ci, unsigned int* rot){       ?


}
  
unsigned int*  logistique(double* Ci){       ?
    
    

}

// Fonction de compression
unsigned char* compression(unsigned char* message, unsigned int[] vect_cle, unsigned int rot) {
    // Vérification de la taille du message
   // if (taille != 16) {
        //printf("Erreur : La taille du message doit être de 16 octets.\n");
       // return;
   // }

    // Compression du message
    permutationLogistique1(message, taille);

    // Division du message en deux parties
    char partie1[9];
    char partie2[9];
    strncpy(partie1, message, 8);
    strncpy(partie2, message + 8, 8);

    // Application de la fonction de recul de 4 et en rotation (logistique 2) sur la première partie
    for (int i = 0; i < 8; i++) {
        partie1[i] = ((partie1[i] >> 4) | (partie1[i] << 4));
    }

    // Application du XOR sur la deuxième partie => deuxième partie finale
    for (int i = 0; i < 8; i++) {
        partie2[i] ^= partie2[i];
    }

    // Application du XOR entre le résultat de la deuxième partie (deuxième partie finale) et le résultat de la première partie après la logistique 2 => première partie finale
    for (int i = 0; i < 8; i++) {
        partie1[i] ^= partie2[i];
    }

    // Assemblage des résultats des deux parties en les permutant de position => partie complète
    char partieComplete[17];
    for (int i = 0; i < 8; i++) {
        partieComplete[i] = partie2[i];
        partieComplete[i + 8] = partie1[i];
    }
    partieComplete[16] = '\0';

    // Génération de la clé
    char cle[17];
    for (int i = 0; i < 16; i++) {
        cle[i] = rand() % 256;  // Génération d'un octet aléatoire
    }
    cle[16] = '\0';

    // XOR entre la partie complète et la clé aléatoire
    for (int i = 0; i < 16; i++) {
        partieComplete[i] ^= cle[i];
    }

    // Affichage du message codé final
    printf("Message codé final : %02x\n", partieComplete);
}

int main() {
    char message[17] =  " coucou you comment tu va toi  ";
    int taille = strlen(message);

    printf("Message original : %s\n", message);

    compression(message, taille);

    return 0;
}
