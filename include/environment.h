#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#pragma once

/** @brief Declarations relating to initialisation & modification of environment
  * @author Salih Ahmed
  * @date 2 Aug 2021 **/

typedef struct {
	/** Struct containing essential environment information
	  * Note: requires full initialisation using `environment_init` & deinitialisation using `environment_fini` **/
	
	char const* USER ;
	
	char const* WORKING_DIRECTORY ;
	
} Environment ;

/** Initialises environment structure
  * @param pointer to Environment struct variable **/ 
void environment_init(Environment*) ;

/** change directory
  * @param pointer to Environment struct variable 
  * @param const char* c-string to new desired path 
  * @return int acting as boolean as to whether operation was sucessful or not **/ 
int change_directory(Environment*, const char*) ;

/** Deinitialises environment structure
  * @param pointer to Environment struct variable **/ 
void environment_fini(Environment*) ;

#endif // ENVIRONMENT_H
