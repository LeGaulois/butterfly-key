#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include "sql.h"



int sql_client_create_tables(sqlite3 *db){
	int counter = 0, return_code;

	static char *sql_command[4]=
    {
        "CREATE TABLE IF NOT EXISTS ecc_keys(id INTEGER PRIMARY KEY \
         AUTOINCREMENT, \
         type INTEGER, \
         name INTEGER, \
         value TEXT);",
         "CREATE TABLE IF NOT EXISTS aes_keys(id INTEGER PRIMARY KEY \
         AUTOINCREMENT, \
         type INTEGER, \
         name INTEGER, \
         value TEXT);",
         "CREATE TABLE IF NOT EXISTS certificates(id INTEGER PRIMARY KEY \
         AUTOINCREMENT, start_timestamp TEXT NOT NULL, \
         end_timestamp TEXT NOT NULL, \
         value TEXT);",
         "CREATE TABLE IF NOT EXISTS private_keys(id INTEGER PRIMARY KEY \
         AUTOINCREMENT, \
         cert_id INTEGER , \
         value TEXT, \
         FOREIGN KEY(cert_id) REFERENCES certificates(id));"
	};
         
    
	while( counter<4 )
	{
		return_code = sql_exec(db, sql_command[counter]);
     	counter++;
     	   
		if(return_code != 0)
			return (-1);
	}
    
	return (0);
}


int sql_write_ecc_key(sqlite3 *database, int type, 
            int keyname, char *value, int size_value)
{
	char *buffer;
	int return_code;

	buffer = sqlite3_mprintf("INSERT INTO ecc_keys \
(type, name, value) VALUES(%d, %d,%Q)",type, keyname,value);

    return_code = sql_exec(database, buffer);
    sqlite3_free(buffer);
    return return_code;
}


int sql_write_aes_key(sqlite3 *database, 
            int keyname, unsigned char *value, int size_value)
{
	char *buffer;

	buffer = sqlite3_mprintf("INSERT INTO aes_keys \
(name, value) VALUES(%d, %.*Q)",keyname, size_value,value);

    sql_exec(database, buffer);
    sqlite3_free(buffer);
    
    return sql_get_aes_key(database, keyname, NULL);
}


int sql_write_certificate(sqlite3 *database, 
            char *start_timestamp, char * end_timestamp, 
            char *value)
{
	char sql_command[4096];
	snprintf(sql_command, 4095,"INSERT INTO certificates \
    (start_timestamp, end_timestamp, value) VALUES(%s, %s, %s);",
    start_timestamp, end_timestamp, value);
    
	return ( sql_exec_prepare(database, sql_command, NULL) );
}


int sql_write_private_key(sqlite3 *database, int cert_id, char *value)
{
	char sql_command[4096];
	snprintf(sql_command, 4095,"INSERT INTO private_keys \
    (cert_id, value) VALUES(%d, %s);",
    cert_id, value);
    
	return ( sql_exec_prepare(database, sql_command, NULL) );
}
