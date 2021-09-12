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
	word_store_init(&word_store) ;
	variable_store_init(&variable_store) ;
	int init = 0 ;
	declare_variable("?", (const void*)&init, 'i', &variable_store) ;
	declare_variable("#", (const void*)&init, 'i', &variable_store) ;
	
	FILE* file_stream = stdin ;
	if(argc >= 2)
	{
		file_stream = fopen(argv[1], "r") ;
		if(!file_stream)
		{
			fprintf(stdout, "File `%s` does not exist!\n", argv[1]) ;
			return -1 ;
		}
	}

	/* main functionality */
	while(1)
	{	
		/* Request input, tinker display */
		if(file_stream == stdin)
		{
			set_display(texture_bold, foreground_white, background_magenta) ;
			char* dir = get_current_dir_name() ; 
			fprintf(stdout, "%s%c%s", getenv("USER"), ':', dir) ;
			free(dir) ;
			reset_display() ;
			printf("%s", "$ ") ;
		}
		
		/* Get, store & parse input */
		word_store_refresh(&word_store) ;
		input_buffer_refresh(&input_buffer) ;
		if(read_input(file_stream, &input_buffer) == -1) break ; // if EOF, end 
		if(dissect(input_buffer.current_start, input_buffer.size - (input_buffer.current_end - input_buffer.buffer), &word_store) == -1) continue ; // load word_store up with parsed data
															       // if no words were found, next line
		substitute_variables(&word_store, &variable_store) ; // now replace any variables
									// seperated this from parse function as, in single line mode, you obviously can't save and use a variable at the same time
									// plus parse function is too big

		/* Execute instructions */
		run_statement(&word_store, &variable_store, &input_buffer) ;
	}

	/* EndOfProgram */
	if(file_stream == stdin)
	{
		reset_display() ;
		fprintf(stdout, "%s%c", "Exiting...", '\n') ;
	}
	fclose(file_stream) ;
	input_buffer_fini(&input_buffer) ;
	word_store_fini(&word_store) ;
	variable_store_fini(&variable_store) ;
	return 0 ;
}
  
