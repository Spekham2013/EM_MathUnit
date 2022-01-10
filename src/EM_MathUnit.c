#include "EM_MathUnit.h"

/**
* This function has two main functions.
* Filling the buffer for calculating the RMS value
* and notifying the user when the buffer is full.
*/
uint8_t fillRMSBuffer(uint16_t sample, struct frequencyCalcParameters* frequencyInfo) {
    if (frequencyInfo->BufferIndex == 0) {
        frequencyInfo->Buffer = 0;
    }

    if (frequencyInfo->BufferIndex < BUFFERLENGTH) {
        // Directly square sample
        frequencyInfo->Buffer = (sample * sample) + frequencyInfo->Buffer;
        frequencyInfo->BufferIndex++;

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
    uint64_t total = frequencyInfo->Buffer;
    total          = total / BUFFERLENGTH;
    
    // Reset the buffer index
    frequencyInfo->BufferIndex = 0;

    // Calculate the voltage
    float A_Constant = 0.000805;
    float B_Constant = 0.0;
    if (frequencyInfo->ADCCalibrationData != NULL) {
        A_Constant = frequencyInfo->ADCCalibrationData->A_Constant;
        B_Constant = frequencyInfo->ADCCalibrationData->B_Constant;
    }
    float voltage = (float)sqrtf(total)*A_Constant + B_Constant;

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
