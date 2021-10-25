#ifndef EXECUTION_H
#define EXECUTION_H
#pragma once

#include "parser.h"
#include "variables.h"
#include "aliases.h"

/** @brief Declarations relating to executing different types of statements
  * @author Salih Ahmed
  * @date 3 September 2021 **/

/** run_manager function facilitates function calls to execute parsed input
  * @param pointer to WordStore struct containing words amounting to statement to attempt to execute
  * @param pointer to VariableStore struct to possibly modify
  * @param pointer to AliasStore struct to possibly modify
  * @param pointer to InputBuffer struct containing tape of input (in case we need to resize)
  * @return exit code of the final statement ran **/
int run_manager(WordStore*, VariableStore*, AliasStore*, InputBuffer*) ;

/** run_if function controls if statements and runs the commands within
  * @param pointer to WordStore struct containing words amounting to statement to attempt to execute
  * @param pointer to VariableStore struct to possibly modify
  * @param pointer to AliasStore struct to possibly modify
  * @param pointer to InputBuffer struct containing tape of input (in case we need to resize)
  * @return exit code reflects to whether if statement was ran successfully **/
int run_if(WordStore*, VariableStore*, AliasStore*, InputBuffer*) ;

/** run_statement function evaluates a portion of code resembling a statement & facilitates their execution (eg. calling exec_statement, checking pipes etc.)
  * @param pointer to WordStore struct containing words amounting to statement to attempt to execute
  * @param pointer to VariableStore struct to possibly modify
  * @param pointer to AliasStore struct to possibly modify
  * @param pointer to InputBuffer struct containing tape of input (in case we need to resize)
  * @return exit code of running of statement **/
int run_statement(WordStore*, VariableStore*, AliasStore*, InputBuffer*) ;

/** run_boolean function does a boolean comparison based on a given portion of code
  * @param pointer to WordStore struct containing words amounting to statement to attempt to execute
  * @param pointer to VariableStore struct to possibly modify
  * @param pointer to AliasStore struct to possibly modify
  * @param pointer to InputBuffer struct containing tape of input (in case we need to resize)
  * @return exit code of running of statement (0 is true, 1 otherwise)**/
int run_boolean(WordStore*, VariableStore*, AliasStore*, InputBuffer*) ;

/** exec_statement function purely executes a statement based on given words
  * @param pointer to WordStore struct containing words amounting to statement to attempt to execute
  * @param pointer to VariableStore struct to possibly modify
  * @param pointer to AliasStore struct to possibly modify
  * @return exit code of statement  **/
int exec_statement(WordStore*, VariableStore*, AliasStore*) ;

/** post_statement inline function to store return-status variable, push statement to history etc.
  * @param const int storing return status of last command executed 
  * @param pointer to VariableStore struct to modify **/
static inline void post_statement(const int, VariableStore*) ;

#endif // EXECUTION_H
