#ifndef EXECUTOR_H
#define EXECUTOR_H
#pragma once

#include <unitstd.h> // execvp, fork, wait
#include <string.h> // strcmp
#include <signal.h> // (sig)kill
#include <stdio.h> // printf

int execute_statement(const WordStore*, Environment*) ;

#endif // EXECUTOR_H
