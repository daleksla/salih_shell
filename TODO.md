# salih_shell
## to do list

1. add piping capabilities - all we need to do now is create a pipe whereby `ls|*` format is accepted

2. add proper file streaming capabilities (ie redirect output)

3. if user input expects values, simply open input buffer and request more values
> e.g if `then` keyword isn't detected in if statement, values aren't present after pipe symbol (`|`), etc.

4. save user input via implicit `var=val` command (if var doesn't exist, create as string, else just set data)

5. add proper initial argument parsing (`-c` = execute command (argv onwards), `-f` filename)

6. store variables which are unwriteable except by the shell to do with command execution:
> ie `$@` = what parameters were passed

7. when implementing a full interpretter with controlled scopes (ifs, fors, while, etc.), the user must input the whole thing first
> ie. keyword (eg. `if`) detected, after keyword (eg. `then`) store condition statement, store each command in an array, after endword (eg. `fi`) is detected, evaluate and run

8. add another buffer, which copies a given item of data (ie non-string variable's value) into string format

9. generally speaking, this programs memory usage is heinous and *must* be reduced
a variable should not have a default of 1024 chars. each variable should start with 10 bytes of memory, and realloc on an individual basis. add those to methods which deal with update values
