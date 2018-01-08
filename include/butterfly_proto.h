#ifndef BUTTERFLY_PROTO_H
#define BUTTERFLY_PROTO_H


#include <stdint.h>
#include "list.h"


/*Type de requete butterfly*/
#define QUERY 0
#define RESPONSE 1


/**
 * Type d'éléments butterfly
 */
#define END            0x00
#define EK              0x01
#define CK              0x02
#define F1_PUBLIC_KEY   0x03
#define F2_PUBLIC_KEY   0x04
#define CERTIFICATE     0x05

#define HEADER_FIX_LENGTH 8

typedef struct bproto_header bproto_header;
typedef struct bproto_element bproto_element;


struct bproto_element {
    uint8_t         type;
    uint16_t        length;
    unsigned char   *value;
};


struct bproto_header {
    uint16_t    id;
    uint8_t     type;
    uint8_t     status;
    uint16_t    nbcertificatrequested;
    uint16_t    nbcertificatsending;
    
    list        *list_element;    
};

unsigned char* bproto_element_to_char(bproto_element *bel);
unsigned char* bproto_to_char(bproto_header *bph);

#endif
