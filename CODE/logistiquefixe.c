#include <stdio.h>

double logisticMap(double x, double r) {
    return r * x * (1 - x);
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
    double r1 = 3.9999999;
    double r2 = 3.9999999;
    double ci1 = 0.0001;
    double ci2 = 0.0011;
    int numIterations = 32;
    
    
    printf("Logistic Map (r=%.10f, CI=%.10f):\n", r1, ci1);
    generateLogisticSequence(ci1, r1, numIterations);
    
    printf("\nLogistic Map (r=%.10f, CI=%.10f):\n", r2, ci2);
    generateLogisticSequence(ci2, r2, numIterations);
    
    return 0;
}
