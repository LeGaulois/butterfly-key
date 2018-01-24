#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "device.h"
#include "sql.h"
#include "butterfly_proto.h"



s_bproto_header* bdevice_prepare_request_to_server(
										t_bdevice *bd)
{
	s_bproto_header *bp_h 	= NULL;
	s_bproto_tlv	*tlv 	= NULL;
	
	bp_h = bproto_header_init();
	
	if(! bp_h ) 
		return NULL;
	
	bp_h->id	 	= rand() % 65535;
	bp_h->type   	= BPROTO_TYPE_QUERY;
	bp_h->status 	= 0;
	
	tlv = bdevice_ek_to_tlv(bd);

	
	if( !tlv ) 
		goto erreur;
	bproto_header_add_tlv(bp_h, tlv);
	
	tlv = bdevice_ck_to_tlv(bd);
	if( !tlv ) 
		goto erreur;
	bproto_header_add_tlv(bp_h, tlv);
	

	tlv = bdevice_f1_public_to_tlv(bd);

	if( !tlv ) 
		goto erreur;
	bproto_header_add_tlv(bp_h, tlv);
	
	
	tlv = bdevice_f2_public_to_tlv(bd);

	if( !tlv ) 
		goto erreur;
	bproto_header_add_tlv(bp_h, tlv);
	
	return (bp_h);
	
	erreur:
		bproto_header_free(&bp_h);
		return (NULL);
}


s_bproto_tlv* bdevice_ek_to_tlv(t_bdevice *bd)
{
	s_bproto_tlv *tlv = NULL;
	
	tlv = bproto_tlv_init();
	
	if(!tlv) 
		return (NULL);
	
	tlv->type = BPROTO_TLV_TYPE_EK;
	tlv->length = sql_get_aes_key(bd->db, SQL_AES_TYPE_EK,
					&(tlv->value));
	
	if( tlv->length ==0 )
	{
		bproto_tlv_free((void**)&tlv);
		return (NULL);
	}
	

	return (tlv);
}


s_bproto_tlv* bdevice_ck_to_tlv(t_bdevice *bd)
{
	s_bproto_tlv *tlv = NULL;
	
	tlv = bproto_tlv_init();
	if( !tlv )
		return (NULL);
	
	tlv->type = BPROTO_TLV_TYPE_CK;
	tlv->length = sql_get_aes_key(bd->db, SQL_AES_TYPE_CK,
					&(tlv->value));
	
	if( tlv->length == 0)
	{
		bproto_tlv_free((void**)&tlv);
		return (NULL);
	}
	
	return (tlv);
}


s_bproto_tlv* bdevice_f1_public_to_tlv(t_bdevice *bd)
{
	s_bproto_tlv *tlv = NULL;
	
	tlv = bproto_tlv_init();
	
	if(!tlv) 
		return (NULL);
	
	tlv->type = BPROTO_TLV_TYPE_F1_PUBLIC_KEY;
	tlv->length = sql_get_ecc_key(bd->db, SQL_ECC_TYPE_PUBLIC,
			SQL_ECC_F1, &(tlv->value));
	
	if( tlv->length == 0 )
	{
		bproto_tlv_free((void**)&tlv);
		return (NULL);
	}
	
	return (tlv);
}


s_bproto_tlv* bdevice_f2_public_to_tlv(t_bdevice *bd)
{
	s_bproto_tlv *tlv = NULL;
	
	tlv = bproto_tlv_init();
	
	if(!tlv)
		return (NULL);
	
	tlv->type = BPROTO_TLV_TYPE_F2_PUBLIC_KEY;
	tlv->length = sql_get_ecc_key(bd->db, SQL_ECC_TYPE_PUBLIC,
					SQL_ECC_F2, &(tlv->value));
	
	if( tlv->length == 0 )
	{
		bproto_tlv_free((void**)&tlv);
		return (NULL);
	}
	
	return (tlv);
}
