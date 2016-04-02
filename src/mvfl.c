
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/linenoise.h"
#include "../lib/mpc.h"

#define GRAMMAR_FILE "src/mvfl-grammar.gr"

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

int eval_op( double x, char* op, double y ) {

    if ( strcmp(op,"+") == 0 || strcmp(op,"add") == 0 ) { return x + y; }
    if ( strcmp(op,"-") == 0 || strcmp(op,"sub") == 0 ) { return x - y; }
    if ( strcmp(op,"*") == 0 || strcmp(op,"mul") == 0 ) { return x * y; }
    if ( strcmp(op,"/") == 0 || strcmp(op,"div") == 0 ) { return x / y; }
    if ( strcmp(op,"min") == 0 ) { return x < y ? x : y; }
    if ( strcmp(op,"max") == 0 ) { return y < x ? x : y; }
    // if ( strcmp(op, "%") == 0 ) { return x % y; }
    if ( strcmp(op, "^") == 0 || strcmp(op,"pow") == 0 ) {
        int total = 1;
        int i;
        for ( i = 0; i < y; i++ ) total *= x;
        return total;
    }
    return 0;

}

int eval_arithmetic_expr( mpc_ast_t* ast )
{

    // If tag contains integer, get the value of its contents.
    if ( strstr( ast->tag, "number" ) ) {

        if ( strstr( ast->tag, "integer" ) ) { return atoi( ast->contents ); }
        else return 1337;

    }
    // If root of tree, or if <Base> has children, take the middle one.
    else if ( strstr( ast->tag, "base" ) ||
              strcmp( ast->tag, ">" ) == 0 ) {

        return eval_arithmetic_expr( ast->children[ 1 ] );

    }
    // If tag is a term, and has two children, evaluate the sign of the term.
    else if ( strstr( ast->tag, "term" ) && ast->children_num == 2 ) {

        char* op = ast->children[ 0 ]->contents;
        long x = eval_arithmetic_expr( ast->children[ 1 ] );

        return eval_op( 0, op, x );

    }
    // Check prefix thing.
    else if ( strstr( ast->tag, "prefixExpression" ) ) {

        char* op = ast->children[ 0 ]->contents;
        long x = eval_arithmetic_expr( ast->children[ 1 ] );

        int i;
        for ( i = 2; i < ast->children_num; i += 1 ) {

            x = eval_op( x, op, eval_arithmetic_expr( ast->children[ i ] ) );

        }

        return x;

    }
    // Otherwise, just an expression or an infix expression. Loop from left to right, looking at each operator.
    else {

        long x = eval_arithmetic_expr( ast->children[ 0 ] );

        int i;
        for ( i = 1; i < ast->children_num; i += 2 ) {

            char* op = ast->children[ i ]->contents;
            x = eval_op( x, op, eval_arithmetic_expr( ast->children[ i + 1 ] ) );

        }

        return x;

    }

}

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

    mpc_cleanup( 11,
        Integer, Float, Number, SignedNumber, PrefixOperator, PrefixExpression, InfixExpression, Factor, Term, Base,
        Expression,
        Mvfl );

    return 0;

}
