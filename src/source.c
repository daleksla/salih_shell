#define _GNU_SOURCE
#include <signal.h> // signal handling
#include <stdio.h> // IO ops
#include <stdlib.h> // environmental variables, fopen
#include <string.h> // memset
#include <unistd.h> // get_current_dir_name

#include "variables.h" // custom environment setup
#include "parser.h" // useful functions to help parse string input
#include "display.h" // functions, enums and typedef's to control how text is displayed
#include "execution.h" // functions relating to executing variables

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
	
	input_buffer_init(&input_buffer) ;
	word_store_init(&word_store, 0) ;
	variable_store_init(&variable_store) ;
	int init = 0 ;
	declare_variable("?", (const void*)&init, 'i', &variable_store) ;
	declare_variable("#", (const void*)&init, 'i', &variable_store) ;
	
	if(argc >= 2)
	{
		input_buffer.src = fopen(argv[1], "r") ;
		if(!input_buffer.src)
		{
			fprintf(stdout, "File `%s` does not exist!\n", argv[1]) ;
			return -1 ;
		}
	}

	/* main functionality */
	while(1)
	{	
		/* Request input, tinker display */
		if(input_buffer.src == stdin)
		{
			const char* USER = getenv("USER") ;
			char* dir = get_current_dir_name() ; // stupid gnu function actually mallocs each time.
			set_display(texture_bold, foreground_white, background_magenta) ;
			fprintf(stdout, "%s%c%s", USER, ':', dir) ;
			reset_display() ; free(dir) ;
			strcmp(USER, "root") == 0 ? fprintf(stdout, "%s", "# ") : fprintf(stdout, "%s", "$ ") ;
		}
		
		/* Get, store & parse input */
		word_store_refresh(&word_store) ;
		input_buffer_refresh(&input_buffer) ;
		
		int rets = read_input(&input_buffer) ;
		//fprintf(stdout, "read_input ret: %d\n", rets) ;
		if(rets != 0) break ; // if EOF / no text at all read, end 
		rets = dissect(&input_buffer, &word_store) ;
		//fprintf(stdout, "dissect ret: %d\n", rets) ;
		if(rets != 0) continue ; // load word_store up with parsed data
					       // if no words were found, next line
		
		//fprintf(stdout, "First of Buffer: %c\n", input_buffer.buffer[0]) ;
		//for(size_t i = 0 ; i < word_store.word_count ; ++i) fprintf(stdout, "Input: %s\n", word_store.words[i]) ;
		substitute_variables(&word_store, &variable_store) ; // now replace any variables
									// seperated this from parse function as, in single line mode, you obviously can't save and use a variable at the same time
									// plus parse function is too big

		/* Execute instructions */
		run_statement(&word_store, &variable_store, &input_buffer) ;
		continue ;
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
	return 0 ;
}
  
