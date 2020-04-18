#include "../include/main.h"

int main(int argc, char const *argv[])
{
	uint64_t message = 0x566d4536ed24513f;

	uint64_t K = 0xab3443644698a2f2;

	printf("\nmessage = %lx\n", message);
	printf("\nK = %lx\n", K);

	message = chiffrement_DES(message, K);

	printf("\nchiffré = %lx\n", message);

}