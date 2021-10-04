# salih_shell
## to do list

### HIGH PRIORITY
1. implement `if statement` functionality by creating function, which copies condition and implements body

2. when implementing a full interpretter with controlled scopes (ifs, fors, while, etc.), the user must input the whole thing first
> ie. keyword (eg. `if`) detected, after keyword (eg. `then`) store condition statement, store each command in an array, after endword (eg. `fi`) is detected, evaluate and run

3. substitute variables and aliases for each statement, not for a whole block of text given

4. modify `parse` method to enforce text being inputted after symbols, rather than the `run_statement` function
> only if the src is stdin should a > (indicating for user to input) be displayed when requesting more input

5. add `if` statements wherever memory allocation functions are use - do an assert to make sure they return valid address

### LOW PRIORITY
6. add proper initial argument parsing (`-c` = execute command (`argv` onwards), `-f` filename)

7. store variables which are unwriteable except by the shell to do with command execution:
> ie `$@` = what parameters were passed

8. add another buffer, which copies a given item of data (ie non-string variable's value) into string format

9. generally speaking, this programs memory usage is heinous and *must* be reduced
> a variable should not have a default of 1024 chars. each variable should start with 10 bytes of memory, and realloc on an individual basis. add those to methods which deal with update values
