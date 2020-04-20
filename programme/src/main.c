#include "../include/DES.h"
#include "../include/attaque.h"

int main(int argc, char const *argv[]) {
	uint32_t test = 0xa6a6a6a6;

	struct attaque attaque = initialisation_attaque();

	recherche_K_16(attaque);


}