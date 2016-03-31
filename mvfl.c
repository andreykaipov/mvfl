
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linenoise.h"

static char* prompt = "mvfl> ";
static char* prompt_exit = ":exit";

int main( int argc, char** argv ) {

    char* line;

    printf( "My Very First Language.\n" );
    printf( "Enter command `:exit` to exit.\n\n" );

    /* In a never ending loop */
    while ( ( line = linenoise( prompt ) ) != NULL ) {

        if ( strcmp( line, prompt_exit ) == 0 ) {

            printf( "Bye-bye!\n" );
            free( line );
            break;

        }

        printf( "echo: %s\n", line );

        linenoiseHistoryAdd( line );

        free( line );

    }

    return 0;

}