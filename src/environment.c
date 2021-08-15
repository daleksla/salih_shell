#define _GNU_SOURCE
#include <unistd.h> // working dir, symbolic constants
#include <stddef.h> // type aliases
#include <stdlib.h> // environmental variables, malloc, free
#include <sys/stat.h>
#include <string.h> // strlen

#include "environment.h"

/** @brief Functionality to initialise & modify environment information
  * @author Salih Ahmed
  * @date 2 Aug 2021 **/

void environment_init(Environment* env)
{
	env->USER = getenv("USER") ;
	env->WORKING_DIRECTORY = getenv("PWD") ; // start from directory of launch environment
}

int change_directory(Environment* env, const char* path)
{
	if(chdir(path) == -1)
	{
		return 0 ;
	}
	env->WORKING_DIRECTORY = get_current_dir_name() ;
		
	return 1 ;
} 

void environment_fini(Environment* env)
{	
	env->USER = NULL ;
	env->WORKING_DIRECTORY = NULL ;
}
