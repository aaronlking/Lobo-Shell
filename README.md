# Lobo Shell -- CS450 Project 2

## Contributors:
- [Aaron King](https://github.com/aaronlking)
- [Joseph Henderson](https://github.com/jhendssu)
- [Gary Singh](https://github.com/Gary-Git)
- [Owen Mastropietro](https://github.com/OwenMastropietro)

## Instructions To Run:
 1. `$ make run`
 2. `$ make valgrind`
 3. Or you can figure something else out...

## Important Notes:
- I suggest collapsing all the code / comments if your editor supports that.  

- I have modified his splt_cmd_line() function. I added comments to it that  
hightlight his use of strtok_r() and how I have modified it.  

- I #defined a preprocessor directive, DEBUG, to sort of make use of the functionality behind the verbose flag from Project 1.

- main() will loop on user input, calling his splt_cmd_line() function repeatedly  
to split the line into commands, separated by pipes, |, and will then call my  
sgt_major_pipage() function which will loop through each command in commands,   
further delimiting each command into argv-like arrays, separated by whitespace,  
calling my handle_single_command() function to execute each command as if it  
were a stand-alone command.  

- Most recent update: added logic for attempting arbitrary pipes...
DOES NOT WORK :(. Currently commented out said logic so that it can can still be
ran to test running stand alone commands found between pipes.

- I have often executed the following sequence of commands while testing / creating this logic:  
 1. `$ make valgrind`  
 2. `$ echo hello | echo hi | ls`  
