#include <unity.h>

#include <EM_MathUnit.h>

void TEST_RMS(void) {
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
    TEST_ASSERT_FLOAT_WITHIN(0.01, 31.1909183171895, dBm);

    // Simulate sine wave dived into 10 samples with the sine:
    // sin(pi/10 t) from 0 to 4
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

        fillRMSBuffer(sineValue, &param);
    }
    // Actual value of the simulated sine is 1.29175, but because of steps actual value should be
    getEMVariables(&param, &dBm, &power, &electricField, &magneticField, &powerDensity);
    TEST_ASSERT_FLOAT_WITHIN(0.01, -11.7565872020075, dBm);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();

    // Voltages
    RUN_TEST(TEST_RMS);


    UNITY_END();

    return 0;
}