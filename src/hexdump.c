#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


char* read_ecc_key_file(int fd){
    char *buffer;
    int nb=0;

    buffer = malloc( 4096* sizeof(char) );
    
    if(!buffer){
        return NULL;
    } 
    
    memset(buffer,0, 4096);
    nb = read(fd,(void*)(buffer),4095);
            
    if ( nb>0 )
    {
        buffer[nb]='\0';
        return buffer;
    }
    
    free(buffer);
    return NULL;
}

