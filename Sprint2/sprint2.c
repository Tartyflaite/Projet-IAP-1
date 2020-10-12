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
#define MSG_SPECIALITES "## liste des spécialités : "
#define MSG_TRAVAILLEURS "## consultation des travailleurs competents pour la specialite \"%s\"\n"
#define MSG_CLIENT "le client \"%s\" a commande :"
#define MSG_CLIENT_ERREUR "client inconnu\n"
#define MSG_CLIENT_ID_COMMANDE ""
#define MSG_SUPERVISION "## consultation de l’avancement des commandes\n"
#define MSG_CHARGE "## consultation de la charge de travail de \"%s\"\n"
#define MSG_COMMANDE "## nouvelle commande \"%s\", par client \"%s\"\n"
#define MSG_TOUS_TRAVAILLEURS "## consultation des travailleurs competents pour chaque specialite\n"
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
void traite_client(Clients* rep_clients) {
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
void traite_specialites(Specialites* rep_specialites) {
	int i = 0;
	printf(MSG_SPECIALITES);
	while (i < rep_specialites->nb_specialites) {
		printf("%s",rep_specialites->tab_specialites[i]);
		if (i != rep_specialites->nb_specialites - 1)printf(", ");
		else printf("\n");
		i++;
	}
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
void traite_embauche(Travailleurs* rep_travailleurs, Specialites* rep_specialites) {
	Mot  nom_specialite;
	Travailleur travailleur;
	get_id(travailleur.nom);
	get_id(nom_specialite);
	for (int i = 0; i < MAX_SPECIALITES; i++) {
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
	rep_clients.nb_clients = 0;
	rep_specialites.nb_specialites = 0;
	rep_travailleurs.nb_travailleurs = 0;
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
			traite_travailleurs();
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