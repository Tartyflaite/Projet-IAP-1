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
#define MSG_TACHE "## la commande \"%s\" requiere la specialite \"%s\" (nombre d'heures \"%d\")\n"
#define MSG_PROGRESSION "## pour la commande \"%s\", pour la specialite \"%s\" : \"%d\" heures de plus ont ete realisees\n"
#define MSG_PASSE "## une reallocation est requise\n"
#define MSG_SPECIALITES "specialites traitees : "
#define MSG_SPECIALITES_ERREUR "specialitee inconnue\n"
#define MSG_TRAVAILLEURS "la specialite %s peut etre prise en charge par : "
#define MSG_CLIENT "le client %s a commande : "
#define MSG_CLIENT_ERREUR "client inconnu\n"
#define MSG_CLIENT_ID_COMMANDE "%s"
#define MSG_SUPERVISION "etat des taches pour %s : "
#define MSG_CHARGE "charge de travail pour %s : "
#define MSG_COMMANDE "## nouvelle commande \"%s\", par client \"%s\"\n"
#define MSG_CHARGE_TACHE "%s/%s/%dheure(s)"
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
	int nb_heures_travail;
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
	int idx_trav_tache[MAX_SPECIALITES];
} Commande;
typedef struct {
	Commande tab_commandes[MAX_COMMANDES];
	unsigned int nb_commandes;
} Commandes;

