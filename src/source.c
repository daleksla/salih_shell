#define _GNU_SOURCE
#include <unistd.h> // working dir, symbolic constants, exec*
#include <sys/wait.h> // wait
#include <signal.h> // signal handling
#include <stddef.h> // type aliases
#include <stdio.h> // IO ops
#include <stdlib.h> // environmental variables
#include <string.h> // memset
#include <argp.h> // arg parsing

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
	/* setup */
	InputBuffer input_buffer ;
	WordStore word_store ;
	VariableStore variable_store ;
	
	input_buffer_init(&input_buffer) ;
	word_store_init(&word_store) ;
	variable_store_init(&variable_store) ;
	int init = 0 ;
	declare_variable("?", (const void*)&init, 'i', &variable_store) ; Variable* return_status_variable = find_variable("?", &variable_store) ; // store return_status
	declare_variable("#", (const void*)&init, 'i', &variable_store) ; Variable* pos_arg_count_variable = find_variable("?", &variable_store) ; // store pos. arg count 
	
	/* main functionality */
	while(1)
	{	
		/* Request input, tinker display */
		set_display(texture_bold, foreground_white, background_magenta) ;
		printf("%s%c%s", getenv("USER"), ':', get_current_dir_name()) ;
		reset_display() ;
		printf("%s", "$ ") ;
		
		/* Get, store & parse input */
		word_store_refresh(&word_store) ;
		input_buffer_refresh(&input_buffer) ;
		if(read_input(stdin, &input_buffer) == -1) break ; // if EOF, end 
		dissect(input_buffer.buffer, input_buffer.size, &word_store) ; // load word_store up with parsed data
		substitute_variables(&word_store, &variable_store) ; // now replace any variables
									// seperated this from parse function as, in single line mode, you obviously can't save and use a variable at the same time
									// plus parse function is too big

		/* Execute instructions */
		int return_status = 0 ; // will be used at end of loop to store execution status
		if(word_store.word_count) // if not empty input
		{
			if(strcmp(word_store.words[0], "declare") == 0)
			{
				if(word_store.word_count != 3)
				{
					printf("%s\n", "Incorrect number of arguments provided!") ;
					return_status = -1 ;
				}
				else {
					int data_type = 's' ;
					char* var_name = NULL ;

					data_type = parse_variable_type(word_store.words[1]) ;
					printf("VAL:%d\n", data_type) ;
					if(data_type == -2)
					{
						printf("`%s` starts with invalid option token (use -<type abbreviation> or --<type>)!\n", word_store.words[1]) ;
						return_status = data_type ;
					}
					else if(data_type == -3)
					{
						printf("`%s` is not a valid data-type specification!\n", word_store.words[1]) ;
						return_status = data_type ;
					}
					else { // if deducing datatype was a success
						var_name = word_store.words[2] ;
						// try to find '=' and use value right of it, else use default 0/'\0'/NULL
						// find variable name
						// if exists, update variable type, update data
						// if it doesn't, create
					}
				}
			}
			else if(strcmp(word_store.words[0], "cd") == 0)
			{
				if(word_store.word_count > 2)
				{
					printf("%s\n", "Too many arguments provided!") ;
					return_status = -1 ;
				}
				else {
					return_status = chdir(word_store.words[1]) ; // will return 0 or -1 depending on success
					if(return_status != 0)
					{
						printf("%s\n", "Invalid directory!") ;
						// return_status would've already been set to -1, no need to change
					}
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
	printf("%s%c", "Exiting...", '\n') ;
	input_buffer_fini(&input_buffer) ;
	word_store_fini(&word_store) ;
	variable_store_fini(&variable_store) ;
	return 0 ;
}
  
