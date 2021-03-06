﻿#pragma warning(disable:4996) // on désactive l'avertissement sur les scanf
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef enum { FAUX = 0, VRAI = 1 } Booleen; // on definie le type booléen qui n'es pas inclus dans le C
Booleen EchoActif = FAUX; // le mode de débuggage est désactivé par défaut

// Messages emis par les instructions -----------------------------------------
// on définie ci dessous les messages a afficher selon les fonctions utilisées
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
#define LGMOT 35 //longueur d'un mot (35 caracteres)
#define NBCHIFFREMAX 5 // taille d'un nombre (5 chiffres maximums)
typedef char Mot[LGMOT + 1]; // définition du type mot
void get_id(Mot id) { //fonction de récupréation d'un mot
	scanf("%s", id);
	if (EchoActif) printf(">>echo %s\n", id); // si le mode débuggage est activé on affiche le mot entre precedemment
	assert(strlen(id) > 1); // on verifie que le mot fait plus d'un caractere
}
int get_int() { // fonction de récupération d'un entier positif
	char buffer[NBCHIFFREMAX + 1];
	scanf("%s", buffer);
	if (EchoActif) printf(">>echo %s\n", buffer); // si le mode débuggage est activé on affiche le mot entre precedemment
	assert(atoi(buffer) >= 0); // on verifie que le nombre entre est un entier positif
	return atoi(buffer);
}

// Donnees −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−

// specialites −−−−−−−−−−−−−−−−−−−−−
#define MAX_SPECIALITES 10 // on definie le nombre maximum de specialites
typedef struct {//une specialite est composee d'un nom de type mot et d'un cout horaire entier
	Mot nom; 
	int cout_horaire;
} Specialite;
typedef struct {//l'ensemble des specilites est regroupe dans une structure contenant un tableau de specilites ainsi qu'un entier contenant le nombre de specialites
	Specialite tab_specialites[MAX_SPECIALITES];
	unsigned int nb_specialites;
} Specialites;
// travailleurs −−−−−−−−−−−−−−−−−−−−
#define MAX_TRAVAILLEURS 50 // on definie le nombre maximum de travailleurs
typedef struct { // un travailleur possede un nom de type mot, des competences repertoriees dans un tableau de booleen et un nombres d'heures de travailles a effectues entier
	Mot nom;
	Booleen tags_competences[MAX_SPECIALITES]; // VRAI ou FAUX avec VRAI indiquant que le travailleur maitrise la specialites numero n stockee a la case n du tableau de specialites
	unsigned int nb_heures_travail;
} Travailleur;
typedef struct {//l'ensemble des travailleurs est regroupe dans une structure contenant un tableau de travailleur ainsi qu'un entier contenant le nombre de travailleurs
	Travailleur tab_travailleurs[MAX_TRAVAILLEURS];
	unsigned int nb_travailleurs;
} Travailleurs;
// client −−−−−−−−−−−−−−−−−−−−−−−−−−
#define MAX_CLIENTS 100 // on definie le nombre maximum de clients
typedef struct {// les clients sont repertoriés dans un tableau de mot contenant le nom de chaque client ainsi qu'un entier stockant le nombre de clients
	Mot tab_clients[MAX_CLIENTS];
	unsigned int nb_clients;
} Clients;
// commandes −−−−−−−−−−−−−−−−−−−−−−−
#define MAX_COMMANDES 500 // on definie le nombre maximum de commandes
typedef struct { // une tache est définie pas sont nombre d'heures requises ainsi que son nombre d'heures effectues (les deux étants des entiers naturels)
	unsigned int nb_heures_requises;
	unsigned int nb_heures_effectuees;
} Tache;
// une commande est définie par son nom (mot), les client auquelle elle est associee (entier naturel)
// les taches qui lui sont associeesla traveilleurs en charge des taches ainsi que la facture qui lui est associée
typedef struct {
	Mot nom;
	unsigned int idx_client;
	Tache taches_par_specialite[MAX_SPECIALITES]; 
	// nb_heures_requises==0 <=> pas de tache pour cette specialite,
	// tache numero n est associee a la specialite n du tableau des specialites
	int idx_trav_tache[MAX_SPECIALITES]; // pour la tache numero n est associe l'index du travailleur en charge de la tache
} Commande;
//l'ensemble des commandes est regroupe dans une structure contenant un tableau de travailleur ainsi qu'un entier contenant le nombre de travailleurs 
//et qu'un entier contenant les nombre de commandes facturees
typedef struct {
	Commande tab_commandes[MAX_COMMANDES];
	unsigned int nb_commandes;
} Commandes;

