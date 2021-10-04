#define _GNU_SOURCE
#include <stddef.h> // type aliases
#include <stdlib.h> // environmental variables, malloc, free
#include <string.h> // strlen

#include "variables.h"
#include "parser.h"

/** @brief Functionality to manage variables
  * @author Salih Ahmed
  * @date 2 Aug 2021 **/
  
void variable_store_init(VariableStore* variable_store)
{
	variable_store->_size = 3 ;
	variable_store->variables = malloc(sizeof(Variable) * variable_store->_size) ; // list of max 256 char pointers
	assert(variable_store->variables) ;
	variable_store->variable_count = 0 ;
}

int parse_variable_type(const char* type_declaration)
{
	if(type_declaration[0] != '-')
	{
		return -2 ;
	}
	
	if(type_declaration[1] == 'i' || strcmp(type_declaration, "--integer") == 0)
	{
		return 'd' ;
	}
	else if(type_declaration[1] == 'f' || strcmp(type_declaration, "--float") == 0)
	{
		return 'f' ;
	}
	else if(type_declaration[1] == 's' || strcmp(type_declaration, "--string") == 0)
	{
		return 's' ;
	}

	return -3 ; // if a valid type isn't detected
}

int declare_variable(const char* var_name, const void* data, const char data_type, VariableStore* variable_store)
{
	if(variable_store->variable_count == variable_store->_size)
	{
		variable_store->_size *= 2 ;
		variable_store->variables = realloc(variable_store->variables, variable_store->_size) ;
		assert(variable_store->variables) ;
	}
	++variable_store->variable_count ; // increment variable count
	Variable* variable = variable_store->variables + (variable_store->variable_count - 1) ; // find position of new var

	strcpy(variable->identifier, var_name) ; // set name
	update_variable_type(variable, data_type) ; // set data type of var
	memset(variable->value, '\0', 1024) ; // fill buffer up with null chars

	update_variable_data(variable, data) ;
	
	return 0 ; // sucessful operation
}

int update_variable_type(Variable* variable, const char type)
{
	variable->type = type ;
	return 0 ; // sucessful operation
}

int update_variable_data(Variable* variable, const void* data)
{
	// store with correct data
	if(variable->type == 's')
	{
		strcpy((char*)variable->value, *(const char**)data) ;
		fprintf(stdout, "first letter: %c\n", variable->value[0]) ;
	}
	else if(variable->type == 'd')
	{
		*(int*)variable->value = *(const int*)data ;
	}
	else if(variable->type == 'f')
	{
		*(double*)variable->value = *(const double*)data ;
	}
	return 0 ; // sucessful operation
}

Variable* find_variable(const char* var_name, const VariableStore* variable_store)
{
	for(size_t i = 0 ; i < variable_store->variable_count ; ++i)
	{
		if(strcmp(var_name, variable_store->variables[i].identifier) == 0)
		{
			return &variable_store->variables[i] ;
		}
	}
	return NULL ;
}

void substitute_variables(WordStore* word_store, const VariableStore* variable_store)
{
	for(size_t i = 0 ; i < word_store->word_count ; ++i)
	{
		//char* point = strchr(word_store->words[i], '$') ;
		char* point = word_store->words[i] ;
		if(*point == '$' && !is_whitespace(point[1]))
		{
			Variable* variable = find_variable(word_store->words[i]+1, variable_store) ;
			if(variable) // if variable was found
			{
				word_store->words[i] = (char*)(variable->value) ;
			}
			else { // ie not found in terminal variables
				char* environment_var = getenv(word_store->words[i]+1) ; // then try to find it as a environmental variable
				if(environment_var)
				{
					word_store->words[i] = environment_var ;
				}
				else {
					word_store->words[i][0] = '\0' ; // just make it an empty c-string by replacing $ char (therefore using memory we have whilst nullifying the word we tried to replace)
				}
				// no return code as shells don't typicallly report errors when it comes to variables
			}
		}
	}
	/*
	for(size_t i = 0 ; i < word_store->word_count ; ++i)
	{
		char* point[1] ;
		point[0] = strchr(word_store->words[i], '$') ;
		if(*point && !is_whitespace((*point)[1]))
		{
			Variable* variable = find_variable((*point)+1, variable_store) ;
			if(variable) // if variable was found
			{
				word_store->words[i] = (char*)(variable->value) ;
			}
			else { // ie not found in terminal variables
				char* environment_var = getenv((*point)+1) ; // then try to find it as a environmental variable
				if(environment_var)
				{
					*point = environment_var ;
				}
				else {
					word_store->words[i][0] = '\0' ; // just make it an empty c-string by replacing $ char (therefore using memory we have whilst nullifying the word we tried to replace)
				}
				// no return code as shells don't typicallly report errors when it comes to variables
			}
		}
	}*/
}

void variable_store_fini(VariableStore* variable_store)
{
	free(variable_store->variables) ;
	variable_store->variables = NULL ;
	variable_store->_size = 0 ;
	variable_store->variable_count = 0 ;
}
