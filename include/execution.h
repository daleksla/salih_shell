#ifndef EXECUTION_H
#define EXECUTION_H
#pragma once

#include "parser.h"
#include "variables.h"

/** @brief Declarations relating to executing different types of statements
  * @author Salih Ahmed
  * @date 3 September 2021 **/

/** post_statement inline function to store return-status variable, push statement to history etc.
  * @param const int storing return status of last command executed 
  * @param const int storing positional parameter counts
  * @param pointer to VariableStore struct to modify **/
static inline void post_statement(const int, const int, VariableStore*) ;

/** run_statement function evaluates a portion of code resembling a statement & facilitates their execution (eg. calling exec_statement, checking pipes etc.)
  * @param pointer to WordStore struct containing words amounting to statement to attempt to execute
  * @param pointer to VariableStore struct to possibly modify 
  * @param pointer to InputBuffer struct containing tape of input (in case we need to resize)
  * @return exit code of statement (we do save it as a shell variable but internally its more efficient to simply check if the statement executed right) **/
int run_statement(WordStore*, VariableStore*, InputBuffer*) ;

/** exec_statement function purely executes a statement based on given words
  * @param pointer to WordStore struct containing words amounting to statement to attempt to execute
  * @param pointer to VariableStore struct to possibly modify 
  * @return exit code of statement (we do save it as a shell variable but internally its more efficient to simply check if the statement executed right) **/
int exec_statement(WordStore*, VariableStore*) ;

#endif // EXECUTION_H
