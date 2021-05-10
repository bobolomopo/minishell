# minishell modules

- ## Prompt / line editing:
	* __what is does__: reads from stdin, allowing navigation through the history of commands
	* terminal in noncanonical mode (so we can detect ARROWS up an down key press) and echo off 
	* handles echoing back to the screen and editing the line.
	* It basically replaces get_next_line by something much more complicated...
	* Uses Termcaps to clear a line in the terminal, move cursor, etc. (functions: tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs)
	* __how__: 
		* for putting the terminal in noncanonical mode and disable echo, use functions tcsetattr, tcgetattr
		* Termcaps fuctions: tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
	* __output__: a string that will be fed to the Lexer

- ## Lexer:
	* what it does: breaks down a string into tokens
	* text enclosed in " " and ' ' must be handled
	* output: array of char * (so, a char **)


- ## Parser:
	* what it does: interprets the sequence of tokens into a table of commands, and info about redirections
	* output: table of commands, in_file, out_file, err_out 
	
	ex:  
	$ > ls -ls | grep yo | cat -e  > results.txt
	will be parsed into:

	|num 	| command	| arg 1	| arg 2	|... NULL	|
	|----	|----		|---	|---	|---	|
	|0 		| ls		| -ls	| NULL
	|1		| grep		| yo	| NULL
	|2		| cat		| -e	| NULL

	out_file: results.txt

	each line of this table could be a NULL-terminated array of char *  
	This makes sense because of the way execve() will be called (see module "Executer")
	* we have to somehow store info about pipes ( | ) and separate commands ( ; )
	

- ## Expander (somewhere, not necessarily here...)
	* what: expansion of variables. ex: $VAR -> content_of_var
	* Only for args?
	* this is influenced by "" or '' ... maybe this should be done by the Lexer?

- ## Executer
	* what: executes all the commands of the table of commands, taking care of pipes and redirections
	* how: for each command, except for the the builtins (echo, cd, pwd, export, unset, env, exit), it looks in the directories listed in the PATH environmental variable for the requested command. If found, it forks into a new process. The parent process waits (function wait) for the child to return (function exit). The child calls execve() with the command and the arguments.
	* piping and redirections use functions dup, dup2 and pipe.
	* the builtins are executed by the parent process (so, no fork). builtins are just functions called by the program?
