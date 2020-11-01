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
	Specialite spe; // on crée une variable de type specialite
	get_id(spe.nom);// on recupere le nom de la specialite aupres de l'utilisateur
	spe.cout_horaire = get_int(); // on recupere le cout horaire de la specialite aupres de l'utilisateur
	rep_spe->tab_specialites[rep_spe->nb_specialites] = spe; // on affecte la valeur spe au premier emplacement disponible du repertoire de specialite
	rep_spe->nb_specialites += 1; // on incremente le nombre de specialites enregistrees de 1
}

// Embauche ----------------------------
void traite_embauche(Travailleurs* rep_trav, const Specialites* rep_spe) {
	Mot  nom_specialite; // on cree une variable de type mot qui contriendra le nom de la specialite
	Travailleur travailleur; // on cree une variable de type travailleur qui contiendra les information du travailleur a enregister
	unsigned int j = 0; //on initialie le compteur pour la boucle for
	get_id(travailleur.nom); // on recupere le nom du travailleur aupres de l'utilisateur
	get_id(nom_specialite); // on recupere le nom de la specialite maitrisee par le futur travailleur aupres de l'utilisateur
	for (j = 0; j < rep_trav->nb_travailleurs; j++) { // on parcourt l'ensemble des travailleurs enrigistres
		if (strcmp(travailleur.nom, rep_trav->tab_travailleurs[j].nom) == 0) { // on verifie si le nom du travailleur a enregistrer est le meme que celui du travailleur enregistré a l'emplacement dindex j
			for (unsigned int i = 0; i < rep_spe->nb_specialites; i++) { // on parcourt l'ensemble de specialites enregistrees
				if (strcmp(nom_specialite, rep_spe->tab_specialites[i].nom) == 0) { // on verifie si le nom de la specialite renseignee par l'utilisateur et le meme que celui enregistré a l'emplacement dindex i
					rep_trav->tab_travailleurs[j].tags_competences[i] = VRAI; // on passe la compétence d'index i a VRAI pour le travailleur d'index j
					return; // on termine la fonction
				}
			}
		}
	}
	for (unsigned int i = 0; i < rep_spe->nb_specialites; i++) { //on parcourt l'ensemble des specialites enregistrees
		if (strcmp(nom_specialite, rep_spe->tab_specialites[i].nom) == 0) { // on verifie si le nom de la specialite renseignee par l'utilisateur et le meme que celui enregistré a l'emplacement dindex i
			travailleur.tags_competences[i] = VRAI; // on passe la competence d'index i a vrai pour le travailleur a enregistrer
			break; // on sort de la boucle for
		}
	}
	travailleur.nb_heures_travail = 0; // on initialise a 0 le nombre d'ehure de travail du travailleur
	rep_trav->tab_travailleurs[rep_trav->nb_travailleurs] = travailleur; // on affecte la valeur travailleur au premier emplacement disponible du repertoire des travailleurs
	rep_trav->nb_travailleurs += 1; // on incremente le nombre de travailleurs enregistrees de 1
}

// Demarche ------------------------------
void traite_demarche(Clients* rep_cli) {
	get_id(rep_cli->tab_clients[rep_cli->nb_clients]); // on recupere aupres de l'utilisateur le nom du client a enregister et on le met dans le premier emplacement disponible du repertoire des clients
	rep_cli->nb_clients += 1; // on incremente le nombre de clients enregistrees de 1
}

