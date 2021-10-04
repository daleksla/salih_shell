#define _GNU_SOURCE
#include <unistd.h> // working dir, symbolic constants, exec*, dup, dup2
#include <stddef.h> // type aliases
#include <stdio.h> // IO ops
#include <sys/wait.h> // wait
#include <string.h> // strcmp, strchr, memchr
#include <stdlib.h> // setenv
#include <fcntl.h> // open

#include "parser.h" // WordStore, find_text
#include "variables.h" // VariableStore, variable storing functionality
#include "execution.h"

/** @brief Functionality relating to executing different types of statements
  * @author Salih Ahmed
  * @date 3 September 2021 **/

int run_manager(WordStore* word_store, VariableStore* variable_store, AliasStore* alias_store, InputBuffer* input_buffer)
{
	int run_rets = 0 ;
	
	if(strcmp(word_store->words[0], "if") == 0)
	{
		substitute_variables(word_store, variable_store) ; // now replace any variables
		substitute_aliases(word_store, alias_store) ; // replace with store aliases
		run_rets = run_if(word_store, variable_store, alias_store, input_buffer) ;
	}
	else {
		substitute_variables(word_store, variable_store) ; // now replace any variables
		substitute_aliases(word_store, alias_store) ; // replace with store aliases
		run_rets = run_statement(word_store, variable_store, alias_store, input_buffer) ;
	}
	
	return run_rets ;
}

int run_if(WordStore* word_store, VariableStore* variable_store, AliasStore* alias_store, InputBuffer* input_buffer)
{
	++word_store->words ;
	--word_store->word_count ;
	substitute_variables(word_store, variable_store) ; // now replace any variables
	substitute_aliases(word_store, alias_store) ; // replace with store aliases
	run_statement(word_store, variable_store, alias_store, input_buffer) ;
	--word_store->words ;
	++word_store->word_count ;
	
	Variable* var = find_variable("?", variable_store) ; // find exit status of command run
	int rets = *(int*)var->value ; // see success of condition
	
	if(rets == 0) // if condition of statement is ran successfully (zero exit code)
	{
		/* Run statement(s) until one of the if statement or till one of the statements running have a syntax error */
		while(strcmp(word_store->words[0], "fi") != 0 || rets != 0)
		{	
			rets = read_manager(word_store, input_buffer) ;
			if(rets == 0)
			{
				rets = run_manager(word_store, variable_store, alias_store, input_buffer) ;
			}
			else if(rets == -1)
			{
				fprintf(stderr, "%s\n", "Error: EOF detected before valid delimeters / statements!") ;
			}
			else {
				continue ;
			}
		}
	}
	
	return rets ;
}

