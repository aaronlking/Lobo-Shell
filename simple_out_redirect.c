#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include "include/constants.h"

// clock_t start, end;
// double cpu_time_used;
// start = clock();
// /* Code */
// end = clock();
// cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
// printf("CPU Time: %f (s).\n", cpu_time_used);

void syserror(const char *s)
{
  extern int errno;
  fprintf(stderr, "%s\n", s);
  fprintf(stderr, " (%s)\n", strerror(errno));
  exit(1);
}

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

int split_cmd_line(char *line, char **list_to_populate, char *delimiters)
{
    // char* delimiters = "<\n";
    char *saveptr;
    int i = 0;

    list_to_populate[0] = strtok_r(line, delimiters, &saveptr);

    while (list_to_populate[i] != NULL && i < MAX_LINE_WORDS - 1)
    {
        list_to_populate[++i] = strtok_r(NULL, delimiters, &saveptr);
    }

    return i;
}

void output_redirect(char *command1, char *command2)
{
    char *delimters = " \t";
    
    char *argv1[MAX_LINE_WORDS + 1];
    int argc1 = split_cmd_line(command1, argv1, delimters);

    char *argv2[MAX_LINE_WORDS + 1];
    int argc2 = split_cmd_line(command2, argv2, delimters);

    // Open (or create) File to write to.
    int fd = open(argv2[0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1)
    { syserror("output_redirect() -- Failed to open()"); }
    // else: file is opened and fd contains the lowest numbered, unused fd.

    pid_t pid = fork();

    if (pid == -1) // Confirm successful fork.
    { syserror("output_redirect() -- Failed to Fork"); }

    if (pid == 0)
    {// Child Logic
        dup2(fd, STDOUT_FILENO); // Redirect STDOUT to fd
        
        if (close(fd) == -1)
        { syserror("output_redirect() -- Child Failed to close()"); }
        
        execvp(argv1[0], argv1);
        syserror("output_redirect() -- Child Failed to execvp()");
    }

    if (close(fd) == -1)
    { syserror("output_redirect() -- Parent Failed to close()"); }

    memset(argv1, 0, MAX_LINE_WORDS + 1);
    memset(argv2, 0, MAX_LINE_WORDS + 1);
}

int main()
{
    char line[MAX_LINE_CHARS];

    char *line_commands[MAX_LINE_CHARS + 1];

    char *delimiters = ">\n";

    size_t i;
    while (fgets(line, MAX_LINE_CHARS, stdin))
    {
        memset(line_commands, 0, MAX_LINE_WORDS + 1);

        int num_commands = split_cmd_line(line, line_commands, delimiters);

        output_redirect(line_commands[0], line_commands[1]);
    }
    return 0;
}