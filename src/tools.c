#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "device.h"



/**
 * ERASE_STRING
 * Supprime un string en écrivant des 0 
 * au préalable. 
 */
void erase_string(char **str)
{
    if(! *str){
        return;
    }
    memset(*str, 0, strlen(*str) );
    free(*str);   
}


/**
 * ERASE_FILE
 * Supprime un fichier de façon sécurisé en écrivant des "0"
 * dans tout le fichier.
 *
 * Valeurs de retour
 *  0 -> SUCCESS
 * -1 -> ERREUR
 */
int erase_file(char *filename)
{
    int fd;
    struct stat status;
    
    fd = open(filename, O_WRONLY);
    
    if(fd<0){
        return -1;
    }
    
    if ( fstat(fd, &status)<0 ){
        return -1;
    }
    
    if( write(fd, "0", status.st_size) != status.st_size){
        close(fd);
        return -1;
    }
    
    close(fd);
    return 0;   
}


/**
 * READ_ECC_KEY_FILE
 * Lis un fichier contenant une clé ECC.
 *
 */
int read_ecc_key_file(bdevice *bd, int type, int number)
{
    char *buffer;
    int nb=0, fd;
    char *filename = NULL;
    char **key = NULL;
    
    filename = bdevice_get_filename(bd, type, number);
    fd = open(filename, O_RDONLY, 0);
    
    if(fd<0){
        return -1;
    }
    
    buffer = malloc( 4096* sizeof(char) );
    
    if(!buffer){
        close(fd);
        return -1;
    } 
    
    memset(buffer,0, 4096);
    nb = read(fd,(void*)(buffer),4095);
            
    close(fd);
    
    if ( nb>0 )
    {
        buffer[nb]='\0';
    }
    else {
        return -1;
    }
    
    key = bdevice_get_ecc_key(bd, type, number);
    *key = strndup(buffer, nb);
    free(buffer);
    return 0;
}


/**
 * UINT16_T TO CHAR
 */
unsigned char* uint16_t_to_char(uint16_t u16)
{
    unsigned char *b = NULL;
    
    b = malloc(3*sizeof(unsigned char));
    b[1] = u16 & 0xFF;
    b[0] = u16 >> 8;
    b[2] = '\0';
    return b;   
}
