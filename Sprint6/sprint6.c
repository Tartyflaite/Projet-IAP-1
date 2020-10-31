#pragma warning(disable:4996) // on désactive l'avertissement sur les scanf
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
#define MSG_FACTURATION "facturation %s : "
#define MSG_FACTURATION_FINALE "facturations : "

// Lexemes -------------------------------------------------------------------- 
#define LGMOT 35 //longueur d'un mot (35 caracteres)
#define NBCHIFFREMAX 5  // taille d'un nombre (5 chiffres maximums)
typedef char Mot[LGMOT + 1]; // définition du type mot
void get_id(Mot id) { //fonction de récupréation d'un mot
	scanf("%s", id); // l'utilisateur entre le mot souhaité qui est affecté a la variable id
	if (EchoActif) printf(">>echo %s\n", id); // si le mode débuggage est activé on affiche le mot entre precedemment
	assert(strlen(id) > 1); // on verifie que le mot fait plus d'un caractere
}
int get_int() { // fonction de récupération d'un entier positif
	char buffer[NBCHIFFREMAX + 1]; // on definie un mot de longueur de la taille maximum d'un chiffre (+ le "\0")
	scanf("%s", buffer); // l'utilisateur entre le nombre souhaité qui est affecté a la variable buffer
	if (EchoActif) printf(">>echo %s\n", buffer);// si le mode débuggage est activé on affiche le mot entre precedemment
	assert(atoi(buffer) >= 0); // on vérifie que le nombre entré est positif
	return atoi(buffer); //on renvoie le mot convertit en entier
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
typedef struct {// une commande est définie par son nom (mot), les client auquelle elle est associee (entier naturel), les taches qui lui sont associeesla traveilleurs en charge des taches ainsi que la facture qui lui est associée
	Mot nom;
	unsigned int idx_client;
	Tache taches_par_specialite[MAX_SPECIALITES]; // nb_heures_requises==0 <=> pas de tache pour cette specialite, tache numero n est associee a la specialite n du tableau des specialites
	int idx_trav_tache[MAX_SPECIALITES]; // pour la tache numero n est associe l'index du travailleur en charge de la tache
	long facture;
} Commande;
typedef struct {//l'ensemble des commandes est regroupe dans une structure contenant un tableau de travailleur ainsi qu'un entier contenant le nombre de travailleurs et qu'un entier contenant les nombre de commandes facturees
	Commande tab_commandes[MAX_COMMANDES];
	unsigned int nb_commandes;
	unsigned int nb_facturations;
} Commandes;

// déclaration des fonctions --------------------------------------------------
void traite_developpe(Specialites* rep_spe); // permet des renseigner une nouvelle specialite
void traite_embauche(Travailleurs* rep_trav, const Specialites* rep_spe); // permet des renseigner un nouveau travailleur
void traite_demarche(Clients* rep_cli); // permet de renseigner un nouveau client
void traite_commande(Commandes* rep_com, const Clients* rep_cli);// permet de renseigner une nouvelle commande
void traite_supervision(const Specialites* rep_spe, const Commandes* rep_com); // affiche l'etat de toutes les taches pour toutes les commandes
void traite_client(const Clients* rep_cli, const Commandes* rep_com);// recuper le nom du client a afficher (ou la commande "tous" pour afficher tout les clients)
void affiche_clients(const Clients* rep_cli, const Commandes* rep_com, int i);// affiche les commandes effectues par un client
void traite_travailleurs(const Specialites* rep_spe, const Travailleurs* rep_trav);// recuper le nom du travailleur a afficher (ou la commande "tous" pour afficher tout les travailleurs)
void affiche_travailleurs(const Specialites* rep_spe, const Travailleurs* rep_trav, int i);// affiche les travailleurs maitrisiant la spécialité demandée
void traite_specialites(const Specialites* rep_spe); // affiche les specilaites traitees
void traite_tache(const Specialites* rep_spe, Commandes* rep_com, Travailleurs* rep_trav); //cree une nouvelle tache dans la commande demandee pour la specialite demandee, la tache est ensuite assignee a un travailleur
void traite_progression(const Specialites* rep_spe, Commandes* rep_com, Travailleurs* rep_trav, const Clients* rep_cli, int* idx_spe_passe, int* idx_com_passe); // fait progresser la tache demandee du nombre d'heure spécifiée, si laache est  complétée on enclanche la facturation
void traite_charge(const Travailleurs* rep_trav, const Commandes* rep_com, const Specialites* rep_spe); // affiche les tache assignees au travailleur demande ainsi que lenmbre d'heure qu'il reste  a effetuer pour les tache en question
void traite_passe(const int idx_com, const int idx_spe, Commandes* rep_com, Travailleurs* rep_trav); // reassigne la tahe a un autre travailleur a pres un "traite_porgression" uniquement
void traite_assignation(const int indx_com, const int indx_spe, Commandes* rep_com, Travailleurs* rep_trav); // la tache donnée en arametre est assignée a un travailleur
void traite_facturation(int indx_com,const Specialites* rep_spe, Commandes* rep_com, const Clients* rep_cli); // verifie que toutes les tache d'une commande sont terminees et, si c'est la cas, calcule et enregistre la facture 
void traite_fin(const Commandes* rep_com, const Clients* rep_cli); // verifie que toutes les commnades ont bien étés facturées. Si c'est le cas affiche la liste des clients ainsi que le prix a payer pour chaque client avant de terminer le programme
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
	
	int idx_spe_passe, idx_com_passe; // permettent de recuperer les information sur la tache modifiee dans la fonction progression
	rep_clients.nb_clients = 0; // on initialise le nombre de
	rep_specialites.nb_specialites = 0; // on initialise le nombre de specialites enregistrees
	rep_travailleurs.nb_travailleurs = 0; // on initialise le nombre de travailleurs engages
	rep_commandes.nb_commandes = 0; // on initialise le nombre de commandes effectuees
	rep_commandes.nb_facturations = 0; // on initialise le nombre de facturation effectues
	
	while (VRAI) { // boucle infinie
		get_id(buffer); // on demande la commande a entrer a l'utilisateur
		if (progression == VRAI && strcmp(buffer, "passe") == 0) { // on verifie que l'utilisateur a entré "passe" et que progression vaut VRAI
			traite_passe(idx_com_passe, idx_spe_passe, &rep_commandes, &rep_travailleurs); // on execute la fonction traite_passe
			progression = FAUX; // on passe progression a FAUX
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		else {
			progression = FAUX;// on passe progression a la valeur FAUX
		}
		if (strcmp(buffer, "commande") == 0) {// on verifie si l'utilisateur a entre "commande" dans buffer
			traite_commande(&rep_commandes, &rep_clients); // on execute la fonction traite_commande
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "charge") == 0) {// on verifie si l'utilisateur a entre "charge" dans buffer
			traite_charge(&rep_travailleurs, &rep_commandes, &rep_specialites); // on execute la fonction traite_charge
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "supervision") == 0) {// on verifie si l'utilisateur a entre "supervision" dans buffer
			traite_supervision(&rep_specialites, &rep_commandes); // on execute la fonction traite_supervision
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "client") == 0) {// on verifie si l'utilisateur a entre "client" dans buffer
			traite_client(&rep_clients, &rep_commandes); // on execute la fonction traite_client
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "travailleurs") == 0) {// on verifie si l'utilisateur a entre "travailleurs" dans buffer
			traite_travailleurs(&rep_specialites, &rep_travailleurs); // on execute la fonction traite_travailleurs
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "specialites") == 0) {// on verifie si l'utilisateur a entre "specialite" dans buffer
			traite_specialites(&rep_specialites); // on execute la fonction traite_specialites
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "progression") == 0) {// on verifie si l'utilisateur a entre "progression" dans buffer
			traite_progression(&rep_specialites, &rep_commandes, &rep_travailleurs, &rep_clients,&idx_spe_passe,&idx_com_passe); // on execute la fonction traite_proression
			progression = VRAI;
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "tache") == 0) {// on verifie si l'utilisateur a entre "tache" dans buffer
			traite_tache(&rep_specialites, &rep_commandes, &rep_travailleurs); // on execute la fonction traite_tache
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "demarche") == 0) {// on verifie si l'utilisateur a entre "demarche" dans buffer
			traite_demarche(&rep_clients); // on execute la fonction traite_demarche
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "embauche") == 0) {// on verifie si l'utilisateur a entre "embauche" dans buffer
			traite_embauche(&rep_travailleurs, &rep_specialites); // on execute la fonction traite_embauche
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "developpe") == 0) {// on verifie si l'utilisateur a entre "developpe" dans buffer
			traite_developpe(&rep_specialites); // on execute la fonction traite_developpe
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "interruption") == 0) {// on verifie si l'utilisateur a entre "interruption" dans buffer
			traite_interruption(); // on execute la fonction traite_interruption
			break; // on quitte la boucle while
		}
		printf("!!! instruction inconnue >%s< !!!\n", buffer); // si aucune commande n'as ete reconnue on affiche ce message
	}
	return 0;// en cas de sortie de la boucle infinie on met fin au programme
}


