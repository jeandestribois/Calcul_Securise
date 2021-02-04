#ifndef DES_H
#define DES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define TAILLE_TABLE_IP 64
#define TAILLE_TABLE_PC1 56
#define TAILLE_TABLE_PC2 48
#define TAILLE_TABLE_E 48
#define TAILLE_TABLE_P 32
#define TAILLE_S_BOX 64

// Table de permutation de IP
extern int table_ip[TAILLE_TABLE_IP];

// Table de permutation inverse de IP
extern int table_ip_inv[TAILLE_TABLE_IP];

// Table de permutation de PC1
extern int table_pc1[TAILLE_TABLE_PC1];

// Table de permutation/extraction de PC2
extern int table_pc2[TAILLE_TABLE_PC2];

// Table d'expansion E
extern int table_e[TAILLE_TABLE_E];

// Table de permutation P
extern int table_p[TAILLE_TABLE_P];

// Tablse S-Box (de 1 à 8)
extern int table_S1[TAILLE_S_BOX];
extern int table_S2[TAILLE_S_BOX];
extern int table_S3[TAILLE_S_BOX];
extern int table_S4[TAILLE_S_BOX];
extern int table_S5[TAILLE_S_BOX];
extern int table_S6[TAILLE_S_BOX];
extern int table_S7[TAILLE_S_BOX];
extern int table_S8[TAILLE_S_BOX];



// Fonction appliquant la permutation correspondant
// à la table donné en entrée.
uint64_t permutation(uint64_t source, int *table, int taille_table, int taille_entree);

// Fonction permettant d'appliquer une S-Box au paramètre source
uint8_t application_s_box(uint8_t source, int *table_s);

// Fonction de génération des 16 sous-clé à partir de la clé K
void generation_sous_cles(uint64_t K, uint64_t *k);

// Fonction appliquant toutes les S-Boxs sur source selon la définition du DES
uint64_t fonction_S(uint64_t source);

// Fonction appliquant la fonction F décrite dans le DES 
uint32_t fonction_F(uint32_t source, uint64_t k);

// Fonction executant la génération de sous clés,
// le chiffrement complet du DES et renvoyant le message chiffré.
uint64_t chiffrement_DES(uint64_t message, uint64_t K);

// Fonction executant la génération de sous clés,
// le chiffrement complet du DES et renvoyant le message chiffré.
uint64_t dechiffrement_DES(uint64_t chiffre, uint64_t K);


#endif