
void ini(struct decode *ldpc_decode);
void decode_ldpc(struct decode *ldpc_decode);
double ber_ldpc(int *output_code, int *input_code, int length);

typedef struct ldpc_node{
	int degree;
	int *pass_node;
	int *pass_edge;
	double *output_message;
}node;

struct decode{
	double *receive_code;
	int *output_code;
	node *variable_node;
	node *check_node;
	int check_num;
	int variable_num;
};
