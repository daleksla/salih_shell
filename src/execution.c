#define _GNU_SOURCE
#include <unistd.h> // working dir, symbolic constants, exec*
#include <stddef.h> // type aliases
#include <stdio.h> // IO ops
#include <sys/wait.h> // wait
#include <string.h> // strcmp, strchr
#include <stdlib.h> // setenv

#include "parser.h" // WordStore
#include "variables.h" // VariableStore, variable storing functionality
#include "execution.h"

/** @brief Functionality relating to executing different types of statements
  * @author Salih Ahmed
  * @date 3 September 2021 **/

inline void set_shell_variables(const int return_status, const int positional_param_count, VariableStore* variable_store)
{
	/* set inherent shell variables recording execution information */
	update_variable_data(find_variable("?", variable_store), (const void*)&return_status) ; // store return_status
	update_variable_data(find_variable("#", variable_store), (const void*)&positional_param_count) ; // store pos. arg count	
}

int exec_statement(WordStore* word_store, VariableStore* variable_store)
{
	int return_status = 0 ;	
	if(strcmp(word_store->words[0], "declare") == 0)
	{
		if(word_store->word_count < 2 || word_store->word_count > 3)
		{
			printf("Incorrect number of arguments passed! Format %s\n", "declare -t/--type var_name=val") ;
			return_status = -1 ;
			goto end ;	
		}
	
		 
		// get variable name
		const char* var_name ;
		if(word_store->word_count == 2)
			var_name = word_store->words[1] ;
		else 
			var_name = word_store->words[2] ;
			
		// see whether there is data to assign, else create a default
		char def_data[4] = {'\0', '\0', '\0', '\0'} ; 
		char* data = strchr(var_name, '=') ;
		if((data = strchr(var_name, '=')) != NULL)
		{
			*data = '\0' ;
			++data ;
		}
		else {
			data = def_data ;
		}
					
		Variable* var_itself = find_variable(var_name, variable_store) ;
			
		// see what data type we need to assign, else set a default
		// string if declare is used for first time w variable name, else use old assigneds
		int data_type = 's' ;
		if(word_store->word_count == 2 && var_itself)
		{
			data_type = var_itself->type ;
		}			
		else if(word_store->word_count == 3)
		{
			data_type = parse_variable_type(word_store->words[1]) ;
		
			if(data_type == -2)
			{
				printf("`%s` starts with invalid option token (use -<type abbreviation> or --<type>)!\n", word_store->words[1]) ;
				return_status = data_type ;
				goto end ;
			}
			else if(data_type == -3)
			{
				printf("`%s` is not a valid data-type specification!\n", word_store->words[1]) ;
				return_status = data_type ;
				goto end ;
			}
		}
			
		// set data
		if(var_itself) // if variable is found
		{
			update_variable_type(var_itself, data_type) ;
			update_variable_data(var_itself, &data) ;
		}
		else {
			declare_variable(var_name, &data, data_type, variable_store) ;					
		}
	}
	else if(strcmp(word_store->words[0], "export") == 0)
	{
		const char* format = "Format: export var_name" ;
		if(word_store->word_count < 2)
		{
			printf("%s %s\n", "Too few arguments provided!", format) ;
			return_status = -1 ;
			goto end ;
		}
		else if(word_store->word_count > 2)
		{
			printf("%s %s\n", "Too many arguments provided!", format) ;
			return_status = -1 ;
			goto end ;
		}
		
		Variable* var = find_variable(word_store->words[1], variable_store) ; 
		if(var == NULL)
		{
			printf("%s\n", "Cannot export undeclared variable!") ;
			return_status = -2 ;
			goto end ;
		}
		
		setenv(var->identifier, var->value, 1) ;
	}
	else if(strcmp(word_store->words[0], "cd") == 0)
	{
		if(word_store->word_count > 2)
		{
			printf("%s\n", "Too many arguments provided!") ;
			return_status = -1 ;
			goto end ;
		}
		
		return_status = chdir(word_store->words[1]) ; // will return 0 or -1 depending on success
		if(return_status != 0)
		{
			printf("%s\n", "Invalid directory!") ;
			// return_status would've already been set to -1, no need to change
		}
	}
	else if(strcmp(word_store->words[0], "echo") == 0)
	{
		for(size_t i = 0 ; i < word_store->word_count - 1 ; ++i) // word count includes command 'echo'
		{
			printf("%s ", word_store->words[i+1]) ;					
		}
		
		printf("%c", '\n') ; // flush buffer, end w newline	
	}
	else { // ie just execute a regular command
		int pid = fork() ;
		if(pid == 0)
		{
				if(execvp(word_store->words[0], word_store->words) == -1)
				{
					printf("%s%s%s\n", "Command `", word_store->words[0], "` could not be executed!") ;
					kill(getpid(), SIGTERM) ; // since process couldn't be taken over by executable, kill it manually	
					// may want to implement a 'did you mean?' feature
				}
		}
		else {
			wait(&return_status) ; // saves return status of child process
		}
	}
	end: // i would like to make it clear whilst I hate gotos, I had no choice due to the nesting of the if statements
	     // plus it makes the code look cleaner whilst making my bloody life easier
	set_shell_variables((const int)return_status, (const int)word_store->word_count - 1, variable_store) ;
	return return_status ;
}
