CC=gcc
CFLAGS=-Wall -g -fstack-protector-all -Wstack-protector -I ./include 

client: 	ecc list tools \
		bdevice_client_init bdevice_client_network bdevice_client_tools  sql sql_get sql_set butterfly_proto butterfly_proto_client butterfly_proto_from_char butterfly_proto_to_char butterfly_proto_print
		
	$(CC) $(CFLAGS) butterfly_client_init.o butterfly_client_network.o butterfly_client_tools.o  sql.o sql_get.o sql_set.o butterfly_proto.o butterfly_proto_client.o butterfly_proto_from_char.o butterfly_proto_to_char.o butterfly_proto_print.o list.o ecc_key_generate.o src/client.c -lsqlite3 -lssl -lcrypto tools.o -o client 


serveur: 	ecc list tools \
		bdevice_client_init bdevice_client_network bdevice_client_tools  sql sql_get sql_set butterfly_proto butterfly_proto_client butterfly_proto_from_char butterfly_proto_to_char butterfly_proto_print butterfly_serveur_tools butterfly_serveur_caterpillar
		
	$(CC) $(CFLAGS) ./*.o src/serveur.c -lsqlite3 -lssl -lgmp -lcrypto -o serveur 


list:
	$(CC) $(CFLAGS) -c src/list.c
	
	
	
butterfly_proto:
	$(CC) $(CFLAGS) -c src/butterfly_proto/butterfly_proto.c
butterfly_proto_client:
	$(CC) $(CFLAGS) -c src/butterfly_proto/butterfly_proto_client.c
butterfly_proto_from_char:
	$(CC) $(CFLAGS) -c src/butterfly_proto/butterfly_proto_from_char.c
butterfly_proto_to_char:
	$(CC) $(CFLAGS) -c src/butterfly_proto/butterfly_proto_to_char.c
butterfly_proto_print:
	$(CC) $(CFLAGS) -c src/butterfly_proto/butterfly_proto_print.c	

	
sql:
	$(CC) $(CFLAGS) -c src/sql/sql.c -l sqlite3
sql_get:
	$(CC) $(CFLAGS) -c src/sql/sql_get.c -l sqlite3
sql_set:
	$(CC) $(CFLAGS) -c src/sql/sql_set.c -l sqlite3
	
	
	
bdevice_client_init:
	$(CC) $(CFLAGS) -c src/butterfly_client/butterfly_client_init.c	
bdevice_client_tools:
	$(CC) $(CFLAGS) -c src/butterfly_client/butterfly_client_tools.c
bdevice_client_network:
	$(CC) $(CFLAGS) -c src/butterfly_client/butterfly_client_network.c
	

butterfly_serveur_caterpillar:
	$(CC) $(CFLAGS) -c src/butterfly_server/butterfly_serveur_caterpillar.c
butterfly_serveur_tools:
	$(CC) $(CFLAGS) -c src/butterfly_server/butterfly_serveur_tools.c
	
bserver:
	$(CC) $(CFLAGS) -c src/bserver.c

ecc:
	$(CC) $(CFLAGS) -c src/ecc_key_generate.c  -lssl -lcrypto
	
tools:
	$(CC) $(CFLAGS) -c src/tools.c


	
clean:
	rm *.o
