#include "../include/attaque.h"

// Table de permutation inverse de PC1
// (On met à 1 la position des 8 bits inconnus car ils seront determinés plus tard)
int table_pc1_inv[TAILLE_TABLE_PC1_INV] = {
8, 16, 24, 56, 52, 44, 36, 1,
7, 15, 23, 55, 51, 43, 35, 1,
6, 14, 22, 54, 50, 42, 34, 1,
5, 13, 21, 53, 49, 41, 33, 1,
4, 12, 20, 28, 48, 40, 32, 1,
3, 11, 19, 27, 47, 39, 31, 1,
2, 10, 18, 26, 46, 38, 30, 1,
1, 9, 17, 25, 45, 37, 29, 1};

// Table de permutation inverse de PC2
// (On met à 1 la position des 8 bits inconnus car ils seront determinés plus tard)
int table_pc2_inv[TAILLE_TABLE_PC2_INV] = {
5, 24, 7, 16, 6, 10, 20,
18, 1, 12, 3, 15, 23, 1,
9, 19, 2, 1, 14, 22, 11,
1, 13, 4, 1, 17, 21, 8,
47, 31, 27, 48, 35, 41, 1,
46, 28, 1, 39, 32, 25, 44,
1, 37, 34, 43, 29, 36, 38,
45, 33, 26, 42, 1, 30, 40};


// Table de permutation inverse de P
int table_p_inv[TAILLE_TABLE_P] = {
9, 17, 23, 31,
13, 28, 2, 18,
24, 16, 30, 6,
26, 20, 10, 1,
8, 14, 25, 3,
4, 29, 11, 19,
32, 12, 22, 7,
5, 27, 15, 21};

uint64_t permutation_pc1_inv(uint64_t source) {
	uint64_t resultat=0;
	uint64_t uint64_1 = 1;
	int bit_a_1 = 0;
	for (int i = 0; i < TAILLE_TABLE_PC1_INV; i++) {
		resultat <<= 1;
		// Cette condition est véifié tous les 8 bits et applique un bit de parité.
		if((i+1)%8 == 0) {
			if(bit_a_1%2 == 0)
				resultat |= 1;
			bit_a_1 = 0;
		}
		// Sinon on définit le bit de la même manière que la fonction précédente.
		else if(source & (uint64_1<<(TAILLE_TABLE_PC1 - table_pc1_inv[i]))) {
			resultat |= 1;
			bit_a_1++;
		}
	}
	return resultat;
}

struct attaque initialisation_attaque() {
	struct attaque attaque;
	// Cette variable nous permettra de lire dans les différents fichiers
	// où sont stocké les informations de l'attaque.
	FILE *fichier = NULL;
	// Cette variable stokera le resultat de la permutation inverse de P
	uint64_t p_inv = 0;

	// Récupération du message clair
	fichier = fopen(FICHIER_MESSAGE_CLAIR, "r");
	if (fichier == NULL) {
		printf("Erreur : Impossible d'ouvrir le fichier %s\n", FICHIER_MESSAGE_CLAIR);
		fclose(fichier);
		exit(0);
	}
	fscanf(fichier, "%lx", &attaque.message);

	// Récupération du chiffré juste
	fichier = fopen(FICHIER_CHIFFRE_JUSTE, "r");
	if (fichier == NULL) {
		printf("Erreur : Impossible d'ouvrir le fichier %s\n", FICHIER_CHIFFRE_JUSTE);
		fclose(fichier);
		exit(0);
	}
	fscanf(fichier, "%lx", &attaque.chiffre);
	
	// On trouve le R15 du chiffré juste
	p_inv = permutation(attaque.chiffre, table_ip, TAILLE_TABLE_IP, 64);
	attaque.R_15 = p_inv & 0x00000000ffffffff;
	attaque.L_16 = p_inv >> 32;

	// Récupération des chiffrés fautés et on en profite pour trouver
	// R15 et la faute appliqué sur R15
	fichier = fopen(FICHIER_CHIFFRES_FAUTES, "r");
	if (fichier == NULL) {
		printf("Erreur : Impossible d'ouvrir le fichier %s\n", FICHIER_CHIFFRES_FAUTES);
		fclose(fichier);
		exit(0);
	}
	for (int i = 0; i < NOMBRE_CHIFFRES_FAUTES; ++i) {
		fscanf(fichier, "%lx", &attaque.chiffre_faute[i]);
		p_inv = permutation(attaque.chiffre_faute[i], table_ip, TAILLE_TABLE_IP, 64);
		attaque.R_15_f[i] = p_inv & 0x00000000ffffffff;
		attaque.L_16_f[i] = p_inv >> 32;
		attaque.e[i] = attaque.R_15_f[i] ^ attaque.R_15;
	}
	fclose(fichier);

