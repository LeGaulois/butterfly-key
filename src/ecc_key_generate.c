#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/buffer.h>
#include <openssl/objects.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/pkcs12.h>
#include "device.h"
#include "ecc_key_generate.h"
#include "sql.h"


/**
 * ECC_KEY_GENERATE
 * Fonction permettant de générer une paire de clé ECC
 *
 * Valeurs de retour
 *  1 -> @ERROR
 *  0 -> @SUCCESS
 */
int ecc_key_generate(t_bdevice *bd, int number) 
{
	BIO			*outbio = NULL;
	EC_KEY		*myecc  = NULL;
	EVP_PKEY	*pkey   = NULL;
	int			eccgrp;
   
	OpenSSL_add_all_algorithms();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();

	eccgrp = OBJ_txt2nid("secp256k1");
	myecc = EC_KEY_new_by_curve_name(eccgrp);

	EC_KEY_set_asn1_flag(myecc, OPENSSL_EC_NAMED_CURVE);

	if (! (EC_KEY_generate_key(myecc)))
	{
		fprintf(stderr, "Error generating the ECC key.");
		return 1;
	}
	
   /* -------------------------------------------------------- *
	* Converting the EC key into a PKEY structure let us	   *
	* handle the key just like any other key pair.			 *
	* ---------------------------------------------------------*/
	pkey=EVP_PKEY_new();
	
	if (!EVP_PKEY_assign_EC_KEY(pkey,myecc)){
		fprintf(stderr, "Error assigning ECC key to EVP_PKEY structure.");
		return 1;
	}

	
	//outbio = BIO_new(BIO_s_file());
	outbio = BIO_new_file(bdevice_get_filename(bd, SQL_ECC_TYPE_PRIVATE, number), "w");

	
	if(!PEM_write_bio_PrivateKey(outbio, pkey, NULL, NULL, 0, 0, NULL)){
		fprintf(stderr, "Error writing private key data in PEM format");
		return 1;
	}
	
	BIO_free_all(outbio);
	//outbio  = BIO_new(BIO_s_file());
	outbio = BIO_new_file(bdevice_get_filename(bd, SQL_ECC_TYPE_PUBLIC, number), "w");
	

	if(!PEM_write_bio_PUBKEY(outbio, pkey)){
		fprintf(stderr, "Error writing public key data in PEM format");
		return 1;
	}
	
	EVP_PKEY_free(pkey);
	BIO_free_all(outbio);

	return 0;
}

