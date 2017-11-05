
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
	char temp[8]; //�����洢�����������ַ�
	int i = 0; //������¼��ǰ�ַ������λ��
	int stop = 0; //��־�Ƿ���������֣�����һ�����֣�
	char ch; //��¼��ǰ�������ַ�
	while (!stop)
	{
		ch = fgetc(fp); //��ȡһ���ַ�
		if (ch == EOF)    //�ж��Ƿ�������ļ�β
			break;
		while (ch >= '0'&&ch <= '9')//�������Ƿ���һ�������ַ�
		{
			stop = 1;//��־λ��1
			temp[i++] = ch;//��ǰ�����ַ��浽�����Ȼ��i+1
			ch = fgetc(fp);//������ȡ�ַ�
		}
	}
	temp[i] = 0;//�ڵ�ǰ���������0
	return atol(temp);//���ַ���תΪ������������
}