	return attaque;
}

struct liste *nouvel_element_liste(struct liste *l, uint8_t val) {
	struct liste *elem = malloc(sizeof(struct liste));
	elem->val = val;
	elem->suiv = l;
	return elem;
}

struct liste *liberation_memoire(struct liste* l) {
	if(l != NULL) {
		l->suiv = liberation_memoire(l->suiv);
		free(l);
	}
	return NULL;
}

void recherche_s_box(struct liste *k_partie[8], uint64_t R_15_exp, uint64_t R_15_f_exp, uint32_t L16_xor_L16_f, uint64_t e_exp) {
	// Cette variable nous permettra de filtrer les 6 derniers bits de e_exp pour le test
	uint64_t filtre_e = 0x000000000000003f;
	// Cette variable nous permettra de filtrer les 4 derniers bits de L16_xor_L16_f pour le test
	uint32_t filtre_l = 0x0000000f;
	// Cette variable stockera Si(ki XOR E(R15)) XOR Si(ki XOR E(R15'))
	uint8_t resultat_s_box = 0;
	// Cette variable stockera la partie correspondante de L16_xor_L16_f pour une S-Box 
	uint8_t L16_xor_L16_f_s = 0;

	// On regarde si il y a eu une modification pour chaque S-Box 
	if(e_exp & 0xfc0000000000) {
		// On prend la partie de L16_xor_L16_f correspondant à la S-Box 
		L16_xor_L16_f_s = L16_xor_L16_f>>28 & filtre_l;
		// On teste toutes les parties de clés possible.
		for (uint8_t k = 0; k < 64; ++k) {
			resultat_s_box = application_s_box(((R_15_exp>>42) & filtre_e) ^ k, table_S1);
			resultat_s_box ^= application_s_box(((R_15_f_exp>>42) & filtre_e) ^ k, table_S1);
			if(L16_xor_L16_f_s == resultat_s_box)
				k_partie[0] = nouvel_element_liste(k_partie[0], k);
		}
	}
	if(e_exp & 0x03f000000000) {
		L16_xor_L16_f_s = L16_xor_L16_f>>24 & filtre_l;
		for (uint8_t k = 0; k < 64; ++k) {
			resultat_s_box = application_s_box(((R_15_exp>>36) & filtre_e) ^ k, table_S2);
			resultat_s_box ^= application_s_box(((R_15_f_exp>>36) & filtre_e) ^ k, table_S2);
			if(L16_xor_L16_f_s == resultat_s_box)
				k_partie[1] = nouvel_element_liste(k_partie[1], k);
		}
	}
	if(e_exp & 0x000fc0000000) {
		L16_xor_L16_f_s = L16_xor_L16_f>>20 & filtre_l;
		for (uint8_t k = 0; k < 64; ++k) {
			resultat_s_box = application_s_box(((R_15_exp>>30) & filtre_e) ^ k, table_S3);
			resultat_s_box ^= application_s_box(((R_15_f_exp>>30) & filtre_e) ^ k, table_S3);
			if(L16_xor_L16_f_s == resultat_s_box)
				k_partie[2] = nouvel_element_liste(k_partie[2], k);
		}
	}
	if(e_exp & 0x00003f000000) {
		L16_xor_L16_f_s = L16_xor_L16_f>>16 & filtre_l;
		for (uint8_t k = 0; k < 64; ++k) {
			resultat_s_box = application_s_box(((R_15_exp>>24) & filtre_e) ^ k, table_S4);
			resultat_s_box ^= application_s_box(((R_15_f_exp>>24) & filtre_e) ^ k, table_S4);
			if(L16_xor_L16_f_s == resultat_s_box)
				k_partie[3] = nouvel_element_liste(k_partie[3], k);
		}
	}
	if(e_exp & 0x000000fc0000) {
		L16_xor_L16_f_s = L16_xor_L16_f>>12 & filtre_l;
		for (uint8_t k = 0; k < 64; ++k) {
			resultat_s_box = application_s_box(((R_15_exp>>18) & filtre_e) ^ k, table_S5);
			resultat_s_box ^= application_s_box(((R_15_f_exp>>18) & filtre_e) ^ k, table_S5);
			if(L16_xor_L16_f_s == resultat_s_box)
				k_partie[4] = nouvel_element_liste(k_partie[4], k);
		}
	}
	if(e_exp & 0x00000003f000) {
		L16_xor_L16_f_s = L16_xor_L16_f>>8 & filtre_l;
		for (uint8_t k = 0; k < 64; ++k) {
			resultat_s_box = application_s_box(((R_15_exp>>12) & filtre_e) ^ k, table_S6);
			resultat_s_box ^= application_s_box(((R_15_f_exp>>12) & filtre_e) ^ k, table_S6);
			if(L16_xor_L16_f_s == resultat_s_box)
				k_partie[5] = nouvel_element_liste(k_partie[5], k);
		}
	}
	if(e_exp & 0x000000000fc0) {
		L16_xor_L16_f_s = L16_xor_L16_f>>4 & filtre_l;
		for (uint8_t k = 0; k < 64; ++k) {
			resultat_s_box = application_s_box(((R_15_exp>>6) & filtre_e) ^ k, table_S7);
			resultat_s_box ^= application_s_box(((R_15_f_exp>>6) & filtre_e) ^ k, table_S7);
			if(L16_xor_L16_f_s == resultat_s_box)
				k_partie[6] = nouvel_element_liste(k_partie[6], k);
		}
	}
	if(e_exp & 0x00000000003f) {
		L16_xor_L16_f_s = L16_xor_L16_f & filtre_l;
		for (uint8_t k = 0; k < 64; ++k) {
			resultat_s_box = application_s_box(((R_15_exp) & filtre_e) ^ k, table_S8);
			resultat_s_box ^= application_s_box(((R_15_f_exp) & filtre_e) ^ k, table_S8);
			if(L16_xor_L16_f_s == resultat_s_box)
				k_partie[7] = nouvel_element_liste(k_partie[7], k);
		}
	}
}

