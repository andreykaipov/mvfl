
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/linenoise.h"
#include "../lib/mpc.h"

#define GRAMMAR_FILE "src/mvfl-grammar.gr"

static char* prompt = "mvfl> ";
static char* prompt_exit = ":exit";

typedef struct {
    int type;
    long num;
    int err;
} mvfl_val;

enum { MVFL_NUM, MVFL_ERR };
enum { MVFL_ERR_DIV_ZERO, MVFL_ERR_BAD_OP, MVFL_ERR_BAD_NUM };

mvfl_val mvfl_val_num( long num ) {

    mvfl_val value;
    value.type = MVFL_NUM;
    value.num = num;

    return value;

}

mvfl_val mvfl_val_err( int err ) {

    mvfl_val value;
    value.type = MVFL_ERR;
    value.err = err;

    return value;

}

void mvfl_val_print_num( mvfl_val value ) {

    printf( "%li", value.num );

}

void mvfl_val_print_err( mvfl_val value ) {

    switch ( value.err ) {

        case MVFL_ERR_DIV_ZERO:
            printf( "Error: Cannot divide by zero!" );
            break;
        case MVFL_ERR_BAD_OP:
            printf( "Error: Invalid operator!" );
            break;
        case MVFL_ERR_BAD_NUM:
            printf( "Error: Invalid number!" );
            break;
        default:
            printf( "Oh there's some other kind of error." );

    }

}

void mvfl_val_print( mvfl_val value ) {

    switch ( value.type ) {

        case MVFL_NUM:
            mvfl_val_print_num( value );
            break;

        case MVFL_ERR:
            mvfl_val_print_err( value );
            break;

    }

}

void mvfl_val_println( mvfl_val value ) {

    mvfl_val_print( value );
    putchar( '\n' );

}

mvfl_val eval_op( mvfl_val x, char* op, mvfl_val y ) {

    if ( x.type == MVFL_ERR ) return x;
    if ( y.type == MVFL_ERR ) return y;
    if ( strcmp(op,"+") == 0 || strcmp(op,"add") == 0 ) {
        return mvfl_val_num( x.num + y.num );
    }
    else if ( strcmp(op,"-") == 0 || strcmp(op,"sub") == 0 ) {
        return mvfl_val_num( x.num - y.num );
    }
    else if ( strcmp(op,"*") == 0 || strcmp(op,"mul") == 0 ) {
        return mvfl_val_num( x.num * y.num );
    }
    else if ( strcmp(op,"/") == 0 || strcmp(op,"div") == 0 ) {
        return y.num == 0 ? mvfl_val_err( MVFL_ERR_DIV_ZERO )
                          : mvfl_val_num( x.num / y.num );
    }
    else if ( strcmp(op,"min") == 0 ) {
        return x.num < y.num ? mvfl_val_num( x.num )
                             : mvfl_val_num( y.num );
    }
    else if ( strcmp(op,"max") == 0 ) {
        return y.num < x.num ? mvfl_val_num( x.num )
                             : mvfl_val_num( y.num );
    }
    // if ( strcmp(op, "%") == 0 ) { return x % y; }
    else if ( strcmp(op, "^") == 0 || strcmp(op,"pow") == 0 ) {
        long total = 1;
        int i;
        for ( i = 0; i < y.num ; i++ ) total *= x.num;
        return mvfl_val_num( total );
    }
    return mvfl_val_num( 1338 );

}

mvfl_val eval_arithmetic_expr( mpc_ast_t* ast ) {

    // If tag contains integer, get the value of its contents.
    if ( strstr( ast->tag, "number" ) ) {

        if ( strstr( ast->tag, "integer" ) ) {
            errno = 0;
            long contents = strtol( ast->contents, NULL, 10 );
            return errno != ERANGE ? mvfl_val_num( contents )
                                   : mvfl_val_err( MVFL_ERR_BAD_NUM );
        }
        else return mvfl_val_num( 1337 );

    }
    // If root of tree, or if <Base> has children, take the middle one.
    else if ( strstr( ast->tag, "base" ) ||
              strcmp( ast->tag, ">" ) == 0 ) {

        return eval_arithmetic_expr( ast->children[ 1 ] );

    }
    // If tag is a term, and has two children, evaluate the sign of the term.
    else if ( strstr( ast->tag, "term" ) && ast->children_num == 2 ) {

        char* op = ast->children[ 0 ]->contents;
        mvfl_val x = eval_arithmetic_expr( ast->children[ 1 ] );
        mvfl_val zero = mvfl_val_num( 0 );

        return eval_op( zero, op, x );

    }
    // Check prefix thing.
    else if ( strstr( ast->tag, "prefixExpression" ) ) {

        char* op = ast->children[ 0 ]->contents;
        mvfl_val x = eval_arithmetic_expr( ast->children[ 1 ] );

        int i;
        for ( i = 2; i < ast->children_num; i += 1 ) {

            x = eval_op( x, op, eval_arithmetic_expr( ast->children[ i ] ) );

        }

        return x;

    }
    // Otherwise, just an expression or an infix expression. Loop from left to right, looking at each operator.
    else {

        mvfl_val x = eval_arithmetic_expr( ast->children[ 0 ] );

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

            mvfl_val result = eval_arithmetic_expr( parsed.output );
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
