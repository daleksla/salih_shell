#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#pragma once

/** @brief Declarations relating to initialisation & modification of environment
  * @author Salih Ahmed
  * @date 2 Aug 2021 **/

typedef struct {
	/** Struct containing essential environment information
	  * Note: requires full initialisation using `environment_init` & deinitialisation using `environment_fini` **/
	char const* PATH ;
	
	char const* USER ;
	
	char* WORKING_DIRECTORY ;
	
} Environment ;

/** Initialises environment structure
  * @param pointer to Environment struct variable **/ 
void environment_init(Environment*) ;

/** Deinitialises environment structure
  * @param pointer to Environment struct variable **/ 
void environment_fini(Environment*) ;

#endif // ENVIRONMENT_H
