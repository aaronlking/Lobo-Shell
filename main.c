#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h> // STDIN_FILENO, STDOUT_FILENO
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include "constants.h"
#include "parsetools.h"


/* TODO: Description & Attribution. */
void syserror(const char *s)
{
  extern int errno;
  fprintf(stderr, "%s\n", s);
  fprintf(stderr, " (%s)\n", strerror(errno));
  exit(EXIT_FAILURE);
}

/* Prints the argv-like array passed to it. 
 * Dr. Gondree provided this function as part of our fork-exec checkpoint.
 */
void print_argv(char **a)
{
  printf("arr: (");
  for (int i = 0; a[i] != NULL; i++)
  {
      printf("%s", a[i]);
      if (a[i + 1]) { printf(", "); }
  }
  printf(")\n");
}

/* Under construction... I'm thinking this funciton can be "reused" a bunch
 * within the while-loop, pretty much anytime 
 * Temporary container for some logic to handle a single command.
 */
void handle_single_command(char *argv[])
{
  int wstatus;
  pid_t pid;

  switch (pid = fork())
  {
    case -1:
      syserror("handle_single_command() failed to fork()");
      break;
    case 0:
      DEBUG ? fprintf(stdout, "Child: Executing Command %s\n", argv[0]) : 0;
      execvp(argv[0], argv);
      syserror("handle_single_command() failed to execvp()");
      break;
    default:
      DEBUG ? fprintf(stdout, "Parent: The first child's pid is %d\n", pid) : 0;
      break;
  }
  while( wait(NULL) != -1 );
} /* END: handle_single_command(...) */

/* Important Note:
 * I think I can change to const char* commands[] once I am making a
 * space delimited copy.
 */
void sgt_major_pipage(char *commands[], const int num_commands)
{ // void sgt_major_pipage(const char *commands[], const int num_commands)
  pid_t pid;
  /* Holds separated <args> based on whitespace, including terminating NULL byte.
   * Example: If <commands> contains the above "echo hello \0" followed by 
   *          "echo hi\0" then the first element of <argv> will contain
   *          "echo\0" followed by "hi"
   */
  char *argv[MAX_LINE_WORDS + 1];
  memset(argv, 0, MAX_LINE_WORDS + 1);
  int argc = 0;

  char *delimiters = NULL;

  /* Create Pipes
   * Idk if we need pfds[2] and can reuse them for each pipe,
   * or if we need pfds[2*pipe_count] so each pipe has its own two.
   * Note: "Pipe Inheritence"
  int pipe_count = num_commands - 1;
  int pfds[2 * pipe_count];
  for (size_t i = 0; i < pipe_count; i++)
  {
    (pipe(pfds + 2*i) ==  -1) ? syserror("sgt_major_pipage() ==> Failed to create pipe") : 0;
  }
   */
  
  /* Now that we have the pipes set up, 
   * Deal with a whole bunch of pipes and their pfds...
   * Does it make more sense to loop on pipes or on commands?
   * - Maybe Pipes... else, track pipe_idx?
   */
  int idx = 0;
  for (size_t i = 0; i < num_commands; i++)
  {
    // pid = fork();

    // if (pid < 0)
    // { syserror("sgt_major_pipage() ==> Failed to fork"); }
    
    // if ( pid == 0 ) // Child Logic
    // {
    //   // Redirect STDIN of all but the first command / process.
    //   if (i > 0) // If not first command
    //   {
    //     if (dup2(pfds[i - 2], STDIN_FILENO) < 0) // Redirect with error-check.
    //     { syserror("sgt_major_pipage() ==> Failed to redirect STDIN"); }
    //   }

    //   // Redirect STDOUT of current process.
    //   if (i < num_commands - 1) // If not final command
    //   {
    //     if (dup2(pfds[i + 1], STDOUT_FILENO) < 0) // Redirect with error-check.
    //     { syserror("sgt_major_pipage() ==> Failed to redirect STDOUT"); }
    //   }

    //   // After dup2'ing, we close all pfds.
    //   for (size_t i = 0; i < 2 * pipe_count; i++)
    //   {
    //     if (close(pfds[i]) < 0)
    //     { syserror("sgt_major_pipage() ==> Failed to close pfd(s)"); }
    //   }


    //   // Execute Order 66... a bunch.
    //   argc = split_cmd_line(commands[i], argv, delimiters = " \t\n");
    //   if ( DEBUG ) { printf("i = %d: ", i); print_argv(argv); }
    //   handle_single_command(argv);
    // } /* End: Child Logic */
    
    // // Parent Closes pfds.
    // for (size_t i = 0; i < 2 * pipe_count; i++)
    // {
    //   if (close(pfds[i]) < 0)
    //   { syserror("sgt_major_pipage() ==> Failed to close pfd(s)"); }
    // }

    // // Parent Reaps Child Processes
    // while (wait(NULL) != -1);

    argc = split_cmd_line(commands[i], argv, delimiters = " \t\n");
    if ( DEBUG ) { printf("i = %d: ", i); print_argv(argv); }
    handle_single_command(argv);
  } /* END: for (size_t i = 0; i < num_commands; i+=2) */

} /* END: sgt_major_pipage(...)*/

int main()
{
  /* Initialize variables for use later on. */
  int num_commands = 0;
  char* delimiters = NULL;

  /* Buffer for reading one line of input. */
  char line[MAX_LINE_CHARS];

  /* Holds separated <commands> based on pipes, including terminating NULL byte.
   * Example: If <line> is "echo hello | echo hi" then "echo hello \0" will be
   * the first element stored in <commands>, followed by "echo hi\0"
   * Note the white spaces and NULL bytes.
   */
  char *commands[MAX_LINE_WORDS + 1];

  /* Loop until user hits Ctrl-D or some other input error occurs.
   * Each line can contain zero or more commands.
   */
  while ( fgets(line, MAX_LINE_CHARS, stdin) )
  {
    /* Freshen up <commands> and <args> for their next debut.
     * and reset necessary variables for new iteration.
     */
    memset(commands, 0, MAX_LINE_WORDS + 1);
    num_commands = 0;

    /* Split line into commands found between pipes. */
    num_commands = split_cmd_line(line, commands, delimiters = "|\n");

    sgt_major_pipage(commands, num_commands);

  } /* while ( fgets(line, MAX_LINE_CHARS, stdin) ) */

  return 0;
}
