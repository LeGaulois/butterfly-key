#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "butterfly_proto.h"
#include "device.h"
#include "list.h"


size_t bproto_to_char(const s_bproto_header *bp_h, 
				unsigned char **buffer)
{
	size_t 			size_buffer, size_str_tmp;
	unsigned char 	*str_tmp, *save;
	int counter;

	size_buffer = bproto_get_size(bp_h);
	*buffer = (unsigned char *) calloc(size_buffer,
				sizeof(unsigned char));
				

	if(! *buffer)
		return 0;
		
	save = *buffer;
	size_str_tmp = bproto_header_to_char(bp_h, &str_tmp);
	

	if( size_str_tmp == 0)
	{
		free(buffer);
		return 0;
	}

	counter = 0;
	
	while(counter < BPROTO_HEADER_FIX_LENGTH){
		(*buffer)[counter] = str_tmp[counter];
		counter++;
	}
	
	*buffer += size_str_tmp;
	
	free(str_tmp);
	size_str_tmp = bproto_list_tlv_to_char(bp_h, &str_tmp);
	
	if( size_str_tmp == 0)
	{
		free(*buffer);
		return 0;
	}
	counter = 0;
	
	while(counter < size_str_tmp-BPROTO_HEADER_FIX_LENGTH){
		(*buffer)[counter] = str_tmp[counter];
		counter++;
	}
	
	free(str_tmp);
	*buffer =save;
	return size_buffer;
}


size_t bproto_header_to_char(const s_bproto_header *bp_h,
					unsigned char **buffer)
{	
	if(! bp_h)
		return (0);
	
	*buffer = (unsigned char *) calloc(BPROTO_HEADER_FIX_LENGTH, 
			sizeof(unsigned char));
	
	if(!*buffer)
		return (0);
	
	bproto_header_print(bp_h);	
	(*buffer)[0] 	= bp_h->id >> 8;
	(*buffer)[1] 	= bp_h->id;
	(*buffer)[2]	= bp_h->type;
	(*buffer)[3]	= bp_h->status;
	(*buffer)[4]	= bp_h->nbtlv >> 24;
	(*buffer)[5]	= bp_h->nbtlv >> 16;
	(*buffer)[6]	= bp_h->nbtlv >> 8;
	(*buffer)[7]	= bp_h->nbtlv;
	

	return  (BPROTO_HEADER_FIX_LENGTH);
}


size_t bproto_list_tlv_to_char(const s_bproto_header *bp_h, 
					unsigned char **buffer)
{
	int 			counter_tlv		= 0,
					counter_char	= 0;
	element *		list_element	= NULL;
	unsigned char	*str_tlv		= NULL, *save;
	size_t			buffer_size,
					str_tlv_size;
		
	buffer_size = bproto_get_size(bp_h);
	
	if(buffer_size==0)
		return 0;
	
	*buffer = (unsigned char*) calloc(buffer_size + 1, sizeof(char));
	
	if(! *buffer)
		return 0;
	
	save = *buffer;
	list_element = bp_h->list_tlv->first;
	
	while( counter_tlv < bp_h->list_tlv->size )
	{
		str_tlv_size = bproto_tlv_to_char(list_element->data,
						&str_tlv);
		if(str_tlv_size==0)
		{
			free(str_tlv);
			free(*buffer);
			return 0;
		}
		counter_char = 0;
		while(counter_char < str_tlv_size)
		{
			(*buffer)[counter_char] = str_tlv[counter_char];
			counter_char++;
		}

		*buffer += counter_char;
		list_element = list_element->next;
		free(str_tlv);
		counter_tlv++;
	}
	
	*buffer = save;
	return buffer_size;
}

/**
 * BPROTO_ELEMENT_TO_CHAR
 * Convertie une structure bproto_element en une chaîne 
 * de caractères.
 */
size_t bproto_tlv_to_char(s_bproto_tlv *bp_tlv, 
					unsigned char **buffer)
{
	size_t 			size_buffer;
	int 			buffer_position, tlv_value_position;
	
	size_buffer = 3 + bp_tlv->length;
	*buffer = (unsigned char*)( malloc( (size_buffer+1 )*
								sizeof(unsigned char)) );
	
	if(! *buffer)
		return (0);
	
	memset(*buffer, 0, size_buffer + 1);
	(*buffer)[0] = 	bp_tlv->type;					
	(*buffer)[1] = 	bp_tlv->length >> 8;
	(*buffer)[2] = 	bp_tlv->length;
	
	tlv_value_position 	= 0;
	buffer_position		= 3;
	while( tlv_value_position < bp_tlv->length)
	{
		(*buffer)[buffer_position] = bp_tlv->value[tlv_value_position];
		buffer_position++;
		tlv_value_position++; 
	}
	
				
	return size_buffer;
}
