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
#include "sql.h"



/**
 * BDEVICE_INIT
 * Initie une structure butterfly device.
 */
t_bdevice* bdevice_init()
{
	t_bdevice *bd = NULL;
    
    bd = malloc( 1 * sizeof(t_bdevice) );
    
	if( !bd )
		return (NULL);
	
	memset( bd, 0, sizeof(t_bdevice));    
    return (bd);
}


/**
 * BDEVICE_INIT_DATABASE
 * Initie la base de donnée en créant le fichier @butterfly_database
 * et en créant la structure de la base de donnée si besoin.
 *
 * Valeurs de retour
 *  0: SUCCESS
 * -1: erreur
 */
int bdevice_init_database(t_bdevice *bd)
{
	int return_code;
    
	return_code = sql_open_database(&(bd->db), "butterfly_database");
    
	if(return_code != 0)
        return (-1);

	sql_client_create_tables(bd->db);

	if(return_code != 0)
		return (-1);
    
	return (0);
}


/**
 * BDEVICE_DESTROY
 * Supprime de façon sécurisée 
 * une structure butterfly device.
 */
void bdevice_destroy(t_bdevice **bd)
{   
	if( ! *bd)
        return;
    
	/**
	 * On réécris toute la mémoire contenant
	 * des informations sensibles
	 */
	
	unlink( (*bd)->f1_public_key_filename );
	free( (*bd)->f1_public_key_filename );
	unlink( (*bd)->f1_private_key_filename );
	free( (*bd)->f1_private_key_filename );
	unlink( (*bd)->f2_public_key_filename );
	free( (*bd)->f2_public_key_filename );
	unlink( (*bd)->f2_private_key_filename );
	free((*bd)->f2_private_key_filename);
    
	free((*bd)->srv_ip);
	free((*bd)->srv_port);
	sqlite3_close((*bd)->db);
	memset( *bd, 0, sizeof(t_bdevice));
    
	free(*bd);
}


/**
 * BDEVICE_CREATE_AES_KEY
 * Crée 2 clées AES @ek et @ck et les écris dans la base de donnée
 *
 * Valeurs de retours
 *  0 -> @SUCCESS
 * -1 -> @FAILURE
 */
int bdevice_create_aes_keys(t_bdevice *bd)
{
	int return_code;
	unsigned char aes[129];
    
    while(1)
    {
    	return_code = RAND_bytes(aes, 16);
	
    	if (return_code != 1)
    	{
			fprintf(stderr,"Erreur de generation d'un nombre \
			 aléatoire.");
			return (-1);
    	}
    
		return_code = sql_write_aes_key(bd->db, SQL_AES_TYPE_EK, 
						aes, 16);
		
		if(return_code == 16 )
			break;
    
    }
    
    while(1)
    {
		return_code = RAND_bytes(aes, 16);
    
		if (return_code != 1)
		{
			fprintf(stderr,"Erreur de generation d'un nombre \
			 aléatoire.");
			return (-1);
    	}
    	
    	return_code = sql_write_aes_key(bd->db, SQL_AES_TYPE_CK,
    								 aes,16);
    	if(return_code == 16 )
			break;
    }
    return (0);
}


/**
 * BDEVICE_CREATE_ECC_KEY
 * Crée une paire de clés ECC et les écris dans la base de donnée.
 *
 * Valeurs de retour
 *	0 -> SUCCESS
 * <0 -> ERREUR
 */
int bdevice_create_ecc_keys(t_bdevice *bd)
{
    int return_code1, return_code2;
    
	return_code1 = ecc_key_generate(bd, SQL_ECC_F1);
	if(return_code1 != 0) 
		return -1;
	return_code1 = read_ecc_key_file(bd, SQL_ECC_TYPE_PUBLIC,
						 SQL_ECC_F1);
	return_code2 = read_ecc_key_file(bd, SQL_ECC_TYPE_PRIVATE,
	 SQL_ECC_F1);
    
	if ( (return_code1 != 0) || (return_code2 != 0))
        return -1;
    
	return_code1 = ecc_key_generate(bd, SQL_ECC_F2);
    
    if(return_code1 != 0) 
		return -1;

	return_code1 = read_ecc_key_file(bd, SQL_ECC_TYPE_PUBLIC,
											 SQL_ECC_F2);
	return_code2 = read_ecc_key_file(bd, SQL_ECC_TYPE_PRIVATE, 
							SQL_ECC_F2);
	
	if ( (return_code1 != 0) || (return_code2 != 0))
		return -1;

	return bdevice_erase_files(bd);
}

