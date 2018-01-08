#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "butterfly_proto.h"
#include "device.h"
#include "list.h"



/**
 * BPROTO_TO_CHAR
 * Transforme une structure bproto et ses éléments associés
 * en un string qui pourra être transmise sur au socket pour
 * envoyé au serveur.
 */
unsigned char* bproto_to_char(bproto_header *bph)
{
    size_t total_size = 0;
    int i;
    unsigned char *buffer = NULL, *tmp = NULL;
    element *el = NULL;
    bproto_element *bp_element = NULL;
    
    for(i=0; i< bph->list_element->size; i++){
        if(i==0){
            el = bph->list_element->first;
        }
        else{
            el = el->next;
        }
        bp_element = (bproto_element*)(el->data);
        total_size += bp_element->length + 3; 
    }
    
    buffer = malloc( (HEADER_FIX_LENGTH + total_size) * sizeof(unsigned char));
    
    if(!buffer){
        return NULL;
    }
    
    snprintf(buffer, HEADER_FIX_LENGTH,"%c%c%c%c%c%c%c%c", 
            bph->id&0xFF, bph->id >> 8,
            bph->type, bph->status, 
            bph->nbcertificatrequested & 0xFF,
            bph->nbcertificatrequested >> 8, 
            bph->nbcertificatsending & 0xFF,
            bph->nbcertificatsending >> 8);
    
    el = NULL;
    buffer += HEADER_FIX_LENGTH;
    
    
    for(i=0; i< bph->list_element->size; i++){
        if(i==0){
            el = bph->list_element->first;
        }  
        else{
            el = el->next;
        }  
        tmp = bproto_element_to_char( (bproto_element)(el->data) );
        
        if(tmp==NULL){
            free(buffer);
            return NULL;
        }
        snprintf(buffer, strlen((char *)(tmp)),"%s", tmp);
        buffer += strlen((char*)(tmp));
        free(tmp);
        tmp = NULL;
    }
    
    return buffer;
}


/**
 * BPROTO_ELEMENT_TO_CHAR
 * Convertie une structure bproto_element en une chaîne 
 * de caractères.
 */
unsigned char* bproto_element_to_char(bproto_element *bel)
{
    unsigned char *buff = NULL;
    size_t size;
    int ret;
    
    size = 3 + strlen( (char*)(bel->value) );
    buff = malloc( (size+1 )* sizeof(unsigned char) );
    
    if(! buff) {
        return NULL;
    }
    
    memset(buff, 0, size+1);
    ret = snprintf(buff, size, "%c%c%c%s", bel->type, bel->length & 0xFF,
                bel->length >> 8, bel->value);
     
    if(ret!=size){
        free(buff);
        return NULL;
    }
                
    return buff;
}
