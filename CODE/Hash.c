#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdint.h>


#define BLOCK_SIZE 16 // Taille du bloc en octets (128 bits)


void Collisions_128(unsigned char* K){
    unsigned int RandT = 10000;
    unsigned int R = 10000;
    unsigned char msg1[RandT+1] ;
    unsigned char msg2[RandT+1] ;
    unsigned char* H1;
    unsigned char* H2;
     unsigned int collision[10] = {0,0,0,0,0,0,0,0,0,0};
    unsigned int minAbv=UINT32_MAX , maxAbv=0 ,TotaleAvb=0;
    srand(time(NULL));
         for ( int t = 0 ; t < R ; t++){
               char f ;
               for (int i = 0 ; i < RandT ; i++){
                    msg1[i] = (unsigned char) (rand()%256);
                    msg2[i] = (unsigned char) (rand()%256);
               }
              
               msg1[RandT] = '\0'; msg2[RandT] = '\0';

               unsigned char g;
               unsigned int  u;

 
               unsigned char y = (1 << (7-(u%8)));
               g = (unsigned char)(msg2[u/8] ^ y);
               msg2[u/8] =  g;


            //   H1 =  HASH_FUNCTION(msg1,K);
            //   H2 =  HASH_FUNCTION(msg2,K);



              unsigned int colis = 0;
              unsigned int Abv = 0;

              for (int i = 0 ; i < 16 ; i++){
                   // nombre de collisions
                  if ((unsigned int)(H1[i]) == (unsigned int)(H2[i])){
                                    colis +=1;
                   }
                  // la valeurs absolue
                   Abv  += abs((int)H1[i] - (int)H2[i]);


              }

              if (minAbv > Abv)
                  minAbv = Abv;

              if (maxAbv < Abv)
                  maxAbv = Abv;


              TotaleAvb += Abv;

              collision[colis] +=1;

          free(H1);
          free(H2);
   }
         printf("Collision Teste : ");
            for (int i = 0 ; i < 10 ; i++){
                 printf(" %d : %d \n", i , collision[i]);
            }


}


double logisticMap(double x, double r) {
    return r * x * (1 - x);
}

void preTraitement(unsigned char* cle_secrete, double* CI, int* nombre_reculs, unsigned int msgT) {
    char partie1[50];
    char partie2[50];
    unsigned int ask_partie1 = 0;
    unsigned int ask_partie2 = 0;

    // Diviser la clé secrète en deux parties
    int longueur = 16;
    int moitie = longueur / 2;

    strncpy(partie1,(char*)cle_secrete, moitie);
    partie1[moitie] = '\0';

    strncpy(partie2, (char*)cle_secrete + moitie, longueur - moitie);
    partie2[longueur - moitie] = '\0';

    for (int i = 0; i < 8; i++) {
        ask_partie1 += (int)partie1[i] * pow(2, i);
        ask_partie2 += (int)partie2[i];
    }
    ask_partie2 = (ask_partie2 << 2);
    ask_partie1 = (ask_partie1 %  (unsigned int)pow(2,15));
    // Calculer la CI en effectuant l'opération
    *CI = (double)ask_partie1  / (ask_partie2 * msgT%((unsigned int)(pow(2,15))));
   // printf("CIIIIII : %f \n",*CI);
    // Calculer le nombre de rotation en utilisant l'ask total
    *nombre_reculs = ask_partie1 % 6;
}

void generateLogisticSequence(double ci, double r, int numIterations, unsigned int vectKeys[]) {
    double x = ci;
    for (int i = 0; i < numIterations; i++) {
        x = logisticMap(x, r);
        unsigned int roundedValue = (unsigned int)(x * 1000000000); // Conversion à virgule fixe avec 9 décimales
        vectKeys[i] = roundedValue;
       // printf("%d\n", roundedValue);
    }
}