// Commande -----------------------------
void traite_commande(Commandes* rep_com, const Clients* rep_cli) {
	Commande cmd; // on declare une variable locale cmd de type commande
	Mot nom_client; // on declare une variable locale nom_client de type mot qui contirendra le nom de client
	get_id(cmd.nom); // on recupere aupres de l'utilisateur le nom de la commnde
	get_id(nom_client); // on recupere aupres de l'utilisateur le nom du client
	cmd.facture = -1; // on initialise la raviable facrue de cmd a -1
	for (unsigned int i = 0; i < MAX_SPECIALITES; i++) { // on parcourt l'ensemble du tableau tache_par_specialite de cmd
		cmd.taches_par_specialite[i].nb_heures_effectuees = 0; // on inistialise la valeur de nb_heures_effectuees a 0 pour la tache d'index i
		cmd.taches_par_specialite[i].nb_heures_requises = 0; // on inistialise la valeur de nb_heures_requises a 0 pour la tache d'index i
		cmd.idx_trav_tache[i] = -1; // on initialise ma valeur de l'index du travailleur associé a la tache i a -1 (non assignee)
	}
	for (unsigned int i = 0; i < rep_cli->nb_clients; i++) { // on parcours l'ensemble des clients enregistres
		if (strcmp(nom_client, rep_cli->tab_clients[i]) == 0) {// on verifie si le nom du client renseignee par l'utilisateur et le meme que celui enregistré a l'emplacement dindex i
			cmd.idx_client = i; // on affecte la valeur i pour l'indexedu client dans la commande
			break; // on quitte la boucle for
		}
	}
	rep_com->tab_commandes[rep_com->nb_commandes] = cmd; // on affecte la valeur cmd au premier emplacement disponible du repertoire des commandes
	rep_com->nb_commandes += 1;// on incremente le nombre de commandes enregistrees de 1
}

// Supervision -------------------------
void traite_supervision(const Specialites* rep_spe, const Commandes* rep_com) {
	Booleen suivant = FAUX; // on declare un booleen local qu'on initilalise a FAUX
	int requis, effectuees; // on declare deux entier qui correspondent aux nombres d'heures requises et effectuees pour une tache donnee pour simplefier la lisibilité
	if (rep_com->nb_commandes > 0) { // on verifie si le nombre de commandes enregistrees est non nul
		for (unsigned int i = 0; i < rep_com->nb_commandes; i++) { // on parcours l'ensemble des commandes
			printf(MSG_SUPERVISION, rep_com->tab_commandes[i].nom); // affichage du message de supervision avec le nom de la commande d'index i en argument
			for (unsigned int j = 0; j < rep_spe->nb_specialites; j++) { // parcours de l'ensemble des specialites
				requis = rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_requises; // on affecte a requis le nombre d'heures requises pour la tache j pour la commande i
				effectuees = rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees; // on affecte a effectuees le nombre d'heures effecuees pour la tache j pour la commande i
				if (requis != 0) { // on verifie si requis est non nul
					if (suivant)printf(", "); // si suivant vaut VRAI, affichage de ", "
					else suivant = VRAI; // sinon aucun affichge et passage de suivant a VRAI
					printf("%s:%d/%d", rep_spe->tab_specialites[j].nom, effectuees, requis); // affichage du message sous la forme "nom specialite:nombre d'heure effectuees/nombre d'heures requises"
				}
			}
			printf("\n"); // retour a la ligne
			suivant = FAUX; // passage de suivant a FAUX
		}
	}
}

// Client ------------------------------
void traite_client(const Clients* rep_cli, const Commandes* rep_com) {
	Mot nom_client; // declaration de la variable de type mot qui contiendra le nom du client
	get_id(nom_client); // recuperation du nom du client aupres de l'utilisateur
	unsigned int i = 0; // initialisation du compteur i a 0
	Booleen suivant = FAUX; // declaration d'un booleen local initilalise a FAUX
	if (strcmp(nom_client, "tous") == 0) { // si tous es entré on affiche tous les clients
		while (i < rep_cli->nb_clients) { //parcours de l'ensemble des clients
			affiche_clients(rep_cli,rep_com, i); // affichage du client d'index i et de ses commandes
			i++; // incrementation de i
		}
	}
	else { // sinon on affiche uniquement le client indique par l'utilisateur
		while (i < rep_cli->nb_clients) { // parcourt de l'ensemble des clients
			if (strcmp(nom_client, rep_cli->tab_clients[i]) == 0) { // on verifie si le clients resneigne est le meme que celui d'index i
				affiche_clients(rep_cli, rep_com, i); // affichage du client d'index i et de ses commandes
				return; // interruption de la fonction
			}
			i++; // incrementation de i
		}
		printf(MSG_CLIENT_ERREUR); // affichage d'un message d'erreur si le client renseigne n'est pas enregistre
	}
}

