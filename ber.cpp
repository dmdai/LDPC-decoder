
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"ldpc.h"

double ber_ldpc(int *output_code, int *input_code, int length)
{
	int i;
	int err = 0;
	double ber;

	for (i = 0; i < length; i++)
	{
		if (output_code[i] != input_code[i])
			err += 1;
	}
	
	ber = (double)err / length;

	return(ber);
}