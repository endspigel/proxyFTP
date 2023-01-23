#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/socket.h>
#include  <netdb.h>
#include  <string.h>
#include  <unistd.h>
#include  <stdbool.h>
#include "simpleSocketAPI.h"

#define SERVADDR "127.0.0.1"        // Définition de l'adresse IP d'écoute
#define SERVPORT "0"                // Définition du port d'écoute, si 0 port choisi dynamiquement
#define LISTENLEN 1                 // Taille de la file des demandes de connexion
#define MAXBUFFERLEN 1024           // Taille du tampon pour les échanges de données
#define MAXHOSTLEN 64               // Taille d'un nom de machine
#define MAXPORTLEN 64               // Taille d'un numéro de port

int main(){
    int ecode;                       // Code retour des fonctions
    char serverAddr[MAXHOSTLEN];     // Adresse du serveur
    char serverPort[MAXPORTLEN];     // Port du server
    int descSockRDV;                 // Descripteur de socket de rendez-vous
    int descSockCOM;                 // Descripteur de socket de communication
    int SockComServeur;               // Socket de connection au serveur
    int Client;                      // Descripteur pour les données du Client
    int Serv;                       // Descripteur pour les données du Serveur
    struct addrinfo hints;           // Contrôle la fonction getaddrinfo
    struct addrinfo *res;            // Contient le résultat de la fonction getaddrinfo
    struct sockaddr_storage myinfo;  // Informations sur la connexion de RDV
    struct sockaddr_storage from;    // Informations sur le client connecté
    socklen_t len;                   // Variable utilisée pour stocker les
                                     // longueurs des structures de socket
    char buffer[MAXBUFFERLEN];       // Tampon de communication entre le client et le serveur
    char user [MAXBUFFERLEN];       // Extraction de l'utilisateur
    char FTPServ [MAXBUFFERLEN];    // Extraction du serveurFTP
    int vt1, vt2, vt3, vt4, vt5, vt6;     //

    // Initialisation de la socket de RDV IPv4/TCP
    descSockRDV = socket(AF_INET, SOCK_STREAM, 0);
    if (descSockRDV == -1) {
         perror("Erreur création socket RDV\n");
         exit(2);
    }
    // Publication de la socket au niveau du système
    // Assignation d'une adresse IP et un numéro de port
    // Mise à zéro de hints
    memset(&hints, 0, sizeof(hints));
    // Initialisation de hints
    hints.ai_flags = AI_PASSIVE;      // mode serveur, nous allons utiliser la fonction bind
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_family = AF_INET;        // seules les adresses IPv4 seront présentées par
                                      // la fonction getaddrinfo

     // Récupération des informations du serveur
     ecode = getaddrinfo(SERVADDR, SERVPORT, &hints, &res);
     if (ecode) {
         fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(ecode));
         exit(1);
     }
     // Publication de la socket
     ecode = bind(descSockRDV, res->ai_addr, res->ai_addrlen);
     if (ecode == -1) {
         perror("Erreur liaison de la socket de RDV");
         exit(3);
     }
     // Nous n'avons plus besoin de cette liste chainée addrinfo
     freeaddrinfo(res);

     // Récuppération du nom de la machine et du numéro de port pour affichage à l'écran
     len=sizeof(struct sockaddr_storage);
     ecode=getsockname(descSockRDV, (struct sockaddr *) &myinfo, &len);
     if (ecode == -1)
     {
         perror("SERVEUR: getsockname");
         exit(4);
     }
     ecode = getnameinfo((struct sockaddr*)&myinfo, sizeof(myinfo), serverAddr,MAXHOSTLEN,
                         serverPort, MAXPORTLEN, NI_NUMERICHOST | NI_NUMERICSERV);
     if (ecode != 0) {
             fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(ecode));
             exit(4);
     }
     printf("L'adresse d'ecoute est: %s\n", serverAddr);
     printf("Le port d'ecoute est: %s\n", serverPort);

     // Definition de la taille du tampon contenant les demandes de connexion
     ecode = listen(descSockRDV, LISTENLEN);
     if (ecode == -1) {
         perror("Erreur initialisation buffer d'écoute");
         exit(5);
     }

    len = sizeof(struct sockaddr_storage);
     // Attente connexion du client
     // Lorsque demande de connexion, creation d'une socket de communication avec le client
     descSockCOM = accept(descSockRDV, (struct sockaddr *) &from, &len);
     if (descSockCOM == -1){
         perror("Erreur accept\n");
         exit(6);
     }

    void ftp_ls(int descSockCOM) {
    char buffer[MAXBUFFERLEN];
    send(descSockCOM, "LIST\n", 5, 0);
    recv(descSockCOM, buffer, MAXBUFFERLEN, 0);
    printf("%s\n", buffer);
  }

