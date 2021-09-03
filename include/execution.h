#ifndef EXECUTION_H
#define EXECUTION_H
#pragma once

#include "parser.h"
#include "variables.h"

/** @brief Declarations relating to executing different types of statements
  * @author Salih Ahmed
  * @date 3 September 2021 **/

/** exec_statement function executes a statement based on a given words
  * pointer to WordStore struct containing words amounting to statement to attempt to execute
  * pointer to VariableStore struct to possibly modify
  * pointer to int containing old return status with possibility to update **/
void exec_statement(WordStore*, VariableStore*, int*) ;

#endif // EXECUTION_H