int run_statement(WordStore* word_store, VariableStore* variable_store, AliasStore* alias_store, InputBuffer* input_buffer)
{
	int return_status ;

	/* check for special commands, which will change how we execute the statement (eg pipes) */
	char* pipe_ptr = NULL ;
	char* fs_ptr = NULL ;
	size_t i ; // record where to split word input
	for(i = 0 ; i < word_store->word_count ; ++i)
	{
		pipe_ptr = (word_store->words[i][0] == '|' ? word_store->words[i] : NULL) ;
		fs_ptr = (word_store->words[i][0] == '>' ? word_store->words[i] : NULL) ;
		if(pipe_ptr || fs_ptr)
		{	
			while(word_store->word_count-1 == i)
			{
				fprintf(stdout, "%c ", '<') ;
				//printf("WCIN: %zu\n", word_store->word_count) ;
				input_buffer->current_start = input_buffer->current_end+1 ; // add to the end of current input
				if(read_input(input_buffer) != 0) // if user outright sends EOFs
				{
					fprintf(stderr, "Error: special symbol `%c` requires one-word minimum of text before and after\n", (pipe_ptr ? '|' : '>')) ;
					return -1 ;
				}
				if(dissect(input_buffer, word_store) != 0)
					continue ;
			}
			pipe_ptr = (word_store->words[i][0] == '|' ? word_store->words[i] : NULL) ;
			fs_ptr = (word_store->words[i][0] == '>' ? word_store->words[i] : NULL) ;
			break ; 
		}
	}

	if(pipe_ptr) // if '|' symbol detected first
	{
		/* first half of piping process - run first seen statement, store stdoutput in pipe */
		*pipe_ptr = '\0' ; // replace pipe symbol with null terminator to seperate before+after (if needed)

		WordStore half_store ;
		half_store._size = 0 ;
		
		half_store.words = word_store->words ;
		for(half_store.word_count = 0 ; half_store.word_count != (const size_t)i ; ++half_store.word_count) ;
		//(stdout, "SALIH SAYS: %s\n", half_store.words[0]) ;
		word_store->words[i] = NULL ;

		int filedes[2] ; // create array to store pipes r+w fds
		pipe(filedes) ; // pipe syscall
		int fd_old = dup(fileno(stdout)) ; 
		dup2(filedes[1], fileno(stdout)) ; // redirect stdout to write end of pipe
		return_status = run_statement(&half_store, variable_store, alias_store, input_buffer) ;
		dup2(fd_old, fileno(stdout)) ; // redirect stdout back to stdout
		close(filedes[1]) ;  // now close handle to write end of pipe

		if(return_status != 0)
		{
			return return_status ;
		}
		
		/* second half of piping process - recursively run statement, send pipe data as stdinput in next process */
		half_store.words = word_store->words + i + 1 ;
		//fprintf(stdout, "SALIH SAYS: %s\n", half_store.words[0]) ;
		half_store.word_count = 0 ;
		for(size_t j = i+1 ; j != word_store->word_count ; ++j) ++half_store.word_count ;
		
		fd_old = dup(fileno(stdin)) ; 
		dup2(filedes[0], fileno(stdin)) ; // now redirect stdin to our pipe, such that 'input' will come from pipe's data
		close(filedes[0]) ; // close read fd of pipe, sealing input
		return_status = run_statement(&half_store, variable_store, alias_store, input_buffer) ;
		dup2(fd_old, fileno(stdin)) ; // reset stdin (ie stdin -> stdin), rather than from pipe
		
		*pipe_ptr = '|' ; // when all is said and done, restore pipe symbol
		word_store->words[i] = pipe_ptr ;
	}
	else if(fs_ptr) // if '>' symbol detected first
	{
		*fs_ptr = '\0' ;
		
		const char* filename = word_store->words[word_store->word_count-1] ;
		int fd = (fs_ptr[1] == '>' ? open(filename, O_APPEND | O_WRONLY | O_CREAT, 0777) : open(filename, O_WRONLY | O_CREAT, 0777)) ;
		int old_fd = dup(fileno(stdout)) ;
		dup2(fd, fileno(stdout)) ;
		
		WordStore half_store ;
		half_store._size = 0 ;
		half_store.words = word_store->words ;
		for(half_store.word_count = 0 ; half_store.word_count != (const size_t)i ; ++half_store.word_count) ;
		word_store->words[i] = NULL ;

		return_status = run_statement(&half_store, variable_store, alias_store, input_buffer) ;
		
		dup2(old_fd, fileno(stdout)) ;
		close(fd) ;
		
		*fs_ptr = '>' ; // when all is said and done, restore > symbol
	}
	else {	
		return_status = exec_statement(word_store, variable_store, alias_store) ; // no need to create seperate wordstore containing seperate statement, just pass full statement
	}
	
	return return_status ;
}

