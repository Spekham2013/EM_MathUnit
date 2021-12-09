#include "EM_MathUnit.h"

uint32_t squaredBuffer[BUFFERSIZE];
float RMS;

void calculateRootMeanSquare(uint16_t sample) {
    static uint32_t index = 0;

    if (index < BUFFERSIZE) {
        // Directly square sample
        squaredBuffer[index++] = sample * sample;
    }
    
    if (index == BUFFERSIZE){
        uint64_t total = 0;
        for (uint32_t i = 0; i < BUFFERSIZE; i++) {
            total += squaredBuffer[i];
        }
        RMS = (float)sqrtf(total / BUFFERSIZE)*0.000805 + 0.0;

        index = 0;
    }
}

float getRootMeanSquare(void) {
    return RMS;
}
