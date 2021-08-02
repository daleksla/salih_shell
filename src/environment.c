#include <unistd.h> // working dir, symbolic constants
#include <stddef.h> // type aliases
#include <stdlib.h> // environmental variables

#include "environment.h"

/** @brief Functionality to initialise & modify environment information
  * @author Salih Ahmed
  * @date 2 Aug 2021 **/

void environment_init(Environment* env)
{
	env->PATH = getenv("PATH") ;
	env->USER = getenv("USER") ;
	{
		long n = pathconf(".", _PC_PATH_MAX) ;
		env->WORKING_DIRECTORY = malloc(n * sizeof(char)) ;
		for(size_t i = 0 ; i < (size_t)n ; ++i) env->WORKING_DIRECTORY[i] = '\0' ;
		getcwd(env->WORKING_DIRECTORY, n) ;
	}
}

void environment_fini(Environment* env)
{
	free((void*)env->WORKING_DIRECTORY) ;
}