int exec_statement(WordStore* word_store, VariableStore* variable_store, AliasStore* alias_store)
{
	int return_status = 0 ;
	if(strcmp(word_store->words[0], "source") == 0)
	{
		if(word_store->word_count < 2)
		{
			fprintf(stderr, "%s %s\n", "Too few arguments!", "Format: source `file#1`...`file#n`") ;
			return_status = -1 ;
			goto end ;
		}

		FILE* fsrc = fopen(word_store->words[1], "r") ;
		if(!fsrc)
		{
			fprintf(stderr, "File`%s` not found\n", word_store->words[1]) ;
			return_status = -1 ;
			goto end ;
		}	

		InputBuffer input_buffer_new ;
		input_buffer_init(&input_buffer_new) ;
		input_buffer_new.src = fsrc ; 
		WordStore word_store_new ;
		word_store_init(&word_store_new, 0) ;
			
		int rets = read_manager(&word_store_new, &input_buffer_new) ;
		rets = run_manager(&word_store_new, variable_store, alias_store, &input_buffer_new) ;
		if(rets != 0)
		{
			fprintf(stderr, "Error occured during sourcing of file `%s`\n", word_store->words[1]) ;
			return_status = -1 ;
		}
		input_buffer_fini(&input_buffer_new) ;
	}
	else if(strchr(word_store->words[0], '=') != NULL)
	{	
		char* var_name = word_store->words[0] ;
	
		char* data = strchr(word_store->words[0], '=') ;
		*data = '\0' ; ++data ;

		char** i = environ ; // char** of all environmental variables
		while(*i != NULL)
		{
			//fprintf(stdout, "env iter: %s\n", *i) ;
			char* point = strchr(*i, '=') ;
			*point = '\0' ;
			if(strcmp(*i, var_name) == 0)
			{
				*point = '=' ;
				setenv(var_name, data, 1) ;
				break ;
			}
			*point = '=' ;
			++i ;
		}
		
		if(!*i)
		{
			Variable* var_itself = find_variable(var_name, variable_store) ;		

			int data_type = 's' ;
			if(var_itself)
			{
				data_type = var_itself->type ;
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
		
		--data ; *data = '=' ; // restore equals sign
	}
	else if(strcmp(word_store->words[0], "declare") == 0)
	{
		if(word_store->word_count < 2 || word_store->word_count > 3)
		{
			fprintf(stderr, "Incorrect number of arguments passed! Format %s\n", "declare -t/--type var_name=val") ;
			return_status = -1 ;
			goto end ;	
		}
		 
		// get variable name
		const char* var_name = NULL ;
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

		char** i = environ ; // char** of all environmental variables
		while(*i != NULL)
		{
			fprintf(stdout, "env iter: %s\n", *i) ;
			char* point = strchr(*i, '=') ;
			*point = '\0' ;
			if(strcmp(*i, var_name) == 0)
			{
				*point = '=' ;
				setenv(var_name, data, 1) ;
				break ;
			}
			*point = '=' ;
			++i ;
		}
		
		if(!*i)
		{	
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
					fprintf(stderr, "`%s` starts with invalid option token (use -<type abbreviation> or --<type>)!\n", word_store->words[1]) ;
					return_status = data_type ;
					goto end ;
				}
				else if(data_type == -3)
				{
					fprintf(stderr, "`%s` is not a valid data-type specification!\n", word_store->words[1]) ;
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
	}
	else if(strcmp(word_store->words[0], "export") == 0)
	{
		const char* format = "Format: export var_name" ;
		if(word_store->word_count < 2)
		{
			fprintf(stderr, "%s %s\n", "Too few arguments provided!", format) ;
			return_status = -1 ;
			goto end ;
		}
		else if(word_store->word_count > 2)
		{
			fprintf(stderr, "%s %s\n", "Too many arguments provided!", format) ;
			return_status = -1 ;
			goto end ;
		}
		
		Variable* var = find_variable(word_store->words[1], variable_store) ; 
		if(var == NULL)
		{
			fprintf(stderr, "%s\n", "Cannot export undeclared variable!") ;
			return_status = -2 ;
			goto end ;
		}
		
		setenv(var->identifier, var->value, 1) ;
	}
	else if(strcmp(word_store->words[0], "cd") == 0)
	{
		if(word_store->word_count < 2)
		{
			fprintf(stderr, "%s\n", "Too few arguments provided!") ;
			return_status = -1 ;
			goto end ;
		}
	
		if(word_store->word_count > 2)
		{
			fprintf(stderr, "%s\n", "Too many arguments provided!") ;
			return_status = -2 ;
			goto end ;
		}
		
		return_status = chdir(word_store->words[1]) ; // will return 0 or -1 depending on success
		if(return_status != 0)
		{
			fprintf(stderr, "%s\n", "Invalid directory!") ;
			return_status = -3 ;
		}
	}
	else if(strcmp(word_store->words[0], "echo") == 0)
	{
		for(size_t i = 0 ; i < word_store->word_count - 1 ; ++i) // word count includes command 'echo'
		{
			fprintf(stdout, "%s ", word_store->words[i+1]) ;					
		}
		
		fprintf(stdout, "%c", '\n') ; // flush buffer, end w newline	
	}
	else { // ie just execute a regular command
		int pid = fork() ;
		if(pid == 0)
		{
				if(execvp(word_store->words[0], word_store->words) == -1)
				{
					fprintf(stderr, "%s%s%s\n", "Command `", word_store->words[0], "` could not be executed!") ;
					return_status = -1 ;
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
	post_statement((const int)return_status, variable_store) ;
	return return_status ;
}

inline void post_statement(const int return_status, VariableStore* variable_store)
{
	/* set inherent shell variables recording execution information */
	update_variable_data(find_variable("?", variable_store), (const void*)&return_status) ; // store return_status
}
