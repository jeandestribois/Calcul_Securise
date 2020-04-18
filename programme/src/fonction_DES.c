#include "../include/fonction_DES.h"

// Table de permutation de IP
int table_ip[TAILLE_TABLE_IP] = {
58, 50, 42, 34, 26, 18, 10, 2,
60, 52, 44, 36, 28, 20, 12, 4,
62, 54, 46, 38, 30, 22, 14, 6,
64, 56, 48, 40, 32, 24, 16, 8,
57, 49, 41, 33, 25, 17, 9, 1,
59, 51, 43, 35, 27, 19, 11, 3,
61, 53, 45, 37, 29, 21, 13, 5,
63, 55, 47, 39, 31, 23, 15, 7};

// Table de permutation de IP⁻¹
int table_ip_inv[TAILLE_TABLE_IP] = {
40, 8, 48, 16, 56, 24, 64, 32,
39, 7, 47, 15, 55, 23, 63, 31,
38, 6, 46, 14, 54, 22, 62, 30,
37, 5, 45, 13, 53, 21, 61, 29,
36, 4, 44, 12, 52, 20, 60, 28,
35, 3, 43, 11, 51, 19, 59, 27,
34, 2, 42, 10, 50, 18, 58, 26,
33, 1, 41, 9, 49, 17, 57, 25};

// Table de permutation de PC1
int table_pc1[TAILLE_TABLE_PC1] = {
57, 49, 41, 33, 25, 17, 9,
1, 58, 50, 42, 34, 26, 18,
10, 2, 59, 51, 43, 35, 27,
19, 11, 3, 60, 52, 44, 36,
63, 55, 47, 39, 31, 23, 15,
7, 62, 54, 46, 38, 30, 22,
14, 6, 61, 53, 45, 37, 29,
21, 13, 5, 28, 20, 12, 4};

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


// Table de permutation/extraction de PC2
int table_pc2[TAILLE_TABLE_PC2] = {
14, 17, 11, 24, 1, 5,
3, 28, 15, 6, 21, 10,
23, 19, 12, 4, 26, 8,
16, 7, 27, 20, 13, 2,
41, 52, 31, 37, 47, 55,
30, 40, 51, 45, 33, 48,
44, 49, 39, 56, 34, 53,
46, 42, 50, 36, 29, 32};

// Table d'expansion E
int table_e[TAILLE_TABLE_E] = {
32, 1, 2, 3, 4, 5,
4, 5, 6, 7, 8, 9,
8, 9, 10, 11, 12, 13,
12, 13, 14, 15, 16, 17,
16, 17, 18, 19, 20, 21,
20, 21, 22, 23, 24, 25,
24, 25, 26, 27, 28, 29,
28, 29, 30, 31, 32, 1};

// Table de permutation P
int table_p[TAILLE_TABLE_P] = {
16, 7, 20, 21,
29, 12, 28, 17,
1, 15, 23, 26,
5, 18, 31, 10,
2, 8, 24, 14,
32, 27, 3, 9,
19, 13, 30, 6,
22, 11, 4, 25};

// Tablse S-Box (de 1 à 8)
int table_S1[TAILLE_S_BOX] = {
14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13};

int table_S2[TAILLE_S_BOX] = {
15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9};

int table_S3[TAILLE_S_BOX] = {
10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12};

int table_S4[TAILLE_S_BOX] = {
7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14};

int table_S5[TAILLE_S_BOX] = {
2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3};

int table_S6[TAILLE_S_BOX] = {
12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13};

int table_S7[TAILLE_S_BOX] = {
4, 11, 2, 14, 15, 0, 8 , 3, 3, 12, 9, 7, 5, 10, 6, 1,
13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12};

int table_S8[TAILLE_S_BOX] = { 
13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11};


uint64_t permutation(uint64_t source, int *table, int taille_table, int taille_entree) {
	uint64_t resultat = 0;
	uint64_t uint64_1 = 1;
	// On définit chaque bit de la variable resultat en fonction de source et
	// en fonction de la table de permutation donnée.
	for (int i = 0; i < taille_table; i++) {
		resultat <<= 1;
		if(source & (uint64_1<<(taille_entree-table[i])))
			resultat |= 1;
	}
	return resultat;
}

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

