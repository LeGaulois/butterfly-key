#ifndef DEVICE_H
#define DEVICE_H


#include <netdb.h>
#include <sqlite3.h> 



typedef struct s_butterfly_device t_bdevice;


struct s_butterfly_device {
    /**
     * Fichiers d'enregistrement des clés ECC 
     */
    char            *f1_public_key_filename;
    char            *f1_private_key_filename;
    char            *f2_public_key_filename;
    char            *f2_private_key_filename;
    
    /**
     * Variables de connection au serveur de clés
     */
    char            *srv_ip;
    char            *srv_port;    
    
    sqlite3         *db;
};


t_bdevice* bdevice_init();
int bdevice_init_database(t_bdevice *bd);
void bdevice_destroy(t_bdevice **d);
int bdevice_create_aes_keys(t_bdevice *bd);
int bdevice_create_ecc_keys(t_bdevice *bd);

int bdevice_erase_files(t_bdevice *bd);

char * bdevice_get_filename(t_bdevice *bd, int type, int number);
int bdevice_get_ecc_key(t_bdevice *bd, int type, int number, 
				unsigned char **return_char);


int bdevice_connect_to_server(t_bdevice *bd);
int bdevice_send_cert_query_to_server(t_bdevice *bd, int socket);
int bdevice_build_addrinfo(t_bdevice *bd, struct addrinfo **results);

#endif