uint64_t recherche_K_16(struct attaque attaque) {
	// Variable qui stockera la valeur de L16 XOR L16' (puis P-1(L16 XOR L16'))
	uint32_t L16_xor_L16_f = 0;
	// Variable stockant les parties de K16 qui fonctionnent pour un certain chiffré fauté
	// Pour chaques chiffrés fautés et pour chaque S-Box on a une liste de possibilités
	struct liste *k_partie[NOMBRE_CHIFFRES_FAUTES][8];
	// Variable stockant l'expansion E(e) de la faute
	uint64_t e_exp = 0;
	// Variable stockant l'expansion E(R_15)
	uint64_t R_15_exp = permutation(attaque.R_15, table_e, TAILLE_TABLE_E, 32);
	// Varibale stockant l'expansion E(R_15_f)
	uint64_t R_15_f_exp = 0;
	// Variable qui sera utile pour parcourir la liste de partie de clé
	struct liste *parcours;

	// Initialisation du tableau de liste k_partie
	for (int i = 0; i < NOMBRE_CHIFFRES_FAUTES; ++i)
		for (int j = 0; j < 8; ++j)
			k_partie[i][j] = NULL;

	// On parcours tous les chiffrés fautés
	for(int i = 0; i<NOMBRE_CHIFFRES_FAUTES; i++) {
		// Calcul de P-1(L16 XOR L16')
		L16_xor_L16_f = attaque.L_16 ^ attaque.L_16_f[i];
		L16_xor_L16_f = permutation(L16_xor_L16_f, table_p_inv, TAILLE_TABLE_P, 32);
		// Calcul de E(e)
		e_exp = permutation(attaque.e[i], table_e, TAILLE_TABLE_E, 32);
		// Calcul de E(R15')
		R_15_f_exp = permutation(attaque.R_15_f[i], table_e, TAILLE_TABLE_E, 32);
		// On effectue la recherche sur ce chiffré fauté
		recherche_s_box(k_partie[i], R_15_exp, R_15_f_exp, L16_xor_L16_f, e_exp);
	}

	for (int i = 0; i < NOMBRE_CHIFFRES_FAUTES; ++i) {
		printf("E(e[i]) = %lx\n", permutation(attaque.e[i], table_e, TAILLE_TABLE_E, 32));
		for (int j = 0; j < 8; ++j) {
			parcours = k_partie[i][j];
			while(parcours != NULL) {
				printf("k_partie[%d][%d] = %x\n", i, j, parcours->val);
				parcours = parcours->suiv;
			}
			k_partie[i][j] = liberation_memoire(k_partie[i][j]);
		}
	}

	return 0;
}