// Affichage Client --------------------
void affiche_clients(const Clients* rep_cli, const Commandes* rep_com, int i){
	Booleen suivant = FAUX; // on declare un booleen local qu'on initilalise a FAUX
	printf(MSG_CLIENT, rep_cli->tab_clients[i]); // affichage de MSG_CLIENT avec comme arguement le nom du client d'index i
	for (unsigned int j = 0; j < rep_com->nb_commandes; j++) {// pracours de l'ensemble des commandes
		if (rep_com->tab_commandes[j].idx_client == i) { // on verifie si l'index du client associé a la commande j est celui du client indique en parametre
			if (suivant)printf(", "); // si suivant vaut VRAI, affichage de ", "
			else suivant = VRAI; // sinon aucun affichge et assage de suivant a VRAI
			printf(MSG_CLIENT_ID_COMMANDE, rep_com->tab_commandes[j].nom); // affichage du message de commande avec le nom de la commande d'index j en argument
		}
	}
	printf("\n");
	return;
}

// Travailleurs ------------------------
void traite_travailleurs(const Specialites* rep_spe, const Travailleurs* rep_trav) {
	Mot nom_specialite; // declaration de la variable de type mot qui contiendra le nom de la specialite
	get_id(nom_specialite); // recuperation du nom de la specialite aupres de l'utilisateur
	unsigned int i = 0; // initialisation du compteur i a 0
	Booleen suivant = FAUX; // on declare un booleen local qu'on initilalise a FAUX
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
				return; // interruption de la fonction
			}
			i++; // incrementation de i
		}
		printf(MSG_SPECIALITES_ERREUR); // affichage d'un message d'erreur si la specialite renseignee n'est pas enregistree
	}
}

// Affiche travailleurs ---------------
void affiche_travailleurs(const Specialites* rep_spe, const Travailleurs* rep_trav, int i) {
	Booleen suivant = FAUX; // on declare un booleen local qu'on initilalise a FAUX
	printf(MSG_TRAVAILLEURS, rep_spe->tab_specialites[i].nom); // affichage de MSG_TRAVAILLEURS avec comme arguement le nom de la specialite d'index i
	for (unsigned int j = 0; j < rep_trav->nb_travailleurs; j++) { // pracours de l'ensemble des travailleurs
		if (rep_trav->tab_travailleurs[j].tags_competences[i] == VRAI) {  // on verifie si le travailleurs d'index j maitrise la competence d'index i
			if (suivant)printf(", "); // si suivant vaut VRAI, affichage de ", "
			else suivant = VRAI; // sinon aucun affichge et passage de suivant a VRAI
			printf("%s", rep_trav->tab_travailleurs[j].nom); // affichage du nom du travailleur d'index j
		}
	}
	printf("\n"); // retour a la ligne
	return; // fin de la focntion
}

// Specialités -------------------------
void traite_specialites(const Specialites* rep_spe) {
	printf(MSG_SPECIALITES); // affichage de  MSG_SPECIALITE
	if (rep_spe->nb_specialites == 0) { // on verifie si le nombre de specialites enregistrees est nul
		printf("\n"); // retour a la ligne
		return; // interruption de la fonction
	}
	for (unsigned int i = 0; i < rep_spe->nb_specialites; i++) { // parcours de l'ensemble des specialites
		printf("%s/%d", rep_spe->tab_specialites[i].nom, rep_spe->tab_specialites[i].cout_horaire); // affciahge de la specialite et de son cout horaire sous la forme "nom/cout horaire"
		if (i != rep_spe->nb_specialites - 1)printf(", "); // si i est different de l'index de la derniere specialite, affichage de ", "
	}
	printf("\n"); // retour a la ligne
}

