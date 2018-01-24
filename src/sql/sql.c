#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string.h>


int sql_open_database(sqlite3 **db, const char *databasename)
{
	int return_code;
    
	return_code = sqlite3_open_v2(databasename, db, 
                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
                
	if(return_code != 0)
	{
		fprintf(stderr,"Impossible d'ouvrir la base de donnée %s: %s",
			databasename, sqlite3_errmsg(*db));
        return (-1);
    }
    
    return (0);
}


int sql_exec(sqlite3 *db, const char *sql_command)
{
    char *error_msg = NULL;
    int rc;
    
    rc = sqlite3_exec(db, sql_command, NULL, 0, &error_msg);
    
    
    if( rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur d'éxecution de la commande %s: %s",
            sql_command, error_msg);
        sqlite3_free(error_msg);
        return (-1);
    }
    
    return (0);
}


int sql_exec_prepare(sqlite3 *db, const char *sql_command, 
				sqlite3_stmt **result)
{
    int rc;
    
    rc = sqlite3_prepare_v2(db, sql_command, strlen(sql_command),
             result, NULL);
    
    if( rc != SQLITE_OK)
    {
        fprintf(stderr, "Erreur d'éxecution de la commande %s",
         				sql_command);
        return (-1);
    }
    
    return (0);
}

char * escape_special_character(const char *str)
{
	size_t len_str;
	int i;
	char *ret_str; 
    
	i = 0;
	len_str = strlen(str);	
	ret_str = calloc(len_str+1, sizeof(char));
    
	if(ret_str == NULL)
		return NULL;
    	
	while(i<len_str){
		if ( str[i] == '\'' )
			ret_str[i] = 27;
		else
        	ret_str[i] = str[i];
        
        i++;
    }
    
	return (ret_str);
}
