#ifndef PARSER_H
#define PARSER_H
#pragma once

#include <stddef.h> // type aliases

/** @brief Declarations relating to parsing user input
  * @author Salih Ahmed
  * @date 3 Aug 2021 **/
 
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

/** parse scans user input and splits relevant info it into commands, arguments etc.
  * @param pointer to input buffer
  * @param const size_t of input buffer
  * @param pointer to WordStore struct variable to load with information 
  * @return int acting as boolean, to whether operation occured sucessfully **/ 
int parse(char*, const size_t, WordStore*) ;

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

// Below is to do with storing variables

typedef struct {
	/** This struct stores the name, value and datatype of a given variable **/
	char identifier[64] ; // 63 chars is max length reserved for an identifier
	
	char value[1024] ;
	
	char type ;

} Variable ;

typedef struct {
	/** This struct stores pointers to an allocated array of `Variable` structs, as-well as max capacity of arrays and how many are currently in store **/
	size_t _size ;

	Variable* variables ;

	size_t variable_count ;

} VariableStore ;

/** Initialises VariableStore structure
  * @param pointer to VariableStore struct variable **/ 
void variable_store_init(VariableStore*) ;

/** Attempts to locate a variable by a given name
  * @param const char* c-string storing identifier for variable
  * @param pointer to VariableStore struct variable to add variable to
  * @return pointer to a Variable struct (if found) **/ 
Variable* find_variable(const char*, const VariableStore*) ; 

/** Adds or modifies a variable to VariableStore structure
  * @param const char* c-string storing identifier for variable
  * @param void* generic pointer to data item
  * @param char holding value c(har), i(nt), d(ouble) - will show how to dereference generic / void pointer
  * @param pointer to VariableStore struct variable to add variable to **/ 
int store_variable(const char*, const void*, const char, VariableStore*) ; 

/** Deinitialises VariableStore structure
  * @param pointer to VariableStore struct variable **/ 
void variable_store_fini(VariableStore*) ;

#endif // PARSER_H