// Tâches ------------------------------
void traite_tache(const Specialites* rep_spe, Commandes* rep_com, Travailleurs* rep_trav) {
	Mot nom_commande, nom_specialite;// declaration de deux variables de type mot qui contiendront respectivene le nom de la commande et le nom de la specialite
	get_id(nom_commande); // recuperation du nom de la commande aupres de l'utilisateur
	get_id(nom_specialite); // recuperation du nom de la specialite aupres de l'utilisateur
	int nbr_heure = get_int();// declaration d'une variable de type int et recuperation aupres de l'utilisateur du nombred'heure de la tache a effectuer
	for (unsigned int i = 0; i < rep_com->nb_commandes; i++) { // parcours de l'ensemble des commandes 
		if (strcmp(rep_com->tab_commandes[i].nom, nom_commande) == 0) { // on verifie si la commande renseignee est la meme que celle d'index i
			for (unsigned int j = 0; j < rep_spe->nb_specialites; j++) { // prcours de l'ensemble des specialites
				if (strcmp(rep_spe->tab_specialites[j].nom, nom_specialite) == 0) { // on verifie si la specialite renseignee est la meme que celle d'index j
					rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_requises = nbr_heure; // assignation de la valeur nbr_heure au nombre d'heure requises pour la tache d'index j pour la commandes d'index i
					traite_assignation(i, j, rep_com, rep_trav); // affectation de la tache d'index j pour la commandes d'index i a un travailleur
					break; // interruption de la boucle
				}
			}
			break; // interruption de la boucle
		}
	}
}

// Progression -------------------------
void traite_progression(const Specialites* rep_spe, Commandes* rep_com, Travailleurs* rep_trav, const Clients* rep_cli, int* idx_spe_passe, int* idx_com_passe) {
	Mot nom_commande, nom_specialite; // declaration de deux variables de type mot qui contiendront respectivene le nom de la commande et le nom de la specialite
	get_id(nom_commande); // recuperation du nom de la commande aupres de l'utilisateur
	get_id(nom_specialite);  // recuperation du nom de la specialite aupres de l'utilisateur
	int nbr_heure = get_int(),diff, requis, effectuees; // declaration d'une variable de type int et recuperation aupres de l'utilisateur du nombred'heure de la tache a effectuer, declaration de traois variables simplificatrices
	for (unsigned int i = 0; i < rep_com->nb_commandes; i++) { // parcours de l'ensemble des commandes
		if (strcmp(rep_com->tab_commandes[i].nom, nom_commande) == 0 && rep_com->tab_commandes[i].facture<0) { // on verifie si la commande renseignee est la meme que celle d'index i et que la facture pour la commande d'index i est negative
			
			for (unsigned int j = 0; j < rep_spe->nb_specialites; j++) { // parcoursde l'ensemble des specialites
				requis = rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_requises; // on affecte a requis le nombre d'heures requises pour pour la tache d'index j de la commandes d'index i
				if (strcmp(rep_spe->tab_specialites[j].nom, nom_specialite) == 0 && requis!=0) {// on verifie si la specialite renseignee est la meme que celle d'index j et que requis est non nul

					rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees += nbr_heure; //on ajoute nbr_heures au nombre d'heures effectuees pour la tache d'index j
					rep_trav->tab_travailleurs[rep_com->tab_commandes[i].idx_trav_tache[j]].nb_heures_travail -= nbr_heure; // on enleve nbr_heure au nmbre d'heure de travail du travailleur en charge de a tache d'index j

					requis = rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_requises; // on affecte a requis le nombre d'heures requises pour la tache j
					effectuees = rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees; // on affecte a effectuees la nombre d'heures effectuees pour la tache j
					diff = requis - effectuees; // on affecte a diff la diffecrence entre requis et effectuees

					if (diff<=0) { // on verifie si diff est inferieur ou egal a zero <=> nb d'heures effectuees >= nb d'heures requises
						rep_trav->tab_travailleurs[rep_com->tab_commandes[i].idx_trav_tache[j]].nb_heures_travail += diff; // compensation du nombres d'heures de travail a effectuer si depassement (effectuees>requis)
						rep_com->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees = requis; // mise a niveau du nb d'heures effectues (en cas de dépassement)
						rep_com->tab_commandes[i].idx_trav_tache[j] = -1; // reinitialisation de l'asssignation
						traite_facturation(i, rep_spe, rep_com,rep_cli); // facturation de la commande d'index i
					}
					*idx_com_passe = i; // l'index de la commande est mis dans idx_com_passe dans l'evntualite de l'utilisation de la commande traite_passe
					*idx_spe_passe = j; // l'index de la specialite est mis dans idx_spe_passe dans l'evntualite de l'utilisation de la commande traite_passe
					break; // interruption de la boucle
				}
			}
			break; // interruption de la boucle
		}
	}
}

