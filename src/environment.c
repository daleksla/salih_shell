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
	env->PATH = getenv("PATH") ;
	env->USER = getenv("USER") ;
	{
		size_t n = pathconf(".", _PC_PATH_MAX) ;
		env->WORKING_DIRECTORY = malloc(n * sizeof(char)) ;
		for(size_t i = 0 ; i < (size_t)n ; ++i) env->WORKING_DIRECTORY[i] = '\0' ;
		getcwd(env->WORKING_DIRECTORY, n) ;
		env->max_size_ = n ;
	}
}

int change_directory(Environment* env, const char* path)
{
	if(chdir(path) == -1)
	{
		return 0 ;
	}
	
	size_t n = pathconf(".", _PC_PATH_MAX) ;

	if(env->max_size_ > n)
	{
		env->WORKING_DIRECTORY = realloc(env->WORKING_DIRECTORY, n) ;
		getcwd(env->WORKING_DIRECTORY, n) ;
		env->max_size_ = n ;
	}
	else if(env->max_size_ < n)
	{
		memset(env->WORKING_DIRECTORY + n, '\0', env->max_size_ - n) ;
		getcwd(env->WORKING_DIRECTORY, n) ;
	}
	else {
		getcwd(env->WORKING_DIRECTORY, n) ;
	}
		
	return 1 ;
} 

void environment_fini(Environment* env)
{	
	env->PATH = NULL ;
	env->USER = NULL ;
	free((void*)env->WORKING_DIRECTORY) ;
	env->WORKING_DIRECTORY = NULL ;
	env->max_size_ = 0 ;
}
