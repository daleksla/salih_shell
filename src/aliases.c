#define _GNU_SOURCE
#include <stddef.h> // type aliases
#include <stdlib.h> // environmental aliases, malloc, free
#include <string.h> // strlen

#include "aliases.h"
#include "parser.h"

/** @brief Functionality to manage aliases
  * @author Salih Ahmed
  * @date 2 Aug 2021 **/
  
void alias_store_init(AliasStore* alias_store)
{
	alias_store->_size = 3 ;
	alias_store->aliases = malloc(sizeof(Alias) * alias_store->_size) ; // list of max 256 char pointers
	alias_store->alias_count = 0 ;
}

int declare_alias(const char* alias_name, const char* data, AliasStore* alias_store)
{
	if(alias_store->alias_count == alias_store->_size)
	{
		alias_store->_size *= 2 ;
		alias_store->aliases = realloc(alias_store->aliases, alias_store->_size) ;
	}
	++alias_store->alias_count ; // increment alias count
	Alias* alias = alias_store->aliases + (alias_store->alias_count - 1) ; // find position of new var

	strcpy(alias->identifier, alias_name) ; // set name
	memset(alias->value, '\0', 1024) ; // initialise buffer up with null chars
	update_alias(alias, data) ;
	
	return 0 ; // sucessful operation
}

int update_alias(Alias* alias, const char* data)
{
	// store with correct data
	const size_t str_size = strlen(data) ;
	alias->word_count = 1 ;
	for(size_t i = 0 ; i < str_size ; ++i)
	{
		if(data[i] == ' ')
		{
			alias->value[i] = '\0' ;
			++alias->word_count ;
		}
		else {
			alias->value[i] = data[i] ;
		}
	}

	return 0 ; // sucessful operation
}

Alias* find_alias(const char* alias_name, const AliasStore* alias_store)
{
	for(size_t i = 0 ; i < alias_store->alias_count ; ++i)
	{
		if(strcmp(alias_name, alias_store->aliases[i].identifier) == 0)
		{
			return &alias_store->aliases[i] ;
		}
	}
	return NULL ;
}

void substitute_aliases(WordStore* word_store, const AliasStore* alias_store)
{
	for(size_t i = 1 ; i <= word_store->word_count ; ++i)
	{
		Alias* alias = find_alias(word_store->words[i-1], alias_store) ;
		if(alias) // if alias was found
		{
			word_store_replace(word_store, alias->value, i) ; // where j represents an offset
			
			char* tmp = strchr(alias->value, '\0') + 1 ;
			for(size_t j = 1 ; j < alias->word_count ; ++j)
			{
				word_store_insert(word_store, tmp, i+j) ; // where j represents an offset
				tmp = strchr(tmp, '\0') + 1 ; // go to end of single word, then go one char next
				//++i ;	
			}
		}
	}
}

void alias_store_fini(AliasStore* alias_store)
{
	free(alias_store->aliases) ;
	alias_store->aliases = NULL ;
	alias_store->_size = 0 ;
	alias_store->alias_count = 0 ;
}
