﻿#pragma warning(disable:4996)
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
#define MSG_TACHE "## la commande \"%s\" requiere la specialite \"%s\" (nombre d'heures \"%d\")\n"
#define MSG_PROGRESSION "## pour la commande \"%s\", pour la specialite \"%s\" : \"%d\" heures de plus ont ete realisees\n"
#define MSG_PASSE "## une reallocation est requise\n"
#define MSG_SPECIALITES "specialites traitees : "
#define MSG_SPECIALITES_ERREUR "specialitee inconnue\n"
#define MSG_TRAVAILLEURS "la specialite %s peut etre prise en charge par : "
#define MSG_CLIENT "le client %s a commande : "
#define MSG_CLIENT_ERREUR "client inconnu\n"
#define MSG_CLIENT_ID_COMMANDE ""
#define MSG_SUPERVISION "## consultation de l'avancement des commandes\n"
#define MSG_CHARGE "## consultation de la charge de travail de \"%s\"\n"
#define MSG_COMMANDE "## nouvelle commande \"%s\", par client \"%s\"\n"
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
// Donnees −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−

// specialites −−−−−−−−−−−−−−−−−−−−−
#define MAX_SPECIALITES 10
typedef struct {
	Mot nom;
	int cout_horaire;
} Specialite;
typedef struct {
	Specialite tab_specialites[MAX_SPECIALITES];
	unsigned int nb_specialites;
} Specialites;
// travailleurs −−−−−−−−−−−−−−−−−−−−
#define MAX_TRAVAILLEURS 50
typedef struct {
	Mot nom;
	Booleen tags_competences[MAX_SPECIALITES];
} Travailleur;
typedef struct {
	Travailleur tab_travailleurs[MAX_TRAVAILLEURS];
	unsigned int nb_travailleurs;
} Travailleurs;
// client −−−−−−−−−−−−−−−−−−−−−−−−−−
#define MAX_CLIENTS 10
typedef struct {
	Mot tab_clients[MAX_CLIENTS];
	unsigned int nb_clients;
} Clients;
// commandes −−−−−−−−−−−−−−−−−−−−−−−
#define MAX_COMMANDES 500
typedef struct {
	unsigned int nb_heures_requises;
	unsigned int nb_heures_effectuees;
} Tache;
typedef struct {
	Mot nom;
	unsigned int idx_client;
	Tache taches_par_specialite[MAX_SPECIALITES]; // nb_heures_requises==0 <=> pas de tache pour cette specialite
} Commande;
typedef struct {
	Commande tab_commandes[MAX_COMMANDES];
	unsigned int nb_commandes;
} Commandes;

// Instructions --------------------------------------------------------------- 

