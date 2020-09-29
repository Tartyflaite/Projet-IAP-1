#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { FAUX = 0, VRAI = 1 } Booleen;
Booleen EchoActif = FAUX;

// Messages emis par les instructions -----------------------------------------
#define MSG_DEVELOPPE "## nouvelle specialite \"%s \" ; cout horaire \"%d\"\n" 
#define MSG_INTERRUPTION "## fin de programme\n" 
#define MSG_EMBAUCHE "## nouveau travailleur \"%s\" competent pour la specialite \"%s\"\n"
#define MSG_CLIENT "## nouveau client \"%s\"\n"
#define MSG_TACHE "## la commande \"%s\" requiere la specialite \"%s\" (nombre d’heures \"%d\")\n"
#define MSG_PROGRESSION "## pour la commande \"%s\", pour la specialite \"%s\" : \"%d\" heures de plus ont ete realisees\n"

// Lexemes -------------------------------------------------------------------- 
#define LGMOT 35
#define NBCHIFFREMAX 5 
typedef char Mot[LGMOT + 1];
void get_id(Mot id) {
	scanf("%s", id);
	if (EchoActif) printf(">>echo %s\n", id);
}
int get_int() {
	char buffer[NBCHIFFREMAX + 1];
	scanf("%s", buffer);
	if (EchoActif) printf(">>echo %s\n", buffer);
	return atoi(buffer);
}

// Instructions --------------------------------------------------------------- 

// Progression -------------------------
void traite_progression() {
	Mot nom_commade, nom_specialite;
	get_id(nom_commade);
	get_id(nom_specialite);
	int nbr_heure = get_int;
	printf(MSG_PROGRESSION, nom_commade, nom_specialite, nbr_heure);
}

// Tâches ------------------------------
void traite_tache() {
	Mot nom_commande, nom_specialite;
	getid(nom_commande);
	get_id(nom_specialite);
	int nbr_heure = get_int();
	printf(MSG_TACHE,nom_commande,nom_specialite,nbr_heure);
}

// Client ------------------------------
void traite_client() {
	Mot nom_client;
	get_id(nom_client);
	printf(MSG_CLIENT, nom_client);
}
// Embauche ----------------------------
void traite_embauche() {
	Mot nom_travailleur, nom_specialite;
	get_id(nom_travailleur);
	get_id(nom_specialite);
	printf(MSG_EMBAUCHE, nom_travailleur, nom_specialite);
}
// developpe --------------------------- 
void traite_developpe() {
	Mot nom_specialite;
	get_id(nom_specialite);
	int cout_horaire = get_int();
	printf(MSG_DEVELOPPE, nom_specialite, cout_horaire);
}
// interruption ------------------------ 
void traite_interruption() {
	printf(MSG_INTERRUPTION);
}

//Boucle principale ---------------------------------------------------------- 
int main(int argc, char* argv[]) {
	if (argc >= 2 && strcmp("echo", argv[1]) == 0) {
		EchoActif = VRAI;
	}
	Mot buffer;
	while (VRAI) {
		get_id(buffer);
		if (strcmp(buffer, "developpe") == 0) {
			traite_developpe();
			continue;
		}
		if (strcmp(buffer, "interruption") == 0) {
			traite_interruption();
			break;
		}
		printf("!!! instruction inconnue >%s< !!!\n", buffer);
	}
	return 0;
}
