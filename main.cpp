
#include<stdio.h>
#include <stdlib.h> 
#include<math.h>
#include<time.h>
#include"AWGNChannel.h"
#include"ldpc.h"

#define number 12000

int main(void)
{
	int i, k;
	int temp = 0, input_code[number] = { 0 }, output_code[number] = { 0 };
	double snr_db = 0, code_rate = 0, receive_code[number] = { 0 }, BER[50] = { 0 };
	FILE *fq;


	struct decode ldpc_decode;
	struct AWGN awgn_channel;

	ini(&ldpc_decode);
	AWGN_ini(&awgn_channel, 100);

	fq = fopen("E:\\123\\a.txt", "w");

	for (temp = 0; temp < 50; temp++)
	{
		snr_db = (temp + 1)*0.2;
		code_rate = 1 - (double)ldpc_decode.check_num / (double)ldpc_decode.variable_num;
		awgn_channel.snr = pow(10, snr_db / 10.0);
		awgn_channel.sigma = sqrt(1 / (awgn_channel.snr*code_rate * 2));
		AWGNChannel(receive_code, input_code, &awgn_channel, number);

		for (k = 0; k < (number / ldpc_decode.variable_num); k++)
		{

			for (i = 0; i < ldpc_decode.variable_num; i++)
			{
				*(ldpc_decode.receive_code + i) = receive_code[k*ldpc_decode.variable_num + i];
			}
			
			decode_ldpc(&ldpc_decode);

			
			
			


			for (i = 0; i < ldpc_decode.variable_num; i++)
			{
				output_code[k*ldpc_decode.variable_num + i] = *(ldpc_decode.output_code + i);
			}

			

			
		}

		
		

		/*
		printf("\n译码结果为\n");
		for (i = 0; i < number; i++)
		{
		printf("%d", output_code[i]);
		if ((i + 1) % ldpc_decode.variable_num == 0)
		printf("\t");
		}
		*/
		

		BER[temp] = (ber_ldpc(output_code, input_code, number));
	}
	
	free(ldpc_decode.check_node->output_message);
	free(ldpc_decode.check_node->pass_edge);
	free(ldpc_decode.check_node->pass_node);
	free(ldpc_decode.check_node);

	free(ldpc_decode.variable_node->output_message);
	free(ldpc_decode.variable_node->pass_edge);
	free(ldpc_decode.variable_node->pass_node);
	free(ldpc_decode.variable_node);

	printf("\n\n\n");
	for (temp = 0; temp < 50; temp++)
	{
		snr_db = (temp + 1)*0.2;
		printf("信噪比SNR等于%.1fdb时，误码率为", snr_db);
		if (BER[temp] == 0)
		{
			BER[temp] = 0;
			printf("0\n", BER[temp]);
		}
		else
		{
			printf("%f\n", BER[temp]);
		}
		/*
		fprintf(fq, "%f ", BER[temp]);
		*/
	}
	fclose(fq);
}