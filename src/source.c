#include <unistd.h> // working dir, symbolic constants
#include <signal.h> // signal handling
#include <stddef.h> // type aliases
#include <stdio.h> // IO ops
#include <stdlib.h> // environmental variables

#include "environment.h" // custom environment setup

/** @brief Source code to manage shell
  * @author Salih Ahmed
  * @date 2 Aug 2021 **/

/** main control code
  * @param int correlating to number to arguments
  * @param arguments themselves
  * @return int as exit code **/
int main(int, char**) ;

int main(int argc, char** argv)
{
	// variable setup(s) & config
	Environment env ;
	environment_init(&env) ;

	// main functionality
	printf("Current directory: %s\n", env.WORKING_DIRECTORY) ;
	printf("Current user: %s\n", env.USER) ;
	printf("Current path: %s\n", env.PATH) ;
	//
	return 0 ;
}
  
