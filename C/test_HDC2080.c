#include "C_HDC2080.h"
#include <stdio.h>

int main()
{
	float temperature, humidity; 

	if(setup_hdc2080() != 0 ) return 0; 

	if(read_from_hdc2080(&temperature, &humidity)!=0) return 0;

	printf("{ temperature : %f, humidity : %f }\n", temperature, humidity);

	return 0;
}
