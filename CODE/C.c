
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define BLOCK_SIZE 16 // Taille du bloc en octets (128 bits)

double logisticMap(double x, double r) {
    return r * x * (1 - x);
}

void preTraitement(char* cle_secrete, double* CI, int* nombre_reculs) {
    char partie1[50];
    char partie2[50];
    int ask_partie1 = 0;
    int ask_partie2 = 0;

    // Diviser la clé secrète en deux parties
    int longueur = strlen(cle_secrete);
    int moitie = longueur / 2;

    strncpy(partie1, cle_secrete, moitie);
    partie1[moitie] = '\0';

    strncpy(partie2, cle_secrete + moitie, longueur - moitie);
    partie2[longueur - moitie] = '\0';

    for (int i = 0; i < 8; i++) {
        ask_partie1 += (int)partie1[i] * pow(2, i);
        ask_partie2 += (int)partie2[i];
    }
    ask_partie2 = (ask_partie2 << 2);

    // Calculer la CI en effectuant l'opération
    *CI = (double)ask_partie1 * pow(2, 5) / ((double)ask_partie2 * pow(2, 15));

    // Calculer le nombre de rotation en utilisant l'ask total
    *nombre_reculs = ask_partie1 % 6;
}

void generateLogisticSequence(double ci, double r, int numIterations) {
    double x = ci;

    for (int i = 0; i < numIterations; i++) {
        x = logisticMap(x, r);
        int roundedValue = (int)(x * 1000000000); // Conversion à virgule fixe avec 9 décimales
        printf("%d\n", roundedValue);
    }
}

// Fonction de permutation logistique
void permute_vector(unsigned char* vector, unsigned char* keys, int keySize) {
    for (int i = 0; i < keySize; i++) {
        int roundedValue = ((((unsigned int)(keys[i] * 1)) % (keySize - i)) + i);
        unsigned char tmp = vector[roundedValue];
        vector[roundedValue] = vector[i];
        vector[i] = tmp;
    }
}

// Fonction de rotation
unsigned char trans_hex(unsigned int nbr_rot) {
    unsigned char nbr_hex = 0x00;
    unsigned char un_hex = 0x80;
    for (int i = 0; i < (unsigned int)nbr_rot; i++) {
        nbr_hex = (nbr_hex >> 1);
        nbr_hex = (nbr_hex + un_hex);
    }
    return nbr_hex;
}

void rot_G(unsigned char* vect, unsigned int nbr_rot) {
    unsigned char rot_hex = trans_hex(nbr_rot);
    unsigned char R = (vect[0] & rot_hex);

    for (int i = 0; i < 7; i++) {
        vect[i] = (vect[i] << (nbr_rot));
        vect[i] = (vect[i] + (vect[i + 1] & rot_hex));
    }
    vect[7] = (vect[7] << (nbr_rot));
    vect[7] = (vect[7] + R);
}

// Fonction de compression
void compression(unsigned char* message, unsigned char* cle_secrete, int keySize, unsigned int rot) {
    // Permutation du message
    permute_vector(message, cle_secrete, keySize);

    // Division du message en deux parties
    int messageSize = BLOCK_SIZE;
    int halfSize = messageSize / 2;
    unsigned char* partie1 = (unsigned char*)malloc((halfSize + 1) * sizeof(unsigned char));
    unsigned char* partie2 = (unsigned char*)malloc((halfSize + 1) * sizeof(unsigned char));
    strncpy((char*)partie1, (char*)message, halfSize);
    strncpy((char*)partie2, (char*)message + halfSize, halfSize);
    partie1[halfSize] = '\0';
    partie2[halfSize] = '\0';

    // Application de la fonction de rotation pour la partie1
    rot_G(partie1, rot);

    // XOR entre le résultat de la deuxième partie (deuxième partie finale) et le résultat de la première partie après la logistique 2 => première partie finale
    for (int i = 0; i < halfSize; i++) {
        partie2[i] ^= (partie2[i] ^ cle_secrete[i % keySize]);
    }

    // XOR entre le résultat de la deuxième partie (deuxième partie finale) et le résultat de la première partie après la logistique 2 => première partie finale
    for (int i = 0; i < halfSize; i++) {
        partie1[i] ^= partie2[i];
    }

    // Assemblage des résultats des deux parties en les permutant de position => partie complète
    unsigned char* partieComplete = (unsigned char*)malloc((messageSize + 1) * sizeof(unsigned char));
    for (int i = 0; i < halfSize; i++) {
        partieComplete[i] = partie2[i];
        partieComplete[i + halfSize] = partie1[i];
    }
    partieComplete[messageSize] = '\0';

    // Affichage du message codé final
    printf("Message codé final : ");
    for (int i = 0; i < messageSize; i++) {
        printf("%02X ", partieComplete[i]);
    }
    printf("\n");

    // Libération de la mémoire allouée
    free(partie1);
    free(partie2);
    free(partieComplete);
}

void addPadding(unsigned char *paddedMessage, int messageLength, int paddedLength) {
    paddedMessage[messageLength] = 0x80;
    for (int i = messageLength+1 ; i < paddedLength-8 ; i++) {
        paddedMessage[i] = 0x00;
    }
    if (messageLength > (unsigned long int)pow(2,64)){
       messageLength = messageLength % (unsigned long int)pow(2,64);
    }
    int msgt = messageLength;
    for (int i = paddedLength-1 ; i > paddedLength-8 ; i--){
        paddedMessage[i] = (msgt & 0x000000FF);
        msgt = msgt >> 8;
    }
}

void divideIntoBlocks(unsigned char *message, int messageLength) {
    int numBlocks = messageLength / BLOCK_SIZE;

    printf("Message Blocks:\n");
    for (int i = 0; i < numBlocks; i++) {
        printf("Block %d: ", i + 1);
        for (int j = 0; j < BLOCK_SIZE; j++) {
            printf("%02X ", message[i * BLOCK_SIZE + j]);
        }
        printf("\n");
    }
}

int main() {
    char cle_secrete[100];
    double CI;
    int nombre_reculs;

    printf("Entrez la clé secrète : ");
    fgets(cle_secrete, sizeof(cle_secrete), stdin);
    cle_secrete[strcspn(cle_secrete, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne

    preTraitement(cle_secrete, &CI, &nombre_reculs);

    printf("Condition initiale (CI) : %f\n", CI);
    printf("Nombre de rotation : %d\n", nombre_reculs);

    double r = 3.9999999;
    int numIterations = 32;

    printf("\nLogistic Map (r=%.10f, CI=%.10f):\n", r, CI);
    generateLogisticSequence(CI, r, numIterations);

  char messageString[] = "les donnees";
unsigned char message[BLOCK_SIZE];
memcpy(message, messageString, strlen(messageString));
    int keySize = strlen(cle_secrete);

    printf("\nMessage original : ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02X ", message[i]);
    }
    printf("\n");

    // Ajout du padding
    int messageLength = BLOCK_SIZE;
    int paddedLength = (BLOCK_SIZE - (messageLength % BLOCK_SIZE));
    if (paddedLength < 8) {
        paddedLength += BLOCK_SIZE;
    }
    paddedLength += messageLength;
    unsigned char *paddedMessage = malloc(paddedLength);
    memcpy(paddedMessage, message, messageLength); // Copie du message d'origine
    addPadding(paddedMessage, messageLength, paddedLength);

    // Division du message en blocs de 128 bits
    divideIntoBlocks(paddedMessage, paddedLength);

    // Compression du message
    compression(paddedMessage, (unsigned char*)cle_secrete, keySize, nombre_reculs);

    free(paddedMessage);

    return 0;
}
