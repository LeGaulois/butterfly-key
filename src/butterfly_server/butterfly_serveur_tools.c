#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <gmp.h>
#include "butterfly_serveur.h"
#include <openssl/rand.h>


unsigned char* generate_rand_bytes(int number_bytes)
{
	unsigned char 	*return_char;
	int				return_code;
	
	return_char = calloc(number_bytes, sizeof(unsigned char));
	
	if(! return_char)
		return NULL;
		
	return_code = RAND_bytes(return_char, number_bytes);
	
	if( return_code !=1 )
	{
		free(return_char);
		return NULL;
	}
	
	return return_char;
}


void unsigned_char_add_one(unsigned char *str, int size_str)
{
	int 			counter_char = size_str - 1;
		
	while( counter_char >= 0 )
	{
		if( (str[counter_char] == 0xFF) )
		{
			if( counter_char ==0)
				counter_char = size_str;
				
			str[counter_char] = 0x00;
		}
		else 
		{
			str[counter_char] = str[counter_char] + 0x01;
			return;
		}
		
		counter_char--;
	}
}

unsigned char* unsigned_char_xor(unsigned char *r2, unsigned char *d2,
						int size_chars)
{
	int counter = 0;
	unsigned char *return_char;
	
	return_char = calloc(size_chars, sizeof(unsigned char));
	
	if( ! return_char )
		return NULL;
		
	while( counter < size_chars)
	{
		return_char[counter] = r2[counter] ^d2[counter];
		counter++;
	}
	
	return return_char;
					
}

unsigned char * unsigned_char_concatenate(	unsigned char *r1, 
											unsigned char *r2,
											unsigned char *r3, 
											int size_chars)
{
	
	int 			counter = 0, counter_char=0;
	unsigned char 	*return_char,
					*current_r;
					
	return_char = calloc(size_chars*3, sizeof(unsigned char));
	
	if( ! return_char )
		return NULL;
	
	while( counter < 3)
	{
		if (counter == 0 )
			current_r = r1;
		else if (counter == 1 )
			current_r = r2;
		
		else
			current_r = r3;
			
		while( counter_char < size_chars*3)
		{
			return_char[counter_char] = current_r[counter_char % size_chars];
			counter_char++;
		}
		counter++;
		
	}
	return return_char;
}


mpz_t* unsigned_char_to_gmp_format(char *to_convert, int base)
{
	mpz_t *bigint;
	
	bigint = calloc(1, sizeof(mpz_t));
	
	if(! bigint )
		return NULL;
		
	mpz_init( *bigint );
	mpz_set_str(*bigint, to_convert, base);
	return bigint;
}


char * unsigned_char_to_char(unsigned char *hexadecimal, int size_hexa)
{
	char	*return_char 	= NULL;
	int		counter			= 0;
	
	return_char = calloc(size_hexa + 3, sizeof(char) );
	
	if( ! return_char )
		return NULL;
		
	return_char[0] = '0';
	return_char[1] = 'x';
	
	while( counter < size_hexa )
	{
		return_char[ counter + 2 ] = hexadecimal[counter];
		counter++;
	}
	
	return return_char;
}
