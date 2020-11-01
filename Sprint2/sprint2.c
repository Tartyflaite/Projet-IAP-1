#pragma warning(disable:4996) // on désactive l'avertissement sur les scanf
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#define MSG_CLIENT_ID_COMMANDE ""
#define MSG_SUPERVISION "## consultation de l'avancement des commandes\n"
#define MSG_CHARGE "## consultation de la charge de travail de \"%s\"\n"
#define MSG_COMMANDE "## nouvelle commande \"%s\", par client \"%s\"\n"
// Lexemes -------------------------------------------------------------------- 
#define LGMOT 35 //longueur d'un mot (35 caracteres)
#define NBCHIFFREMAX 5  // taille d'un nombre (5 chiffres maximums)
typedef char Mot[LGMOT + 1]; // définition du type mot
void get_id(Mot id) { //fonction de récupréation d'un mot
	scanf("%s", id); // l'utilisateur entre le mot souhaité qui est affecté a la variable id
	if (EchoActif) printf(">>echo %s\n", id); // si le mode débuggage est activé on affiche le mot entre precedemment
}
int get_int() { // fonction de récupération d'un entier positif
	char buffer[NBCHIFFREMAX + 1]; // on definie un mot de longueur de la taille maximum d'un chiffre (+ le "\0")
	scanf("%s", buffer); // l'utilisateur entre le nombre souhaité qui est affecté a la variable buffer
	if (EchoActif) printf(">>echo %s\n", buffer); // si le mode débuggage est activé on affiche le mot entre precedemment
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

// Instructions --------------------------------------------------------------- 

//commande -----------------------------
void traite_commande() {
	Mot nom_commande, nom_client;// on declare deux variables de type mot 
	get_id(nom_commande);// on recupere aupres de l'utilisateur le nom de la commnde
	get_id(nom_client);// on recupere aupres de l'utilisateur le nom du client
	printf(MSG_COMMANDE, nom_commande, nom_client); // affichage de MSG_COMMANDE avec nom_commande et nom_lient comma argument
}

// Charge ------------------------------
void traite_charge() {
	Mot nom_travailleur;// declaration de la variable de type mot qui contiendra le nom du travailleur
	get_id(nom_travailleur); // recuperation du nom du travailleur aupres de l'utilisateur
	printf(MSG_CHARGE, nom_travailleur); // affichage du message MSG_CHARGE avec nom_travailleur en argument
}

// Supervision -------------------------
void traite_supervision() {
	printf(MSG_SUPERVISION); // affiche le message MSG_SUPERVISION
}

// Affichage Client --------------------
void affiche_clients(const Clients* rep_cli, int i){
	Booleen suivant = FAUX; // on declare un booleen local qu'on initilalise a FAUX
	printf(MSG_CLIENT, rep_cli->tab_clients[i]); // affichage de MSG_CLIENT avec comme arguement le nom du client d'index i
	printf(MSG_CLIENT_ID_COMMANDE); //affichage de MSG_CLIENT_ID_COMMANDE
	printf("\n"); //retour a la ligne
	return; // interruption de la fonction
}

// Client ------------------------------
void traite_client(const Clients* rep_cli) {
	Mot nom_client; // declaration de la variable de type mot qui contiendra le nom du client
	get_id(nom_client); // recuperation du nom du client aupres de l'utilisateur
	unsigned int i = 0; // initialisation du compteur i a 0
	Booleen suivant = FAUX; // declaration d'un booleen local initilalise a FAUX
	if (strcmp(nom_client, "tous") == 0) { // si tous es entré on affiche tous les clients
		while (i < rep_cli->nb_clients) { //parcours de l'ensemble des clients
			affiche_clients(rep_cli, i); // affichage du client d'index i et de ses commandes
			i++; // incrementation de i
		}
	}
	else { // sinon on affiche uniquement le client indique par l'utilisateur
		while (i < rep_cli->nb_clients) { // parcourt de l'ensemble des clients
			if (strcmp(nom_client, rep_cli->tab_clients[i]) == 0) { // on verifie si le clients resneigne est le meme que celui d'index i
				affiche_clients(rep_cli, i); // affichage du client d'index i et de ses commandes
				return; // interruption de la fonction
			}
			i++; // incrementation de i
		}
		printf(MSG_CLIENT_ERREUR); // affichage d'un message d'erreur si le client renseigne n'est pas enregistre
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


// Specialités -------------------------
void traite_specialites(const Specialites* rep_specialites) {
	printf(MSG_SPECIALITES);
	if (rep_specialites->nb_specialites == 0) {
		printf("\n");
		return;
	}
	for (int i = 0; i < rep_specialites->nb_specialites;i++) {
		printf("%s/%d",rep_specialites->tab_specialites[i].nom, rep_specialites->tab_specialites[i].cout_horaire);
		if (i != rep_specialites->nb_specialites - 1)printf(", ");
	}
	printf("\n");
}

// Progression -------------------------
void traite_progression() {
	Mot nom_commade, nom_specialite; // declaration de deux variables de type mot qui contiendront respectivene le nom de la commande et le nom de la specialite
	get_id(nom_commade); // recuperation du nom de la commande aupres de l'utilisateur
	get_id(nom_specialite); // recuperation du nom de la specialite aupres de l'utilisateur
	int nbr_heure = get_int(); // declaration d'une variable de type int et recuperation aupres de l'utilisateur du nombred'heure de la tache a effectuer
	printf(MSG_PROGRESSION, nom_commade, nom_specialite, nbr_heure); // affichage de MSG_PROGRESSION avec nom de la commande et de la specialite ainsi que le nombre d'heure
}

// Passe -------------------------------
void traite_passe() {
	printf(MSG_PASSE); // affiche MSG_PASSE
}

// Tâches ------------------------------
void traite_tache() {
	Mot nom_commande, nom_specialite;  // declaration de deux variables de type mot qui contiendront respectivene le nom de la commande et le nom de la specialite
	get_id(nom_commande); // recuperation du nom de la commande aupres de l'utilisateur
	get_id(nom_specialite); // recuperation du nom de la specialite aupres de l'utilisateur
	int nbr_heure = get_int(); // declaration d'une variable de type int et recuperation aupres de l'utilisateur du nombred'heure de la tache a effectuer
	printf(MSG_TACHE, nom_commande, nom_specialite, nbr_heure); // affiche MSG_TACHE avec nom_commande, nom_specialite et nbr_heure en arguments
}

// Demarche ------------------------------
void traite_demarche(Clients* rep_cli) {
	get_id(rep_cli->tab_clients[rep_cli->nb_clients]); // on recupere aupres de l'utilisateur le nom du client a enregister et on le met dans le premier emplacement disponible du repertoire des clients
	rep_cli->nb_clients += 1; // on incremente le nombre de clients enregistrees de 1
}

// Embauche ----------------------------
void traite_embauche(Travailleurs* rep_trav, const Specialites* rep_spe) {
	Mot nom_specialite; // on cree une variable de type mot qui contriendra le nom de la specialite
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
	rep_trav->tab_travailleurs[rep_trav->nb_travailleurs] = travailleur; // on affecte la valeur travailleur au premier emplacement disponible du repertoire des travailleurs
	rep_trav->nb_travailleurs += 1; // on incremente le nombre de travailleurs enregistrees de 1
}

// developpe --------------------------- 
void traite_developpe(Specialites* rep_spe) {
	Specialite spe; // on crée une variable de type specialite
	get_id(spe.nom);// on recupere le nom de la specialite aupres de l'utilisateur
	spe.cout_horaire = get_int(); // on recupere le cout horaire de la specialite aupres de l'utilisateur
	rep_spe->tab_specialites[rep_spe->nb_specialites] = spe; // on affecte la valeur spe au premier emplacement disponible du repertoire de specialite
	rep_spe->nb_specialites += 1; // on incremente le nombre de specialites enregistrees de 1
}

// interruption ------------------------ 
void traite_interruption() {
	printf(MSG_INTERRUPTION); // affiche MSG_INTERRUPTION
}

//Boucle principale ---------------------------------------------------------- 
int main(int argc, char* argv[]) {
	if (argc >= 2 && strcmp("echo", argv[1]) == 0) {
		EchoActif = VRAI;
	}
	Mot buffer;
	Booleen progression = FAUX; // permet de verifier si la derniere valeur de "buffer" etait on non "progression"
	Specialites rep_specialites; // repertoire des specialites
	Travailleurs rep_travailleurs; // repertoire des travailleurs
	Clients rep_clients; // repertoire des clients
	rep_clients.nb_clients = 0; // on initialise le nombre de
	rep_specialites.nb_specialites = 0; // on initialise le nombre de specialites enregistrees
	rep_travailleurs.nb_travailleurs = 0; // on initialise le nombre de travailleurs engages
	while (VRAI) { // boucle infinie
		get_id(buffer); // on demande la commande a entrer a l'utilisateur
		if (progression == VRAI && strcmp(buffer, "passe") == 0) { // on verifie que l'utilisateur a entré "passe" et que progression vaut VRAI
			traite_passe(); // on execute la fonction traite_passe
			progression = FAUX; // on passe progression a FAUX
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		else {
			progression = FAUX; // on passe progression a la valeur FAUX
		}
		if (strcmp(buffer, "commande") == 0) { // on verifie si l'utilisateur a entre "commande" dans buffer
			traite_commande(); // on execute la fonction traite_commande
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "charge") == 0) { // on verifie si l'utilisateur a entre "charge" dans buffer
			traite_charge(); // on execute la fonction traite_charge
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "supervision") == 0) { // on verifie si l'utilisateur a entre "supervision" dans buffer
			traite_supervision(); // on execute la fonction traite_supervision
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "client") == 0) { // on verifie si l'utilisateur a entre "client" dans buffer
			traite_client(&rep_clients); // on execute la fonction traite_client
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "travailleurs") == 0) { // on verifie si l'utilisateur a entre "travailleurs" dans buffer
			traite_travailleurs(&rep_specialites, &rep_travailleurs); // on execute la fonction traite_travailleurs
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "specialites") == 0) { // on verifie si l'utilisateur a entre "specialite" dans buffer
			traite_specialites(&rep_specialites); // on execute la fonction traite_specialites
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "progression") == 0) { // on verifie si l'utilisateur a entre "progression" dans buffer
			traite_progression(); // on execute la fonction traite_proression
			progression = VRAI; // passage de progression a VRAI
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "tache") == 0) { // on verifie si l'utilisateur a entre "tache" dans buffer
			traite_tache(); // on execute la fonction traite_tache
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "demarche") == 0) { // on verifie si l'utilisateur a entre "demarche" dans buffer
			traite_demarche(&rep_clients); // on execute la fonction traite_demarche
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "embauche") == 0) { // on verifie si l'utilisateur a entre "embauche" dans buffer
			traite_embauche(&rep_travailleurs, &rep_specialites); // on execute la fonction traite_embauche
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "developpe") == 0) { // on verifie si l'utilisateur a entre "developpe" dans buffer
			traite_developpe(&rep_specialites); // on execute la fonction traite_developpe
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "interruption") == 0) { // on verifie si l'utilisateur a entre "interruption" dans buffer
			traite_interruption(); // on execute la fonction traite_interruption
			break; // on quitte la boucle while
		}
		printf("!!! instruction inconnue >%s< !!!\n", buffer); // si aucune commande n'as ete reconnue on affiche ce message
	}
	return 0;// en cas de sortie de la boucle infinie on met fin au programme
}