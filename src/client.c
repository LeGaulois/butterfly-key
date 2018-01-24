#include <stdio.h>
#include <stdlib.h>
#include "device.h"
#include <string.h>
#include "tools.h"
#include "sql.h"



int main()
{
    int socket;
    t_bdevice *bd  = NULL;
    
    bd = bdevice_init();
    bd->f1_public_key_filename = strdup("f1_public.key");
    bd->f1_private_key_filename = strdup("f1_private.key");
    bd->f2_public_key_filename = strdup("f2_public.key");
    bd->f2_private_key_filename = strdup("f2_private.key");
    bd->srv_ip = strndup("127.0.0.1", 9);
    bd->srv_port = strndup("2018",4);
    
    bdevice_init_database(bd);
    
    bdevice_create_aes_keys(bd);
    bdevice_create_ecc_keys(bd);
    
    
    socket = bdevice_connect_to_server(bd);
    
    if(socket<0)
    	return EXIT_FAILURE;
    	
    bdevice_send_cert_query_to_server(bd, socket);
    bdevice_destroy(&bd);
    
    exit(0);
}