// déclaration des fonctions --------------------------------------------------
void traite_developpe(Specialites* rep_spe); // permet des renseigner une nouvelle specialite
void traite_embauche(Travailleurs* rep_trav, const Specialites* rep_spe); // permet des renseigner un nouveau travailleur
void traite_demarche(Clients* rep_cli); // permet de renseigner un nouveau client
void traite_commande(Commandes* rep_com, const Clients* rep_cli);// permet de renseigner une nouvelle commande
void traite_supervision(const Specialites* rep_spe, const Commandes* rep_com); // affiche l'etat de toutes les taches pour toutes les commandes
void traite_client(const Clients* rep_cli, const Commandes* rep_com);// recuper le nom du client a afficher (ou la commande "tous" pour afficher tout les clients)
void affiche_clients(const Clients* rep_cli, const Commandes* rep_com, int i);// affiche les commandes effectues par un client
// recuper le nom du travailleur a afficher (ou la commande "tous" pour afficher tout les travailleurs)
void traite_travailleurs(const Specialites* rep_spe, const Travailleurs* rep_trav);
void affiche_travailleurs(const Specialites* rep_spe, const Travailleurs* rep_trav, int i);// affiche les travailleurs maitrisiant la spécialité demandée
void traite_specialites(const Specialites* rep_spe); // affiche les specilaites traitees
//cree une nouvelle tache dans la commande demandee pour la specialite demandee, la tache est ensuite assignee a un travailleur
void traite_tache(const Specialites* rep_spe, Commandes* rep_com, Travailleurs* rep_trav); 
 // fait progresser la tache demandee du nombre d'heure spécifiée, si laache est  complétée on enclanche la facturation
void traite_progression(const Specialites* rep_spe, Commandes* rep_com, Travailleurs* rep_trav);
 // affiche les tache assignees au travailleur demande ainsi que lenmbre d'heure qu'il reste  a effetuer pour les tache en question
void traite_charge(const Travailleurs* rep_trav, const Commandes* rep_com, const Specialites* rep_spe);
void traite_passe(); //affiche un message
void traite_assignation(const int indx_com, const int indx_spe, Commandes* rep_com, Travailleurs* rep_trav); // la tache donnée en arametre est assignée a un travailleur
void traite_interruption(); // interromp le programme avant une fin complete du fonctionnement



