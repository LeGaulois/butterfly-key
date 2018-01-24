#ifndef BUTTERFLY_SERVEUR
#define BUTTERFLY_SERVEUR

#include <gmp.h>

#define CATERPILLAR_SIGNING_KEY 	0
#define CATERPILLAR_ENCRYPTION_KEY 	1


typedef struct s_client_request t_client_request;
typedef struct s_key_worker		t_key_worker;

struct 	s_key_worker {
	unsigned char 	*signing_public_key,
					*encryption_public_key;
	
};

struct 	s_client_request {
	unsigned char	ek[16],
					ck[16];
			
	size_t			size_f1_public_key,
					size_f2_public_key;
					
	unsigned char	*f1_public_key,
					*f2_public_key;
					
	t_key_worker	key_worker;
};




unsigned char* f1_int(t_client_request *cr, int type,
				unsigned char *randbytes, int size_randbytes);
unsigned char* generate_rand_bytes(int number_bytes);
void unsigned_char_add_one(unsigned char *str, int size_str);
unsigned char* butterfly_aes_encrypt(unsigned char *aes_key,
		int size_aes_key, unsigned char *input, int size_input);
unsigned char* unsigned_char_xor(unsigned char *r2, unsigned char *d2,
						int size_chars);
unsigned char * unsigned_char_concatenate(	unsigned char *r1, 
											unsigned char *r2,
											unsigned char *r3, 
											int size_chars);
											
char * unsigned_char_to_char(	unsigned char 	*hexadecimal, 
								int 			size_hexa);

mpz_t* unsigned_char_to_gmp_format(char *to_convert, int base);								
				
						
#endif