void ftp_cd(int descSockCOM, char* directory) {
  char buffer[MAXBUFFERLEN];
  sprintf(buffer, "CWD %s\n", directory);
  send(descSockCOM, buffer, strlen(buffer), 0);
  recv(descSockCOM, buffer, MAXBUFFERLEN, 0);
}
    // Echange de données avec le client connecté

    /*****
     * Testez de mettre 220 devant BLABLABLA ...
     * **/
    strcpy(buffer, "220\n");
    write(descSockCOM, buffer, strlen(buffer));
    /** A vous de continuer ! **/

    ecode = read(descSockCOM, buffer, MAXBUFFERLEN-1);
    if (ecode == -1) {
        perror("Problème de lecture1\n");
        exit(3);
    }
    buffer[ecode] = '\0';
    printf("%s\n", buffer);

    //Découpage de l'user et du serveur
    sscanf(buffer,"%[^@]@%s", user, FTPServ);
    printf("%s\n", user );
    printf("%s\n", FTPServ);

    //Création du Socket pour la connexion au serveur
    printf("----------Création du Socket pour la connexion au serveur----------\n");
    ecode = connect2Server (FTPServ,"21",&SockComServeur);
    if (ecode == -1) {
        perror("Problème de lecture1\n");
    }else {
        printf("Connection réussi\n" );
    }

    // Afficher le contenu du buffer dans la console
    printf("----------Afficher le contenu du buffer dans la console----------\n");
    ecode = read(SockComServeur, buffer, MAXBUFFERLEN);
    if (ecode == -1) {
        perror("Problème de lecture2\n");
        exit(3);
    }
    buffer[ecode] = '\0';
    printf("%s\n", buffer);

    //Envoi de l'user au serveur
    printf("----------Envoie de l'user au serveur----------\n");
    strcat(user, "\r\n");
    write(SockComServeur, user, strlen(user));
    if (ecode == -1) {
        perror("probleme d'ecriture1\n");
        exit(3);
    }

    //Message de demande du mot de passe au client
    printf("----------Message de demande du mot de passe au client---------- \n");
    ecode = read(SockComServeur, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1) {
        perror("probleme de lecture3\n");
        exit(3);
    }
    buffer[ecode] = '\0';
    printf("%s\n",buffer);

    //Lecture du mot de passe entree
    ecode = write(descSockCOM, buffer, strlen(buffer));
    if (ecode == -1) {
        perror("probleme d'ecriture2\n");
        exit(3);
    }

    //Transmission du mot de passe entree
    ecode = read(descSockCOM, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1) {
        perror("probleme de lecture4\n");
        exit(3);
    }
    buffer[ecode] = '\0';
    printf("%s\n", buffer);

    //Envoi du mot de passe au serveur
    printf("----------Envoie du mdp au serveur en cours----------\n");
    ecode = write(SockComServeur, buffer, strlen(buffer));
    if (ecode == -1) {
        perror("probleme d'ecriture3\n");
        exit(3);
    }

    //Reception de la confirmation de connexion du serveur
    printf("----------Réception de la confirmation de connexion au serveur----------\n");
    ecode = read(SockComServeur, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1) {
        perror("probleme de lecture5\n");
        exit(3);
    }
    buffer[ecode] = '\0';
    printf("%s\n",buffer);

    //Transmission du SYST au client
    printf("----------Transmission du SYST au client----------\n");
    ecode = write(descSockCOM, buffer, strlen(buffer));
    if (ecode == -1) {
        perror("probleme d'ecriture4\n");
        exit(3);
    }

    //Lire la commande systeme (SYST)
    printf("----------Lecture de la commande SYST----------\n");
    ecode = read(descSockCOM, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1) {
        perror("probleme de lecture6\n");
        exit(3);
    }
    buffer[ecode] = '\0';
    printf("%s\n",buffer);

    //Envoyer la commande systeme au client (SYST)
    printf("----------Envoie de la commande système au client----------\n");
    ecode = write(SockComServeur, buffer, strlen(buffer));
    if (ecode == -1) {
        perror("probleme d'ecriture5\n");
        exit(3);
    }

    //Reception des information systemes reçus par serveur
    ecode = read(SockComServeur, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1) {
        perror("probleme de lecture7\n");
        exit(3);
    }
    buffer[ecode] = '\0';
    printf("%s\n",buffer);

    //Envoi des informations systeme au client
    printf("----------Envoie des infos syst au client----------\n");
    ecode = write(descSockCOM, buffer, strlen(buffer));
    if (ecode == -1) {
        perror("probleme d'ecriture6\n");
        exit(3);
    }

    //Réception du PORT pour le client
    printf("----------Réception du port pour le client----------\n");
    ecode = read(descSockCOM, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1) {
        perror("probleme de lecture8\n");
        exit(3);
    }
    buffer[ecode]='\0';
    printf("%s\n", buffer);

    //Récupération de adresse et port pour création du client
    printf("----------Récupération des adresses et port pour création du client----------\n");
    sscanf(buffer, "PORT %d,%d,%d,%d,%d,%d", &vt1, &vt2, &vt3, &vt4, &vt5, &vt6);
    char AClient[MAXHOSTLEN];
    char PClient[MAXPORTLEN];
    sprintf(AClient, "%d.%d.%d.%d", vt1, vt2, vt3, vt4);
    sprintf(PClient, "%d", vt5 * 256 + vt6);

    //Création du client
    ecode = connect2Server (AClient, PClient, &Client);
    if (ecode == -1) {
        perror("Problème de lecture9\n");
    }else {
        printf("Connection réussi\n" );
    }

    //Passage du serveur en mode passif
    char passive[10] = "PASV\n";
    strncat(buffer, passive, strlen(passive));
    ecode = write(SockComServeur, buffer, strlen(buffer));
    if (ecode == -1) {
        perror("probleme d'ecriture7\n");
        exit(3);
    }
    ecode = read(SockComServeur, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1) {
        perror("probleme de lecture10\n");
        exit(3);
    }
    buffer[ecode] = '\0';
    printf("%s\n", buffer);

    //Récupération de adresse et du port pour le serveur
    vt1, vt2, vt3, vt4, vt5, vt6 = 0;
    sscanf(buffer, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)", &vt1, &vt2, &vt3, &vt4, &vt5, &vt6);
    char AServ[MAXHOSTLEN];
    char PServ[MAXPORTLEN];
    sprintf(AServ, "%d.%d.%d.%d", vt1, vt2, vt3, vt4);
    sprintf(PServ, "%d", vt5 * 256 + vt6);


    //Création du Socket données Serveur
    ecode = connect2Server (AServ, PServ, &Serv);
    if (ecode == -1) {
        perror("Problème de lecture11\n");
    }else {
        printf("Connection réussi\n" );
    }

    //Vérification connexion port
    char estConnecterP[30] = "200 connecté sur le bon port\n";
    strncat(buffer, estConnecterP, strlen(estConnecterP));

    //Envois réponse serveur au client
    ecode = write(descSockCOM, buffer, strlen(buffer));
    if (ecode == -1) {
        perror("probleme d'ecriture8\n");
        exit(3);
    }

    //Reception sur le client
    ecode = read(descSockCOM, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1) {
        perror("probleme de lecture12\n");
        exit(3);
    }
    buffer[ecode] = '\0';
    printf("%s\n", buffer);


    ecode = write(SockComServeur, buffer, strlen(buffer));
    if (ecode == -1) {
        perror("probleme d'ecriture9\n");
        exit(3);
    }

    // Reception de la reponse par le serveur
    ecode = read(SockComServeur, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1) {
        perror("probleme de lecture13\n");
        exit(3);
    }
    buffer[ecode] = '\0';
    printf("%s\n",buffer);

    //Envoi de la reponse serveur au client
    ecode = write(descSockCOM, buffer, strlen(buffer));
    if (ecode == -1) {
        perror("probleme de lecture14\n");
        exit(3);
    }

    //Reception de la réponse serveur
    ecode = read(Serv, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1) {
        perror("Probleme de lecture15\n");
        exit(3);
    }
    buffer[ecode] = '\0';
    printf("%s\n",buffer);

    //Boucle pour envoyer les données du serveur sur le client
    while (ecode != 0) {
        ecode = write(Client, buffer, strlen(buffer));
        ecode = read(Serv, buffer, MAXBUFFERLEN - 1);
        if (ecode == -1) {
            perror("probleme de lecture16\n");
            exit(3);
        }
    }

    close(Client);
    close(Serv);

    //Affichage commande ls
    ecode = read(SockComServeur, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1) {
        perror("probleme de lecture17\n");
        exit(3);
    }
    buffer[ecode] = '\0';
    printf("%s\n",buffer);

    //Transfert des données sur le client
    ecode = write(descSockCOM, buffer, strlen(buffer));

    //Commande pour la fermeture du client
    char Fermeture[10] = "quit\n";
    strncat(buffer, Fermeture, strlen(Fermeture));
    ecode = write(SockComServeur, buffer, strlen(buffer));

    //Reception des données sur le serveur
    ecode = read(SockComServeur, buffer, MAXBUFFERLEN - 1);
    if (ecode == -1) {
        perror("probleme de lecture18\n");
        exit(3);
    }
    buffer[ecode] = '\0';
    printf("%s\n",buffer);

    //Transfert des données sur le client
    ecode = write(descSockCOM, buffer, strlen(buffer));

    //Fermeture de la connexion
    close(descSockCOM);
    close(descSockRDV);
}
