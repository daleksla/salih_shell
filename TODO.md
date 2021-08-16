# salih_shell
## to do list

1. Find a way to store variables. Might want to use two arrays - one storing string names, one storing values also as strings & one char storing whether it should be interpretted as a char, int, string, etc.
> e.g. `typedef struct { char[63] id, char[1024] val, char type } Variable ;`
>      `typedef struct { Variable variables[100], size_t var_count } Variables ;`
May want to use dynamic memory allocation, start at like 10 and just double up storage if need be.
2. Swap in variable names into final command if need be (e.g. echo ${greeting} -> echo "Hello world")
4. Currently, lines are parsed and run, regardless of content. Code will only be run if it's marked as a statement (doesn't contain keyword like `if`
