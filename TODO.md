# salih_shell
## to do list

### HIGH PRIORITY
4. modify `parse` method to enforce text being inputted after symbols, rather than the `run_statement` function

5. add proper initial argument parsing (`-c` = execute command (`argv` onwards), `-f` filename)

7. when it comes to functions and loops, save into seperate store areas

### LOW PRIORITY
8. store variables which are unwriteable except by the shell to do with command execution:
> ie `$@` = what parameters were passed

9. when implementing a full interpretter with controlled scopes (ifs, fors, while, etc.), the user must input the whole thing first
> ie. keyword (eg. `if`) detected, after keyword (eg. `then`) store condition statement, store each command in an array, after endword (eg. `fi`) is detected, evaluate and run

10. add another buffer, which copies a given item of data (ie non-string variable's value) into string format

11. generally speaking, this programs memory usage is heinous and *must* be reduced
> a variable should not have a default of 1024 chars. each variable should start with 10 bytes of memory, and realloc on an individual basis. add those to methods which deal with update values
