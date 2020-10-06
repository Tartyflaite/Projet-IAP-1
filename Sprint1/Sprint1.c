#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { FAUX = 0, VRAI = 1 } Booleen;
Booleen EchoActif = FAUX;

// Messages emis par les instructions -----------------------------------------
#define MSG_DEVELOPPE "## nouvelle specialite \"%s\" ; cout horaire \"%d\"\n" 
#define MSG_INTERRUPTION "## fin de programme\n" 
#define MSG_EMBAUCHE "## nouveau travailleur \"%s\" competent pour la specialite \"%s\"\n"
#define MSG_DEMARCHE "## nouveau client \"%s\"\n"
#define MSG_TACHE "## la commande \"%s\" requiere la specialite \"%s\" (nombre d’heures \"%d\")\n"
#define MSG_PROGRESSION "## pour la commande \"%s\", pour la specialite \"%s\" : \"%d\" heures de plus ont ete realisees\n"
#define MSG_PASSE "## une reallocation est requise\n"
#define MSG_SPECIALITES "## consultation des specialites\n"
#define MSG_TRAVAILLEURS "## consultation des travailleurs competents pour la specialite \"%s\"\n"
#define MSG_CLIENT "## consultation des commandes effectuees par \"%s\"\n"
#define MSG_SUPERVISION "## consultation de l’avancement des commandes\n"
#define MSG_CHARGE "## consultation de la charge de travail de \"%s\"\n"
#define MSG_COMMANDE "## nouvelle commande \"%s\", par client \"%s\"\n"
#define MSG_TOUS_TRAVAILLEURS "## consultation des travailleurs competents pour chaque specialite\n"
#define MSG_TOUS_CLIENT "## consultation des commandes effectuees par chaque client\n"
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

//commande -----------------------------
void traite_commande() {
	Mot nom_commande, nom_client;
	get_id(nom_commande);
	get_id(nom_client);
	printf(MSG_COMMANDE, nom_commande, nom_client);
}

// Charge ------------------------------
void traite_charge() {
	Mot nom_travailleur;
	get_id(nom_travailleur);
	printf(MSG_CHARGE, nom_travailleur);
}

// Supervision -------------------------
void traite_supervision() {
	printf(MSG_SUPERVISION);
}

// Client ------------------------------
void traite_client() {
	Mot nom_client;
	get_id(nom_client);
	if (strcmp(nom_client, "tous") == 0) {
		printf(MSG_TOUS_CLIENT);
	}
	else {
		printf(MSG_CLIENT, nom_client);
	}
}

// Travailleurs ------------------------
void traite_travailleurs() {
	Mot nom_specialite;
	get_id(nom_specialite);
	if (strcmp(nom_specialite, "tous") == 0) {
		printf(MSG_TOUS_TRAVAILLEURS);
	}
	else {
		printf(MSG_TRAVAILLEURS, nom_specialite);
	}
}

// Specialités -------------------------
void traite_specialites() {
	printf(MSG_SPECIALITES);
}

// Progression -------------------------
void traite_progression() {
	Mot nom_commade, nom_specialite;
	get_id(nom_commade);
	get_id(nom_specialite);
	int nbr_heure = get_int();
	printf(MSG_PROGRESSION, nom_commade, nom_specialite, nbr_heure);
}

// Passe -------------------------------
void traite_passe() {
	printf(MSG_PASSE);
}

// Tâches ------------------------------
void traite_tache() {
	Mot nom_commande, nom_specialite;
	get_id(nom_commande);
	get_id(nom_specialite);
	int nbr_heure = get_int();
	printf(MSG_TACHE, nom_commande, nom_specialite, nbr_heure);
}

// Client ------------------------------
void traite_demarche() {
	Mot nom_client;
	get_id(nom_client);
	printf(MSG_DEMARCHE, nom_client);
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
	Booleen progression = FAUX;
	while (VRAI) {
		get_id(buffer);
		if (progression==VRAI && strcmp(buffer, "passe") == 0) {
			traite_passe();
			progression = FAUX;
			continue;
		}
		else {
			progression = FAUX;
		}
		if (strcmp(buffer, "commande") == 0) {
			traite_commande();
			continue;
		}
		if (strcmp(buffer, "charge") == 0) {
			traite_charge();
			continue;
		}
		if (strcmp(buffer, "supervision") == 0) {
			traite_supervision();
			continue;
		}
		if (strcmp(buffer, "client") == 0) {
			traite_client();
			continue;
		}
		if (strcmp(buffer, "travailleurs") == 0) {
			traite_travailleurs();
			continue;
		}
		if (strcmp(buffer, "specialites") == 0) {
			traite_specialites();
			continue;
		}
		if (strcmp(buffer, "progression") == 0) {
			traite_progression();
			progression = VRAI;
			continue;
		}

		if (strcmp(buffer, "tache") == 0) {
			traite_tache();
			continue;
		}
		if (strcmp(buffer, "demarche") == 0) {
			traite_demarche();
			continue;
		}
		if (strcmp(buffer, "embauche") == 0) {
			traite_embauche();
			continue;
		}
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
