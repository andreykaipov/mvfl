
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/linenoise.h"
#include "../lib/mpc.h"

#define GRAMMAR_FILE "src/mvfl-grammar.gr"

static char* prompt = "mvfl> ";
static char* prompt_exit = ":exit";

// Our ints our C's longs and our floats is C's doubles.
typedef long mvfl_int_t;
typedef double mvfl_float_t;

// Our types will be stored in enums.
typedef enum mvfl_type_t mvfl_type_t;

enum mvfl_type_t {
    MVFL_INTEGER,
    MVFL_FLOAT,
    MVFL_ERROR
};

// Our error codes are also in enums.
typedef enum mvfl_err_code_t mvfl_err_code_t;

enum mvfl_err_code_t {
    MVFL_ERROR_DIV_ZERO,
    MVFL_ERROR_BAD_OP,
    MVFL_ERROR_BAD_NUM
};

// Our number can either be an integer or floating-point decimals.
typedef union mvfl_number_t mvfl_number_t;

union mvfl_number_t {
    mvfl_int_t integer;
    mvfl_float_t floating;
};

// The manifestation of our values can either be error codes or numbers.
typedef union mvfl_manifestation_t mvfl_manifestation_t;

union mvfl_manifestation_t {
    mvfl_err_code_t err_code;
    mvfl_number_t number;
};

// Our values have a type, and a manifestation.
typedef struct mvfl_val_t mvfl_val_t;

struct mvfl_val_t {
    mvfl_type_t type;
    mvfl_manifestation_t manifestation;
};

/* ~~~~~~~~~~ */

mvfl_val_t mvfl_val_from_mvfl_int( mvfl_int_t i ) {

    mvfl_val_t value;

    value.type = MVFL_INTEGER;
    value.manifestation.number.integer = i;

    return value;

}

mvfl_val_t mvfl_val_from_mvfl_float( mvfl_float_t x ) {

    mvfl_val_t value;

    value.type = MVFL_FLOAT;
    value.manifestation.number.floating = x;

    return value;

}

mvfl_val_t mvfl_val_error( mvfl_err_code_t err_code ) {

    mvfl_val_t value;

    value.type = MVFL_ERROR;
    value.manifestation.err_code = err_code;

    return value;

}

void mvfl_val_print_err( mvfl_val_t value ) {

    switch ( value.manifestation.err_code ) {

        case MVFL_ERROR_DIV_ZERO:
            printf( "Error: Cannot divide by zero!" );
            break;

        case MVFL_ERROR_BAD_OP:
            printf( "Error: Invalid operator!" );
            break;

        case MVFL_ERROR_BAD_NUM:
            printf( "Error: Invalid number!" );
            break;

        default:
            printf( "Oh man there's some other kind of error." );
            break;

    }

}

void mvfl_val_print( mvfl_val_t value ) {

    switch ( value.type ) {

        case MVFL_INTEGER:
            printf( "%li", value.manifestation.number.integer );
            break;

        // This is just a stupid trick to make floats of value 0 have one leading zero.
        // It's just for asthetics.
        case MVFL_FLOAT:
            if ( value.manifestation.number.floating == (int) value.manifestation.number.floating )
                printf( "%g.0", value.manifestation.number.floating );
            else
                printf( "%.16g", value.manifestation.number.floating );
            break;

        case MVFL_ERROR:
            mvfl_val_print_err( value );
            break;

        default:
            printf( "Lol what am I even printing." );
            break;

    }

}

void mvfl_val_println( mvfl_val_t value ) {

    mvfl_val_print( value );
    putchar( '\n' );

}