// Instructions --------------------------------------------------------------- 

// developpe --------------------------- 
void traite_developpe(Specialites* rep_spe) {
	Specialite spe;
	get_id(spe.nom);
	spe.cout_horaire = get_int();
	rep_spe->tab_specialites[rep_spe->nb_specialites] = spe;
	rep_spe->nb_specialites += 1;
}

// Embauche ----------------------------
void traite_embauche(Travailleurs* rep_trav, const Specialites* rep_spe) {
	Mot  nom_specialite;
	Travailleur travailleur;
	unsigned int j = 0;
	get_id(travailleur.nom);
	get_id(nom_specialite);
	for (j = 0; j < rep_trav->nb_travailleurs; j++) {
		if (strcmp(travailleur.nom, rep_trav->tab_travailleurs[j].nom) == 0) {
			for (unsigned int i = 0; i < rep_spe->nb_specialites; i++) {
				if (strcmp(nom_specialite, rep_spe->tab_specialites[i].nom) == 0) {
					rep_trav->tab_travailleurs[j].tags_competences[i] = VRAI;
					return;
				}
			}
		}
	}
	for (unsigned int i = 0; i < rep_spe->nb_specialites; i++) {
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
	get_id(cmd.nom);
	get_id(nom_client);
	cmd.facture = -1;
	for (unsigned int i = 0; i < MAX_SPECIALITES; i++) {
		cmd.taches_par_specialite[i].nb_heures_effectuees = 0;
		cmd.taches_par_specialite[i].nb_heures_requises = 0;
		cmd.idx_trav_tache[i] = -1;
	}
	for (unsigned int i = 0; i < rep_cli->nb_clients; i++) {
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
	int requis, effectuees;
	if (rep_com->nb_commandes > 0) {
		for (unsigned int i = 0; i < rep_com->nb_commandes; i++) {
			printf(MSG_SUPERVISION, rep_com->tab_commandes[i].nom);
			for (unsigned int j = 0; j < rep_spe->nb_specialites; j++) {
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
	get_id(nom_client);
	unsigned int i = 0;
	Booleen suivant = FAUX;
	if (strcmp(nom_client, "tous") == 0) {
		while (i < rep_cli->nb_clients) {
			affiche_clients(rep_cli,rep_com, i);
			i++;
		}
	}
	else {
		while (i < rep_cli->nb_clients) {
			if (strcmp(nom_client, rep_cli->tab_clients[i]) == 0) {
				affiche_clients(rep_cli, rep_com, i);
				return;
			}
			i++;
		}
		printf(MSG_CLIENT_ERREUR);
	}
}

// Affichage Client --------------------
void affiche_clients(const Clients* rep_cli, const Commandes* rep_com, int i){
	Booleen suivant = FAUX;
	printf(MSG_CLIENT, rep_cli->tab_clients[i]);
	for (unsigned int j = 0; j < rep_com->nb_commandes; j++) {
		if (rep_com->tab_commandes[j].idx_client == i) {
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
	get_id(nom_specialite);
	unsigned int i = 0;
	Booleen suivant = FAUX;
	if (strcmp(nom_specialite, "tous") == 0) {
		while (i < rep_spe->nb_specialites) {
			affiche_travailleurs(rep_spe, rep_trav, i);
			i++;
		}
	}
	else {
		while (i < rep_spe->nb_specialites) {
			if (strcmp(nom_specialite, rep_spe->tab_specialites[i].nom) == 0) {
				affiche_travailleurs(rep_spe, rep_trav, i);
				return;
			}
			i++;
		}
		printf(MSG_SPECIALITES_ERREUR);
	}
}

// Affiche travailleurs
void affiche_travailleurs(const Specialites* rep_spe, const Travailleurs* rep_trav, int i) {
	Booleen suivant = FAUX;
	printf(MSG_TRAVAILLEURS, rep_spe->tab_specialites[i].nom);
	for (unsigned int j = 0; j < rep_trav->nb_travailleurs; j++) {
		if (rep_trav->tab_travailleurs[j].tags_competences[i] == VRAI) {
			if (suivant)printf(", ");
			else suivant = VRAI;
			printf("%s", rep_trav->tab_travailleurs[j].nom);
		}
	}
	printf("\n");
	return;
}

// Specialités -------------------------
void traite_specialites(const Specialites* rep_spe) {
	printf(MSG_SPECIALITES);
	if (rep_spe->nb_specialites == 0) {
		printf("\n");
		return;
	}
	for (unsigned int i = 0; i < rep_spe->nb_specialites; i++) {
		printf("%s/%d", rep_spe->tab_specialites[i].nom, rep_spe->tab_specialites[i].cout_horaire);
		if (i != rep_spe->nb_specialites - 1)printf(", ");
	}
	printf("\n");
}

// Tâches ------------------------------
void traite_tache(const Specialites* rep_spe, Commandes* rep_com, Travailleurs* rep_trav) {
	Mot nom_commande, nom_specialite;
	get_id(nom_commande);
	get_id(nom_specialite);
	int nbr_heure = get_int();
	for (unsigned int i = 0; i < rep_com->nb_commandes; i++) {
		if (strcmp(rep_com->tab_commandes[i].nom, nom_commande) == 0) {
			for (unsigned int j = 0; j < rep_spe->nb_specialites; j++) {
				if (strcmp(rep_spe->tab_specialites[j].nom, nom_specialite) == 0) {
					rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_requises = nbr_heure;
					traite_assignation(i, j, rep_com, rep_trav);
					break;
				}
			}
			break;
		}
	}
}

// Progression -------------------------
void traite_progression(const Specialites* rep_spe, Commandes* rep_com, Travailleurs* rep_trav, const Clients* rep_cli, int* idx_spe_passe, int* idx_com_passe) {
	Mot nom_commande, nom_specialite;
	get_id(nom_commande);
	get_id(nom_specialite);
	int nbr_heure = get_int(),diff, requis, effectuees;
	for (unsigned int i = 0; i < rep_com->nb_commandes; i++) {
		if (strcmp(rep_com->tab_commandes[i].nom, nom_commande) == 0 && rep_com->tab_commandes[i].facture<0) {
			for (unsigned int j = 0; j < rep_spe->nb_specialites; j++) {
				requis = rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_requises;
				if (strcmp(rep_spe->tab_specialites[j].nom, nom_specialite) == 0 && requis!=0) {
					rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees += nbr_heure;
					rep_trav->tab_travailleurs[rep_com->tab_commandes[i].idx_trav_tache[j]].nb_heures_travail -= nbr_heure;
					requis = rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_requises;
					effectuees = rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees;
					diff = requis - effectuees;
					if (diff<=0) {
						rep_trav->tab_travailleurs[rep_com->tab_commandes[i].idx_trav_tache[j]].nb_heures_travail += diff;
						rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees = requis;
						rep_com->tab_commandes[i].idx_trav_tache[j] = -1;
						traite_facturation(i, rep_spe, rep_com,rep_cli);
					}
					*idx_com_passe = i;
					*idx_spe_passe = j;
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
	int requis, effectuees, diff;
	Booleen suivant = FAUX;
	if (rep_trav->nb_travailleurs == 0)return;
	for (unsigned int i = 0; i < rep_trav->nb_travailleurs; i++) {
		if (strcmp(nom_travailleur, rep_trav->tab_travailleurs[i].nom) == 0) {
			printf(MSG_CHARGE, nom_travailleur);
			if (rep_com->nb_commandes == 0) {
				printf("\n");
				return;
			}
			for (unsigned int j = 0; j < rep_com->nb_commandes; j++) {
				for (unsigned int k = 0; k < rep_spe->nb_specialites; k++) {
					if (rep_com->tab_commandes[j].idx_trav_tache[k] == i) {
						if (suivant)printf(", ");
						else suivant = VRAI;
						requis = rep_com->tab_commandes[j].taches_par_specialite[k].nb_heures_requises;
						effectuees = rep_com->tab_commandes[j].taches_par_specialite[k].nb_heures_effectuees;
						diff = requis - effectuees;
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
void traite_passe(const int idx_com, const int idx_spe, Commandes* rep_com, Travailleurs* rep_trav) {
	int idx_trav = rep_com->tab_commandes[idx_com].idx_trav_tache[idx_spe];
	int requis = rep_com->tab_commandes[idx_com].taches_par_specialite[idx_spe].nb_heures_requises;
	int effectuees = rep_com->tab_commandes[idx_com].taches_par_specialite[idx_spe].nb_heures_effectuees;
	int nb_heures = requis - effectuees;
	if (nb_heures == 0)return;
	rep_trav->tab_travailleurs[idx_trav].nb_heures_travail -= nb_heures;
	rep_com->tab_commandes[idx_com].idx_trav_tache[idx_spe] = -1;
	traite_assignation(idx_com, idx_spe, rep_com, rep_trav);
}

// Assignation -------------------------
void traite_assignation(const int idx_com, const int idx_spe, Commandes* rep_com, Travailleurs* rep_trav) {
	Booleen suivant = FAUX;
	int affecter = -1, requis, effectuees, diff;
	for (unsigned int i = 0; i < rep_trav->nb_travailleurs; i++) {
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
	rep_com->tab_commandes[idx_com].idx_trav_tache[idx_spe] = affecter;
	rep_trav->tab_travailleurs[affecter].nb_heures_travail += diff;
}

// Facturation -------------------------
void traite_facturation(int idx_com, const Specialites* rep_spe, Commandes* rep_com, const Clients* rep_cli) {
	long facture = 0;
	int requis, cout_horaire;
	Booleen suivant = FAUX;
	if (rep_com->tab_commandes[idx_com].facture > 0)return;
	for (unsigned int i = 0; i < rep_spe->nb_specialites; i++) {
		requis = rep_com->tab_commandes[idx_com].taches_par_specialite[i].nb_heures_requises;
		if (rep_com->tab_commandes[idx_com].taches_par_specialite[i].nb_heures_effectuees == requis) {
			cout_horaire = rep_spe->tab_specialites[i].cout_horaire;
			facture += cout_horaire *requis;
		}
		else {
			return;
		}
	}
	printf(MSG_FACTURATION, rep_com->tab_commandes[idx_com].nom);
	for (unsigned int i = 0; i < rep_spe->nb_specialites; i++) {
		requis = rep_com->tab_commandes[idx_com].taches_par_specialite[i].nb_heures_requises;
		if (requis > 0) {
			if (suivant)printf(", ");
			else suivant = VRAI;
			cout_horaire = rep_spe->tab_specialites[i].cout_horaire;
			printf("%s:%ld", rep_spe->tab_specialites[i].nom, cout_horaire * requis);
		}
	}
	printf("\n");
	rep_com->tab_commandes[idx_com].facture = facture;
	rep_com->nb_facturations += 1;
	traite_fin(rep_com, rep_cli);
}

// Fin du programme --------------------
void traite_fin(const Commandes* rep_com,const Clients* rep_cli) {
	int fact = 0;
	Booleen suivant = FAUX;
	if (rep_com->nb_commandes == rep_com->nb_facturations) {
		printf(MSG_FACTURATION_FINALE);
		for (unsigned int i = 0; i < rep_cli->nb_clients; i++) {
			for (unsigned int j = 0; j < rep_com->nb_commandes; j++) {
				if (rep_com->tab_commandes[j].idx_client == i) {
					fact += rep_com->tab_commandes[j].facture;
				}
			}
			if (suivant)printf(", ");
			else suivant = VRAI;
			printf("%s:%ld", rep_cli->tab_clients[i], fact);
			fact = 0;
		}
		printf("\n");
		exit(0);
	}
}

// interruption ------------------------ 
void traite_interruption() {
	printf(MSG_INTERRUPTION);
}