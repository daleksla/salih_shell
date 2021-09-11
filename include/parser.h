#ifndef PARSER_H
#define PARSER_H
#pragma once

#include <stddef.h> // type aliases
#include <stdio.h>

/** @brief Declarations relating to parsing user input (ie literally dissecting input aswell as understanding and running it)
  * @author Salih Ahmed
  * @date 3 Aug 2021 **/
 
// Below is simply a struct storing input buffer and size of storage (used for tracking expansion) and method to read in input, expanding memory if needed
 
typedef struct {
	/** Struct storing buffer and size **/

	char* buffer ;
	
	size_t size ;
	
	char* current ;
	
} InputBuffer ;
 
/** Initialises InputBuffer structure
  * @param pointer to InputBuffer struct variable **/ 
void input_buffer_init(InputBuffer*) ;
 
/** Resets InputBuffer structure
  * @param pointer to InputBuffer struct variable **/ 
void input_buffer_refresh(InputBuffer*) ;
 
/** Reads user input into InputBuffer structure from a given FILE stream
  * @param pointer to FILE stream struct source to read from
  * @param pointer to InputBuffer struct variable
  * @return exit code status (0 if success and something was read, -1 if EOF) **/ 
int read_input(FILE*, InputBuffer*) ;

/** Deinitialises InputBuffer structure
  * @param pointer to InputBuffer struct variable **/ 
void input_buffer_fini(InputBuffer*) ;

// Below is to do with breaking user input down into words and storing them (or pointers to each word in a given buffer)

typedef struct {
	/** Struct storing parsed information from input, such as main command, arguments etc. **/
	
	char** words ;

	size_t _size ; // allocated size of args, just double up each time
	
	size_t word_count ;
	
} WordStore ;

/** Initialises WordStore structure
  * @param pointer to WordStore struct variable **/ 
void word_store_init(WordStore*) ;

/** Resets WordStore structure
  * @param pointer to WordStore struct variable **/ 
void word_store_refresh(WordStore*) ;

/** dissect scans user input and splits relevant info it into commands, arguments etc.
  * @param pointer to input buffer
  * @param const size_t available to read from input buffer
  * @param pointer to WordStore struct variable to load with information 
  * @return int acting as boolean, to whether operation occured sucessfully **/ 
int dissect(char*, const size_t, WordStore*) ;

/** is_whitespace determines whether a given character is a 'whitespace char'
  * @param const char to determine whether its a whitespace character
  * @return int storing boolean, where true means given char is a whitespace, else false **/
int is_whitespace(const char) ;

/** find_text finds first instance of text (ie not whitespace characters), returns NULL if no text is available 
  * @param const char* being the string buffer we're searching through
  * @param const size_t relating to the maximum amount of bytes we can search through from the starting point
  * @return pointer to byte storing first instance of text **/
char* find_text(const char*, const size_t) ; 

/** find_whitespace finds first instance of whitespace character, returns NULL if none are present (ie text till end of buffer)
  * @param const char* being the string buffer we're searching through
  * @param const size_t relating to the maximum amount of bytes we can search through from the starting point
  * @return pointer to byte storing first instance of whitespace **/
char* find_whitespace(const char*, const size_t) ;

/** Deinitialises WordStore structure
  * @param pointer to WordStore struct variable **/ 
void word_store_fini(WordStore*) ;

#endif // PARSER_H
