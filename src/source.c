#define _GNU_SOURCE
#include <signal.h> // signal handling
#include <stdio.h> // IO ops
#include <stdlib.h> // environmental variables, fopen
#include <unistd.h> // get_current_dir_name

#include "aliases.h" // access user-defined aliases
#include "variables.h" // structs and functions to access local and env environments
#include "parser.h" // useful functions to help parse string input
#include "execution.h" // functions relating to executing variables
#include "display.h"

/** @brief Source code to manage shell
  * @author Salih Ahmed
  * @date 2 Aug 2021 **/

/** main control code
  * @param int correlating to number to arguments
  * @param arguments themselves
  * @return int as exit code **/
int main(int argc, char** argv)
{
	/* setup */
	InputBuffer input_buffer ;
	WordStore word_store ;
	VariableStore variable_store ;
	AliasStore alias_store ;
	
	input_buffer_init(&input_buffer) ;
	word_store_init(&word_store, 0) ;
	variable_store_init(&variable_store) ;
	int init = 0 ;
	declare_variable("?", (const void*)&init, 'i', &variable_store) ;
	declare_variable("#", (const void*)&argc, 'i', &variable_store) ;
	alias_store_init(&alias_store) ;
	
	declare_alias("ls", "ls --color=auto", &alias_store) ;
	declare_alias("grep", "grep --color=auto", &alias_store) ;
	
	if(argc >= 2)
	{
		if(argv[1][0] != '-')
		{
			input_buffer.src = fopen(argv[1], "r") ;
			if(!input_buffer.src)
			{
				fprintf(stdout, "File `%s` does not exist!\n", argv[1]) ;
				return -1 ;
			}
		} 
		else {
			for(int i = 1 ; i < argc ; ++i) // from first actual arg
			{
				if(argv[i-1][0] != '-') 
				{
					fprintf(stderr, "Option `%s` is invalid! (see `man salih_shell`)\n", argv[i-1]) ;
					return -1 ;
				}
				// here, use a bunch of if statements to determine what to do
			}
		}
	}

	/* main functionality */
	int read_rets = 0, run_rets = 0 ;
	while(read_rets != -1) // if no EOF
	{
		read_rets = read_manager(&word_store, &input_buffer) ;
		if(word_store.word_count)
		{
			run_rets = run_manager(&word_store, &variable_store, &alias_store, &input_buffer) ;
			if(input_buffer.src != stdin && run_rets != 0)
			{
				break ;
			}
		}
	}

	/* EndOfProgram */
	if(input_buffer.src == stdin)
	{
		reset_display() ;
		fprintf(stdout, "%s%c", "Exiting...", '\n') ;
	}
	input_buffer_fini(&input_buffer) ;
	word_store_fini(&word_store) ;
	variable_store_fini(&variable_store) ;
	alias_store_fini(&alias_store) ;
	return 0 ;
}
  
