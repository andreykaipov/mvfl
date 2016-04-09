#include "../lib/mpc.h"

#include "mvfl.h"
#include "arithmetic.h"

mvfl_val_t mvfl_val_from_int( mvfl_int_t i ) {

    mvfl_val_t value;

    value.type = MVFL_INTEGER;
    value.manifestation.num.as_int = i;

    return value;

}

mvfl_val_t mvfl_val_from_float( mvfl_float_t x ) {

    mvfl_val_t value;

    value.type = MVFL_FLOAT;
    value.manifestation.num.as_float = x;

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

        case MVFL_ERROR_MOD_ON_FLOATS:
            printf( "Error: Euclidean modulus not defined on floats." );
            break;

        default:
            printf( "Oh man there's some other kind of error." );
            break;

    }

}

void mvfl_val_print( mvfl_val_t value ) {

    switch ( value.type ) {

        case MVFL_INTEGER:
            printf( "%li", value.manifestation.num.as_int );
            break;

        // This is just a stupid trick to make floats of value 0 have one leading zero.
        // It's just for asthetics.
        case MVFL_FLOAT:
            if ( value.manifestation.num.as_float == (int) value.manifestation.num.as_float )
                printf( "%g.0", value.manifestation.num.as_float );
            else
                printf( "%.16g", value.manifestation.num.as_float );
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

    if ( strcmp(op,"+") == 0 || strcmp(op,"add") == 0 ) { return mvfl_add_intern( v, w ); }
    if ( strcmp(op,"-") == 0 || strcmp(op,"sub") == 0 ) { return mvfl_sub_intern( v, w ); }
    if ( strcmp(op,"*") == 0 || strcmp(op,"mul") == 0 ) { return mvfl_mul_intern( v, w ); }
    if ( strcmp(op,"/") == 0 || strcmp(op,"div") == 0 ) { return mvfl_div_intern( v, w ); }
    if ( strcmp(op,"%") == 0 ) { return mvfl_mod_intern( v, w ); }
    if ( strcmp(op,"min") == 0 ) { return mvfl_min_intern( v, w ); }
    if ( strcmp(op,"max") == 0 ) { return mvfl_max_intern( v, w ); }

    return mvfl_val_error( MVFL_ERROR_BAD_OP );

}

mvfl_val_t eval_arithmetic_expr( mpc_ast_t* ast ) {

    // If tag contains integer, get the value of its contents.
    if ( strstr( ast->tag, "number" ) ) {

        if ( strstr( ast->tag, "integer" ) ) {

            errno = 0;
            mvfl_int_t contents = strtol( ast->contents, NULL, 10 );

            return errno != ERANGE ? mvfl_val_from_int( contents )
                                   : mvfl_val_error( MVFL_ERROR_BAD_NUM );

        }
        else if ( strstr( ast->tag, "float" ) ) {

            errno = 0;
            mvfl_float_t contents = strtod( ast->contents, NULL );

            return errno != ERANGE ? mvfl_val_from_float( contents )
                                   : mvfl_val_error( MVFL_ERROR_BAD_NUM );

        }
        else return mvfl_val_from_int( 1337 );

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
        mvfl_val_t zero = mvfl_val_from_int( 0 );

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

