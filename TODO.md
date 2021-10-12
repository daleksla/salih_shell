# salih_shell
## to do list

### HIGH PRIORITY
1. implement `if statement` functionality by creating function, which copies condition and implements body
> save condition as local variable until keyword `then`
> then keep writing to the `input_buffer`, seperating each line of input with a `NULL` ptr
> once `fi` is detected, run condition

2. ensure only in `source.c`'s call of `read/run_manager` is the `input_buffer_refresh` & `word_store_refresh` method call

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
