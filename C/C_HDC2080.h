#ifndef C_HDC2080_H
#define C_HDC2080_H

#include <stdint.h>    				//for uint





//negative on error, positive on success
int setup_hdc2080();
int read_from_hdc2080(float * temperature, float * humidity);





#endif