//commande -----------------------------
void traite_commande() {
	Commande cmd;
	Mot nom_commande, nom_client;
	get_id(nom_commande);
	for (int i = 0; i < MAX_SPECIALITES; i++) {

	}
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
void traite_client(const Clients* rep_clients) {
	Mot nom_client;
	get_id(nom_client);
	int i = 0;
	if (strcmp(nom_client, "tous") == 0) {
		while (i < rep_clients->nb_clients) {
			printf(MSG_CLIENT, rep_clients->tab_clients[i]);
			printf(MSG_CLIENT_ID_COMMANDE);
			printf("\n");
			i++;
		}
	}
	else {
		while (i < rep_clients->nb_clients) {
			if (strcmp(nom_client, rep_clients->tab_clients[i]) == 0) {
				printf(MSG_CLIENT, nom_client);
				printf(MSG_CLIENT_ID_COMMANDE);
				printf("\n");
				return;
			}
			i++;
		}
		printf(MSG_CLIENT_ERREUR);
	}
}


// Travailleurs ------------------------
void traite_travailleurs(const Specialites* rep_specialites, const Travailleurs* rep_travailleurs) {
	Mot nom_specialite;
	get_id(nom_specialite);
	int i = 0;
	Booleen suivant = FAUX;
	if (strcmp(nom_specialite, "tous") == 0) {
		while (i < rep_specialites->nb_specialites) {
			printf(MSG_TRAVAILLEURS, rep_specialites->tab_specialites[i].nom);
			for (int j = 0; j < rep_travailleurs->nb_travailleurs; j++) {
				if (rep_travailleurs->tab_travailleurs[j].tags_competences[i] == VRAI) {
					if (suivant)printf(", ");
					else suivant = VRAI;
					printf("%s", rep_travailleurs->tab_travailleurs[j].nom);
				}
			}
			printf("\n");
			suivant = FAUX;
			i++;
		}
	}
	else {
		while (i < rep_specialites->nb_specialites) {
			if (strcmp(nom_specialite, rep_specialites->tab_specialites[i].nom) == 0) {
				printf(MSG_TRAVAILLEURS, nom_specialite);
				for (int j = 0; j < rep_travailleurs->nb_travailleurs; j++) {
					if (rep_travailleurs->tab_travailleurs[j].tags_competences[i] == VRAI) {
						if (suivant)printf(", ");
						else suivant = VRAI;
						printf("%s", rep_travailleurs->tab_travailleurs[j].nom);
					}
				}
				printf("\n");
				return;
			}
			i++;
		}
		printf(MSG_SPECIALITES_ERREUR);
	}
}


// Specialités -------------------------
void traite_specialites(const Specialites* rep_specialites) {
	printf(MSG_SPECIALITES);
	if (rep_specialites->nb_specialites == 0) {
		printf("\n");
		return;
	}
	for (int i = 0; i < rep_specialites->nb_specialites; i++) {
		printf("%s/%d", rep_specialites->tab_specialites[i].nom, rep_specialites->tab_specialites[i].cout_horaire);
		if (i != rep_specialites->nb_specialites - 1)printf(", ");
	}
	printf("\n");
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
void traite_demarche(Clients* rep_clients) {
	get_id(rep_clients->tab_clients[rep_clients->nb_clients]);
	rep_clients->nb_clients += 1;
}

// Embauche ----------------------------
void traite_embauche(Travailleurs* rep_travailleurs, const Specialites* rep_specialites) {
	Mot  nom_specialite;
	Travailleur travailleur;
	int j = 0;
	get_id(travailleur.nom);
	get_id(nom_specialite);
	for (j = 0; j < rep_travailleurs->nb_travailleurs; j++) {
		if (strcmp(travailleur.nom, rep_travailleurs->tab_travailleurs[j].nom) == 0) {
			for (int i = 0; i < rep_specialites->nb_specialites; i++) {
				if (strcmp(nom_specialite, rep_specialites->tab_specialites[i].nom) == 0) {
					rep_travailleurs->tab_travailleurs[j].tags_competences[i] = VRAI;
					return;
				}
			}
		}
	}
	for (int i = 0; i < rep_specialites->nb_specialites; i++) {
		if (strcmp(nom_specialite, rep_specialites->tab_specialites[i].nom) == 0) {
			travailleur.tags_competences[i] = VRAI;
			break;
		}
	}
	rep_travailleurs->tab_travailleurs[rep_travailleurs->nb_travailleurs] = travailleur;
	rep_travailleurs->nb_travailleurs += 1;
}

// developpe --------------------------- 
void traite_developpe(Specialites* rep_specialites) {
	Specialite spe;
	get_id(spe.nom);
	spe.cout_horaire = get_int();
	rep_specialites->tab_specialites[rep_specialites->nb_specialites] = spe;
	rep_specialites->nb_specialites += 1;
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
	Specialites rep_specialites;
	Travailleurs rep_travailleurs;
	Clients rep_clients;
	Commandes rep_commandes;
	rep_clients.nb_clients = 0;
	rep_specialites.nb_specialites = 0;
	rep_travailleurs.nb_travailleurs = 0;
	rep_commandes.nb_commandes = 0;
	while (VRAI) {
		get_id(buffer);
		if (progression == VRAI && strcmp(buffer, "passe") == 0) {
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
			traite_client(&rep_clients);
			continue;
		}
		if (strcmp(buffer, "travailleurs") == 0) {
			traite_travailleurs(&rep_specialites, &rep_travailleurs);
			continue;
		}
		if (strcmp(buffer, "specialites") == 0) {
			traite_specialites(&rep_specialites);
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
			traite_demarche(&rep_clients);
			continue;
		}
		if (strcmp(buffer, "embauche") == 0) {
			traite_embauche(&rep_travailleurs, &rep_specialites);
			continue;
		}
		if (strcmp(buffer, "developpe") == 0) {
			traite_developpe(&rep_specialites);
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
