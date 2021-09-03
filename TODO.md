# salih_shell
## to do list

1. add user comments, beginning with `#` and ending with newline

2. save user input via implicit `var=val` command (if var doesn't exist, create as string, else just set data)

3. add piping capabilities
> e.g. if pipe symbol `|` detected in general executor, open pipe, run first, if first returns non-zero exit code, close, else run next statement
> might want to add a `run_statement` function, which is different from `exec_statement`, as one checks pipes etc and one just does any needed pipe openings etc) 

3. add proper initial argument parsing (`-c` = execute command (argv onwards), `-f` filename)

3. store variables which are unwriteable except by the shell to do with command execution:
> ie `$@` = what parameters were passed

4. when implementing a full interpretter with controlled scopes (ifs, fors, while, etc.), the user must input the whole thing first
> ie. keyword (eg. `if`) detected, after keyword (eg. `then`) store condition statement, store each command in an array, after endword (eg. `fi`) is detected, evaluate and run

5. add another buffer, which copies a given item of data (ie non-string variable's value) into string format

6. generally speaking, this programs memory usage is heinous and *must* be reduced
a variable should not have a default of 1024 chars. each variable should start with 10 bytes of memory, and realloc on an individual basis. add those to methods which deal with update values