// Fonction de permutation logistique
unsigned char* permute_vector(unsigned char* vector, unsigned int* keys,unsigned int num_block) {
    unsigned char* tab = malloc(sizeof(unsigned char) * 16);
   // printf("Fonction Permut 1  \n");
    for (int i = 0; i < 16; i++) {
      //  printf("%02x ",vector[i]);
    }
    printf("\n");
    for (int i = 0; i < 16; i++) {
        int roundedValue = ((((unsigned int)(keys[i+(num_block*25)])) % (16 - i)) + i);
        unsigned char tmp = vector[roundedValue];
        vector[roundedValue] = vector[i];
        vector[i] = tmp;
    }

    //printf("Fonction Permut 2 \n");
    for (int i = 0; i < 16; i++) {
        tab[i] = vector[i];
       // printf("%02x ",vector[i]);
    }
    printf("\n");
    return tab;
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

void addPadding(unsigned char *paddedMessage, int messageLength, int paddedLength) {
  //  printf("Message padding 1\n");
    paddedMessage[messageLength] = 0x80;
  //  printf("Message padding 2\n");
    for (int i = messageLength+1 ; i < paddedLength-8 ; i++) {
        paddedMessage[i] = 0x00;
    }
  //  printf("Message padding3\n");
  /*  if (messageLength > (unsigned int)pow(2,31)){
       messageLength = messageLength % (unsigned int)pow(2,31);
    }   */
  //  printf("Message padding4\n");
    int msgt = messageLength;
  //  printf("Message padding5\n");
    for (int i = paddedLength-1 ; i > paddedLength-8 ; i--){
        paddedMessage[i] = (msgt & 0x000000FF);
        msgt = msgt >> 8;
    }
  //  printf("Message padding6\n");
}

void divideIntoBlocks(unsigned char *message, int messageLength) {
    int numBlocks = messageLength / BLOCK_SIZE;

   // printf("Message Blocks:\n");
    for (int i = 0; i < numBlocks; i++) {
       // printf("Block %d: ", i + 1);
        for (int j = 0; j < BLOCK_SIZE; j++) {
            //printf("%02X ", message[i * BLOCK_SIZE + j]);
        }
      //  printf("\n");
    }
}


// Fonction de compression
unsigned char* compression(unsigned char* message_P,unsigned int padd_L, unsigned int* cle_secrete, unsigned int rot) {
  //  printf("Padded message : %s \n",message_P);
    // Permutation du message
    unsigned char* message =  malloc(sizeof(unsigned char)* 16);
    unsigned char* partieComplete = malloc(sizeof(unsigned char)*16);
    for (int i = 0 ; i < 16 ; i++){
        partieComplete[i] = 0;
    }
    partieComplete[16] = '\0';
    
    for (int k = 0 ; k < padd_L/16 ; k++) {
    //printf("Block N : %d ", k + 1);
        for (int l = 0 ; l < 16 ; l++) message[l] = message_P[k*16+l];

    //printf("Message Pre-permut : %s \n", message);
    message = permute_vector(message, cle_secrete,k);
    //printf("Message permut : %s \n", message);
    // Division du message en deux parties
    int messageSize = strlen((char*)message);
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
        partie2[i] ^= (partie2[i] ^ cle_secrete[i+(k*25+16)]);
    }

    // XOR entre le résultat de la deuxième partie (deuxième partie finale) et le résultat de la première partie après la logistique 2 => première partie finale
    for (int i = 0; i < halfSize; i++) {
        partie1[i] ^= partie2[i];
    }

    // Assemblage des résultats des deux parties en les permutant de position => partie complète
    for (int i = 0; i < halfSize; i++) {
        partieComplete[i] = partieComplete[i] ^ partie2[i];
        partieComplete[i + halfSize] =  partieComplete[i + halfSize] ^ partie1[i];
    }
    // Libération de la mémoire allouée
    free(partie1);
    free(partie2);
    }
    return partieComplete;
}

unsigned char* HASH_FUNCTION (unsigned char * message , unsigned char* cle_secrete){

unsigned char* hash;
double CI;
int nombre_reculs;
int messageLength = strlen((char*)message);
unsigned int msgT = 0;
for (unsigned int i = 0 ; i < messageLength ; i++) {
    msgT += (unsigned int)message[i];
    msgT %= (unsigned int)(pow(2,31));
}
 preTraitement(cle_secrete, &CI, &nombre_reculs,msgT);

 //printf("Condition initiale (CI) : %f\n", CI);
 //printf("Nombre de rotation : %d\n", nombre_reculs);

int keySize = 16;

 //printf("\nMessage original : %s\n", message);
    // Ajout du padding

    
    int paddedLength = (BLOCK_SIZE - (messageLength % BLOCK_SIZE));
    if (paddedLength < 8) {
        paddedLength += BLOCK_SIZE;
    }
    paddedLength += messageLength;
    unsigned char *paddedMessage = malloc(paddedLength);
    memcpy(paddedMessage, message, messageLength); // Copie du message d'origine

    
    addPadding(paddedMessage, messageLength, paddedLength);


 double r = 3.9999999;
    int numIterations = ((paddedLength/16)*25);
  //  printf("\nLogistic Map (r=%.10f, CI=%.10f):\n", r, CI);
    unsigned int vectKeys[numIterations];
    generateLogisticSequence(CI, r, numIterations,vectKeys);
// Division du message en blocs de 128 bits
    divideIntoBlocks(paddedMessage, paddedLength);
    
      // Compression du message
    
    hash = compression(paddedMessage,paddedLength,vectKeys,nombre_reculs);

    free(paddedMessage);
    return hash;
} 


int main(){

unsigned char cle_secrete[17];
    char buff ;
    unsigned char* hash;
    double CI;
    int nombre_reculs;
    unsigned char message[100];
    printf("Entrez le message a hacher : ");
    scanf("%s",message);
    scanf("%c",&buff);
    printf("Entrez la clé secrète : ");
    scanf("%s",cle_secrete);
    scanf("%c",&buff);    
  //  cle_secrete[strcspn(cle_secrete, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne
     hash = HASH_FUNCTION(message,cle_secrete);
    // Affichage du message codé final
    printf("Message codé final : ");
    for (int i = 0; i < 16; i++) {
        printf("%02x ", hash[i]);
    }
    printf("\n");
    return 0;
}
