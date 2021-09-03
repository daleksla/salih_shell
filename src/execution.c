#define _GNU_SOURCE
#include <unistd.h> // working dir, symbolic constants, exec*
#include <stddef.h> // type aliases
#include <stdio.h> // IO ops
#include <sys/wait.h> // wait
#include <string.h> // strcmp, strchr

#include "execution.h"

/** @brief Functionality relating to executing different types of statements
  * @author Salih Ahmed
  * @date 3 September 2021 **/

void exec_statement(WordStore* word_store, VariableStore* variable_store, int* return_status)
{
	if(word_store->word_count) // if not empty input
	{
		{	
		if(strcmp(word_store->words[0], "declare") == 0)
		{
			if(word_store->word_count == 2 || word_store->word_count == 3) 
			{
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
						*return_status = data_type ;
						return;
					}
					else if(data_type == -3)
					{
						printf("`%s` is not a valid data-type specification!\n", word_store->words[1]) ;
						*return_status = data_type ;
						return;
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
				
				//printf("Was var created?: %d\n", find_variable(var_name, &variable_store) ? 1 : 0) ;
				//printf("var val?: %s\n", find_variable(var_name, &variable_store)->value) ;
			}
			else {
				printf("Incorrect number of arguments passed! Format %s\n", "declare -t/--type var_name=val") ;
				*return_status = -1 ;
			}
		}
		else if(strcmp(word_store->words[0], "cd") == 0)
		{
			if(word_store->word_count > 2)
			{
				printf("%s\n", "Too many arguments provided!") ;
				*return_status = -1 ;
			}
			else {
				*return_status = chdir(word_store->words[1]) ; // will return 0 or -1 depending on success
				if(*return_status != 0)
				{
					printf("%s\n", "Invalid directory!") ;
					// return_status would've already been set to -1, no need to change
				}
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
				wait(return_status) ; // saves return status of child process
			}
		}
	
		/* set inherent shell variables recording execution information */
		update_variable_data(find_variable("?", variable_store), (const void*)return_status) ; // store return_status
		int positional_param_count = word_store->word_count - 1 ; // the arg count variable only includes positional parameter counts - ie ignore first word / main command
		update_variable_data(find_variable("#", variable_store), (const void*)&positional_param_count) ; // store pos. arg count
		}
	}
}