//Boucle principale ---------------------------------------------------------- 
int main(int argc, char* argv[]) {
	if (argc >= 2 && strcmp("echo", argv[1]) == 0) { // on verifie si le mot echo a ete mis en parametre du programme lors de l'execution en console
		EchoActif = VRAI; // activation du mode debuggage
	}
	// declaration des variables utilisees dans le main
	Mot buffer;
	Booleen progression = FAUX; // permet de verifier si la derniere valeur de "buffer" etait on non "progression"
	Specialites rep_specialites; // repertoire des specialites
	Travailleurs rep_travailleurs; // repertoire des travailleurs
	Clients rep_clients; // repertoire des clients
	Commandes rep_commandes;  // repertoire des commandes
	
	rep_clients.nb_clients = 0;
	rep_specialites.nb_specialites = 0;
	rep_travailleurs.nb_travailleurs = 0;
	rep_commandes.nb_commandes = 0;
		while (VRAI) { 
		get_id(buffer); // on demande la commande a entrer a l'utilisateur
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
			traite_charge(&rep_travailleurs, &rep_commandes, &rep_specialites); 
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
void traite_developpe(Specialites* rep_spe) {
	Specialite spe;
	get_id(spe.nom);// on recupere le nom de la specialite aupres de l'utilisateur
	spe.cout_horaire = get_int(); // on recupere le cout horaire de la specialite aupres de l'utilisateur
	rep_spe->tab_specialites[rep_spe->nb_specialites] = spe;
	rep_spe->nb_specialites += 1;
}

// Embauche ----------------------------
void traite_embauche(Travailleurs* rep_trav, const Specialites* rep_spe) {
	Mot nom_specialite; 
	Travailleur travailleur;
	unsigned int j = 0;
	get_id(travailleur.nom); // on recupere le nom du travailleur aupres de l'utilisateur
	get_id(nom_specialite); // recuperation de la specialite a maitriser
	for (j = 0; j < rep_trav->nb_travailleurs; j++) { // on parcourt l'ensemble des travailleurs enrigistres
		if (strcmp(travailleur.nom, rep_trav->tab_travailleurs[j].nom) == 0) { 
			for (unsigned int i = 0; i < rep_spe->nb_specialites; i++) { // on parcourt l'ensemble de specialites enregistrees
				if (strcmp(nom_specialite, rep_spe->tab_specialites[i].nom) == 0) { 
					rep_trav->tab_travailleurs[j].tags_competences[i] = VRAI;
					return;
				}
			}
		}
	}
	for (unsigned int i = 0; i < rep_spe->nb_specialites; i++) { // on parcourt l'ensemble des specialites enregistrees
		if (strcmp(nom_specialite, rep_spe->tab_specialites[i].nom) == 0) {
			travailleur.tags_competences[i] = VRAI;
			break;
		}
	}
	travailleur.nb_heures_travail = 0;
	rep_trav->tab_travailleurs[rep_trav->nb_travailleurs] = travailleur;
	rep_trav->nb_travailleurs += 1; 
}

// Demarche ------------------------------
void traite_demarche(Clients* rep_cli) {
	get_id(rep_cli->tab_clients[rep_cli->nb_clients]);
	rep_cli->nb_clients += 1;
}

// Commande -----------------------------
void traite_commande(Commandes* rep_com, const Clients* rep_cli) {
	Commande cmd;
	Mot nom_client;
	get_id(cmd.nom); // on recupere aupres de l'utilisateur le nom de la commande
	get_id(nom_client); // on recupere aupres de l'utilisateur le nom du client
	for (unsigned int i = 0; i < MAX_SPECIALITES; i++) { // on parcourt l'ensemble du tableau tache_par_specialite de cmd
		cmd.taches_par_specialite[i].nb_heures_effectuees = 0;
		cmd.taches_par_specialite[i].nb_heures_requises = 0;
		cmd.idx_trav_tache[i] = -1; // on initialise ma valeur de l'index du travailleur associé a la tache i a -1 (non assignee)
	}
	for (unsigned int i = 0; i < rep_cli->nb_clients; i++) { // on parcours l'ensemble des clients enregistres
		if (strcmp(nom_client, rep_cli->tab_clients[i]) == 0) {
			cmd.idx_client = i;
			break; 
		}
	}
	rep_com->tab_commandes[rep_com->nb_commandes] = cmd;
	rep_com->nb_commandes += 1;
}

// Supervision -------------------------
void traite_supervision(const Specialites* rep_spe, const Commandes* rep_com) {
	Booleen suivant = FAUX; 
	int requis, effectuees; // on declare deux entier qui correspondent aux nombres d'heures requises et effectuees pour une tache donnee pour simplefier la lisibilité
	if (rep_com->nb_commandes > 0) {
		for (unsigned int i = 0; i < rep_com->nb_commandes; i++) { // on parcours l'ensemble des commandes
			printf(MSG_SUPERVISION, rep_com->tab_commandes[i].nom);
			for (unsigned int j = 0; j < rep_spe->nb_specialites; j++) { // parcours de l'ensemble des specialites
				
				requis = rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_requises;
				effectuees = rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees;
				
				if (requis != 0) {
					if (suivant)printf(", ");
					else suivant = VRAI;
					printf("%s:%d/%d", rep_spe->tab_specialites[j].nom, effectuees, requis);
				}
			}
			printf("\n");
			suivant = FAUX;
		}
	}
}

// Client ------------------------------
void traite_client(const Clients* rep_cli, const Commandes* rep_com) {
	Mot nom_client;
	get_id(nom_client); // recuperation du nom du client aupres de l'utilisateur
	unsigned int i = 0;
	Booleen suivant = FAUX;
	
	if (strcmp(nom_client, "tous") == 0) { // si tous est entré on affiche tous les clients
		while (i < rep_cli->nb_clients) { //parcours de l'ensemble des clients
			affiche_clients(rep_cli,rep_com, i);
			i++;
		}
	}
	else { // sinon on affiche uniquement le client indique par l'utilisateur
		while (i < rep_cli->nb_clients) { // parcourt de l'ensemble des clients
			if (strcmp(nom_client, rep_cli->tab_clients[i]) == 0) {
				affiche_clients(rep_cli, rep_com, i);
				return;  
			}
			i++;
		}
		printf(MSG_CLIENT_ERREUR); // si client non enregistre
	}
}

// Affichage Client --------------------
void affiche_clients(const Clients* rep_cli, const Commandes* rep_com, int i){
	Booleen suivant = FAUX; 
	printf(MSG_CLIENT, rep_cli->tab_clients[i]);
	for (unsigned int j = 0; j < rep_com->nb_commandes; j++) {// pracours de l'ensemble des commandes
		if (rep_com->tab_commandes[j].idx_client == i) { // on verifie si l'index du client associé a la commande j est celui du client indique en parametre
			if (suivant)printf(", ");
			else suivant = VRAI;
			printf(MSG_CLIENT_ID_COMMANDE, rep_com->tab_commandes[j].nom); 
		}
	}
	printf("\n"); 
	return;  
}

// Travailleurs ------------------------
void traite_travailleurs(const Specialites* rep_spe, const Travailleurs* rep_trav) {
	Mot nom_specialite;
	get_id(nom_specialite); // recuperation du nom de la specialite aupres de l'utilisateur
	unsigned int i = 0; // initialisation du compteur i a 0
	Booleen suivant = FAUX; 
	if (strcmp(nom_specialite, "tous") == 0) { // si tous es entré on affiche tous les travailleurs
		while (i < rep_spe->nb_specialites) { //parcours de l'enesmble des specialites
			affiche_travailleurs(rep_spe, rep_trav, i); // affiche les travailleurs competatents pour la specialiye d'index i
			i++; // incrementation de i
		}
	}
	else { // sinon on affiche uniquement le client indique par l'utilisateur
		while (i < rep_spe->nb_specialites) { //parcours de l'enesmble des specialites
			if (strcmp(nom_specialite, rep_spe->tab_specialites[i].nom) == 0) { // on verifie si la specialite renseignee est la meme que celle d'index i
				affiche_travailleurs(rep_spe, rep_trav, i); // affiche les travailleurs competatents pour la specialiye d'index i
				return;
			}
			i++; // incrementation de i
		}
		printf(MSG_SPECIALITES_ERREUR);
	}
}

// Affiche travailleurs ---------------
void affiche_travailleurs(const Specialites* rep_spe, const Travailleurs* rep_trav, int i) {
	Booleen suivant = FAUX; 
	printf(MSG_TRAVAILLEURS, rep_spe->tab_specialites[i].nom);
	for (unsigned int j = 0; j < rep_trav->nb_travailleurs; j++) { // pracours de l'ensemble des travailleurs
		if (rep_trav->tab_travailleurs[j].tags_competences[i] == VRAI) {  // on verifie si le travailleurs d'index j maitrise la competence d'index i
			if (suivant)printf(", ");
			else suivant = VRAI;
			printf("%s", rep_trav->tab_travailleurs[j].nom);
		}
	}
	printf("\n");  
	return; // fin de la focntion
}

// Specialités -------------------------
void traite_specialites(const Specialites* rep_spe) {
	printf(MSG_SPECIALITES);
	if (rep_spe->nb_specialites == 0) { // on verifie si le nombre de specialites enregistrees est nul
		printf("\n");  
		return;  
	}
	for (unsigned int i = 0; i < rep_spe->nb_specialites; i++) { // parcours de l'ensemble des specialites
		printf("%s/%d", rep_spe->tab_specialites[i].nom, rep_spe->tab_specialites[i].cout_horaire);
		if (i != rep_spe->nb_specialites - 1)printf(", ");
	}
	printf("\n");  
}


// Tâches ------------------------------
void traite_tache(const Specialites* rep_spe, Commandes* rep_com, Travailleurs* rep_trav) {
	Mot nom_commande, nom_specialite;
	get_id(nom_commande); // recuperation du nom de la commande aupres de l'utilisateur
	get_id(nom_specialite); // recuperation du nom de la specialite aupres de l'utilisateur
	int nbr_heure = get_int();
	for (unsigned int i = 0; i < rep_com->nb_commandes; i++) { // parcours de l'ensemble des commandes 
		if (strcmp(rep_com->tab_commandes[i].nom, nom_commande) == 0) {
			for (unsigned int j = 0; j < rep_spe->nb_specialites; j++) { // prcours de l'ensemble des specialites
				if (strcmp(rep_spe->tab_specialites[j].nom, nom_specialite) == 0) {
					rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_requises = nbr_heure;
					traite_assignation(i, j, rep_com, rep_trav); // affectation de la tache d'index j pour la commandes d'index i a un travailleur
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
	get_id(nom_commande); // recuperation du nom de la commande aupres de l'utilisateur
	get_id(nom_specialite); // recuperation du nom de la specialite aupres de l'utilisateur
	int nbr_heure = get_int(),diff, requis, effectuees;
	for (unsigned int i = 0; i < rep_com->nb_commandes; i++) { // parcours de l'ensemble des commandes
		if (strcmp(rep_com->tab_commandes[i].nom, nom_commande) == 0) {
			
			for (unsigned int j = 0; j < rep_spe->nb_specialites; j++) { // parcoursde l'ensemble des specialites
				requis = rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_requises;
				
				// on verifie si la specialite renseignee est la meme que celle d'index j et que requis est non nul
				if (strcmp(rep_spe->tab_specialites[j].nom, nom_specialite) == 0 && requis!=0) {

					rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees += nbr_heure; //on ajoute nbr_heures au nombre d'heures effectuees pour la tache d'index j
					rep_trav->tab_travailleurs[rep_com->tab_commandes[i].idx_trav_tache[j]].nb_heures_travail -= nbr_heure;

					requis = rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_requises;
					effectuees = rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees;
					diff = requis - effectuees;

					if (diff<=0) {
						// compensation du nombres d'heures de travail a effectuer si depassement (effectuees>requis)
						rep_trav->tab_travailleurs[rep_com->tab_commandes[i].idx_trav_tache[j]].nb_heures_travail += diff; 
						rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees = requis; // mise a niveau du nb d'heures effectues (en cas de dépassement)
						rep_com->tab_commandes[i].idx_trav_tache[j] = -1; // reinitialisation de l'asssignation
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
	get_id(nom_travailleur); // recuperation du nom du travailleur aupres de l'utilisateur
	int requis, effectuees, diff;
	Booleen suivant = FAUX;
	if (rep_trav->nb_travailleurs == 0)return; // si il n'y a aucun travailleur enregistré la fonction d'interrompt
	for (unsigned int i = 0; i < rep_trav->nb_travailleurs; i++) { // parcours de l'ensemble des travailleurs
		if (strcmp(nom_travailleur, rep_trav->tab_travailleurs[i].nom) == 0) {
			printf(MSG_CHARGE, nom_travailleur); 
			if (rep_com->nb_commandes == 0) { 
				printf("\n"); 
				return;
			}
			for (unsigned int j = 0; j < rep_com->nb_commandes; j++) { // parcours de l'ensemble des commandes enregistrees
				for (unsigned int k = 0; k < rep_spe->nb_specialites; k++) { // parcours de l'ensemble des specialites enregistrees
					if (rep_com->tab_commandes[j].idx_trav_tache[k] == i) {
						if (suivant)printf(", ");
						else suivant = VRAI;
						requis = rep_com->tab_commandes[j].taches_par_specialite[k].nb_heures_requises;
						effectuees = rep_com->tab_commandes[j].taches_par_specialite[k].nb_heures_effectuees;
						diff = requis - effectuees; // on affecte a diff la diffecrence entre requis et effectuees
						printf(MSG_CHARGE_TACHE, rep_com->tab_commandes[j].nom, rep_spe->tab_specialites[k].nom, diff);
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
// assigne a un travilleur la tache d'index idx_spe de la commande d'index idx_com
void traite_assignation(const int idx_com, const int idx_spe, Commandes* rep_com, Travailleurs* rep_trav) { 
	Booleen suivant = FAUX; 
	int affecter = -1, requis, effectuees, diff;
	for (unsigned int i = 0; i < rep_trav->nb_travailleurs; i++) { // parcours des travailleurs
		if (rep_trav->tab_travailleurs[i].tags_competences[idx_spe] == VRAI) {
			if (suivant) {
				if (rep_trav->tab_travailleurs[i].nb_heures_travail < rep_trav->tab_travailleurs[affecter].nb_heures_travail) {
					affecter = i;
				}
			}
			else {
				affecter = i;
				suivant = VRAI;
			}
		}
	}
	assert(affecter >= 0);
	requis = rep_com->tab_commandes[idx_com].taches_par_specialite[idx_spe].nb_heures_requises;
	effectuees = rep_com->tab_commandes[idx_com].taches_par_specialite[idx_spe].nb_heures_effectuees;
	diff = requis - effectuees;
	rep_com->tab_commandes[idx_com].idx_trav_tache[idx_spe] = affecter; // on affecte a la tache le travailleur selectionne precedemment
	rep_trav->tab_travailleurs[affecter].nb_heures_travail += diff; // on ajoute au travailleur en charge de la tache le nombre d'heure a effectuer pour la terminer
}

// interruption ------------------------ 
void traite_interruption() {
	printf(MSG_INTERRUPTION); // affichage du message MSG_INTERRUPTION
}// fin de traite_interrupton