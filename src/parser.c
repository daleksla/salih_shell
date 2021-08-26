#include <stddef.h> // type aliases
#include <stdio.h> // EOF
#include <stdlib.h> // malloc, free
#include <string.h> // strcmp, memset

#include "parser.h"

/** @brief Functionality relating to parsing input
  * @author Salih Ahmed
  * @date 3 Aug 2021 **/
  
void word_store_init(WordStore* word_store)
{
	word_store->_size = 25 ;
	word_store->words = malloc(sizeof(char*) * word_store->_size) ; // list of max 256 char pointers
	for(size_t i = 0 ; i < word_store->_size ; ++i)
	{
		word_store->words[i] = NULL ;
	}
	word_store->word_count = 0 ;
}

void word_store_refresh(WordStore* word_store)
{
	for(size_t i = 0 ; i < word_store->word_count ; ++i)
	{
		word_store->words[i] = NULL ;
	}
	word_store->word_count = 0 ;
}

int parse(char* buffer, size_t available, WordStore* word_store)
{		
	char* i = find_text(buffer, available) ;
	
	if(i == NULL) // if no text was found
		return -1 ; // indicate failure

	// if text was found
	++word_store->word_count ;
	word_store->words[word_store->word_count-1] = i ; // save it as main command, since it's our first bit of text found
	available -= i - buffer ; // calculate number of bytes left in buffer
	i = find_whitespace(i, available) ; // find next gap 
	*i = '\0' ; // place null terminator at first whitespace to create a valid c-string
	
	available -= i - buffer ;

	while(available > 0) 	// loop is responsible for generating arguments
				// searches for text, finds first instance of whitespace (therefore end of argument) and adds NULL terminator, then repeats
	{
		i = find_text(i, available) ; // now, find next starting point of text
		
		if(i == NULL) // if there's no more text (and therefore no arguments left)
			break ; // then stop searching for them
			
		// if we do find text
		++word_store->word_count ; // then we've definitely found an arg, so increment count
			
		// check whether its a single word or double worded word
		// if quotes are being used, treat from start->end quote as single arg
		enum { double_quotes = 34, single_quotes = 39, uptick_quotes = 96 } ; // local enum w ASCII values
		if(*i == double_quotes || *i == single_quotes || *i == uptick_quotes)
		{
			const char quotation = *i ; // store quotation mark
			++i ; // move to next value, since we don't want to read / store quotation 
			word_store->words[word_store->word_count-1] = i ; // save it as an argument
			while(*i != quotation) // keep going till you see the char again
			{
				++i ;
			}
			*i = '\0' ; // replace end quotation with null termination char to create a valid c-string
			++i ; // move to next char
			available -= i - buffer ; // calculate what remains of the buffer
		}
		else {	// if it's a single word arg
			word_store->words[word_store->word_count-1] = i ; // save it as an argument
			available -= i - buffer ; // calculate what remains of the buffer using the pointers			
			i = find_whitespace(i, available) ; // first find next gap
			*i = '\0' ; // replace whitespace with null termination char to create a valid c-string
			available -= i - buffer ; // calculate what remains of the buffer using the positioned pointers
		}
	}
	
	return 0 ; // indicate success
}
 
char* find_text(const char* input, const size_t size)
{
	for(char const* i = input ; i < input + size ; ++i)
	{
		if(*i == ' ' || *i == '\t' || *i == '\n' || *i == '\0') continue ;
		else if(*i == EOF) break ;
		else return (char*)i ;
	}
	return NULL ;
}

char* find_whitespace(const char* input, const size_t size)
{
	for(char const* i = input ; i < input + size ; ++i)
	{
		if(*i == ' ' || *i == '\t' || *i == '\n' || *i == EOF || *i == '\0') return (char*)i ;
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
