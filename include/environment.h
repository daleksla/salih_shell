#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#pragma once

/** @brief Declarations relating to initialisation & modification of environment
  * @author Salih Ahmed
  * @date 2 Aug 2021 **/

#include "parser.h" // WordStore

/** change directory
  * @param pointer to Environment struct variable 
  * @param const char* c-string to new desired path 
  * @return int acting as boolean as to whether operation was sucessful or not **/ 
int change_directory(const char*) ;

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

/** Creates a variable within a given VariableStore structure
  * @param const char* c-string storing identifier for variable
  * @param void* generic pointer to data item
  * @param char holding value s(tring), d(int), f(double) - will show how to dereference generic / void pointer
  * @param pointer to VariableStore struct variable to add variable to 
  * @return exit code (where 0 indicates success) **/ 
int declare_variable(const char*, const void*, const char, VariableStore*) ; 

/** Modifies indicator to data type stored within contents
  * @param pointer to a Variable
  * @param char holding value s(tring), d(int), f(double)
  * @return exit code (where 0 indicates success) **/ 
int update_variable_type(Variable*, const void*) ; 

/** Modifies stored contents of a created variable
  * @param pointer to a Variable
  * @param void* generic pointer to data item
  * @return exit code (where 0 indicates success) **/ 
int update_variable_data(Variable*, const void*) ; 

/** Attempts to locate a variable by a given name
  * @param const char* c-string storing identifier for variable
  * @param pointer to VariableStore struct variable to find variable in
  * @return pointer to a Variable struct (if found), else NULL **/ 
Variable* find_variable(const char*, const VariableStore*) ; 

/** substitute_variables is a function which goes through every word parsed and see it starts with $. It adjusts the word accordingly
  * @param pointer to WordStore
  * @param const pointer to VariableStore **/ 
void substitute_variables(WordStore*, const VariableStore*) ;

/** Deinitialises VariableStore structure
  * @param pointer to VariableStore struct variable **/ 
void variable_store_fini(VariableStore*) ;

#endif // ENVIRONMENT_H
