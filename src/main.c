#include "../lib/mpc.h"
#include "../lib/linenoise.h"

#include "mvfl.h"

#define GRAMMAR_FILE "src/mvfl-grammar.gr"

static char* prompt = "mvfl> ";
static char* prompt_exit = ":exit";

int main( int argc, char** argv ) {

    mpc_parser_t* Integer = mpc_new("integer");
    mpc_parser_t* Float = mpc_new("float");
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* SignedNumber = mpc_new("signedNumber");
    mpc_parser_t* PrefixOperator = mpc_new("prefixOperator");
    mpc_parser_t* PrefixExpression = mpc_new("prefixExpression");
    mpc_parser_t* InfixExpression = mpc_new("infixExpression");
    mpc_parser_t* Factor = mpc_new("factor");
    mpc_parser_t* Term = mpc_new("term");
    mpc_parser_t* Base = mpc_new("base");
    mpc_parser_t* Expression = mpc_new("expression");
    mpc_parser_t* Mvfl = mpc_new("mvfl");

    mpca_lang_contents( MPCA_LANG_DEFAULT, GRAMMAR_FILE,
        Integer, Float, Number, SignedNumber, PrefixOperator, PrefixExpression, InfixExpression, Factor, Term, Base,
        Expression,
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
        if ( mpc_parse( "<stdin>", line, Mvfl, &parsed ) ) {

            mvfl_val_t result = eval_arithmetic_expr( parsed.output );
            mvfl_val_println( result );

            printf("Parse tree:\n");
            mpc_ast_print( parsed.output );
            mpc_ast_delete( parsed.output );

        }
        else {

            mpc_err_print( parsed.error );
            mpc_err_delete( parsed.error );

        }

        free( line );

    }

    mpc_cleanup( 11,
        Integer, Float, Number, SignedNumber, PrefixOperator, PrefixExpression, InfixExpression, Factor, Term, Base,
        Expression,
        Mvfl );

    return 0;

}
