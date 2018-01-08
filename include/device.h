#ifndef DEVICE_H
#define DEVICE_H


#include <netdb.h>


#define F1 0
#define F2 1
#define PUBLIC 0
#define PRIVATE 1

#define LG_BUFFER	1024

typedef struct BUTTERFLY_DEVICE bdevice;

struct BUTTERFLY_DEVICE {
    /** 
     * clé AES de 128 bytes pour le chiffrement
     */ 
    unsigned char   ek[128];
    
    /** 
     * clé AES de 128 bits pour la signature
     */ 
    unsigned char   ck[128];
    
    /**
     * Fichiers d'enregistrement des clés ECC 
     */
    char            *f1_public_key_filename;
    char            *f1_private_key_filename;
    char            *f2_public_key_filename;
    char            *f2_private_key_filename;
    
    /**
     * Variables contenant les différentes clées
     */
    char            *f1_public_key;
    char            *f1_private_key;
    char            *f2_public_key;
    char            *f2_private_key;
    
    /**
     * Variables de connection au serveur de clés
     */
    char            *srv_ip;
    char            *srv_port;    
};

bdevice* bdevice_init();
void bdevice_destroy(bdevice **d);
int bdevice_create_aes_keys(bdevice *bd);
int bdevice_create_ecc_keys(bdevice *bd);

int bdevice_erase_files(bdevice *bd);

char * bdevice_get_filename(bdevice *bd, int type, int number);
char ** bdevice_get_ecc_key(bdevice *bd, int type, int number);

int bdevice_connect_to_server (bdevice *bd);
int bdevice_build_addrinfo(bdevice *bd, struct addrinfo **results);

#endif
