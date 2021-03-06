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
#define LGMOT 35 //longueur d'un mot (35 caracteres)
#define NBCHIFFREMAX 5  // taille d'un nombre (5 chiffres maximums)
typedef char Mot[LGMOT + 1]; // définition du type mot

void get_id(Mot id) { //fonction de récupréation d'un mot
	scanf("%s", id);
	if (EchoActif) printf(">>echo %s\n", id); // si le mode débuggage est activé on affiche le mot entre precedemment
}

int get_int() { // fonction de récupération d'un entier positif
	char buffer[NBCHIFFREMAX + 1];
	scanf("%s", buffer);
	if (EchoActif) printf(">>echo %s\n", buffer); // si le mode débuggage est activé on affiche le mot entre precedemment
	return atoi(buffer);
}

// Instructions --------------------------------------------------------------- 

//commande -----------------------------
void traite_commande() {
	Mot nom_commande, nom_client;
	get_id(nom_commande);// on recupere aupres de l'utilisateur le nom de la commnde
	get_id(nom_client);// on recupere aupres de l'utilisateur le nom du client
	printf(MSG_COMMANDE, nom_commande, nom_client);
}

// Charge ------------------------------
void traite_charge() {
	Mot nom_travailleur;
	get_id(nom_travailleur); // recuperation du nom du travailleur aupres de l'utilisateur
	printf(MSG_CHARGE, nom_travailleur);
}

// Supervision -------------------------
void traite_supervision() {
	printf(MSG_SUPERVISION);
}

// Client ------------------------------
void traite_client() {
	Mot nom_client;
	get_id(nom_client); // recuperation du nom du client aupres de l'utilisateur
	if (strcmp(nom_client, "tous") == 0) {
		printf(MSG_TOUS_CLIENT);
	}
	else { // sinon on affiche uniquement le client indique par l'utilisateur
		printf(MSG_CLIENT, nom_client);
	}
}

// Travailleurs ------------------------
void traite_travailleurs() {
	Mot nom_specialite;
	get_id(nom_specialite);// recuperation du nom de la specialite aupres de l'utilisateur
	if (strcmp(nom_specialite, "tous") == 0) {
		printf(MSG_TOUS_TRAVAILLEURS); 
	}
	else { // sinon on affiche uniquement le client indique par l'utilisateur
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
	get_id(nom_commade); // recuperation du nom de la commande aupres de l'utilisateur
	get_id(nom_specialite); // recuperation du nom de la specialite aupres de l'utilisateur
	int nbr_heure = get_int(); // drecuperation aupres de l'utilisateur du nombred'heure de la tache a effectuer
	printf(MSG_PROGRESSION, nom_commade, nom_specialite, nbr_heure);
}

// Passe -------------------------------
void traite_passe() {
	printf(MSG_PASSE);
}

// Tâches ------------------------------
void traite_tache() {
	Mot nom_commande, nom_specialite;
	get_id(nom_commande); // recuperation du nom de la commande aupres de l'utilisateur
	get_id(nom_specialite); // recuperation du nom de la specialite aupres de l'utilisateur
	int nbr_heure = get_int(); // recuperation aupres de l'utilisateur du nombred'heure de la tache a effectuer
	printf(MSG_TACHE, nom_commande, nom_specialite, nbr_heure);
}

// Demarche ------------------------------
void traite_demarche() {
	Mot nom_client;
	get_id(nom_client); // recuperation aupres de l'utilisateur de la valeur de nom_client
	printf(MSG_DEMARCHE, nom_client);
}

// Embauche ----------------------------
void traite_embauche() {
	Mot nom_travailleur, nom_specialite;
	get_id(nom_travailleur); // recuperation aupres de l'utilisateur de nom_travailleur
	get_id(nom_specialite); // recuperation aupres de l'utilisateur de nom_specialite
	printf(MSG_EMBAUCHE, nom_travailleur, nom_specialite);
}

// developpe --------------------------- 
void traite_developpe() {
	Mot nom_specialite;
	get_id(nom_specialite); // recuperation aupres de l'utilisateur de nom_specialite
	int cout_horaire = get_int(); //recuperation aupres de l'utilisateur du cout horaire de la specialite
	printf(MSG_DEVELOPPE, nom_specialite, cout_horaire);
}

// interruption ------------------------ 
void traite_interruption() {
	printf(MSG_INTERRUPTION);
}

//Boucle principale ---------------------------------------------------------- 
int main(int argc, char* argv[]) {
	if (argc >= 2 && strcmp("echo", argv[1]) == 0) { // on verifie si le mot echo a ete mis en parametre du programme lors de l'execution en console
		EchoActif = VRAI; // activation du mode debuggage
	}
	// declaration des variables utilisees dans le main
	Mot buffer;
	Booleen progression = FAUX; // permet de verifier si la derniere valeur de "buffer" etait on non "progression"
	while (VRAI) {
		get_id(buffer); // on demande la commande a entrer a l'utilisateur
		if (progression==VRAI && strcmp(buffer, "passe") == 0) {
			traite_passe(); // on execute la fonction traite_passe
			progression = FAUX;
			continue;
		}
		else {
			progression = FAUX;
		}
		if (strcmp(buffer, "commande") == 0) {
			traite_commande(); // on execute la fonction traite_commande
			continue;	
		}
		if (strcmp(buffer, "charge") == 0) {
			traite_charge(); // on execute la fonction traite_charge
			continue;
		}
		if (strcmp(buffer, "supervision") == 0) {
			traite_supervision(); // on execute la fonction traite_supervision
			continue;
		}
		if (strcmp(buffer, "client") == 0) {
			traite_client();// on execute la fonction traite_client
			continue;
		}
		if (strcmp(buffer, "travailleurs") == 0) {
			traite_travailleurs(); // on execute la fonction traite_travailleurs
			continue;
		}
		if (strcmp(buffer, "specialites") == 0) {
			traite_specialites(); // on execute la fonction traite_specialites
			continue;
		}
		if (strcmp(buffer, "progression") == 0) {
			traite_progression(); // on execute la fonction traite_proression
			progression = VRAI;
			continue;
		}

		if (strcmp(buffer, "tache") == 0) {
			traite_tache(); // on execute la fonction traite_tache
			continue;
		}
		if (strcmp(buffer, "demarche") == 0) {
			traite_demarche(); // on execute la fonction traite_demarche
			continue;
		}
		if (strcmp(buffer, "embauche") == 0) {
			traite_embauche(); // on execute la fonction traite_embauche
			continue;
		}
		if (strcmp(buffer, "developpe") == 0) {
			traite_developpe(); // on execute la fonction traite_developpe
			continue;
		}
		if (strcmp(buffer, "interruption") == 0) {
			traite_interruption(); // on execute la fonction traite_interruption
			break;
		}
		printf("!!! instruction inconnue >%s< !!!\n", buffer); // si aucune commande n'as ete reconnue on affiche ce message
	}
	return 0;
}
