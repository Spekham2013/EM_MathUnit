#ifndef EM_MATHUNIT_H
#define EM_MATHUNIT_H

#include <stddef.h>
#include <stdint.h>
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// 
// Math constants
// 
#define PI120 376.99111843077518861551720599354034610366032792501269851699
#define LIGHTSPEED 299792458.0

// 
// Functional constants
// 
#define BUFFERFULL 1

#ifndef SUCCES
#define SUCCES 0
#endif

#ifndef FAILURE
#define FAILURE -1
#endif

#define BUFFERLENGTH 1024

// 
// Structs
// 

#define TAG_MATHUNIT "MATHUNIT"

/** @struct ADCCalibration
 *  @brief  This struct is used for storing ADC calibration data.
 *  @var ADCCalibration::A_Constant 
 *  The A variable for calculating the voltage
 *  @var ADCCalibration::B_Contsant 
 *  The A variable for calculating the voltage
 */
struct ADCCalibration {
    float A_Constant;
    float B_Constant;
};

/** @struct frequencyCalcParameters
 *  @brief This struct is used for storing and passing information to user functions
 *  @var frequencyCalcParameters::ID 
 *  This value must be unique
 *  @var frequencyCalcParameters::Buffer
 *  Used to calculate the RMS no initialization required
 *  @var frequencyCalcParameters::BufferIndex 
 *  Used for keeping track of the index, MUST be initialized to 0
 *  @var frequencyCalcParameters::dBm_A_Constant 
 *  The A variable for calculating the power in dBM
 *  @var frequencyCalcParameters::dBm_B_Constant 
 *  The A variable for calculating the power in dBM
 *  @var frequencyCalcParameters::ADCCalibrationData 
 *  Pointer to an optional struct for the ADC calibration data
 */
struct frequencyCalcParameters {
    uint32_t ID;
    uint64_t Buffer;
    uint32_t BufferIndex;
    float dBm_A_Constant;
    float dBm_B_Constant;
    float antennaFactor;
    float frequency;
    struct ADCCalibration* ADCCalibrationData;
};

// 
// User functions
// 

/**
* @brief Function for storing ADC values in a buffer for later converting to a RMS value
* @param sample The ADC sample value
* @param frequencyInfo The pointer to the struct array with constants and the buffer
* @return Returns 0 for succes, -1 for failure and 1 for a full buffer.
*/
uint8_t fillRMSBuffer (uint16_t sample, struct frequencyCalcParameters* frequencyInfo);

/**
* @brief Function for getting EM variables ONCE the RMS buffer is full
* @param frequencyInfo The pointer to the struct array with constants and the buffer
* @param dBm Pointer to a float for storing the power in dBm
* @param power Pointer to a float for storing the power in watts
* @param powerDensity Pointer to a float for storing the power density
* @param electricField Pointer to a float for storing the electric field
*/
void    getEMVariables(struct frequencyCalcParameters* frequencyInfo, float* dBm, float* power, float* powerDensity, float* electricField);

// 
// Math functions
// 

/**
* @brief Function for converting the power detector voltage to dBm
* @param voltage The RMS voltage of the power detector over a period
* @param frequencyInfo The pointer to the struct array with constants and the buffer
* @return Returns power in dBm.
*/
float convertVoltageTodBm(float voltage, struct frequencyCalcParameters* frequencyInfo);

/**
* @brief Function for converting power in dBm to watts
* @param dBm Power in dbm
* @return Returns power in watts.
*/
float convertdBmToWatts(float dBm);

/**
* @brief Function for calculating the electric field strength
* @param powerDensity The power density
* @return Returns the electric field strength.
*/
float calculateElectricField(float powerDensity);

/**
* @brief Function for calculating the power density
* @param power The power in watt
* @param antennaFactor The antennaFactor
* @param frequency The frequency of the signal
* @return Returns the power density.
*/
float calculatePowerDensity(float power, float antennaFactor, float frequency);

#endif
