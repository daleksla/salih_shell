#define _GNU_SOURCE
#include <unistd.h> // working dir, symbolic constants, exec*
#include <sys/wait.h> // wait
#include <signal.h> // signal handling
#include <stddef.h> // type aliases
#include <stdio.h> // IO ops
#include <stdlib.h> // environmental variables
#include <string.h> // memset

#include "variables.h" // custom environment setup
#include "parser.h" // useful functions to help parse string input
#include "display.h" // functions, enums and typedef's to control how text is displayed

/** @brief Source code to manage shell
  * @author Salih Ahmed
  * @date 2 Aug 2021 **/

/** main control code
  * @param int correlating to number to arguments
  * @param arguments themselves
  * @return int as exit code **/
int main(int, char**) ;

int main(int argc, char** argv)
{
	/* variable setup(s) & config */
	WordStore word_store ;
	VariableStore variable_store ;
	
	word_store_init(&word_store) ;
	variable_store_init(&variable_store) ;
	int init = 0 ;
	declare_variable("?", (const void*)&init, 'i', &variable_store) ; Variable* return_status_variable = find_variable("?", &variable_store) ; // store return_status
	declare_variable("#", (const void*)&init, 'i', &variable_store) ; Variable* pos_arg_count_variable = find_variable("?", &variable_store) ; // store pos. arg count 
	
	/* main functionality */
	while(1)
	{	
		/* Initialise variables & display */
		word_store_refresh(&word_store) ;
		int return_status = 0 ; // will be used at end of loop for execution status
		char buffer[4096] ; // serves as input buffer
		size_t buffer_size = 4096 ; // stores available size in buffer for input
		memset(buffer, EOF, buffer_size) ;
		set_display(texture_bold, foreground_white, background_magenta) ;
		printf("%s%s%s", getenv("USER"), ":", get_current_dir_name()) ;
		reset_display() ;
		printf("%s", "$ ") ;
		
		/* Get, store & parse input */
		if(fgets(buffer, buffer_size, stdin) == NULL) // if read isn't sucessful / is manually quit
		{
			break ; // terminate shell
		}

		parse(buffer, buffer_size, &word_store) ; // load word_store up with parsed data
		substitute_variables(&word_store, &variable_store) ; // now replace any variables
									// seperated this from parse function as, in single line mode, you obviously can't save and use a variable at the same time
									// plus parse function is too big
		
		/* Execute instructions */
		if(word_store.word_count >= 1) // if not empty input
		{
			if(strcmp(word_store.words[0], "declare") == 0)
			{
				Variable* var = find_variable(word_store.words[1], &variable_store) ;
				if(!var)
				{
					// parse stupid input / flags, var_name, getting datatype (if none, its a string), data
					// declare_variable
				}				
				else {
					// update_variable_type
					// if data is provided, update_variable_data
				}
			}
			else if(1!=1/* it is just variable=data*/)
			{
				Variable* var = find_variable(word_store.words[1], &variable_store) ;
				if(!var)
				{
					// declare_variable, as string, with value if provided
				}				
				else {
					// just update_variable_data
				}				
			}
			else if(strcmp(word_store.words[0], "cd") == 0)
			{
				if(word_store.word_count > 2)
				{
					printf("%s\n", "Too many arguments provided!") ;
					return_status = -1 ;
				}
				
				return_status = chdir(word_store.words[1]) ; // will return 0 or -1 depending on success
				if(return_status != 0)
				{
					printf("%s\n", "Invalid directory!") ;
					// return_status would've already been set to -1, no need to change
				}
			}
			else if(strcmp(word_store.words[0], "echo") == 0)
			{
				for(size_t i = 0 ; i < word_store.word_count - 1 ; ++i) // word count includes command 'echo'
				{
					printf("%s ", word_store.words[i+1]) ;					
				}
				
				printf("%c", '\n') ; // flush buffer, end w newline	
			}
			else { // ie just execute a regular command
				int pid = fork() ;
				if(pid == 0)
				{
 					if(execvp(word_store.words[0], word_store.words) == -1)
 					{
 						printf("%s%s%s\n", "Command `", word_store.words[0], "` could not be executed!") ;
 						kill(getpid(), SIGTERM) ; // since process couldn't be taken over by executable, kill it manually	
 						// may want to implement a 'did you mean?' feature
 					}
				}
				else {
					wait(&return_status) ; // saves return status of child process
				}
			}
		
			/* set inherent shell variables recording execution information */
			update_variable_data(return_status_variable, (const void*)&return_status) ; // store return_status
			int positional_param_count = word_store.word_count - 1 ; // the arg count variable only includes positional parameter counts - ie ignore first word / main command
			update_variable_data(pos_arg_count_variable, (const void*)&positional_param_count) ; // store pos. arg count
		}
	
	}

	/* EndOfProgram */
	reset_display() ;
	printf("%c", '\n') ;
	word_store_fini(&word_store) ;
	variable_store_fini(&variable_store) ;
	return 0 ;
}
  
