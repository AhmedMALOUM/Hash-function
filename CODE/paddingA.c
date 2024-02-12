#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define BLOCK_SIZE 16 // Taille du bloc en octets (128 bits)

void addPadding(unsigned char *paddedMessage, int messageLength, int paddedLength) {
  //  int paddingLength = BLOCK_SIZE - (messageLength % BLOCK_SIZE);
    
//    for (int i = messageLength; i < messageLength + paddingLength; i++) {
//        message[i] = rand() % 256; // Génère un octet aléatoire (0-255)
//    }
    
    // Vérifier si le padding est nécessaire
    /*if (paddingLength == BLOCK_SIZE) {
        // Ajouter un bloc vide de 128 bits
        for (int i = messageLength + paddingLength; i < messageLength + paddingLength + BLOCK_SIZE; i++) {
            message[i] = 0;
        }
        paddingLength += BLOCK_SIZE;
    }*/
   
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

    
    
           // printf(" ");

    
}

void divideIntoBlocks(unsigned char *message, int messageLength) {
    int numBlocks = messageLength / BLOCK_SIZE;
    
    printf("Message Blocks:\n");
    for (int i = 0; i < numBlocks; i++) {
        printf("Block %d: ", i + 1);
        for (int j = 0; j < BLOCK_SIZE; j++) {
           //unsigned char byte = message[i * BLOCK_SIZE + j];  // cest en binaire 
           //for (int k = 7; k >= 0; k--) {
          //      printf("%d", (byte >> k) & 1);
          //  }
           // printf(" ");
             printf("%02X ", message[i * BLOCK_SIZE + j]);  // cest en hexadecimale
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL)); // Initialisation de la graine pour la génération aléatoire
    
    unsigned char message[] = "Hello, World! hmedh ysponjayi lmokhiwwww ";
    int messageLength = strlen((char*)message);
    int paddedLength =  (BLOCK_SIZE - (messageLength % BLOCK_SIZE)) ;
    if (paddedLength < 8) {
        paddedLength += BLOCK_SIZE;
    }
   paddedLength += messageLength ;

   printf("la Taille : %d \n",paddedLength * 8);
    unsigned char *paddedMessage = malloc(paddedLength);
    
    for (int i = 0 ; i < messageLength ; i++ )
        printf("%02X ",message[i]);

    printf("\n");

    memcpy(paddedMessage, message, messageLength); // Copie du message d'origine
    
    // Ajout du padding
    addPadding(paddedMessage, messageLength, paddedLength);
    
    // Affichage du message avec padding en binaire
    printf("Padded Message (Binary):\n");
    for (int i = 0; i < paddedLength; i++) {
       // unsigned char byte = paddedMessage[i];        // cest en binaire 
        //for (int j = 7; j >= 0; j--) {
         //   printf("%d", (byte >> j) & 1);
       // }
       // printf(" ");
          printf("%02X ", paddedMessage[i]); // cest en hexadecimale 
    }
    printf("\n");
    
    // Division du message en blocs de 128 bits
    divideIntoBlocks(paddedMessage, paddedLength);
    
    free(paddedMessage);
    
    return 0;
}
