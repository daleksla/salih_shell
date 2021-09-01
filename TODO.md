# salih_shell
## to do list

1. store variables which are unwriteable except by the shell to do with command execution:
> ie `$@` = what parameters were passed

2. add additional case for saving variables from given user input. two ways of doing so, but favouring strings as most things are such, and to deduce and store as other than strings is unneeded
> using `declare`, with options specifying data types (-i, --integer, -f, --float = double float, -s, --string = string). one declare, with a given datatype, overwrites another. 
> basic and implicit `var=2`, which will store all given values as strings. equivalent of using `declare` command with options `--` `-s` when variable is being created the first time. if previous declaration has been made, it'll store that value as the type that has been previously declared.
when writing variables, format should use `find_variable`, then `update_variable` or `declare_variable` function calls (former where no declare command is used, latter otherwise)

3. when implementing a full interpretter with controlled scopes (ifs, fors, while, etc.), the user must input the whole thing first
> ie. keyword (eg. `if`) detected, after keyword (eg. `then`) store condition statement, store each command in an array, after endword (eg. `fi`) is detected, evaluate and run

4. add another buffer, which copies a given item of data (ie non-string variable's value) into string format

5. generally speaking, this programs memory usage is heinous and *must* be reduced. a variable should not have a default of 1024 chars. each variable should start with 10 bytes of memory, and realloc on an individual basis. add those to methods which deal with update values
