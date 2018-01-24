#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <gmp.h>
#include "butterfly_serveur.h"
#include "ecc_curve_order.h"


unsigned char* butterfly_aes_encrypt(unsigned char *aes_key,
		int size_aes_key, unsigned char *input, int size_input)
{
	unsigned char 	iv[AES_BLOCK_SIZE];
	unsigned char 	*enc_out = NULL;
	AES_KEY 		enc_key;
	int 			return_code;
	
	memset(iv, 0x00, AES_BLOCK_SIZE);	
	enc_out = calloc(size_input, sizeof(unsigned char));
	
	if( ! enc_out)
		return NULL;
		
	return_code = AES_set_encrypt_key(aes_key, size_aes_key*8, &enc_key);
	
	if(return_code != 0)
	{
		free(enc_out);
		return NULL;
	}
	
	AES_cbc_encrypt(input, enc_out, size_input, &enc_key, iv, AES_ENCRYPT);
	
	return enc_out;
}


mpz_t* butterfly_serveur_f1(unsigned char *f1_int, int size_f1_int )
{
	mpz_t 	*bigint, *order;
	char	*str_bigint, *str_order;
	
	str_bigint = unsigned_char_to_char(f1_int, size_f1_int);
	
	if( ! str_bigint )
		return NULL;

	str_order = unsigned_char_to_char((unsigned char *)secp256k1_order, 33);
	
	if( ! str_order )
	{
		free(str_bigint);
		return NULL;
	}
	
	bigint = unsigned_char_to_gmp_format(str_bigint, 10);
	order = unsigned_char_to_gmp_format(str_order, 10);
	
	return bigint;
	
}


unsigned char* f1_int(t_client_request *cr, int type,
				unsigned char *randbytes, int size_randbytes)
{
	unsigned char	*cipher, *cipher_xor[3], *cipher_xor_concat;
	int 			counter = 0;
		
		
	while(counter < 4)
	{
		unsigned_char_add_one(randbytes, size_randbytes);
		cipher = butterfly_aes_encrypt(cr->ek, size_randbytes, 
						randbytes, size_randbytes);
	
		if( !cipher )
			return NULL;

		cipher_xor[counter] = unsigned_char_xor(randbytes, cipher,
								 size_randbytes);
	
		if( ! cipher_xor[counter] )
		{
			free(cipher);
			return NULL;
		}
	}
	
	 cipher_xor_concat = unsigned_char_concatenate (cipher_xor[0],
							cipher_xor[1],cipher_xor[2], size_randbytes);
										 
	free(cipher_xor[0]);
	free(cipher_xor[1]);
	free(cipher_xor[2]);
	
	if(! cipher_xor_concat)
		return NULL;
		
	return cipher_xor_concat;
}






