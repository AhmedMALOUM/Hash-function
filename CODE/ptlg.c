#include <stdio.h>
#include <string.h>
#include <math.h>

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

    // Calculer le nombre de récursions en utilisant l'ask total
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

int main() {
    char cle_secrete[100];
    double CI;
    int nombre_reculs;

    printf("Entrez la clé secrète : ");
    fgets(cle_secrete, sizeof(cle_secrete), stdin);
    cle_secrete[strcspn(cle_secrete, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne

    preTraitement(cle_secrete, &CI, &nombre_reculs);

    printf("Condition initiale (CI) : %f\n", CI);
    printf("Nombre de récursions : %d\n", nombre_reculs);

    double r = 3.9999999;
    int numIterations = 32;

    printf("\nLogistic Map (r=%.10f, CI=%.10f):\n", r, CI);
    generateLogisticSequence(CI, r, numIterations);

    return 0;
}
