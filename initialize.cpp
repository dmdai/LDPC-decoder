
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include"AWGNChannel.h"
#include"ldpc.h"

int Read_Num(FILE *fp);

void AWGN_ini(struct AWGN *awgn, long seed_num)
{
	awgn->seed = (struct SEED*)malloc(1 * sizeof(struct SEED));
	awgn->seed->ix = seed_num;
	awgn->seed->iy = seed_num;
	awgn->seed->iz = seed_num;
}

void ini(decode *ldpc_decode)
{
	int i, j;
	node *current_node;
	FILE*fp;

	fp = fopen("ldpc_data.txt", "r");
	if (fp == NULL)
	{
		printf("OPEN FALID\n");
		exit(1);
	}

	ldpc_decode->check_num = Read_Num(fp);
	ldpc_decode->variable_num = Read_Num(fp);
	ldpc_decode->variable_node = (node*)malloc(ldpc_decode->variable_num*sizeof(node));
	if (ldpc_decode->variable_node == NULL)
	{
		printf("memory allocation failure for ldpc_decode->variable_node!\n");
		exit(1);
	}
	ldpc_decode->check_node = (node*)malloc(ldpc_decode->check_num*sizeof(node));
	if (ldpc_decode->check_node == NULL)
	{
		printf("memory allocation failure for ldpc_decode->check_node!\n");
		exit(1);
	}
	for (i = 0; i < ldpc_decode->variable_num; i++)
	{
		current_node = ldpc_decode->variable_node + i;
		current_node->degree = Read_Num(fp);
		current_node->pass_node = (int*)malloc(current_node->degree*sizeof(int));
		if (current_node->pass_node == NULL)
		{
			printf("memory allocation failure for variable_node->pass_node!\n");
			exit(1);
		}
		current_node->pass_edge = (int*)malloc(current_node->degree*sizeof(int));
		if (current_node->pass_edge == NULL)
		{
			printf("memory allocation failure for variable_node->pass_edge!\n");
			exit(1);
		}
		current_node->output_message = (double*)malloc(current_node->degree*sizeof(double));
		if (current_node->output_message == NULL)
		{
			printf("memory allocation failure for variable_node->output_message!\n");
			exit(1);
		}
	}

	for (i = 0; i < ldpc_decode->check_num; i++)
	{
		current_node = ldpc_decode->check_node + i;
		current_node->degree = Read_Num(fp);
		current_node->pass_node = (int*)malloc(current_node->degree*sizeof(int));
		if (current_node->pass_node == NULL)
		{
			printf("memory allocation failure for check_node->pass_node!\n");
			exit(1);
		}
		current_node->pass_edge = (int*)malloc(current_node->degree*sizeof(int));
		if (current_node->pass_edge == NULL)
		{
			printf("memory allocation failure for check_node->pass_edge!\n");
			exit(1);
		}
		current_node->output_message = (double*)malloc(current_node->degree*sizeof(double));
		if (current_node->output_message == NULL)
		{
			printf("memory allocation failure for check_node->output_message!\n");
			exit(1);
		}
	}

	for (i = 0; i < ldpc_decode->variable_num; i++)
	{
		current_node = ldpc_decode->variable_node + i;
		for (j = 0; j < current_node->degree; j++)
		{
			*(current_node->pass_node + j) = Read_Num(fp);
			*(current_node->pass_node+j)-=1;
		}
	}

	for (i = 0; i < ldpc_decode->check_num; i++)
	{
		current_node = ldpc_decode->check_node + i;
		for (j = 0; j < current_node->degree; j++)
		{
			*(current_node->pass_node + j) = Read_Num(fp);
			*(current_node->pass_node+j)-=1;
		}
	}

	for (i = 0; i < ldpc_decode->variable_num; i++)
	{
		current_node = ldpc_decode->variable_node + i;
		for (j = 0; j < current_node->degree; j++)
		{
			*(current_node->pass_edge + j) = Read_Num(fp);
		}
	}

	for (i = 0; i < ldpc_decode->check_num; i++)
	{
		current_node = ldpc_decode->check_node + i;
		for (j = 0; j < current_node->degree; j++)
		{
			*(current_node->pass_edge + j) = Read_Num(fp);
		}
	}
	
	

	fclose(fp);

	ldpc_decode->receive_code = (double*)malloc(ldpc_decode->variable_num*sizeof(double));
	if (ldpc_decode->receive_code == NULL)
	{
		printf("memory allocation failure for receive_code!\n");
		exit(1);
	}

	ldpc_decode->output_code = (int*)malloc(ldpc_decode->variable_num*sizeof(int));
	if (ldpc_decode->output_code == NULL)
	{
		printf("memory allocation failure for output_code!\n");
		exit(1);
	}
}


int Read_Num(FILE *fp)
{
	char temp[8]; //用来存储读到的数字字符
	int i = 0; //用来记录当前字符数组的位置
	int stop = 0; //标志是否读到了数字（仅读一组数字）
	char ch; //记录当前读到的字符
	while (!stop)
	{
		ch = fgetc(fp); //读取一个字符
		if (ch == EOF)    //判断是否读到了文件尾
			break;
		while (ch >= '0'&&ch <= '9')//读到的是否是一个数字字符
		{
			stop = 1;//标志位置1
			temp[i++] = ch;//当前数字字符存到数组里，然后i+1
			ch = fgetc(fp);//继续读取字符
		}
	}
	temp[i] = 0;//在当前数组最后填0
	return atol(temp);//将字符串转为长整数并返回
}