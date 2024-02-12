#include <stdio.h>
#include <math.h>
#include <time.h>
double logistic_map(double x, double r) {
    return (r * x * (1 - x));
}

void permute_vector(unsigned char* vector, unsigned int* keys) {
  
    for (int i = 0 ; i<16 ; i++){
        int roundedValue = ( (((unsigned int)(keys[i] * 1)) % (16-i)) + i  ) ;
        unsigned char tmp = vector[roundedValue];
        vector[roundedValue] = vector[i];
        vector[i] = tmp;
    }
}

int main() {
   // srand(time(NULL));
    unsigned char vector[16] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k','l', 'm', 'n', 'o', 'p'};

    unsigned int keys[16];
    printf("Entrez les 16 clés logistiques : \n");
    for (int i = 0; i < 16; i++) {
        keys[i] = 34222;
     //   scanf("%lf", &keys[i]);
    }

    permute_vector(vector, keys);

    printf("Positions des cellules après la permutation :\n");
    for (int i = 0; i < 16; i++) {
        printf("V%d : %c\n", i + 1, vector[i]);
    }

    return 0;
}
