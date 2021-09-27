# salih_shell
## to do list

1. modify `substitute_alias` method which places aliases with spaces as an extra word in word store

2. modify variables to store multi word variables, including string args

3. modify Alias and Variable struct so that, within a declaration, the amount of words is recorded & modify respective declare functions accordingly
> e.g `typedef struct { char* identifier, char* value, size_t word_count } Alias ; ` 

1. add proper initial argument parsing (`-c` = execute command (`argv` onwards), `-f` filename)

2. replace the GNU function `get_current_dir_name` as it bloody executes `malloc` each time, which is ridiculous

3. replace creation of initialised `word_store`'s with uninitiliased alternatives which simply do shallow copies

4. store variables which are unwriteable except by the shell to do with command execution:
> ie `$@` = what parameters were passed

5. when implementing a full interpretter with controlled scopes (ifs, fors, while, etc.), the user must input the whole thing first
> ie. keyword (eg. `if`) detected, after keyword (eg. `then`) store condition statement, store each command in an array, after endword (eg. `fi`) is detected, evaluate and run

6. add another buffer, which copies a given item of data (ie non-string variable's value) into string format

7. generally speaking, this programs memory usage is heinous and *must* be reduced
> a variable should not have a default of 1024 chars. each variable should start with 10 bytes of memory, and realloc on an individual basis. add those to methods which deal with update values
