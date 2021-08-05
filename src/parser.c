#include <stddef.h> // type aliases
#include <stdio.h> // EOF
#include <stdlib.h> // malloc, free

#include "parser.h"

/** @brief Functionality relating to parsing input
  * @author Salih Ahmed
  * @date 3 Aug 2021 **/
  
void cmd_store_init(CmdStore* cmd_store)
{
	cmd_store->cmd = NULL ;
	cmd_store->_size = 256 ;
	cmd_store->args = malloc(sizeof(char*) * cmd_store->_size) ; // list of max 256 char pointers
	cmd_store->arg_count = 0 ;
}
  
int parse(char* buffer, size_t available, CmdStore* cmd_store)
{		
	char* i = find_text(buffer, available) ;
	
	if(i == NULL) // if no text was found
		return 0 ; // indicate failure

	// if text was found
	cmd_store->cmd = i ; // save it as main command, since it's our first bit of text found
	available -= cmd_store->cmd - buffer ; // calculate number of bytes left in buffer
	i = find_whitespace(i, available) ; // find next gap 
	*i = '\0' ; // place null terminator at first whitespace to create a valid c-string
	
	//printf("cmd: %sEND\n", cmd_store->cmd) ;
	
	available -= i - buffer ;
	// Now let's find arguments (if any)
	while(available > 0)
	// loop is responsible for generating arguments
	// searches for text, finds first instance of whitespace (therefore end of argument) and adds NULL terminator, then repeats
	{
		i = find_text(i, available) ; // now, find next starting point of text
		
		if(i == NULL) // if there's no more text (and therefore no arguments left)
			break ; // then stop searching for them
			
		// if we do find text	
		++cmd_store->arg_count ;			
		cmd_store->args[cmd_store->arg_count-1] = i ; // save it as an argument
		available -= i - buffer ; // calculate what remains of the buffer using the pointers			
		i = find_whitespace(i, available) ; // first find next gap
		*i = '\0' ; // replace whitespace with null termination char to create a valid c-string
		available -= i - buffer ; // calculate what remains of the buffer using the positioned pointers
		
		//printf("Arg#%ld: %sEND\n", cmd_store->arg_count, cmd_store->args[cmd_store->arg_count]) ;
	}
	
	return 1 ; // indicate success
}
 
char* find_text(const char* input, const size_t size)
{
	for(char const* i = input ; i < input + size ; ++i)
	{
		if(*i == ' ' || *i == '\t' || *i == '\n' || *i == '\0') continue ;
		else if(*i == EOF) break ;
		else return (char*)i ;
	}
	return NULL ;
}

char* find_whitespace(const char* input, const size_t size)
{
	for(char const* i = input ; i < input + size ; ++i)
	{
		if(*i == ' ' || *i == '\t' || *i == '\n' || *i == EOF || *i == '\0') return (char*)i ;
	}
	return NULL ;
}

void cmd_store_fini(CmdStore* cmd_store)
{
	free(cmd_store->args) ;
	cmd_store->arg_count = 0 ;
}
