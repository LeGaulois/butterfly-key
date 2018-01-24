#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string.h>
#include "sql.h"


int sql_get_ecc_key(sqlite3 *database, int type, 
						int keyname, unsigned char **return_char)
{
	sqlite3_stmt *result;
	char sql_command[4096];
	int return_code, size_return_char, counter = 0;
    unsigned char *buffer;
    
	snprintf(sql_command, 4095,"SELECT value FROM ecc_keys \
    WHERE type=%d AND name=%d;", type, keyname);
    
	return_code = sql_exec_prepare(database, sql_command, &result);
	
    if(return_code !=0 )
		return 0;
    
	return_code = sqlite3_step(result);
    
	if( (return_code != SQLITE_OK) && (return_code != SQLITE_ROW))
	{
		sqlite3_finalize(result);
		return 0;
    }
	
	buffer = (unsigned char *) (sqlite3_column_text(result, 0) );
	size_return_char = sqlite3_column_bytes(result, 0);
    *return_char = (unsigned char *) calloc(size_return_char,
    				sizeof(unsigned char));
    
    				
    if(! *return_char)
		return 0;
	
	while(counter < size_return_char)
	{
		(*return_char)[counter] = buffer[counter];
		counter++;
	}
	sqlite3_finalize(result);
	return (size_return_char);
}


int sql_get_aes_key(sqlite3 *database, int keyname, 
							unsigned char **return_char)
{
	sqlite3_stmt *result;
	char sql_command[4096];
	unsigned char *buffer;
	int return_code, size_return_char, counter = 0;
    
	snprintf(sql_command, 4095,"SELECT value FROM aes_keys \
    WHERE name=%d;", keyname);
    
	return_code = sql_exec_prepare(database, sql_command, &result);

	if( return_code != 0 )
		return 0;

	return_code = sqlite3_step(result);
	
	if( (return_code != SQLITE_OK) && (return_code != SQLITE_ROW))
		return 0;
    	
    buffer = (unsigned char *) (sqlite3_column_text(result, 0) );
    size_return_char = sqlite3_column_bytes(result, 0);
    
    if(! return_char)
    	return size_return_char;
    	
    *return_char = (unsigned char *) calloc(size_return_char+1,
    				sizeof(unsigned char));
    
    		
    if(! *return_char)
		return 0;
	
	while(counter < size_return_char)
	{
		(*return_char)[counter] = buffer[counter];
		counter++;
	}
	sqlite3_finalize(result);
	return (size_return_char);
}


int sql_get_certificate(sqlite3 *database, unsigned char **return_char)
{
	sqlite3_stmt *result;
	char sql_command[4096];
	unsigned char *buffer;
	int return_code, size_return_char, counter = 0;
    
	snprintf(sql_command, 4095,"SELECT value FROM certificates \
    WHERE start_timestamp <= DATETIME('now') \
    AND end_timestamp >=  DATETIME('now');");
    
	return_code = sql_exec_prepare(database, sql_command, &result);
    
	if(return_code !=0 ){
		return 0;
	}
    
	return_code = sqlite3_step(result);
	if( (return_code != SQLITE_OK) && (return_code != SQLITE_ROW))
		return 0;
    
	size_return_char = sqlite3_column_bytes(result, 0);
    *return_char = (unsigned char *) calloc(size_return_char,
    				sizeof(unsigned char));
    buffer = (unsigned char *) (sqlite3_column_text(result, 0) );
    				
    if(! *return_char)
		return 0;
	
	while(counter < size_return_char)
	{
		(*return_char)[counter] = buffer[counter];
		counter++;
	}
	sqlite3_finalize(result);

	return (size_return_char);
}


int sql_get_private_key(sqlite3 *database, int public_id, 
					unsigned char **return_char)
{
	sqlite3_stmt *result;
	char sql_command[4096];
	unsigned char *buffer;
	int return_code, size_return_char, counter = 0;
    
	snprintf(sql_command, 4095,"SELECT value FROM private_keys \
            WHERE cert_id=%d;", public_id);
	return_code = sql_exec_prepare(database, sql_command, &result);

	if( return_code !=0 )
		return 0;
    
	return_code = sqlite3_step(result);
    
	if( (return_code != SQLITE_OK) && (return_code != SQLITE_ROW))
		return 0;
    
	size_return_char = sqlite3_column_bytes(result, 0);
    *return_char = (unsigned char *) calloc(size_return_char,
    				sizeof(unsigned char));
    buffer = (unsigned char *) (sqlite3_column_text(result, 0) );
    				
    if(! *return_char)
		return 0;
	

	while(counter < size_return_char)
	{
		(*return_char)[counter] = buffer[counter];
		counter++;
	}
	sqlite3_finalize(result);

	return (size_return_char);
}

