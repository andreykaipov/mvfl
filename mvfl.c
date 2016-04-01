
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linenoise.h"
#include "mpc.h"

static char* prompt = "mvfl> ";
static char* prompt_exit = ":exit";

int number_of_nodes( mpc_ast_t* ast )
{

    if ( ast->children_num == 0 ) {

        return 1;

    }
    else {

        int i = 0;
        int totalNodes = 1;

        for ( i = 0; i < ast->children_num; i++ ) {

            totalNodes += number_of_nodes( ast->children[ i ] );

        }

        return totalNodes;

    }

}

int eval_op( long x, char* op, long y ) {

    if ( strcmp(op, "+") == 0 ) { return x + y; }
    if ( strcmp(op, "-") == 0 ) { return x - y; }
    if ( strcmp(op, "*") == 0 ) { return x * y; }
    if ( strcmp(op, "/") == 0 ) { return x / y; }
    if ( strcmp(op, "%") == 0 ) { return x % y; }
    return 0;

}

int eval_arithmetic_expr( mpc_ast_t* ast )
{

    if ( strstr( ast->tag, "integer" ) ) {
        
        return atoi( ast->contents );

    }
    else if ( strstr( ast->tag, "base" ) || 
              strcmp( ast->tag, ">" ) == 0 ) {
        
        return eval_arithmetic_expr( ast->children[ 1 ] );

    }
    else {

        long x = eval_arithmetic_expr( ast->children[ 0 ] );
        char* op = ast->children[ 1 ]->contents;
        long y =  eval_arithmetic_expr( ast->children[ 2 ] );

        return eval_op( x, op, y );

    }

}

int main( int argc, char** argv ) {

    mpc_parser_t* Integer = mpc_new("integer");
    mpc_parser_t* Float = mpc_new("float");
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* InfixOperator = mpc_new("infixOperator");
    mpc_parser_t* PrefixOperator = mpc_new("prefixOperator");
    mpc_parser_t* Expression = mpc_new("expression");
    mpc_parser_t* Factor = mpc_new("factor");
    mpc_parser_t* Term = mpc_new("term");
    mpc_parser_t* Base = mpc_new("base");
    mpc_parser_t* Exponent = mpc_new("exponent");

    mpc_parser_t* Mvfl = mpc_new("mvfl");

    mpca_lang_contents( MPCA_LANG_DEFAULT, "mvfl-grammar.gr",
            Integer, Float, Number, InfixOperator, PrefixOperator,
            Expression, Factor, Term, Base, Exponent,
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
        if ( mpc_parse("input", line, Mvfl, &parsed) ) {

            long result = eval_arithmetic_expr( parsed.output );

            printf("%li\n", result);

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

    mpc_cleanup( 10, Integer, Float, Number, InfixOperator, PrefixOperator,
            Expression, Factor, Term, Base, Exponent, Mvfl );

    return 0;

}
