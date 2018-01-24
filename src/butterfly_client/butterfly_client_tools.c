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
#include "sql.h"
#include "tools.h"



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
int bdevice_erase_files(t_bdevice *bd)
{
    
	if( erase_file(bd->f1_public_key_filename) != 0)
		return (-1);
    
	if( erase_file(bd->f1_private_key_filename) != 0)
		return (-2);
    
	if( erase_file(bd->f2_public_key_filename) != 0)
		return (-3);
    
	if( erase_file(bd->f2_private_key_filename) != 0)
		return (-4);

	return (0);
}


/**
 * BDEVICE_GET_FILENAME
 * Récupere le nom du fichier en fonction du
 *  @type: parmis PUBLIC, PRIVATE
 *  @number: 0 si f1, autre pour f2
 */
char * bdevice_get_filename(t_bdevice *bd, int type, int number)
{
	if ( type == SQL_ECC_TYPE_PUBLIC)
	{
		if (number == 0)
			return bd->f1_public_key_filename;
            
		return bd->f2_public_key_filename;
    }
    
	if (number == 0)
		return bd->f1_private_key_filename;
    
	return bd->f2_private_key_filename;
}


/**
 * BDEVICE_GET_ECC_KEY
 * Récupere une clé (publique ou privée) en fonction du
 *  @type: parmis PUBLIC, PRIVATE
 *  @number: 0 si f1, autre pour f2
 */
int bdevice_get_ecc_key(t_bdevice *bd, int type, int number, 
				unsigned char **return_char)
{
    return sql_get_ecc_key(bd->db, type, number, return_char);
}

