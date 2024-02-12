#include <stdio.h>

unsigned char trans_hex (unsigned int nbr_rot){
    unsigned char nbr_hex = 0x00;
    unsigned char un_hex = 0x80;
    for (int i = 0 ; i < (unsigned int )nbr_rot ; i++){
        nbr_hex = (nbr_hex >> 1);
        nbr_hex = (nbr_hex + un_hex);
    }
    return nbr_hex;
}


void rot_G(unsigned char* vect, unsigned int nbr_rot){
    unsigned char rot_hex = trans_hex(nbr_rot);
    unsigned char R = (vect[0] & rot_hex);
    
    for (int i = 0 ; i < 7 ; i++){
        vect[i] = (vect[i] << (nbr_rot));
        vect[i] = ( vect[i] + (vect[i+1] & rot_hex) );
    }
        vect[7] = (vect[7] << (nbr_rot));
        vect[7] = ( vect[7] + R );

}


// Fonction pour effectuer une rotation circulaire à gauche d'un seul octet
unsigned char rotateLeft(unsigned char octet, int rotation) {
    return (octet << rotation) | (octet >> (8 - rotation));
}


// Fonction pour convertir un entier en binaire
void convertToBinary(int n) {
    if (n == 0) {
        printf("0");
        return;
    }

    int binary[8];
    int index = 0;
    while (n > 0) {
        binary[index] = n % 2;
        n = n / 2;
        index++;
    }

    for (int i = index - 1; i >= 0; i--) {
        printf("%d", binary[i]);
    }
}

int main() {
    
    unsigned char vecteur[8];
    int rotation;

        printf("Entrez le vecteur de 8 octets (en hexadécimal) : ");
    for (int i = 0; i < 8; i++) {
        scanf("%hhx", &vecteur[i]);
    }

        printf("Entrez le nombre de recul : ");
        scanf("%d", &rotation);
   

    
     rot_G(vecteur, rotation);

         printf("Vecteur après la rotation : ");

    for (int i = 0; i < 8; i++) {
         printf("%02X ", vecteur[i]);
    }
         printf("\n");
    return 0;
}


