#ifndef PARSER_H
#define PARSER_H
#pragma once

#include <stddef.h> // type aliases

/** @brief Declarations relating to parsing user input
  * @author Salih Ahmed
  * @date 3 Aug 2021 **/
 
typedef struct {
	/** Struct storing parsed information from input, such as main command, arguments etc. **/
	
	char* cmd ;
	
	char** args ;

	size_t _size ; // allocated size of args, just double up each time
	
	size_t arg_count ;
	
} CmdStore ;

/** Initialises CmdStore structure
  * @param pointer to CmdStore struct variable **/ 
void cmd_store_init(CmdStore*) ;

/** parse scans user input and splits relevant info it into commands, arguments etc.
  * @param pointer to input buffer
  * @param size_t of input buffer
  * @param pointer to CmdStore struct variable to load with information 
  * @return int acting as boolean, to whether operation occured sucessfully **/ 
int parse(char*, const size_t, CmdStore*) ;

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

/** Deinitialises CmdStore structure
  * @param pointer to CmdStore struct variable **/ 
void cmd_store_fini(CmdStore*) ;

#endif // PARSER_H
