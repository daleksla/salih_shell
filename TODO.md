# salih_shell
## to do list

1. store variables which are unwriteable except by the shell to do with command execution:
> ie `$#` = number of arguments, `$@` = what parameters were passed, `$?` = return value of last command

2. If user's argument begins with "", '', `` - then consider it, until an ending quotation, as a single argument. Report parsing failure if end char isn't found. add to `parse` function

3. Swap in variable names into final command if need be (e.g. `echo ${greeting}` -> `echo "Hello world"`)
> create a function to go ahead and sub variables with the values behind them, ie go to each word in CmdStore, see if it's preceeded by a $. if it is, and it's a valid variable name, substitute (either point straight to value held by variable, else just copy bytes into buffer (try the first, sounds better)). again, do this in parse function upon encountering words.

4. may want to put all the execution stuff in a seperate function. warrants a seperate file. it'll seem easier if a while loop is being implemented, etc. when it comes to writing the full interpretter. probably call it executer/or/however-the-hell-you-spell-that-word

5. Currently, lines are parsed and run, regardless of content. Code will only be run if it's marked as a statement (doesn't contain keyword like `if`