mvfl_val_t eval_op( mvfl_val_t v, char* op, mvfl_val_t w ) {

    mvfl_type_t type_of_v = v.type;
    mvfl_type_t type_of_w = w.type;

    if ( type_of_v == MVFL_ERROR ) return v;
    if ( type_of_w == MVFL_ERROR ) return w;

    mvfl_int_t i = v.manifestation.number.integer;
    mvfl_int_t j = w.manifestation.number.integer;

    mvfl_float_t x = v.manifestation.number.floating;
    mvfl_float_t y = w.manifestation.number.floating;

    if ( strcmp(op,"+") == 0 || strcmp(op,"add") == 0 ) {

        return ( type_of_v == MVFL_INTEGER &&
                 type_of_w == MVFL_INTEGER ) ? mvfl_val_from_mvfl_int( i + j ) :
                   type_of_v == MVFL_INTEGER ? mvfl_val_from_mvfl_float( i + y ) :
                   type_of_w == MVFL_INTEGER ? mvfl_val_from_mvfl_float( x + j ) :
                                               mvfl_val_from_mvfl_float( x + y );

    }
    else if ( strcmp(op,"-") == 0 || strcmp(op,"sub") == 0 ) {

        return ( type_of_v == MVFL_INTEGER &&
                 type_of_w == MVFL_INTEGER ) ? mvfl_val_from_mvfl_int( i - j ) :
                   type_of_v == MVFL_INTEGER ? mvfl_val_from_mvfl_float( i - y ) :
                   type_of_w == MVFL_INTEGER ? mvfl_val_from_mvfl_float( x - j ) :
                                               mvfl_val_from_mvfl_float( x - y );

    }
    else if ( strcmp(op,"*") == 0 || strcmp(op,"mul") == 0 ) {

        return ( type_of_v == MVFL_INTEGER &&
                 type_of_w == MVFL_INTEGER ) ? mvfl_val_from_mvfl_int( i * j ) :
                   type_of_v == MVFL_INTEGER ? mvfl_val_from_mvfl_float( i * y ) :
                   type_of_w == MVFL_INTEGER ? mvfl_val_from_mvfl_float( x * j ) :
                                               mvfl_val_from_mvfl_float( x + y );
    }
    else if ( strcmp(op,"/") == 0 || strcmp(op,"div") == 0 ) {

        return ( j == 0 || y == 0 )          ? mvfl_val_error( MVFL_ERROR_DIV_ZERO ) :
               ( type_of_v == MVFL_INTEGER &&
                 type_of_w == MVFL_INTEGER ) ? mvfl_val_from_mvfl_int( i / j ) :
                   type_of_v == MVFL_INTEGER ? mvfl_val_from_mvfl_float( i / y ) :
                   type_of_w == MVFL_INTEGER ? mvfl_val_from_mvfl_float( x / j ) :
                                               mvfl_val_from_mvfl_float( x / y );

    }
    else if ( strcmp(op,"min") == 0 ) {

        return ( type_of_v == MVFL_INTEGER &&
                 type_of_w == MVFL_INTEGER ) ? (( i < j ) ? mvfl_val_from_mvfl_int( i ) : mvfl_val_from_mvfl_int( j ) ) :
                   type_of_v == MVFL_INTEGER ? (( i < y ) ? mvfl_val_from_mvfl_int( i ) : mvfl_val_from_mvfl_float( y ) ) :
                   type_of_w == MVFL_INTEGER ? (( x < j ) ? mvfl_val_from_mvfl_int( x ) : mvfl_val_from_mvfl_int( j ) ) :
                                               (( x < y ) ? mvfl_val_from_mvfl_int( x ) : mvfl_val_from_mvfl_float( y ) );

    }
    else if ( strcmp(op,"max") == 0 ) {

        return ( type_of_v == MVFL_INTEGER &&
                 type_of_w == MVFL_INTEGER ) ? (( i > j ) ? mvfl_val_from_mvfl_int( i ) : mvfl_val_from_mvfl_int( j ) ) :
                   type_of_v == MVFL_INTEGER ? (( i > y ) ? mvfl_val_from_mvfl_int( i ) : mvfl_val_from_mvfl_float( y ) ) :
                   type_of_w == MVFL_INTEGER ? (( x > j ) ? mvfl_val_from_mvfl_int( x ) : mvfl_val_from_mvfl_int( j ) ) :
                                               (( x > y ) ? mvfl_val_from_mvfl_int( x ) : mvfl_val_from_mvfl_float( y ) );

    }
    // if ( strcmp(op, "%") == 0 ) { return x % y; }
    // else if ( strcmp(op, "^") == 0 || strcmp(op,"pow") == 0 ) {
    //     long total = 1;
    //     int i;
    //     for ( i = 0; i < s ; i++ ) total *= r;
    //     return mvfl_val_from_mvfl_float( total );
    // }

    return mvfl_val_error( MVFL_ERROR_BAD_OP );

}

mvfl_val_t eval_arithmetic_expr( mpc_ast_t* ast ) {

    // If tag contains integer, get the value of its contents.
    if ( strstr( ast->tag, "number" ) ) {

        if ( strstr( ast->tag, "integer" ) ) {

            errno = 0;
            mvfl_int_t contents = strtol( ast->contents, NULL, 10 );

            return errno != ERANGE ? mvfl_val_from_mvfl_int( contents )
                                   : mvfl_val_error( MVFL_ERROR_BAD_NUM );

        }
        else if ( strstr( ast->tag, "float" ) ) {

            errno = 0;
            mvfl_float_t contents = strtod( ast->contents, NULL );

            return errno != ERANGE ? mvfl_val_from_mvfl_float( contents )
                                   : mvfl_val_error( MVFL_ERROR_BAD_NUM );

        }
        else return mvfl_val_from_mvfl_int( 1337 );

    }
    // If root of tree, or if <Base> has children, take the middle one.
    else if ( strstr( ast->tag, "base" ) ||
              strcmp( ast->tag, ">" ) == 0 ) {

        return eval_arithmetic_expr( ast->children[ 1 ] );

    }
    // If tag is a term, and has two children, evaluate the sign of the term.
    else if ( strstr( ast->tag, "term" ) && ast->children_num == 2 ) {

        char* op = ast->children[ 0 ]->contents;
        mvfl_val_t x = eval_arithmetic_expr( ast->children[ 1 ] );
        mvfl_val_t zero = mvfl_val_from_mvfl_int( 0 );

        return eval_op( zero, op, x );

    }
    // Check prefix thing.
    else if ( strstr( ast->tag, "prefixExpression" ) ) {

        char* op = ast->children[ 0 ]->contents;
        mvfl_val_t x = eval_arithmetic_expr( ast->children[ 1 ] );

        int i;
        for ( i = 2; i < ast->children_num; i += 1 ) {

            x = eval_op( x, op, eval_arithmetic_expr( ast->children[ i ] ) );

        }

        return x;

    }
    // Otherwise, just an expression or an infix expression. Loop from left to right, looking at each operator.
    else {

        mvfl_val_t x = eval_arithmetic_expr( ast->children[ 0 ] );

        int i;
        for ( i = 1; i < ast->children_num; i += 2 ) {

// (find_appropriate_evaln( x, ast->children[ i ] ))
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
