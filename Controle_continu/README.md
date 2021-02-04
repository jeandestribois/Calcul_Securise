# Projet Calcul Sécurisé

Projet du module Calcul Sécurisé du M1 Informatique de Paris-Saclay.

## Concepteur

Jean Destribois

## Description

Ce projet concerne l'étude et l'implémentation d'une attaque par fautes sur R15 dans le DES.
Ce dépot contient :
* Un dossier "ressources" dans lequel se trouve le sujet et l'énoncé personnel (un message en clair, son chiffré, et 32 chiffrés fautés de ce message).
* Un dossier "compte_rendu" dans lequel se trouve le compte rendu de ce projet (en format PDF et latex).
* Un dossier "programme" dans lequel se trouve le programme permettant de réaliser cette attaque.
* Et un dossier "schemas" dans lequel se trouve des schémas utiles pour le compte rendu.

## Installation

Aucune installation n'est requise.
Cependant il est nécessaire de posèder gcc pour compiler le programme.

## Utilisation du programme

Une fois placé dans le dossier "programme", la commande "make" permet de compiler et d'exécuter le programme. Celui-ci viendra lire dans le dossier "données_attaque" le message en clair, son chiffré, et 32 chiffrés fautés de ce message. Il effectura ensuite l'attaque sur ces données et affichera la clé K16 et K dans le terminal si tout s'est bien passé.