// Charge ------------------------------
void traite_charge(const Travailleurs* rep_trav, const Commandes* rep_com, const Specialites* rep_spe) {
	Mot nom_travailleur; // declaration de la variable de type mot qui contiendra le nom du travailleur
	get_id(nom_travailleur); // recuperation du nom du travailleur aupres de l'utilisateur
	int requis, effectuees, diff; // decaration de 3 variables de simlification de lecture
	Booleen suivant = FAUX; // on declare un booleen local qu'on initilalise a FAUX
	if (rep_trav->nb_travailleurs == 0)return; // si il n'y a aucun travailleur enregistré la fonction d'interrompt
	for (unsigned int i = 0; i < rep_trav->nb_travailleurs; i++) { // parcours de l'ensemble des travailleurs
		if (strcmp(nom_travailleur, rep_trav->tab_travailleurs[i].nom) == 0) {// on verifie si le nom du travailleur a enregistrer est le meme que celui du travailleur enregistré a l'emplacement dindex i
			printf(MSG_CHARGE, nom_travailleur); // affichage du message MSG_CHARGE avec nom_travailleur en argument
			if (rep_com->nb_commandes == 0) { // si il n'y a aucune commande enregistree on applique un retour a la ligne et la fonction s'interrompt
				printf("\n"); 
				return;
			}
			for (unsigned int j = 0; j < rep_com->nb_commandes; j++) { // parcours de l'ensemble des commandes enregistrees
				for (unsigned int k = 0; k < rep_spe->nb_specialites; k++) { // parcours de l'ensemble des specialites enregistrees
					if (rep_com->tab_commandes[j].idx_trav_tache[k] == i) { // on verifie si le travailleur i est en charge de la tache d'index k de la commande d'index j
						if (suivant)printf(", "); // si suivant vaut VRAI, affichage de ", "
						else suivant = VRAI; // sinon aucun affichge et passage de suivant a VRAI
						requis = rep_com->tab_commandes[j].taches_par_specialite[k].nb_heures_requises; // on affecte a requis le nombre d'heures requises pour la tache k
						effectuees = rep_com->tab_commandes[j].taches_par_specialite[k].nb_heures_effectuees; // on affecte a effectuees la nombre d'heures effectuees pour la tache k
						diff = requis - effectuees; // on affecte a diff la diffecrence entre requis et effectuees
						printf(MSG_CHARGE_TACHE, rep_com->tab_commandes[j].nom, rep_spe->tab_specialites[k].nom, diff); // on affiche le message MSG_CHARGE_TACHE vec comme parametres le nom de la commande, le nom de la specialite et le nb d'heures a effectuer
					}
				}
			}
			printf("\n"); // retour a la ligne
			suivant = FAUX; // passage de suivant a FAUX
		}
	}
}

