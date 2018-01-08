#include <stdio.h>
#include <stdlib.h>
#include "device.h"
#include <string.h>
#include "tools.h"


int main(){
    bdevice *bd  = NULL;

    
    bd = bdevice_init();
    bd->f1_public_key_filename = strdup("f1_public.key");
    bd->f1_private_key_filename = strdup("f1_private.key");
    bd->f2_public_key_filename = strdup("f2_public.key");
    bd->f2_private_key_filename = strdup("f2_private.key");
    
    bdevice_create_aes_keys(bd);
    bdevice_create_ecc_keys(bd);
    bdevice_destroy(&bd);
    
    exit(0);
}

