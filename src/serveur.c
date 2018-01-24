#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "bserver.h"
#include "butterfly_proto.h"
#include "butterfly_serveur.h"


/**
 * BSERVER_INIT
 * Initie une structure BSERVER
 */
bserver* bserver_init(){
    bserver *bs = NULL;
    
    bs = malloc( 1*sizeof(bserver) );
    
    if(!bs){
        return NULL;
    }
    
    memset(bs, 0, 1*sizeof(bserver) );
    return bs;
}


/**
 * BS_CHECK_IPADDRESS_TO_USE
 * Vérifie que l'adresse IP à binder existe sur le serveur.
 */
struct addrinfo* bs_check_ipaddress_to_use(bserver *bs, 
                    struct addrinfo *results)
{
    struct addrinfo *info;
    char ip[16];
    
    if(!bs->ip){
        return results;
    }
    for (info = results; info != NULL; info = info->ai_next) {
	    getnameinfo(info->ai_addr, info->ai_addrlen,
			            ip, sizeof(ip), NULL, 0, NI_NUMERICHOST);
		
		if(strcmp(bs->ip, ip)==0){
		    return info;   
		};
	}
    
    return NULL;
}

int serveur_tcp (bserver *bs)
{
    
	int err, autorisation = 1;
	int sock_serveur;
	int sock_connectee;
	struct addrinfo  hints;
	struct addrinfo *results;

	struct sockaddr_in * addr_in;
	socklen_t length = 0;


	/* Creer la socket serveur et lui attribuer un numero de service.*/
	if ((sock_serveur = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		fprintf(stderr,"Erreur de création de la socket: %s", strerror(errno));
		return -1;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family     = AF_INET;
	hints.ai_socktype   = SOCK_STREAM;
	hints.ai_flags      = AI_PASSIVE;
	
	if ((err = getaddrinfo(bs->ip, bs->port,  &hints, &results)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
		close(sock_serveur);
		return -1;
	}
    
    setsockopt(sock_serveur, SOL_SOCKET, SO_REUSEADDR, &autorisation,
    	sizeof(int));
	err = bind(sock_serveur, results->ai_addr, results->ai_addrlen);
	freeaddrinfo(results);
	
	if (err < 0) {
		fprintf(stderr,"Erreurs lors du bind: %s.", strerror(errno));
		close(sock_serveur);
		return -1;
	}

	length = sizeof(struct sockaddr_in);
	
	if (getsockname(sock_serveur, (struct sockaddr *) &addr_in, &length) < 0) 
	{
		fprintf(stderr,"Erreur lors de getsockname: %s.", strerror(errno));
		return -1;
	}

	listen(sock_serveur, 5);

	while (! quitter_le_serveur()) {
		sock_connectee = accept(sock_serveur, NULL, NULL);
		
		if (sock_connectee < 0) {
			perror("accept");
			return -1;
		}
		switch (fork()) {
			case 0 :
				close(sock_serveur);
				traite_connexion(sock_connectee);
				exit(EXIT_SUCCESS);
			case -1 :
				perror("fork");
				return -1;
			default :
				close(sock_connectee);
		}
	}
	return 0;
}

int quitter_le_serveur (void)
{
	return 0;
}

void traite_connexion (int sock)
{
	struct sockaddr * sockaddr;
	socklen_t length = 0;
	char hostname [NI_MAXHOST];
	char servname [NI_MAXSERV];
	unsigned char buffer[8192];
	size_t nb_read;
	s_bproto_header *bp_h;
	
	
	getpeername(sock, NULL, &length);
	if (length == 0)
		return;
	sockaddr = malloc(length);
	if (getpeername(sock, sockaddr, & length) < 0) {
		perror ("getpeername");
		free(sockaddr);
		return;
	}
	getnameinfo(sockaddr, length,
	                hostname, NI_MAXHOST,
	                servname, NI_MAXSERV,
	                NI_NUMERICHOST | NI_NUMERICSERV);
	
	nb_read = read(sock, buffer, 8192);
	bp_h = string_to_bproto_header(buffer, nb_read);
	bproto_header_print(bp_h);
	bproto_header_free(&bp_h);
	
	free(sockaddr);
	close(sock);
	
}



int main(){
    bserver *bs = NULL;
    
    bs = bserver_init();
    if(!bs) exit(1);
    
    bs->port = strndup("2018",4);
    bs->ip = strndup("127.0.0.1", 9);
    serveur_tcp(bs);
    exit(0);
}
