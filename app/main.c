#include <stdio.h>
#include "../src/EM_MathUnit.h"

int main() {
    for (uint32_t i = 0; i < BUFFERSIZE; i++) {
        uint16_t sineValue = 0;

        switch (i % 10) {
            case 0: sineValue = 0; break;
            case 1: sineValue = 1266; break;
            case 2: sineValue = 2408; break;
            case 3: sineValue = 3314; break;
            case 4: sineValue = 3896; break;
            // case 5: sineValue = 4096; break;
            // case 6: sineValue = 3896; break;
            // case 7: sineValue = 3314; break;
            // case 8: sineValue = 2408; break;
            // case 9: sineValue = 1266; break;
            default: sineValue = 0; break;
        }

        calculateRootMeanSquare(sineValue);
    }
    printf("%f \n", getRootMeanSquare());

    return 0;
}
