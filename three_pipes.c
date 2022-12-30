#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(int argc, char* argv[])
{
    int pfds[3][2];
    
    // Create Pipes
    int i;
    for (i = 0; i < 3; i++)
    {
        if (pipe(pfds[i]) < 0)
        {
            /* If we are in a case where opening two pipes works,
             * but opening the third leads to an error, we would want to
             * close the pipes that were opened.
             */
            return 1;
        }
    }

    pid_t pid1 = fork();
    if ( pid1 < 0 )
        { return 2; }
    if ( pid1 == 0)
    {
        close(pfds[0][1]);
        close(pfds[1][0]);
        close(pfds[2][0]);
        close(pfds[2][1]);
        
        int x;
        if ( read(pfds[0][0], &x, sizeof(int)) < 0)
            { return 3; }
        
        x += 5;
        if ( write(pfds[1][1], &x, sizeof(int)) < 0)
            { return 4; }
        
        close(pfds[0][0]);
        close(pfds[1][1]);
        return 0;
    }

    pid_t pid2 = fork();
    if ( pid2 < 0 )
        { return 5; }
    
    if ( pid2 == 0)
    {
        close(pfds[0][0]);
        close(pfds[0][1]);
        close(pfds[1][1]);
        close(pfds[2][0]);
        
        int x;
        if ( read(pfds[1][0], &x, sizeof(int)) < 0)
            { return 6; }
        
        x += 5;
        if ( write(pfds[2][1], &x, sizeof(int)) < 0)
            { return 7; }
        
        close(pfds[1][0]);
        close(pfds[2][1]);
        return 0;
    }

    // Parent Closes
    close(pfds[0][0]);
    close(pfds[1][0]);
    close(pfds[1][1]);
    close(pfds[2][1]);
    
    int x = 0;
    if ( write(pfds[0][1], &x, sizeof(int)) < 0)
    { return 8; }
    
    if ( read(pfds[2][0], &x, sizeof(int)) < 0)
    { return 9; }
    
    printf("Result is %d\n", x);
    // Parent Closes All
    close(pfds[0][1]);
    close(pfds[2][0]);

    while(wait(NULL) != -1);

    return 0;
}