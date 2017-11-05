#include<stdio.h>
#include <stdlib.h> 
#include<math.h>
#include<time.h>
#include"AWGNChannel.h"

void AWGN_ini(struct AWGN*awgn,long seed_num);

/***************************************************************************************
* PURPOSE:  generate a random variable uniformly distributed between 0 and 1.
* Input:    SEED: random seed struct		
* Output:   No.
* Return:   a random variable uniformly distributed between 0 and 1
***************************************************************************************/
double Randomu(struct SEED *seed)
{ 
	double temp = 0.0;

	seed->ix = (seed->ix * 249) % 61967; 
	seed->iy = (seed->iy * 251) % 63443;
	seed->iz = (seed->iz * 252) % 63599;
	temp = (((double)seed->ix) / ((double)61967)) + (((double)seed->iy) / ((double)63443))
		   + (((double)seed->iz) / ((double)63599));
	temp -= (int)temp;

	
	return (temp);     
}

/***************************************************************************************
* PURPOSE:  generate the receive sequence with puncturing
* Input:    codeseq: the input channel sequence
*           awgn:    AWGN channel struct
*           length:  the number of bits of a frame 				
* Output:   receiveseq:  the received sequence after transfer through AWGN channel
***************************************************************************************/
void AWGNChannel(double *receiveseq, int *codeseq, struct AWGN *awgn,int length)
{
	int i = 0;
	double u1 = 0;
	double u2 = 0;
	double temp = 0;
	const double PI = 3.141592654;

	for (i=0; i<length; i++)
	{
		srand(unsigned int(time(NULL)+i));
		u1 = Randomu(awgn->seed);
		u2 = Randomu(awgn->seed);
		temp = (double)sqrt((double)(-2) * log((double)1 - u1));
		*(receiveseq + i) = (awgn->sigma) * cos(2 * PI * u2) * temp + (1-2 * (*(codeseq + i))); 
	}
	/*
	printf("经过AWGN信道后，输出为：\n");
	for (i=0;i<length;i++)
	{
		printf("%f\t",*(receiveseq+i));
	}
	printf("\n\n");
	*/
}

