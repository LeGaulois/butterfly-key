#ifndef BSERVER_H
#define BSERVER_H

typedef struct bserver bserver;

struct bserver {
    int     socket;
    char    *port;
    char    *ip;
};

void traite_connexion (int sock);
int quitter_le_serveur (void);
int serveur_tcp (bserver *bs);
bserver* bserver_init();

#endif
