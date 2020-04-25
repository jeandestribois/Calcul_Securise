#include "../include/DES.h"
#include "../include/attaque.h"

int main(int argc, char const *argv[]) {

	struct attaque attaque = initialisation_attaque();

	uint64_t K_16 = recherche_K_16(attaque);

	printf("K_16 = %lx\n", K_16);

	uint64_t K = recherche_K(attaque, K_16);

	printf("K = %lx\n", K);

}