// Passe -------------------------------
void traite_passe(const int idx_com, const int idx_spe, Commandes* rep_com, Travailleurs* rep_trav) { //reassigne la tache d'index ids_spe pour la commande d'index idx_com
	int idx_trav = rep_com->tab_commandes[idx_com].idx_trav_tache[idx_spe]; // idx_trav est initialisée a la valeur de l'index du travailleur en charge de la tache d'index idx_spe pour la commande d'index idx_com
	int requis = rep_com->tab_commandes[idx_com].taches_par_specialite[idx_spe].nb_heures_requises;// variable de simplifiacattion d'ecriture (nb d'heures requise pour la tache a reassigner)
	int effectuees = rep_com->tab_commandes[idx_com].taches_par_specialite[idx_spe].nb_heures_effectuees; // variable de simplifiacattion d'ecriture (nb d'heures effectuees pour la tache a reassigner)
	int nb_heures = requis - effectuees;// variable de simplifiacattion d'ecriture (nb d'heure restantes a effectuer)
	if (nb_heures == 0)return; // si la tach est terminee la fonction s'interrompt
	rep_trav->tab_travailleurs[idx_trav].nb_heures_travail -= nb_heures; // on deduis le nombres d'heures restantes sur la tache au nombres d'heures a effectuer pour le travailleur en charge
	rep_com->tab_commandes[idx_com].idx_trav_tache[idx_spe] = -1; // reinitialisation de l'assignation de la tache
	traite_assignation(idx_com, idx_spe, rep_com, rep_trav); // nouvelle assignation de la tache 
}

// Assignation -------------------------
void traite_assignation(const int idx_com, const int idx_spe, Commandes* rep_com, Travailleurs* rep_trav) { // assigne a un travilleur la tache d'index idx_spe de la commande d'index idx_com
	Booleen suivant = FAUX; // on declare un booleen local qu'on initilalise a FAUX
	int affecter = -1, requis, effectuees, diff; // declaration de 3 varibles de simplification (requis, effectues, diff) ainsi que affecter qui indique le travailleur a qui affecter la tache, initialise a -1
	for (unsigned int i = 0; i < rep_trav->nb_travailleurs; i++) { // parcours des travailleurs
		if (rep_trav->tab_travailleurs[i].tags_competences[idx_spe] == VRAI) { //on verifie si le travailleur i est competent pour la specialite d'index idx_spe
			if (suivant) {// on verifie si suivant vaut VRAI
				if (rep_trav->tab_travailleurs[i].nb_heures_travail < rep_trav->tab_travailleurs[affecter].nb_heures_travail) { //si le travailleur d'index i a moins d'heure de travail que le travailleur d'indice affecter:
					affecter = i; // travailler passe a i
				}
			}
			else {// si suivant vaut FAUX
				affecter = i; // affecter vaut i
				suivant = VRAI; // suivant passe a VRAI
			}
		}
	}
	assert(affecter >= 0); // on verifie si affecter est positif, sinon on interromp le programme
	requis = rep_com->tab_commandes[idx_com].taches_par_specialite[idx_spe].nb_heures_requises;// variable de simplifiacattion d'ecriture (nb d'heures requise pour la tache a assigner)
	effectuees = rep_com->tab_commandes[idx_com].taches_par_specialite[idx_spe].nb_heures_effectuees;// variable de simplifiacattion d'ecriture (nb d'heures requise pour la tache a assigner)
	diff = requis - effectuees;// variable de simplifiacattion d'ecriture (nb d'heure restantes a effectuer)
	rep_com->tab_commandes[idx_com].idx_trav_tache[idx_spe] = affecter; // on affecte a la tache le travailleur selectionne precedemment
	rep_trav->tab_travailleurs[affecter].nb_heures_travail += diff; // on ajoute au travailleur en charge de la tache le nombre d'heure a effectuer pour la terminer
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
		printf("\n");// retour a la ligne
		exit(0); // sortie du programme avec le code 0 indiquant une absence d'erreur
	}
}

// interruption ------------------------ 
void traite_interruption() {
	printf(MSG_INTERRUPTION); // affichage du message MSG_INTERRUPTION
}// fin dde traite_interrupton