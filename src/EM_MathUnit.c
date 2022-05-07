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
void getEMVariables(struct frequencyCalcParameters* frequencyInfo, float* dBm, float* power, float* powerDensity, float* electricField) {
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
    *powerDensity   = calculatePowerDensity (*power, frequencyInfo->antennaFactor, frequencyInfo->frequency);
    *electricField  = calculateElectricField(*powerDensity);
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

float convertdBmToWatts(float dBm) {
    float intermediate = dBm / 10.0;

    return (1.0e-3) * pow(10.0, intermediate);
}

float calculatePowerDensity(float power, float antennaFactor, float frequency) {
    float antennaGain = pow(10, antennaFactor / 10.0);
    float antenna_app = antennaGain * (pow(LIGHTSPEED, 2)) / (4 * M_PI * pow(frequency, 2));

    return power / antenna_app;
}

float calculateElectricField(float powerDensity) {
    float intermediate = powerDensity * PI120;

    return sqrt(intermediate);
}