// déclaration des fonctions --------------------------------------------------
void traite_developpe(Specialites* rep_specialites);
void traite_embauche(Travailleurs* rep_travailleurs, const Specialites* rep_specialites);
void traite_demarche(Clients* rep_clients);
void traite_commande(Commandes* rep_commandes, const Clients* rep_clients);
void traite_supervision(const Specialites* rep_specialites, const Commandes* rep_commandes);
void traite_client(const Clients* rep_clients, const Commandes* rep_commandes);
void traite_travailleurs(const Specialites* rep_specialites, const Travailleurs* rep_travailleurs);
void traite_specialites(const Specialites* rep_specialites);
void traite_tache(const Specialites* rep_specialites, Commandes* rep_commandes, Travailleurs* rep_trav);
void traite_progression(const Specialites* rep_spe, Commandes* rep_com, Travailleurs* rep_trav);
void traite_charge(const Travailleurs* rep_trav, const Commandes* rep_com, const Specialites* rep_spe);
void traite_passe();
void traite_assignation(const int indx_com, const int indx_spe, Commandes* rep_com, Travailleurs* rep_trav);
void traite_interruption();



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
			traite_commande(&rep_commandes, &rep_clients);
			continue;
		}
		if (strcmp(buffer, "charge") == 0) {
			traite_charge(&rep_travailleurs,&rep_commandes,&rep_specialites);
			continue;
		}
		if (strcmp(buffer, "supervision") == 0) {
			traite_supervision(&rep_specialites, &rep_commandes);
			continue;
		}
		if (strcmp(buffer, "client") == 0) {
			traite_client(&rep_clients, &rep_commandes);
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
			traite_progression(&rep_specialites, &rep_commandes, &rep_travailleurs);
			progression = VRAI;
			continue;
		}

		if (strcmp(buffer, "tache") == 0) {
			traite_tache(&rep_specialites, &rep_commandes, &rep_travailleurs);
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


// Instructions --------------------------------------------------------------- 

// developpe --------------------------- 
void traite_developpe(Specialites* rep_specialites) {
	Specialite spe;
	get_id(spe.nom);
	spe.cout_horaire = get_int();
	rep_specialites->tab_specialites[rep_specialites->nb_specialites] = spe;
	rep_specialites->nb_specialites += 1;
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
	travailleur.nb_heures_travail = 0;
	rep_travailleurs->tab_travailleurs[rep_travailleurs->nb_travailleurs] = travailleur;
	rep_travailleurs->nb_travailleurs += 1;
}

// Demarche ------------------------------
void traite_demarche(Clients* rep_clients) {
	get_id(rep_clients->tab_clients[rep_clients->nb_clients]);
	rep_clients->nb_clients += 1;
}

// Commande -----------------------------
void traite_commande(Commandes* rep_commandes, const Clients* rep_clients) {
	Commande cmd;
	Mot nom_client;
	get_id(cmd.nom);
	get_id(nom_client);
	for (int i = 0; i < MAX_SPECIALITES; i++) {
		cmd.taches_par_specialite[i].nb_heures_effectuees = 0;
		cmd.taches_par_specialite[i].nb_heures_requises = 0;
		cmd.idx_trav_tache[i] = -1;
	}
	for (int i = 0; i < rep_clients->nb_clients; i++) {
		if (strcmp(nom_client, rep_clients->tab_clients[i]) == 0) {
			cmd.idx_client = i;
			break;
		}
	}
	rep_commandes->tab_commandes[rep_commandes->nb_commandes] = cmd;
	rep_commandes->nb_commandes += 1;
}



// Supervision -------------------------
void traite_supervision(const Specialites* rep_specialites, const Commandes* rep_commandes) {
	Booleen suivant = FAUX;
	if (rep_commandes->nb_commandes > 0) {
		for (int i = 0; i < rep_commandes->nb_commandes; i++) {
			printf(MSG_SUPERVISION, rep_commandes->tab_commandes[i].nom);
			for (int j = 0; j < rep_specialites->nb_specialites; j++) {
				if (rep_commandes->tab_commandes[i].taches_par_specialite[j].nb_heures_requises != 0) {
					if (suivant)printf(", ");
					else suivant = VRAI;
					printf("%s:%d/%d", rep_specialites->tab_specialites[j].nom, rep_commandes->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees, rep_commandes->tab_commandes[i].taches_par_specialite[j].nb_heures_requises);
				}
			}
			printf("\n");
			suivant = FAUX;
		}
	}
}

// Client ------------------------------
void traite_client(const Clients* rep_clients, const Commandes* rep_commandes) {
	Mot nom_client;
	get_id(nom_client);
	int i = 0;
	Booleen suivant = FAUX;
	if (strcmp(nom_client, "tous") == 0) {
		while (i < rep_clients->nb_clients) {
			printf(MSG_CLIENT, rep_clients->tab_clients[i]);
			for (int j = 0; j < rep_commandes->nb_commandes; j++) {
				if (rep_commandes->tab_commandes[j].idx_client == i) {
					if (suivant)printf(", ");
					else suivant = VRAI;
					printf(MSG_CLIENT_ID_COMMANDE, rep_commandes->tab_commandes[j].nom);
				}
			}
			printf("\n");
			suivant = FAUX;
			i++;
		}
	}
	else {
		while (i < rep_clients->nb_clients) {
			if (strcmp(nom_client, rep_clients->tab_clients[i]) == 0) {
				printf(MSG_CLIENT, nom_client);
				for (int j = 0; j < rep_commandes->nb_commandes; j++) {
					if (rep_commandes->tab_commandes[j].idx_client == i) {
						if (suivant)printf(", ");
						else suivant = VRAI;
						printf(MSG_CLIENT_ID_COMMANDE, rep_commandes->tab_commandes[j].nom);
					}
				}
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

// Tâches ------------------------------
void traite_tache(const Specialites* rep_specialites, Commandes* rep_commandes, Travailleurs* rep_trav) {
	Mot nom_commande, nom_specialite;
	get_id(nom_commande);
	get_id(nom_specialite);
	int nbr_heure = get_int();
	for (int i = 0; i < rep_commandes->nb_commandes; i++) {
		if (strcmp(rep_commandes->tab_commandes[i].nom, nom_commande) == 0) {
			for (int j = 0; j < rep_specialites->nb_specialites; j++) {
				if (strcmp(rep_specialites->tab_specialites[j].nom, nom_specialite) == 0) {
					rep_commandes->tab_commandes[i].taches_par_specialite[j].nb_heures_requises = nbr_heure;
					traite_assignation(i, j, rep_commandes, rep_trav);
					break;
				}
			}
			break;
		}
	}
}

// Progression -------------------------
void traite_progression(const Specialites* rep_spe, Commandes* rep_com, Travailleurs* rep_trav) {
	Mot nom_commande, nom_specialite;
	get_id(nom_commande);
	get_id(nom_specialite);
	int nbr_heure = get_int();
	for (int i = 0; i < rep_com->nb_commandes; i++) {
		if (strcmp(rep_com->tab_commandes[i].nom, nom_commande) == 0) {
			for (int j = 0; j < rep_spe->nb_specialites; j++) {
				if (strcmp(rep_spe->tab_specialites[j].nom, nom_specialite) == 0) {
					rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees += nbr_heure;
					rep_trav->tab_travailleurs[rep_com->tab_commandes[i].idx_trav_tache[j]].nb_heures_travail -= nbr_heure;
					if (rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees >= rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_requises) {
						rep_trav->tab_travailleurs[rep_com->tab_commandes[i].idx_trav_tache[j]].nb_heures_travail += rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees - rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_requises;
						rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees = rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_requises;
						rep_com->tab_commandes[i].idx_trav_tache[j] = -1;
					}
					break;
				}
			}
			break;
		}
	}
}

// Charge ------------------------------
void traite_charge(const Travailleurs* rep_trav, const Commandes* rep_com, const Specialites* rep_spe) {
	Mot nom_travailleur;
	get_id(nom_travailleur);
	Booleen suivant = FAUX;
	if (rep_trav->nb_travailleurs == 0)return;
	for (int i = 0; i < rep_trav->nb_travailleurs; i++) {
		if (strcmp(nom_travailleur, rep_trav->tab_travailleurs[i].nom) == 0) {
			printf(MSG_CHARGE, nom_travailleur);
			if (rep_com->nb_commandes == 0) {
				printf("\n");
				return;
			}
			for (int j = 0; j < rep_com->nb_commandes; j++) {
				for (int k = 0; k < rep_spe->nb_specialites; k++) {
					if (rep_com->tab_commandes[j].idx_trav_tache[k] == i) {
						if (suivant)printf(", ");
						else suivant = VRAI;
						printf(MSG_CHARGE_TACHE, rep_com->tab_commandes[j].nom, rep_spe->tab_specialites[k].nom, (rep_com->tab_commandes[j].taches_par_specialite[k].nb_heures_requises - rep_com->tab_commandes[j].taches_par_specialite[k].nb_heures_effectuees));
					}
				}
			}
			printf("\n");
			suivant = FAUX;
		}
	}
}

// Passe -------------------------------
void traite_passe() {
	return;
}

// Assignation -------------------------
void traite_assignation(const int indx_com, const int indx_spe, Commandes* rep_com, Travailleurs* rep_trav) {
	for (int i = 0; i < rep_trav->nb_travailleurs; i++) {
		if (rep_trav->tab_travailleurs[i].tags_competences[indx_spe] == VRAI) {
			rep_com->tab_commandes[indx_com].idx_trav_tache[indx_spe] = i;
			rep_trav->tab_travailleurs[i].nb_heures_travail += rep_com->tab_commandes[indx_com].taches_par_specialite[indx_spe].nb_heures_requises - rep_com->tab_commandes[indx_com].taches_par_specialite[indx_spe].nb_heures_effectuees;
			break;
		}
	}
}

// interruption ------------------------ 
void traite_interruption() {
	printf(MSG_INTERRUPTION);
}