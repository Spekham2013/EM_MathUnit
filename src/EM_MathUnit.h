#ifndef EM_MATHUNIT_H
#define EM_MATHUNIT_H

#include <stdint.h>
#include <math.h>

#define SUCCES 0
#define BUFFERSIZE 1024

void  calculateRootMeanSquare(uint16_t sample);
float getRootMeanSquare(void);

#endif