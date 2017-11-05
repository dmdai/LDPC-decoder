
#ifndef CHANNEL_H
#define CHANNEL_H

//random seed struct
struct SEED
{
	long ix;					//long seed
	long iy;					//long seed
	long iz;					//long seed
};

//AWGN channel struct
 struct AWGN
{
	struct SEED *seed;			//random seed to generate Gauss variable
	double  snr;				//channel SNR ,not db
	double  sigma;				//variation of Gauss process
};

//void BECChannel(int *receiveseq,const int *codeseq,double epison,int length);
//#define  normal_rand()    ((double)rand()/(RAND_MAX+1))   //to generate a number between 0-1
double Randomu(struct SEED *seed);
void AWGN_ini(struct AWGN*awgn,long seed_num);
void AWGNChannel(double *receiveseq, int *codeseq, struct AWGN *awgn,int length);

#endif