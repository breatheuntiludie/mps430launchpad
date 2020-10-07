#include <iostream>
#include <stdio.h>
#include <stdlib.h>

unsigned long int   seed = 1; 
char check = 0;


int main()
{
	unsigned long int a = 1103515245;
	unsigned long int m = 32768;
	int c = 12345;
	int i = 0;
	int n = 1; //
	int series = 225;
	int num = 0;
	int res = 10;
    	char ccc[2];
	unsigned long int *ptr = (unsigned long int *)malloc(sizeof(unsigned long int) * series * n);

    ccc[0]=10;
    ccc[1] = 0;
	while (i < series * n)
	{
        if (ccc[1] == 10 && ccc[0] == 10)
            ccc[0] = 8;
        else
        {
	        seed = ((seed * a + c) / 65536) % m;
	        ccc[0] = seed % 9;
        }
        if (ccc[0] == ccc[1] && ccc[0] == 8)
            ccc[0] = 10;
        ptr[i] = ccc[0];
		printf("ptr[%i]:%i, check:%i\n", i, ccc[0], ccc[1]);
		i++;
        ccc[1] = ccc[0];
	}
	i = 0;
	while (i < series * n)
	{
		if (ptr[i] == 8)
			num++;
		i++;
	}
	printf("\n\nsignificant num of stimuls:%i", num);

    return 0;
}