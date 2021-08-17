#define _GNU_SOURCE
#include <unistd.h> // working dir, symbolic constants, exec*
#include <sys/wait.h> // wait
#include <signal.h> // signal handling
#include <stddef.h> // type aliases
#include <stdio.h> // IO ops
#include <stdlib.h> // environmental variables
#include <string.h> // memset

#include "environment.h" // custom environment setup
#include "parser.h" // useful functions to help parse string input
#include "display.h" // functions, enums and typedef's to control how text is displayed

/** @brief Source code to manage shell
  * @author Salih Ahmed
  * @date 2 Aug 2021 **/

/** main control code
  * @param int correlating to number to arguments
  * @param arguments themselves
  * @param environmental variables
  * @return int as exit code **/
int main(int, char**, char**) ;

int main(int argc, char** argv, char** envp)
{
	/* variable setup(s) & config */
	Environment env ;
	WordStore word_store ;
	VariableStore variable_store ;
	
	environment_init(&env) ;
	word_store_init(&word_store) ;
	variable_store_init(&variable_store) ;
	
	/* main functionality */
	while(1)
	{
		word_store_refresh(&word_store) ;
	
		// Initialise variables
		char buffer[1024] ; // serves as input buffer
		memset(buffer, EOF, 1024) ;
		size_t available = 1024 ; // stores available size in buffer for input
		
		// Get input
		set_display(texture_bold, foreground_white, background_magenta) ;
		printf("%s%s%s", env.USER, ":", env.WORKING_DIRECTORY) ;
		reset_display() ;
		printf("%s", "$ ") ;
		
		// Parse input
		if(!fgets(buffer, available, stdin)) // if read isn't sucessful
		{				      // and if it fails ...
			break ;                      // try again
		}
		
		if(!parse(buffer, available, &word_store)) // load word_store up with parsed data
		{				           // and if it fails ...
			continue ;                        // restart loop
		}

		// Execute instructions
		// first, read through words. scan for keywords. assign enum declaring statement-type
		// look at visual basic dialect example
		//if() // scan words, 
		{ // ie if statement_type == 
			// check for shell specific commands
			if(strcmp(word_store.words[0], "cd") == 0)
			{
			
				if(word_store.word_count > 1)
				{
					printf("%s\n", "Too many arguments provided!") ;
				}
				else if(!change_directory(&env, word_store.words[1]))
				{
					printf("%s\n", "Invalid directory!") ;
				}
			
			}
			else if(strcmp(word_store.words[0], "echo") == 0)
			{
			
				for(size_t i = 0 ; i < word_store.word_count ; ++i)
				{
					printf("%s", word_store.words[i+1]) ;
				}
				
				printf("%c", '\n') ; // flush buffer, end w newline
				
			}
			else if(strcmp(word_store.words[0], "quit") == 0)
			{
			
				break ; // quit loop, EOP
				
			}
			else { // ie just execute a regular command
			
				int pid = fork() ;
				int return_status ;
				if(pid == 0)
				{
 					if(execvpe(word_store.words[0], word_store.words, envp) == -1)
 					{
 						printf("%s%s%s\n", "Command `", word_store.words[0], "` could not be executed!") ;
 						kill(getpid(), SIGTERM) ; // since process couldn't be taken over by executable, kill it manually 					
 						// may want to implement a 'did you mean?' feature
 					}
				}
				else {
					wait(&return_status) ;
					store_variable("?", &return_status, 'i', &variable_store) ;
					printf("immediate code: %d\n", return_status) ;
					printf("Return code: %d\n", *(int*)variable_store.variables[0].value) ;
				}

			}
		}
			
	}
	
	/* EndOfProgram */
	reset_display() ;
	printf("%c", '\n') ;	
	environment_fini(&env) ;
	word_store_fini(&word_store) ;
	variable_store_fini(&variable_store) ;
	return 0 ;
}
  
