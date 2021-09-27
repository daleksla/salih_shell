#ifndef ALIASES_H
#define ALIASES_H
#pragma once

/** @brief Declarations relating to management of aliases
  * @author Salih Ahmed
  * @date 2 Aug 2021 **/

#include "parser.h" // struct WordStore

// Below is to do with storing Aliases

typedef struct {
	/** This struct stores the name & value of a given alias **/
	char identifier[64] ; // 63 chars is max length reserved for an identifier
	
	char value[1024] ;
	
	size_t word_count ;
	
} Alias ;

typedef struct {
	/** This struct stores pointers to an allocated array of `Alias` structs, as-well as max capacity of arrays and how many are currently in store **/
	size_t _size ;

	Alias* aliases ;

	size_t alias_count ;

} AliasStore ;

/** Initialises AliasStore structure
  * @param pointer to AliasStore struct variable **/ 
void alias_store_init(AliasStore*) ;

/** Creates an alias within a given AliasStore structure
  * @param const char* c-string storing identifier for alias
  * @param const char* c-string storing contents of given alias
  * @param pointer to AliasStore struct variable to add variable to 
  * @return exit code (where 0 indicates success) **/ 
int declare_alias(const char*, const char*, AliasStore*) ;

/** Modifies stored contents of a given Alias
  * @param pointer to an Alias
  * @param const char* string being the contents of Alias
  * @return exit code (where 0 indicates success) **/ 
int update_alias(Alias*, const char*) ; 

/** Attempts to locate an alias by a given name
  * @param const char* c-string storing identifier for alias
  * @param pointer to AliasStore struct variable to find variable in
  * @return pointer to a Alias struct (if found), else NULL **/ 
Alias* find_alias(const char*, const AliasStore*) ; 

/** substitute_aliases is a function which goes through every word parsed and see if the word matches a given alias
  * @param pointer to WordStore
  * @param const pointer to AliasStore **/ 
void substitute_aliases(WordStore*, const AliasStore*) ;

/** Deinitialises AliasStore structure
  * @param pointer to AliasStore struct variable **/ 
void alias_store_fini(AliasStore*) ;

#endif // ALIASES_H
