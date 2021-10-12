#include <stddef.h> // type aliases
#include <stdio.h> // EOF, fflush
#include <stdlib.h> // malloc, free
#include <string.h> // strcmp, memset, strchr
#include <unistd.h> // read
#include <assert.h> // assert
#include <limits.h> // PATH_MAX

#include "display.h"
#include "parser.h"

/** @brief Functionality relating to parsing input (ie literally dissecting input aswell as understanding and running it)
  * @author Salih Ahmed
  * @date 3 Aug 2021 **/

void input_buffer_init(InputBuffer* input_buffer)
{
	input_buffer->size = 25 ; // starting bytes to allocate
	input_buffer->buffer = malloc(input_buffer->size) ;
	assert(input_buffer->buffer) ; // validate realloc success, else abort
	input_buffer->current_start = input_buffer->buffer ;
	input_buffer->current_end = input_buffer->buffer ;
	input_buffer->src = stdin ;
}

void input_buffer_refresh(InputBuffer* input_buffer)
{
	memset(input_buffer->buffer, EOF, input_buffer->size) ;
	input_buffer->current_start = input_buffer->buffer ;
	input_buffer->current_end = input_buffer->buffer ;
}

int read_input(InputBuffer* input_buffer)
{
	fflush(stdout) ;
	fflush(stderr) ;
	// initial read
	char* temp_start = input_buffer->current_start ;
	
	int was_sucess = (int)fgets(temp_start, input_buffer->size - (temp_start - input_buffer->buffer), input_buffer->src) ;
	if(!was_sucess) // if we couldn't read anything even once
	{
		return -1 ;
	}

	while(strchr(temp_start, '\n') == NULL)  // if theres still text before user hit enter key / new line
	{
		// increase and initialise buffer and adjust pointers
		input_buffer->current_end = strchr(temp_start, '\0') ;
		size_t ptr_diff_start = input_buffer->current_start - input_buffer->buffer ;
		size_t ptr_diff_end = input_buffer->current_end - input_buffer->buffer ;
		input_buffer->size *= 2 ;
		input_buffer->buffer = realloc((void*)input_buffer->buffer, input_buffer->size) ; // original_chars * 2
		assert(input_buffer->buffer) ; // validate realloc success, else abort
		input_buffer->current_start = input_buffer->buffer + ptr_diff_start ;
		input_buffer->current_end = input_buffer->buffer + ptr_diff_end ;
		temp_start = input_buffer->current_end ; // set place to now start writing to
		memset(temp_start, EOF, input_buffer->size / 2) ;
		was_sucess = (int)fgets(temp_start, input_buffer->size / 2, input_buffer->src) ;
	}
	
	input_buffer->current_end = strchr(temp_start, '\n') ;

	return 0 ;
}

void input_buffer_fini(InputBuffer* input_buffer)
{
	free(input_buffer->buffer) ;
	input_buffer->buffer = NULL ;
	input_buffer->size = 0 ;
	input_buffer->current_start = NULL ;
	input_buffer->current_end = NULL ;
	fclose(input_buffer->src) ;
}

//

void word_store_init(WordStore* word_store, const size_t sz)
{
	word_store->_size = (sz ? sz : 25) ;
	word_store->words = malloc(sizeof(char*) * word_store->_size) ; // list of max 256 char pointers
	assert(word_store->words) ;
	word_store->word_count = 0 ;
	word_store_refresh(word_store) ;
}

void word_store_refresh(WordStore* word_store)
{
	for(size_t i = 0 ; i < word_store->word_count ; ++i)
	{
		word_store->words[i] = NULL ;
	}
	word_store->word_count = 0 ;
}

void word_store_add(WordStore* word_store, char* word)
{
	++word_store->word_count ;
	if(word_store->word_count+1 >= word_store->_size) // need one of them to be NULL at the end, so cannot completely fill store
	{
		word_store->_size = (word_store->_size + 1) * 2 ;
		word_store->words = realloc(word_store->words, word_store->_size) ;
		assert(word_store->words) ; // validate realloc success, else abort
		for(size_t i = word_store->word_count ; i < word_store->_size ; ++i)
		{
			word_store->words[i] = NULL ;
		}
	}

	word_store->words[word_store->word_count-1] = word ;
}

void word_store_replace(WordStore* word_store, char* word, const size_t pos)
{
	word_store->words[pos-1] = word ;
}

void word_store_insert(WordStore* word_store, char* word, const size_t pos)
{
	++word_store->word_count ;
	if(word_store->word_count+1 >= word_store->_size) // need one of them to be NULL at the end, so cannot completely fill store
	{
		word_store->_size = (word_store->_size + 1) * 2 ;
		word_store->words = realloc(word_store->words, word_store->_size) ;
		assert(word_store->words) ; // validate realloc success, else abort
		for(size_t i = word_store->word_count ; i < word_store->_size ; ++i)
		{
			word_store->words[i] = NULL ;
		}
	}
	
	for(size_t i = word_store->word_count ; i >= pos ; --i)
	{
		word_store->words[i] = word_store->words[i-1] ;
	}

	word_store->words[pos-1] = word ;
}

