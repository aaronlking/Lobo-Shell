#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "parsetools.h"



/* Parse a command line into a list of tokens, separated by <delimiters>.
 * Returns the number of tokens between <delimiters>.
 */
int split_cmd_line(char* line, char** list_to_populate, char* delimiters) {
   char* saveptr;
   int i = 0;

    /* strtok() & strtok_r()
     *
     * http://linux.die.net/man/3/strtok_r
     * 
     * < char *strtok(char *str, const char *delim); >
     * - On the first call, <str> should be specified as the string to parse.
     * - In each subsequent call that should parse the same string,
     *   <str> should be NULL.
     * - <delim> specifies a set of bytes that delimit the tokens in the parsed
     *   string. The caller may specify different strings in <delim> in
     *   successigve calls that parse the same string.
     * - Returns a pointer to a null-terminated string containing the next
     *   token. This string does not include the delimiting byte.
     *   If not more tokens are found, strtok() returns NULL.
     * 
     * < char *strtok_r(char *str, const char *delim, char **saveptr); >
     * - On the first call to strtok_r(), line should point to the string to be
     *   parsed. And the value of saveptr is ignored.
     * - In subsequent calls, line should be NULL, and saveptr should be
     *   unchanged since the previous call.
     */
   *(list_to_populate + 0) = strtok_r(line, delimiters, &saveptr);

   while( *(list_to_populate + i) != NULL && i < MAX_LINE_WORDS - 1 )
   { *(list_to_populate + (++i)) = strtok_r(NULL, delimiters, &saveptr); }

   return i;
}
