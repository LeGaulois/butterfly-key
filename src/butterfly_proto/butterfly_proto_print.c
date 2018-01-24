#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "butterfly_proto.h"



void bproto_header_print(const s_bproto_header *bp_h)
{
	int counter = 0, aes_counter;
	element *el;
	s_bproto_tlv *tlv;
	
	fprintf(stdout,"ID: %d\nTYPE: %d\nSTATUS: %d\nNBTLV: %d\n",
		bp_h->id, bp_h->type, bp_h->status, bp_h->nbtlv);
		
	if(bp_h->nbtlv <= 0)
		return;
	
	el = bp_h->list_tlv->first;
	tlv = (s_bproto_tlv*) el->data;
	
	while(counter < bp_h->nbtlv)
	{
		if(counter==0)
			el = bp_h->list_tlv->first;
		else
			el = el->next;
			
		tlv = (s_bproto_tlv*) el->data;
	
		fprintf(stdout,"\tTYPE: %d\n\tLENGTH:%d\n\tVALUE:\n\t",
			tlv->type, tlv->length);
		
		if( (tlv->type == BPROTO_TLV_TYPE_EK) || 
				(tlv->type == BPROTO_TLV_TYPE_CK))
		{
			aes_counter = 0;
			while( aes_counter < tlv->length)
			{
				fprintf(stdout, "%02x", tlv->value[aes_counter]);
				aes_counter++;
			}
			fprintf(stdout,"\n\n");
		}
		else
			write(STDOUT_FILENO,tlv->value,tlv->length );
		counter += 1;
	}
}
