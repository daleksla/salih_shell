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
	CmdStore cmd_store ;
	
	environment_init(&env) ;
	cmd_store_init(&cmd_store) ;
	
	/* main functionality */
	while(1)
	{
		cmd_store_refresh(&cmd_store) ;
	
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
		
		if(!parse(buffer, available, &cmd_store)) // load cmd_store up with parsed data
		{				           // and if it fails ...
			continue ;                        // restart loop
		}

		// Execute instructions
		if(strcmp(cmd_store.args[0], "cd") == 0)
		{
		
			if(cmd_store.arg_count > 1)
			{
				printf("%s\n", "Too many arguments provided!") ;
			}
			else if(!change_directory(&env, cmd_store.args[1]))
			{
				printf("%s\n", "Invalid directory!") ;
			}
			
		}
		else if(strcmp(cmd_store.args[0], "echo") == 0)
		{
		
			for(size_t i = 0 ; i < cmd_store.arg_count ; ++i)
			{
				printf("%s", cmd_store.args[i+1]) ;
			}
			
			printf("%c", '\n') ; // flush buffer, end w newline
			
		}
		else if(strcmp(cmd_store.args[0], "quit") == 0)
		{
		
			break ; // quit loop, EOP
			
		}
		else { // ie just execute a regular command
		
			int pid = fork() ;
			if(pid == 0)
			{
 				if(execvpe(cmd_store.args[0], cmd_store.args+1, envp) == -1)
 				{
 					printf("%s%s%s\n", "Command `", cmd_store.args[0], "` could not be executed!") ;
 					kill(getpid(), SIGTERM) ; // since process couldn't be taken over by executable, kill it manually 					
 					// may want to implement a 'did you mean?' feature
 				}
			}
			else {
				wait(NULL) ;
			}
			
		}
	}
	
	/* EndOfProgram */
	reset_display() ;
	printf("%c", '\n') ;
	
	environment_fini(&env) ;
	cmd_store_fini(&cmd_store) ;
	return 0 ;
}
  
