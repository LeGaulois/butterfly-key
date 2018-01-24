#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "device.h"
#include "butterfly_proto.h"
#include "list.h"

	

s_bproto_header* bproto_header_init(void)
{
	s_bproto_header *bp_h = NULL;
	
	bp_h = (s_bproto_header*) calloc( 1, sizeof(s_bproto_header) );
	if( !bp_h ) 
		return (NULL);
	
	bp_h->list_tlv = list_init(&bproto_tlv_free, &bproto_tlv_compare);
	
	if( !bp_h->list_tlv )
	{
		free(bp_h);
		bp_h = NULL;
	}
	return (bp_h);
}


int bproto_header_free(s_bproto_header **bp_h)
{
	int return_code = 0;
	
	if(! *bp_h) 
		return (-1);
	
	if( (*bp_h)->list_tlv )
		return_code = list_destroy( &((*bp_h)->list_tlv) );

	if( return_code != 0) 
		return (-1);
		
	free(*bp_h);
	*bp_h = NULL;
	return (0);
}


int bproto_header_add_tlv(s_bproto_header *bp_h, s_bproto_tlv *tlv)
{
	if( (!bp_h) || (!tlv) ) 
		return (-1);
		
	list_rpush(bp_h->list_tlv, (void*)tlv);
	bp_h->nbtlv ++;
	
	return 0;
}


s_bproto_tlv* bproto_tlv_init(void)
{
	return (s_bproto_tlv*) calloc(1, sizeof(s_bproto_tlv)); 
}


int bproto_tlv_free(void **tlv)
{
	if(	!*tlv ) 
		return (1);
	
	if( ((s_bproto_tlv*)(*tlv))->value )
		free( ((s_bproto_tlv*)(*tlv))->value);
	
	free(*tlv);
	*tlv = NULL;
	return (0);
}


int bproto_tlv_compare(void *tlv1, void *tlv2)
{
	int return_code;
	size_t s_tlv1, s_tlv2;
	
	if( (!tlv1) ||(!tlv2))
		return (1);
	
	s_tlv1 = 3 + ((s_bproto_tlv*)tlv1)->length;
	s_tlv2 = 3 + ((s_bproto_tlv*)tlv2)->length;
	
	if( s_tlv1 != s_tlv2 ) 
		return (1);
		
	return_code = memcmp(tlv1, tlv2, s_tlv1);
	
	if ( return_code == 0)
		return (0);
		
	else 
		return (1);
}


size_t bproto_get_size(const s_bproto_header *bp_h)
{
	if(! bp_h)
		return (0);
		
	return bproto_get_list_tlv_size(bp_h) +  
			BPROTO_HEADER_FIX_LENGTH;
}


size_t bproto_get_list_tlv_size(const s_bproto_header *bp_h)
{
	size_t 			total_size 		= 0;
	int				counter_tlv		= 0;
	s_bproto_tlv	*bp_tlv 		= NULL;
	element 		*list_element	= NULL;
	
	if(!bp_h)
		return (0);
		
	while( counter_tlv < bp_h->list_tlv->size)
	{
		if( counter_tlv == 0 )
			list_element = bp_h->list_tlv->first;
		else
			list_element = list_element->next;

		bp_tlv = (s_bproto_tlv*)(list_element->data);
		total_size += bp_tlv->length + 3; 
		counter_tlv++;
	}
	
	return (total_size);
}

