#include "EM_MathUnit.h"

/**
* This function has two main functions.
* Filling the buffer for calculating the RMS value
* and notifying the user when the buffer is full.
*/
uint8_t fillRMSBuffer(uint16_t sample, struct frequencyCalcParameters* frequencyInfo) {

    if (frequencyInfo->BufferIndex < BUFFERLENGTH) {
        // Directly square sample
        frequencyInfo->Buffer[frequencyInfo->BufferIndex++] = (sample & 0x0FFF) * (sample & 0x0FFF);

        return SUCCES;
    }
    
    if (frequencyInfo->BufferIndex == BUFFERLENGTH){
        return BUFFERFULL;
    }

    return FAILURE;
}

/**
* This function works in two parts the first part is
* calculating the RMS voltage of the raw ADC value's
* This voltage is then used to convert to dBm and 
* other EM related values
*/
void getEMVariables(struct frequencyCalcParameters* frequencyInfo, float* dBm, float* power, float* electricField, float* magneticField, float* powerDensity) {
    // Calculate RMS value
    uint64_t total = 0;
    for (uint32_t i = 0; i < BUFFERLENGTH; i++) {
        total += frequencyInfo->Buffer[i];
    }

    // Reset the buffer index
    frequencyInfo->BufferIndex = 0;

    // Calculate the voltage
    float voltage = (float)sqrtf(total / BUFFERLENGTH)*0.000805 + 0.0;

    // Calculate the EM variables
    *dBm            = convertVoltageTodBm   (voltage, frequencyInfo);
    *power          = convertdBmToWatts     (*dBm);
    *electricField  = calculateElectricField(*power);
    *magneticField  = calculateMagneticField(*electricField);
    *powerDensity   = 0.0;
}

/**
* The formula for this function is:
* dBm = ax + b
*/
float convertVoltageTodBm   (float voltage, struct frequencyCalcParameters* frequencyInfo) {
    float a = frequencyInfo->dBm_A_Constant;
    float b = frequencyInfo->dBm_B_Constant;

    return (a * voltage) + b;
}

/**
* The formula for this function is:
* P = 10^{dBm / 10}
*/
float convertdBmToWatts     (float dBm) {
    float intermediate = dBm / 10.0;

    return pow(10.0, intermediate);
}

/**
* The formula for this function is:
* |E| = \sqrt{P * 120pi}
*/
float calculateElectricField(float power) {
    float intermediate = power * PI120;

    return sqrt(intermediate);
}

/**
* The formula for this function is:
* |H| = |E|/120pi
*/
float calculateMagneticField(float electricField) {
    float intermediate = electricField / PI120;

    return intermediate;
}
