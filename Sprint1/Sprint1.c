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
	scanf("%s", id); // l'utilisateur entre le mot souhaité qui est affecté a la variable id
	if (EchoActif) printf(">>echo %s\n", id); // si le mode débuggage est activé on affiche le mot entre precedemment
}
int get_int() { // fonction de récupération d'un entier positif
	char buffer[NBCHIFFREMAX + 1]; // on definie un mot de longueur de la taille maximum d'un chiffre (+ le "\0")
	scanf("%s", buffer); // l'utilisateur entre le nombre souhaité qui est affecté a la variable buffer
	if (EchoActif) printf(">>echo %s\n", buffer);// si le mode débuggage est activé on affiche le mot entre precedemment
	return atoi(buffer); //on renvoie le mot convertit en entier
}

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

// Client ------------------------------
void traite_client() {
	Mot nom_client; // declaration de la variable de type mot qui contiendra le nom du client
	get_id(nom_client); // recuperation du nom du client aupres de l'utilisateur
	if (strcmp(nom_client, "tous") == 0) { // si tous es entré on affiche tous les client
		printf(MSG_TOUS_CLIENT); // affiche le message MSG_CLIENT_TOUS
	}
	else { // sinon on affiche uniquement le client indique par l'utilisateur
		printf(MSG_CLIENT, nom_client);// affichage de MSG_CLIENT avec comme arguement le nom du client
	}
}

// Travailleurs ------------------------
void traite_travailleurs() {
	Mot nom_specialite;// declaration de la variable de type mot qui contiendra le nom de la specialite
	get_id(nom_specialite);// recuperation du nom de la specialite aupres de l'utilisateur
	if (strcmp(nom_specialite, "tous") == 0) {// si tous es entré on affiche tous les travailleurs
		printf(MSG_TOUS_TRAVAILLEURS); // affichage de MSG_TOUS_TRAVAILLEURS
	}
	else { // sinon on affiche uniquement le client indique par l'utilisateur
		printf(MSG_TRAVAILLEURS, nom_specialite);// affichage de MSG_TRAVAILLEURS avec comme arguement le nom de la specialite
	}
}

// Specialités -------------------------
void traite_specialites() {
	printf(MSG_SPECIALITES); // affichage de MSG_SPECIALITES
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
void traite_demarche() {
	Mot nom_client; // declaration de la variable nom_client
	get_id(nom_client); // recuperation aupres de l'utilisateur de la valeur de nom_client
	printf(MSG_DEMARCHE, nom_client);// affichage de MSG_DEMARCHE avec comme argument nom_client
}

// Embauche ----------------------------
void traite_embauche() {
	Mot nom_travailleur, nom_specialite; // declaration des variables de type mot nom_travailleur et nom_specialite 
	get_id(nom_travailleur); // recuperation aupres de l'utilisateur de nom_travailleur
	get_id(nom_specialite); // recuperation aupres de l'utilisateur de nom_specialite
	printf(MSG_EMBAUCHE, nom_travailleur, nom_specialite); // affichage MSG_EMBAUCHE avec en paramtre nom_travailleur et nom_specialite 
}

// developpe --------------------------- 
void traite_developpe() {
	Mot nom_specialite; // declaration de la variable de type mot nom_specialite
	get_id(nom_specialite); // recuperation aupres de l'utilisateur de nom_specialite
	int cout_horaire = get_int(); // declaration d'une variable de type int et recuperation aupres de l'utilisateur du cout horaire de la specialite
	printf(MSG_DEVELOPPE, nom_specialite, cout_horaire); // affiche MSG_DEVELOPPE avec en parametres nom_specialite et cout_horaire
}

// interruption ------------------------ 
void traite_interruption() {
	printf(MSG_INTERRUPTION); // affiche MSG_INTERRUPTION
}

//Boucle principale ---------------------------------------------------------- 
int main(int argc, char* argv[]) {
	if (argc >= 2 && strcmp("echo", argv[1]) == 0) { // on verifie si le mot echo a ete mis en parametre du programme lors de l'execution en console
		EchoActif = VRAI; // activation du mode debuggage
	}
	// declaration des variables utilisees dans le main
	Mot buffer;
	Booleen progression = FAUX; // permet de verifier si la derniere valeur de "buffer" etait on non "progression"
	while (VRAI) { // boucle infinie
		get_id(buffer); // on demande la commande a entrer a l'utilisateur
		if (progression==VRAI && strcmp(buffer, "passe") == 0) { // on verifie que l'utilisateur a entré "passe" et que progression vaut VRAI
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
		if (strcmp(buffer, "charge") == 0) {
			traite_charge(); // on execute la fonction traite_charge
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "supervision") == 0) {
			traite_supervision(); // on execute la fonction traite_supervision
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "client") == 0) {
			traite_client();// on execute la fonction traite_client
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "travailleurs") == 0) {
			traite_travailleurs(); // on execute la fonction traite_travailleurs
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "specialites") == 0) {
			traite_specialites(); // on execute la fonction traite_specialites
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "progression") == 0) {
			traite_progression(); // on execute la fonction traite_proression
			progression = VRAI; // passage de progression a VRAI
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}

		if (strcmp(buffer, "tache") == 0) {
			traite_tache(); // on execute la fonction traite_tache
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "demarche") == 0) {
			traite_demarche(); // on execute la fonction traite_demarche
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "embauche") == 0) {
			traite_embauche(); // on execute la fonction traite_embauche
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "developpe") == 0) {
			traite_developpe(); // on execute la fonction traite_developpe
			continue; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		if (strcmp(buffer, "interruption") == 0) {
			traite_interruption(); // on execute la fonction traite_interruption
			break; // on saute les instructions suivante et on recommence a début de la boucle while
		}
		printf("!!! instruction inconnue >%s< !!!\n", buffer); // si aucune commande n'as ete reconnue on affiche ce message
	}
	return 0;// en cas de sortie de la boucle infinie on met fin au programme
}