int dissect(InputBuffer* input_buffer, WordStore* word_store)
{
	size_t initial_available = input_buffer->current_end - input_buffer->current_start ;
	size_t available = initial_available ;
	char* i = input_buffer->current_start ;
	do {	// loop is responsible for generating arguments
		// searches for content, finds first instance of whitespace (therefore end of argument) and adds NULL terminator, then repeats
		// now, find next starting point of next bit of content 
		i = find_content(i, available) ;
		//fprintf(stdout, "SAKICHAR: %c\n", *i) ;
		
		if(available == initial_available && (i == NULL || *i == '#')) // if we haven't read anything yet
		{								// && if there's no more content or we've hit a comment
				return -1 ; // then stop searching for them
		}
		// else (if we've at least read before)
		if(i == NULL || *i == '#') // if there's no more content or we've hit a comment
			break ; // then stop searching for them
						
		available = initial_available - (i - input_buffer->current_start) ;
				
		// if we do find content...	
		// check whether its a single word, special symbol or double worded word
		// if quotes are being used, treat from start->end quote as single arg
		enum { double_quotes = 34, single_quotes = 39, uptick_quotes = 96 } ; // local enum w ASCII values
		if(*i == double_quotes || *i == single_quotes || *i == uptick_quotes)
		{
			const char quotation = *i ; // store quotation mark
			++i ; // move to next value, since we don't want to read / store quotation 
			word_store_add(word_store, i) ; // save it as an argument
			while(*i != quotation) // keep going till you see the char again
			{
				++i ;
			}
			*i = '\0' ; // replace end quotation with null termination char to create a valid c-string
			++i ; // move to next char
		}
		else if(is_special(*i))
		{
			word_store_add(word_store, i) ; // save it as an argument
			available = initial_available - (i - input_buffer->current_start) ; // calculate what remains of the buffer using the pointers			
			if(is_special(*i))
			{
				char* tmpI = find_whitespace(i, available) ;
				char* tmpII = find_text(i, available) ;
				if(tmpI == NULL && tmpII)
				{
					i = tmpII ;
				}
				else if(tmpI && tmpII == NULL)
				{
					i = tmpI ;
					*i = '\0' ; // replace whitespace with null termination char to create a valid c-string
				}
				else {
					i = (tmpI < tmpII ? tmpI : tmpII) ;
					if(is_whitespace(*i)) *i = '\0' ;
				}
				// if its the pipe symbol, it'll be replaced within the run_statement function temporary will '\0' to seperate words if needed
			}
		}
		else { // either text or special symbol
			word_store_add(word_store, i) ; // save it as an argument
			available = initial_available - (i - input_buffer->current_start) ; // calculate what remains of the buffer using the pointers			

			char* tmpI = find_whitespace(i, available) ;
			char* tmpII = find_special(i, available) ;
			if(tmpI == NULL && tmpII)
			{
				i = tmpII ;
			}
			else if(tmpI && tmpII == NULL)
			{
				i = tmpI ;
				*i = '\0' ; // replace whitespace with null termination char to create a valid c-string
			}
			else {
				i = (tmpI < tmpII ? tmpI : tmpII) ;
				if(is_whitespace(*i)) *i = '\0' ;
			}
		}
		
		available = initial_available - (i - input_buffer->current_start) ; // calculate what remains of the buffer using the positioned pointers
	
	} while (available > 0) ;
	
	return 0 ; // indicate success
}

int is_whitespace(const char i)
{
	if(i == ' ' || i == '\t' || i == '\n' || i == EOF || i == '\0')
	{
		return 1 ;
	}
	return 0 ;
}

int is_special(const char i)
{
	if(i == '|' || i == '>')
	{
		return 1 ;
	}
	return 0 ;
}

char* find_content(const char* input, const size_t size)
{
	for(char const* i = input ; i <= input + size ; ++i)
	{
		if(!is_whitespace(*i)) return (char*)i ;
	}
	return NULL ;
}

char* find_special(const char* input, const size_t size)
{
	for(char const* i = input ; i <= input + size ; ++i)
	{
		if(is_special(*i)) return (char*)i ;
	}
	return NULL ;
}

char* find_text(const char* input, const size_t size)
{
	for(char const* i = input ; i <= input + size ; ++i)
	{
		if(*i == EOF) break ;
		else if(!is_whitespace(*i) && !is_special(*i)) return (char*)i ;
	}
	return NULL ;
}

char* find_whitespace(const char* input, const size_t size)
{
	for(char const* i = input ; i <= input + size ; ++i)
	{
		if(is_whitespace(*i)) return (char*)i ;
	}
	return NULL ;
}

void word_store_fini(WordStore* word_store)
{
	free(word_store->words) ;
	word_store->words = NULL ;
	word_store->_size = 0 ;
	word_store->word_count = 0 ;
}

//

int read_manager(WordStore* word_store, InputBuffer* input_buffer, int append_mode)
{
	int pre_rets = 0 ;

	/* Request input, tinker display */
	if(input_buffer->src == stdin)
	{
		static const char* USER = NULL ;
		if(!USER) USER = getenv("USER") ;
		
		char dir[PATH_MAX] ;
		getcwd(dir, PATH_MAX) ;
		set_display(texture_bold, foreground_white, background_magenta) ;
		fprintf(stdout, "%s%c%s", USER, ':', dir) ;
		reset_display() ;
		strcmp(USER, "root") == 0 ? fprintf(stdout, "%s", "# ") : fprintf(stdout, "%s", "$ ") ;
	}
	
	/* Get, store & parse input */
	if(!append_mode)
	{
		word_store_refresh(word_store) ;
		input_buffer_refresh(input_buffer) ;
	}
	else {
		input_buffer->current_start = input_buffer->current_end+1 ;	
	}

	pre_rets = read_input(input_buffer) ;
	//fprintf(stdout, "read_input ret: %d\n", rets) ;
	if(pre_rets != 0) return -1 ; // if EOF / no text at all read, end 
	pre_rets = dissect(input_buffer, word_store) ;
	//fprintf(stdout, "dissect ret: %d\n", rets) ;
	if(pre_rets != 0) return -2 ; // load word_store up with parsed data
				       // if no words were found, next line
				       
	return 0 ;
}
