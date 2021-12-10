#include <stdio.h>
#include "../src/EM_MathUnit.h"

int main() {
    struct frequencyCalcParameters param;
    param.ID                = 0;
    param.BufferIndex       = 0;
    param.dBm_A_Constant    = 50.1882057716437;
    param.dBm_B_Constant    = -85.8243412797992;


    for (uint32_t i = 0; i < BUFFERSIZE; i++) {
        fillRMSBuffer((i % 2) ? 0 : 4096, &param);
    }
    float dBm;
    float power;
    float electricField; 
    float magneticField; 
    float powerDensity;
    getEMVariables(&param, &dBm, &power, &electricField, &magneticField, &powerDensity);

    return 0;
}
