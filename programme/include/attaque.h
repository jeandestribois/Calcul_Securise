#ifndef ATTAQUE_H
#define ATTAQUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "attaque.h"
#include "DES.h"

#define TAILLE_TABLE_PC1_INV 64
#define TAILLE_TABLE_PC2_INV 56
#define NOMBRE_CHIFFRES_FAUTES 32
#define FICHIER_MESSAGE_CLAIR "donnees_attaque/message_clair.txt"
#define FICHIER_CHIFFRE_JUSTE "donnees_attaque/chiffre_juste.txt"
#define FICHIER_CHIFFRES_FAUTES "donnees_attaque/chiffres_fautes.txt"

// Table de permutation inverse de PC1
extern int table_pc1_inv[TAILLE_TABLE_PC1_INV];

// Table de permutation inverse de PC2
extern int table_pc2_inv[TAILLE_TABLE_PC2_INV];

// Table de permutation inverse de P
extern int table_p_inv[TAILLE_TABLE_P];

// Structure de liste chainée permettant de stocker les entrees des S-Box possible
struct liste {
	uint8_t val;
	struct liste *suiv;
};

// Structure permettant de stocker les informations nécessaire à l'attaque
struct attaque {
	uint64_t message;								// Contient le message clair
	uint64_t chiffre;								// Chiffre juste
	uint64_t chiffre_faute[NOMBRE_CHIFFRES_FAUTES]; // Contient les chiffres fautés
	uint32_t R_15;									// Contient R15
	uint32_t L_16;									// Contient L16
	uint32_t R_15_f[NOMBRE_CHIFFRES_FAUTES];		// Contient les R15 fautés
	uint32_t L_16_f[NOMBRE_CHIFFRES_FAUTES];		// Contient les L16 fautés
	uint32_t e[NOMBRE_CHIFFRES_FAUTES];				// Contient les fautes injectés sur R15 du chiffré au même indice
};


// Cette fonction n'est à appliquer seulement si on veut obtenir PC1⁻¹(x) avec les bits de partité
uint64_t permutation_pc1_inv(uint64_t source);

// Cette fonction permet de donner au différents attribut de la structure
// struct attaque toutes les valeurs nécessaire à l'attaque
struct attaque initialisation_attaque();

// Libération de la mémoire pour la liste
struct liste *liberation_memoire(struct liste* l); 

// Fonction rajoutant un élément dans la lites de partie de clés possible
struct liste *nouvel_element_liste(struct liste *l, uint8_t val);

// Cette fonction cherche pour un chiffré fauté toutes les parties de clés
// possibles pour chaques S-Boxs
void recherche_s_box(struct liste *k_partie[8], uint64_t R_15_exp, uint64_t R_15_f_exp, uint32_t L16_xor_L16_f, uint64_t e_exp);

// Fonction effectuant la recherche de K16
uint64_t recherche_K_16(struct attaque attaque);

#endif