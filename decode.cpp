
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"ldpc.h"


int sgn(double x)
{
	if (x > 0)
		return 1;
	else if (x < 0)
		return -1;
	else
		return 0;
}

double min(double *x,int no)
{
	int i;
	double min = x[0];

	for (i = 1; i < no; i++)
	{
		if (min > x[i])
			min = x[i];
	}

	return(min);
}

void check_to_variable(node *check_node, node *variable_node)
{
	int i, j;
	int variable_position, variable_edge;
	double *variable_message = 0;
	

	variable_message = (double*)malloc(check_node->degree*sizeof(double));
	if (variable_message == NULL)
	{
		printf("memory allocation failure for variable_message!\n");
		exit(1);
	}

	for (i = 0; i < check_node->degree; i++)
	{
		variable_position = *(check_node->pass_node + i);
		variable_edge = *(check_node->pass_edge + i);
		*(variable_message + i) = *((variable_node + variable_position)->output_message + variable_edge);
		for (j = 0; j < check_node->degree; j++)
		{
			if (j != i)
				*(check_node->output_message + i) = sgn(*(variable_message + j))*min(&*(variable_message + j), check_node->degree - 1);
		}
	}
	free(variable_message);

}


int variable_to_check(node *variable_node, node *check_node, double receive_code)
{
	int i;
	int check_position, check_edge;
	int output = 0;
	double temp = 0, LLR = 0;
	double *check_message = 0;

	check_message = (double*)malloc(variable_node->degree*sizeof(double));
	if (check_message == NULL)
	{
		printf("memory allocation failure for check_message!\n");
		exit(1);
	}

	for (i = 0; i < variable_node->degree; i++)
	{
		check_position = *(variable_node->pass_node + i);
		check_edge = *(variable_node->pass_edge + i);
		*(check_message + i) = *((check_node + check_position)->output_message + check_edge);
		temp += *(check_message + i);
	}
	LLR = receive_code + temp;
	if (LLR <= 0)
		output = 1;
	
	for (i = 0; i < variable_node->degree; i++)
	{
		*(variable_node->output_message + i) = LLR - *(check_message + i);
	}

	return output;

	free(check_message);
}

int judge(struct decode *ldpc_decode)
{
	int i, j, variable_position;
	node *current_node;
	int temp;
	for (i = 0; i < ldpc_decode->check_num; i++)
	{
		current_node = ldpc_decode->check_node + i;
		temp = 0;
		for (j = 0; j < current_node->degree; j++)
		{
			variable_position = *(current_node->pass_node + j);
			temp += *(ldpc_decode->output_code + variable_position);
		}
		if (temp % 2 == 1)
			return 0;
	}
	return 1;
}

void decode_ldpc(struct decode *ldpc_decode)
{
	int i, j, k;
	int result = 0;
	int max_iteration = 100;
	node *current_node;


	for (i = 0; i < ldpc_decode->variable_num; i++)
	{
		current_node = ldpc_decode->variable_node + i;
		for (j = 0; j < current_node->degree; j++)
			*(current_node->output_message + j) = *(ldpc_decode->receive_code + i);
	}
	
	for (k = 0; k < max_iteration; k++)
	{
		for (i = 0; i < ldpc_decode->check_num; i++)
		{
			current_node = ldpc_decode->check_node + i;
			check_to_variable(current_node, ldpc_decode->variable_node);
			
			
		}

		for (i = 0; i < ldpc_decode->variable_num; i++)
		{
			current_node = ldpc_decode->variable_node + i;
			*(ldpc_decode->output_code + i) = variable_to_check(current_node, ldpc_decode->check_node, *(ldpc_decode->receive_code + i));
			

		}
		
		result = judge(ldpc_decode);
		
		if (result == 1)
			break;
	

		
	}

	

}
