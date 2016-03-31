
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linenoise.h"
#include "mpc.h"

static char* prompt = "mvfl> ";
static char* prompt_exit = ":exit";

int main( int argc, char** argv ) {

    mpc_parser_t* Zero       = mpc_new("zero");
    mpc_parser_t* PosDigit   = mpc_new("posDigit");
    mpc_parser_t* Digit      = mpc_new("digit");
    mpc_parser_t* Digits     = mpc_new("digits");
    mpc_parser_t* PosInteger = mpc_new("posInteger");
    mpc_parser_t* NegInteger = mpc_new("negInteger");
    mpc_parser_t* Integer    = mpc_new("integer");
    mpc_parser_t* Float      = mpc_new("float");
    mpc_parser_t* Number     = mpc_new("number");
    mpc_parser_t* Zeros      = mpc_new("zeros");
    mpc_parser_t* PaddedNumber = mpc_new("paddedNumber");
    mpc_parser_t* Constant = mpc_new("constant");
    mpc_parser_t* SignedConstant = mpc_new("signedConstant");

    mpc_parser_t* Expression = mpc_new("expression");
    mpc_parser_t* Term = mpc_new("term");
    mpc_parser_t* Xponent = mpc_new("xponent");
    mpc_parser_t* Factor = mpc_new("factor");

    mpc_parser_t* Mvfl = mpc_new("mvfl");

    mpca_lang_contents( MPCA_LANG_DEFAULT, "mvfl-grammar.gr",
           Zero, PosDigit, Digit, Digits,
           PosInteger, NegInteger, Integer, Float, Number, Zeros, PaddedNumber, Constant, SignedConstant,
           Expression, Term, Xponent, Factor,
           Mvfl );


    /* Finish initializing grammar. */

    printf( "My Very First Language.\n" );
    printf( "Enter command `:exit` to exit.\n\n" );

    char* line;
    while ( ( line = linenoise( prompt ) ) != NULL ) {

        if ( strcmp( line, prompt_exit ) == 0 ) {

            printf( "Bye-bye!\n" );
            free( line );
            break;

        }

        linenoiseHistoryAdd( line );

        mpc_result_t parsed;
        if ( mpc_parse("<stdin>", line, Mvfl, &parsed) ) {
            mpc_ast_print( parsed.output );
            mpc_ast_delete( parsed.output );
        }
        else {
            mpc_err_print( parsed.error );
            mpc_err_delete( parsed.error );
        }

        free( line );

    }

    mpc_cleanup( 18, Zero, PosDigit, Digit, Digits,
                     PosInteger, NegInteger, Integer, Float, Number, Zeros, PaddedNumber, Constant, SignedConstant,
                     Expression, Term, Xponent, Factor,
                     Mvfl );

    return 0;

}