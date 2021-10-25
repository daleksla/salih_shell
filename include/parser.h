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
	
	char* current_start ;
	
	char* current_end ;
	
	FILE* src ;
	
} InputBuffer ;
 
/** Initialises InputBuffer structure
  * @param pointer to InputBuffer struct variable **/ 
void input_buffer_init(InputBuffer*) ;
 
/** Resets InputBuffer structure
  * @param pointer to InputBuffer struct variable **/ 
void input_buffer_refresh(InputBuffer*) ;
 
/** Reads user input into InputBuffer structure from a given FILE stream
  * @param pointer to InputBuffer struct variable
  * @return exit code status (0 if success and something was read, -1 if EOF) **/ 
int read_input(InputBuffer*) ;

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
  * @param pointer to WordStore struct variable
  * @param const size_t specific amount of memory to allocate (if word count is known, 0 indicates to allocate arbitary amount) **/ 
void word_store_init(WordStore*, const size_t) ;

/** Resets WordStore structure
  * @param pointer to WordStore struct variable **/ 
void word_store_refresh(WordStore*) ;

/** Adds record of a detected word to WordStore structure
  * @param pointer to WordStore struct variable
  * @param char* to first character of a word **/  
void word_store_add(WordStore*, char*) ;

/** Replace word stored in WordStore structure with another
  * @param pointer to WordStore struct variable
  * @param char* to first character of a word
  * @param const size_t relating to position to enter word into **/  
void word_store_replace(WordStore*, char*, const size_t) ;

/** Inserts record of a detected word to WordStore structure into specified position (to avoid word_store_add having unneeded runtime overhead)
  * @param pointer to WordStore struct variable
  * @param char* to first character of a word
  * @param const size_t relating to position to enter word into **/  
void word_store_insert(WordStore*, char*, const size_t) ;

/** dissect scans user input and splits relevant info it into commands, arguments etc.
  * @param pointer to InputBuffer struct containing start and end point to read in contained buffer
  * @param pointer to WordStore struct variable to load with information 
  * @return int acting as boolean, to whether operation occured sucessfully **/ 
int dissect(InputBuffer*, WordStore*) ;

/** is_whitespace determines whether a given character is a 'whitespace char'
  * @param const char to determine whether its a whitespace character
  * @return int storing boolean, where true means given char is a whitespace, else false **/
int is_whitespace(const char) ;

/** is_special determines whether a given character is a 'special symbol' (ie not plain text, but a command)
  * @param const char to determine whether its a special character
  * @return int storing boolean, where true means given char is a special symbol, else false **/
int is_special(const char) ;

/** find_special finds any type of non whitespace
  * @param const char* being the string buffer we're searching through
  * @param const size_t relating to the maximum amount of bytes we can search through from the starting point
  * @return pointer to byte storing first instance of text **/
char* find_content(const char*, const size_t) ; 

/** find_special finds first a special symbol, returns NULL if no special symbols are available 
  * @param const char* being the string buffer we're searching through
  * @param const size_t relating to the maximum amount of bytes we can search through from the starting point
  * @return pointer to byte storing first instance of text **/
char* find_special(const char*, const size_t) ; 

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

// Function below uses these functions and facilitates calls, from read in to break down

/** read_manager function facilitates function calls to receive input & to parse it
  * @param pointer to AliasStore struct to possibly modify
  * @param pointer to InputBuffer struct containing tape of input (in case we need to resize)
  * @param const int as to whether we are to add to current input stored (ie complete a statement) or just write from the start
  * @return exit code relating to outcome occured of reading **/
int read_manager(WordStore*, InputBuffer*, const int) ;

#endif // PARSER_H
