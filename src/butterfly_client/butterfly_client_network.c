#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <netdb.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include "device.h"
#include "tools.h"
#include "ecc_key_generate.h"
#include "sql.h"
#include "butterfly_proto.h"


/**
 * BDEVICE_CONNECT_TO_SERVER
 * Connexion au serveur SCMS.
 *
 * Valeur de retour
 *  >0: socket TCP sur la connexion.
 *  -1: erreurs
 */
int bdevice_connect_to_server (t_bdevice *bd)
{
	int    sock;
	
	struct addrinfo *results;
	
	if (bdevice_build_addrinfo(bd, &results) < 0)
	{
	    return -1;
	}


	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		fprintf(stderr,"Erreur de création du socket: %s.",
		        strerror(errno));
		return -1;
	}
	if (connect(sock, results->ai_addr, results->ai_addrlen) < 0) {
		fprintf(stderr,"Erreur de connection: %s.",
		        strerror(errno));
		return -1;
	}
	freeaddrinfo(results);
	
	return sock;
}


/**
 * BDEVICE_SEND_CERT_QUERY_TO_SERVER
 * Envoie au serveur SCMS le nécessaire pour générer
 * et nous renvoyer l'ensemble des certificats.
 *
 * Valeurs de retour
 *  -1 -> ERREUR
 *   0 -> SUCCES
 */
int bdevice_send_cert_query_to_server(t_bdevice *bd, int socket)
{
	s_bproto_header *bp_h;
	unsigned char *request;
	size_t size_request;
	int return_code;
	
	bp_h = bdevice_prepare_request_to_server(bd);
	
	if(!bp_h){
		return -1;
	}
	
	size_request = bproto_to_char(bp_h, &request);
	
	if(size_request==0)
	{
		bproto_header_free(&bp_h);
		return -1;
	}
	
	return_code = write(socket, request, size_request);
	
	if(return_code<0)
	{
		fprintf(stderr,"Erreur d'envoi de la requete au serveur: %s",
			strerror(errno));
		return_code = -1;
	}
	else
		return_code = 0;
		
	free(request);	
	bproto_header_free(&bp_h);
	return return_code;
}


/**
 * BDEVICE_BUILD_ADDRINFO
 * Crée une structure addrinfo en fonction de l'adresse IP du serveur
 * et du port de connexion présent dans la structure t_bdevice
 *
 * Valeur de retour
 *  0 -> SUCCES
 * -1 -> ERREUR
 */
int bdevice_build_addrinfo(t_bdevice *bd, struct addrinfo **results)
{
	int    err;	
	struct addrinfo  hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if ((err = getaddrinfo(bd->srv_ip, bd->srv_port, 
	        &hints, results)) != 0) 
	{
		fprintf(stderr, "Erreur avec getaddrinfo: %s\n", 
		        gai_strerror(err));
		return -1;
	}
	return 0;
}

