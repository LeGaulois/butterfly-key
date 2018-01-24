#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "butterfly_proto.h"
#include "device.h"
#include "list.h"



s_bproto_header* string_to_bproto_header(unsigned char *str, size_t len_str)
{
	s_bproto_header *bp_h = NULL;
	int return_code;
	
	if( len_str < BPROTO_HEADER_FIX_LENGTH)
		return NULL;
	
	bp_h = bproto_header_init();
	if(!bp_h) 
		return NULL;
	
	bp_h->id = (str[0]<<8) + str[1];
	bp_h->type = str[2];	
	bp_h->status = str[3];
	bp_h->nbtlv = (str[4]<<24) + (str[5]<<16) + (str[6]<<8) + str[7];
	
	return_code = string_to_bproto_tlv_list(str + BPROTO_HEADER_FIX_LENGTH,
					len_str - BPROTO_HEADER_FIX_LENGTH, bp_h);
					
	if ( return_code !=0 )
	{
		bproto_header_free(&bp_h);
		return NULL;
	}	
	fprintf(stderr,"OK\n");
	return bp_h;
}


int string_to_bproto_tlv_list(unsigned char *str_list_tlv,
				size_t len_str_list_tlv, s_bproto_header *bp_h)
{
	int 			counter_tlv = 0;
	s_bproto_tlv	*tlv;
	unsigned char	*buffer;
	size_t			len_buffer;
	
	len_buffer	= len_str_list_tlv;
	buffer		= str_list_tlv;
	
	fprintf(stderr,"NBTLMV: %d\n", bp_h->nbtlv);
	
	while(counter_tlv < bp_h->nbtlv )
	{
		tlv = string_to_bproto_tlv(buffer, len_buffer);
		
		fprintf(stderr,"tlv[%d/%d]: %p\n", counter_tlv, bp_h->nbtlv,tlv);
		if(!tlv)
			return -1;
		
		bproto_header_add_tlv(bp_h, tlv);
		bp_h->nbtlv --;
		buffer += tlv->length + TLV_FIX_LENGTH;
		len_buffer -= tlv->length + TLV_FIX_LENGTH;
		counter_tlv++;
	}
	
	return 0;
}


s_bproto_tlv* string_to_bproto_tlv(unsigned char *str_tlv,
							size_t len_str_list_tlv)
{
	s_bproto_tlv 	*tlv 		= NULL;
	int counter = 0;
	if( len_str_list_tlv < TLV_FIX_LENGTH)
		return NULL;
		
	tlv = bproto_tlv_init();
	if( !tlv )
		return NULL;
		
	tlv->type 	= str_tlv[0];
	tlv->length = (str_tlv[1]<<8) + str_tlv[2];
	tlv->value	= (unsigned char *) calloc(tlv->length, 
									sizeof(unsigned char));
	
	fprintf(stderr,"\tTYPE:%d\n\tLENGTH:%d\n\n", tlv->type, tlv->length);
	if( !tlv->value)
	{
		bproto_tlv_free((void **)&tlv);
		return NULL;
	}		

	while(counter < tlv->length)
	{
		tlv->value[counter] = str_tlv[3+counter];
		counter++;
	}
	return tlv;	
}