void generation_sous_cles(uint64_t K, uint64_t *k) {
	uint32_t C = 0;
	uint32_t D = 0;
	// Cette variable stockera le premier bit de C (et D) pour le remettre à la fin (pour le shift).
	uint32_t C_circulaire = 0;
	uint32_t D_circulaire = 0;
	uint64_t pc1_K = 0;
	// Table régissant le nombre de shifts à faire par tour
	int table_shift[16] = {
	1, 1, 2, 2, 2, 2, 2, 2,
	1, 2, 2, 2, 2, 2, 2, 1};

	// On applique pc1 sur K
	pc1_K = permutation(K, table_pc1, TAILLE_TABLE_PC1, 64);

	// On sépare pc1_K en deux
	C = (pc1_K >> 28) & 0x000000000fffffff;
	D = pc1_K & 0x000000000fffffff;

	// Boucle de génération des sous clés
	for (int i = 0; i < 16; ++i){
		// On recupère les 1 ou 2 premiers bits de C et D (en fonction de la valeur du shift)
		if(table_shift[i] == 2) {
			C_circulaire = (0X0c000000 & C)>>26;
			D_circulaire = (0X0c000000 & D)>>26;
		}
		else {
			C_circulaire = (0X08000000 & C)>>27;
			D_circulaire = (0X08000000 & D)>>27;	
		}

		// On applique le shift et on rajoute la variable obtenu précédement à la fin.
		C = ((C << table_shift[i]) | C_circulaire) & 0x0fffffff;
		D = ((D << table_shift[i]) | D_circulaire) & 0x0fffffff;

		// On concatène la variable C et D en réutilisant la variable pc1_K;
		pc1_K = C;
		pc1_K = ((pc1_K << 28) | (D & 0x0fffffff)) & 0x00ffffffffffffff;

		// On applique pc2 sur la concatenation de C et D et on met tous les bits après le 48ème à 0.
		k[i] = permutation(pc1_K, table_pc2, TAILLE_TABLE_PC2, 56) & 0x0000ffffffffffff;
	}
}

uint8_t application_s_box(uint8_t source, int *table_s) {
	int indice = 0;
	// On définit l'indice à partir de source tel que : source = b1b2b3b4b5b6 -> indice = b1b6b2b3b4b5
	indice = (source & 0x20) | ((source & 0x01)<<4) | ((source & 0x1e)>>1);
	return table_s[indice];
}

uint64_t fonction_S(uint64_t source) {
	uint64_t resultat = 0;
	// Variable qui servira comme entrée et sortie des S-Box
	uint64_t s_box_val = 0;

	// On applique chaque partie de source sur les S-Box correspondantes
	s_box_val = (source & 0x0000fc0000000000)>>42;
	s_box_val = application_s_box(s_box_val, table_S1);
	resultat |= s_box_val<<28;

	s_box_val = (source & 0x000003f000000000)>>36;
	s_box_val = application_s_box(s_box_val, table_S2);
	resultat |= s_box_val<<24;

	s_box_val = (source & 0x0000000fc0000000)>>30;
	s_box_val = application_s_box(s_box_val, table_S3);
	resultat |= s_box_val<<20;

	s_box_val = (source & 0x000000003f000000)>>24;
	s_box_val = application_s_box(s_box_val, table_S4);
	resultat |= s_box_val<<16;

	s_box_val = (source & 0x0000000000fc0000)>>18;
	s_box_val = application_s_box(s_box_val, table_S5);
	resultat |= s_box_val<<12;

	s_box_val = (source & 0x00000000003f000)>>12;
	s_box_val = application_s_box(s_box_val, table_S6);
	resultat |= s_box_val<<8;

	s_box_val = (source & 0x0000000000000fc0)>>6;
	s_box_val = application_s_box(s_box_val, table_S7);
	resultat |= s_box_val<<4;

	s_box_val = (source & 0x00000000000003f);
	s_box_val = application_s_box(s_box_val, table_S8);
	resultat |= s_box_val;

	return resultat;
}

uint32_t fonction_F(uint32_t source, uint64_t k) {
	uint64_t resultat = 0;

	// Expansion E
	resultat = permutation(source, table_e, TAILLE_TABLE_E, 32);
	// XOR k
	resultat = resultat ^ k;
	// S-Box
	resultat = fonction_S(resultat);
	// Permutation P
	resultat = permutation(resultat, table_p, TAILLE_TABLE_P, 32);

	return resultat;
}

uint64_t chiffrement_DES(uint64_t message, uint64_t K) {
	uint64_t resultat = 0;
	// Variable qui permettra de sauvegarder temporairement la valeur de L;
	uint32_t L_tmp = 0;
	// Partie gauche et droite du message
	uint32_t L = 0;
	uint32_t R = 0;
	// Sous-clés de K
	uint64_t k[16] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0};

	// Génération des sous clés
	generation_sous_cles(K, k);
	// Passage dans IP
	message = permutation(message, table_ip, TAILLE_TABLE_IP, 64);

	// Séparation en 2 de message
	L = message>>32;
	R = message & 0x00000000ffffffff;

	// Boucle principale du shéma de Festel
	for (int i = 0; i < 15; ++i) {
		L_tmp = L;
		L = R;
		R = fonction_F(R, k[i]) ^ L_tmp;
	}

	// Dernier tour un peu different
	L = L ^ fonction_F(R, k[15]);

	// Concatenation de L et R
	resultat = L;
	resultat = (resultat<<32) | R;

	// Passage dans IP⁻¹
	resultat = permutation(resultat, table_ip_inv, TAILLE_TABLE_IP, 64);

	return resultat;
}

