#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <netdb.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include "device.h"
#include "tools.h"
#include "ecc_key_generate.h"



/**
 * BDEVICE_INIT
 * Initie une structure butterfly device.
 */
bdevice* bdevice_init()
{
    bdevice *bd = NULL;
    
    bd = malloc( 1 * sizeof(bdevice) );
    
    if(bd==NULL){
        return NULL;
    }
    memset( bd, 0, sizeof(bdevice));
    
    return bd;
}


/**
 * BDEVICE_DESTROY
 * Supprime de façon sécurisée 
 * une structure butterfly device.
 */
void bdevice_destroy(bdevice **bd)
{   
    if(*bd==NULL){
        return;
    }
    
    /**
     * On réécris toute la mémoire contenant
     * des informations sensibles
     */
    unlink( (*bd)->f1_public_key_filename );
    erase_string(& ( (*bd)->f1_public_key_filename) );
    unlink( (*bd)->f1_private_key_filename );
    erase_string(& ( (*bd)->f1_private_key_filename) );
    unlink( (*bd)->f2_public_key_filename );
    erase_string(& ( (*bd)->f2_public_key_filename) );
    unlink( (*bd)->f2_private_key_filename );
    erase_string(& ( (*bd)->f2_private_key_filename) );
    
    erase_string(& ( (*bd)->f1_public_key) );
    erase_string(& ( (*bd)->f1_private_key) );
    erase_string(& ( (*bd)->f2_public_key) );
    erase_string(& ( (*bd)->f2_private_key) );
    free((*bd)->srv_ip);
    free((*bd)->srv_port);
    memset( *bd, 0, sizeof(bdevice));
    
    free(*bd);
}


/**
 * BDEVICE_CREATE_AES_KEY
 * Crée 2 clées AES @ek et @ck et les place dans la structure @bdevice
 *
 * Valeurs de retours
 * 1 -> @SUCCESS
 * 0 -> @FAILURE
 */
int bdevice_create_aes_keys(bdevice *bd)
{
    int ret;
    ret = RAND_bytes(bd->ek, 128);
    
    if (ret != 1){
        fprintf(stderr,"Erreur de generation d'un nombre aléatoire.");
        return 0;
    }
    
    ret = RAND_bytes(bd->ck, 128);
    
    if (ret != 1){
        fprintf(stderr,"Erreur de generation d'un nombre aléatoire.");
        return 0;
    }
    return 1;
    
}


/**
 * BDEVICE_CREATE_ECC_KEY
 * Crée une paire de clés ECC.
 */
int bdevice_create_ecc_keys(bdevice *bd)
{
    int ret1, ret2;
    
    ret1 = ecc_key_generate(bd, F1);
    if(ret1 != 0) return -1;
    ret1 = read_ecc_key_file(bd, PUBLIC, F1);
    ret2 = read_ecc_key_file(bd, PRIVATE, F1);
    
    if ( (ret1!=0) || (ret2!=0)){
        return -1;
    }
    
    ret1 = ecc_key_generate(bd, F2);
    if(ret1 != 0) return -1;
    ret1 = read_ecc_key_file(bd, PUBLIC, F2);
    ret2 = read_ecc_key_file(bd, PRIVATE, F2);
    
    if ( (ret1!=0) || (ret2!=0)){
        return -1;
    }
    
    return bdevice_erase_files(bd);
}


/**
 * BDEVICE_ERASE_FILES
 * Supprime tous les fichiers contenant les clées ECC.
 *
 * Erreurs lors de la suppresion des fichiers
 *  -1 -> f1_public_key_filename
 *  -2 -> f1_private_key_filename
 *  -3 -> f2_public_key_filename
 *  -4 -> f2_private_key_filename
 */
int bdevice_erase_files(bdevice *bd)
{
    
    if( erase_file(bd->f1_public_key_filename) != 0){
        return -1;
    }
    
    if( erase_file(bd->f1_private_key_filename) != 0){
        return -2;
    }
    
    if( erase_file(bd->f2_public_key_filename) != 0){
        return -3;
    }
    
    if( erase_file(bd->f2_private_key_filename) != 0){
        return -4;
    }
    return 0;
}


/**
 * BDEVICE_GET_FILENAME
 * Récupere le nom du fichier en fonction du
 *  @type: parmis PUBLIC, PRIVATE
 *  @number: 0 si f1, autre pour f2
 */
char * bdevice_get_filename(bdevice *bd, int type, int number)
{
    if ( type == PUBLIC ){
        if (number == 0){
            return bd->f1_public_key_filename;
        }
        return bd->f2_public_key_filename;
    }
    
    if (number == 0){
        return bd->f1_private_key_filename;
    }
    
    return bd->f2_private_key_filename;
}


/**
 * BDEVICE_GET_ECC_KEY
 * Récupere une clé (publique ou privée) en fonction du
 *  @type: parmis PUBLIC, PRIVATE
 *  @number: 0 si f1, autre pour f2
 */
char ** bdevice_get_ecc_key(bdevice *bd, int type, int number)
{
    if ( type == PUBLIC ){
        if (number == 0){
            return &(bd->f1_public_key);
        }
        return &(bd->f2_public_key);
    }
    
    if (number == 0){
        return &( bd->f1_private_key);
    }
    
    return &(bd->f2_private_key);
}



int bdevice_connect_to_server (bdevice *bd)
{
	int    sock;
	char   buffer [LG_BUFFER];
	int    nb_lus;
	
	struct addrinfo *results;
	
	if (bdevice_build_addrinfo(bd, &results) < 0)
	{
	    return -1;
	}


	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		fprintf(stderr,"Erreur de création du socket: %s.",
		        strerror(errno));
		return -1;
	}
	if (connect(sock, results->ai_addr, results->ai_addrlen) < 0) {
		fprintf(stderr,"Erreur de connection: %s.",
		        strerror(errno));
		return -1;
	}
	freeaddrinfo(results);
	
	
	while (1) {
		if (fgets(buffer, 256, stdin) == NULL)
			break;
		if (buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = '\0';
		if (write(sock, buffer, strlen(buffer)) < 0) {
			perror("write");
			break;
		}
		if ((nb_lus = read(sock, buffer, LG_BUFFER)) == 0)
			break;
		if (nb_lus < 0) {
			perror("read");
			break;
		}
		fprintf(stdout, "%s\n", buffer);
	}
	return EXIT_SUCCESS;
}


int bdevice_build_addrinfo(bdevice *bd, struct addrinfo **results)
{
	int    err;	
	struct addrinfo  hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if ((err = getaddrinfo(bd->srv_ip, bd->srv_port, 
	        &hints, results)) != 0) 
	{
		fprintf(stderr, "Erreur avec getaddrinfo: %s\n", 
		        gai_strerror(err));
		return -1;
	}
	return 0;
}

