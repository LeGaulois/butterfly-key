CC=gcc
CFLAGS=-Wall -g -fstack-protector-all -Wstack-protector -I ./include 

all: device bserver ecc list tools butterfly_proto
	$(CC) $(CFLAGS) list.o butterfly_proto.o device.o bserver.o ecc_key_generate.o tools.o src/test_bdevice.c -lssl -lcrypto -o main 

list:
	$(CC) $(CFLAGS) -c src/list.c
	
butterfly_proto:
	$(CC) $(CFLAGS) -c src/butterfly_proto.c

device:
	$(CC) $(CFLAGS) -c src/device.c
	
bserver:
	$(CC) $(CFLAGS) -c src/bserver.c

ecc:
	$(CC) $(CFLAGS) -c src/ecc_key_generate.c  -lssl -lcrypto
	
tools:
	$(CC) $(CFLAGS) -c src/tools.c
	
clean:
	rm *.o
