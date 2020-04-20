#include "../include/DES.h"
#include "../include/attaque.h"

int main(int argc, char const *argv[]) {

	struct attaque attaque = initialisation_attaque();

	uint64_t K = recherche_K(attaque, recherche_K_16(attaque));

	printf("K = %lx\n", K);

}