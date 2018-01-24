#ifndef SQL_H
#define SQL_H


#define SQL_AES_TYPE_EK			1
#define SQL_AES_TYPE_CK			2
#define SQL_ECC_F1				1
#define SQL_ECC_F2				2
#define SQL_ECC_TYPE_PUBLIC		1
#define SQL_ECC_TYPE_PRIVATE	2

/**
 * FONCTIONS GENERIQUES
 */
int sql_open_database(sqlite3 **database, const char *databasename);
int sql_exec(sqlite3 *db, const char *sql_command);
int sql_exec_prepare(	sqlite3 *db, 
						const char *sql_command, 
						sqlite3_stmt **result);
char * escape_special_character(const char *str);


 
int sql_client_create_tables(sqlite3 *db);
int sql_write_ecc_key(	sqlite3 *database, int type, 
						int name, char *value, int size_value);
int sql_write_aes_key(	sqlite3 *database, int name, 
						unsigned char *value, int size_value);
int sql_client_write_certificate(sqlite3 *database, 
							char *start_timestamp,
							char * end_timestamp, char *value);
int sql_write_private_key(sqlite3 *database, int cert_id, char *value);
				
									
int sql_get_ecc_key(sqlite3 *database, int type, int name, 
							unsigned char **return_char);
int sql_get_aes_key(sqlite3 *database, int name, unsigned char **return_char);
int sql_get_certificate(sqlite3 *database, unsigned char **return_char);
int sql_get_private_key(sqlite3 *database, int public_id,
				 unsigned char **return_char);


#endif
