#ifndef PARSETOOLS_H
#define PARSETOOLS_H

/* Parse a command line into a list of tokens, separated by <delimiters>.
 * Returns the number of tokens between <delimiters>.
 */
int split_cmd_line(char* line, char** list_to_populate, char* delimiters); 

#endif
