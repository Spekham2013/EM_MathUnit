#ifndef EM_MATHUNIT_H
#define EM_MATHUNIT_H

#include <stdint.h>
#include <math.h>

// 
// Math constants
// 
#define PI120 376.99111843077518861551720599354034610366032792501269851699

// 
// Functional constants
// 
#define BUFFERFULL 1
#define SUCCES     0
#define FAILURE    -1

#define BUFFERSIZE 1024

// 
// Structs
// 

/** @struct frequencyCalcParameters
 *  @brief This struct is used for storing and passing information to user functions
 *  @var frequencyCalcParameters::ID 
 *  This value must be unique
 *  @var frequencyCalcParameters::Buffer[BUFFERSIZE] 
 *  Used to calculate the RMS no initialization required
 *  @var frequencyCalcParameters::BufferIndex 
 *  Used for keeping track of the index, MUST be initialized to 0
 *  @var frequencyCalcParameters::dBm_A_Constant 
 *  The A variable for calculuting the power in dBM
 *  @var frequencyCalcParameters::dBm_B_Constant 
 *  The A variable for calculuting the power in dBM
 */
struct frequencyCalcParameters {
    uint32_t ID;
    uint32_t Buffer[BUFFERSIZE];
    uint32_t BufferIndex;
    float dBm_A_Constant;
    float dBm_B_Constant;
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
* @param electricField Pointer to a float for storing the electric field
* @param magneticField Pointer to a float for storing the magnetic field
* @param powerDensity Pointer to a float for storing the power density
*/
void    getEMVariables(struct frequencyCalcParameters* frequencyInfo, float* dBm, float* power, float* electricField, float* magneticField, float* powerDensity);

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
* @param power The power in watts
* @return Returns the electric field strength.
*/
float calculateElectricField(float power);

/**
* @brief Function for calculating the magnetic field strength
* @param electricField The electicfield strength
* @return Returns the magnetic field strength.
*/
float calculateMagneticField(float electricField);

#endif
