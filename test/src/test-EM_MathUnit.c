#include <unity.h>

#include <EM_MathUnit.h>

void TEST_RMS(void) {
    for (uint32_t i = 0; i < BUFFERSIZE; i++) {
        calculateRootMeanSquare((i % 2) ? 0 : 4096);
    }
    TEST_ASSERT_FLOAT_WITHIN(0.000805, 2.33152904747, getRootMeanSquare());


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

        calculateRootMeanSquare(sineValue);
    }
    // Actual value of the simulated sine is 1.29175, but because of steps actual value should be
    TEST_ASSERT_FLOAT_WITHIN(0.000805, 1.4758, getRootMeanSquare());
}

int main(int argc, char **argv) {
    UNITY_BEGIN();

    // Voltages
    RUN_TEST(TEST_RMS);


    UNITY_END();

    return 0;
}