#ifndef EXECUTION_H
#define EXECUTION_H
#pragma once

#include "parser.h"
#include "variables.h"

/** @brief Declarations relating to executing different types of statements
  * @author Salih Ahmed
  * @date 3 September 2021 **/

/** set_shell_variables inline function to tidily save inherent shell variables values based on statement executions
  * @param const int storing return status of last command executed 
  * @param const int storing positional parameter counts
  * @param pointer to VariableStore struct to modify **/
static inline void set_shell_variables(const int, const int, VariableStore*) ;

/** exec_statement function executes a statement based on a given words
  * @param pointer to WordStore struct containing words amounting to statement to attempt to execute
  * @param pointer to VariableStore struct to possibly modify 
  * @return exit code of statement (we do save it as a shell variable but internally its more efficient to simply check if the statement executed right) **/
int exec_statement(WordStore*, VariableStore*) ;

#endif // EXECUTION_H
