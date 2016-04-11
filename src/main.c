#include "../lib/mpc.h"
#include "../lib/linenoise.h"

#include "mvfl.h"

#define GRAMMAR_FILE "src/mvfl-grammar.gr"

// static char* prompt = "mvfl> ";
// static char* prompt_exit = ":exit";

void mvfl_val_println( mvfl_val_t* value );
mvfl_val_t eval_arithmetic_expr( mpc_ast_t* ast );
mvfl_val_t* mvfl_val_read( mpc_ast_t* tree );

mvfl_sexpr_t mvfl_init_sexpr( void );
mvfl_val_t* mvfl_val_from_int( mvfl_int_t i );
mvfl_val_t* mvfl_val_from_float( mvfl_float_t i );
mvfl_sexpr_t mvfl_sexpr_append( mvfl_sexpr_t sexpr, mvfl_val_t* val );
void mvfl_sexpr_print( mvfl_sexpr_t sexpr, char open, char close );
void mvfl_sexpr_delete( mvfl_sexpr_t sexpr );
mvfl_val_t* mvfl_val_from_sexpr( mvfl_sexpr_t sexpr );

int main( int argc, char** argv ) {

    mvfl_val_t* a = mvfl_val_from_int( 2 );
    mvfl_val_t* b = mvfl_val_from_int( 5 );
    mvfl_val_t* c1 = mvfl_val_from_int( 10 );
    mvfl_val_t* c2 = mvfl_val_from_int( 10 );
    mvfl_val_t* d = mvfl_val_from_float( 13.3 );

    mvfl_sexpr_t sexpr1 = mvfl_init_sexpr();
    mvfl_sexpr_t sexpr2 = mvfl_init_sexpr();

    mvfl_sexpr_print( sexpr1, '(', ')' );
    putchar('\n');
    mvfl_sexpr_print( sexpr2, '(', ')' );
    putchar('\n');

    sexpr1 = mvfl_sexpr_append( sexpr1, a );
    sexpr1 = mvfl_sexpr_append( sexpr1, b );

    sexpr2 = mvfl_sexpr_append( sexpr2, c1 );
    sexpr2 = mvfl_sexpr_append( sexpr2, c2 );

    mvfl_sexpr_print( sexpr1, '(', ')' );
    putchar('\n');
    mvfl_sexpr_print( sexpr2, '(', ')' );
    putchar('\n');

    // printf( "2nd element is " );
    // mvfl_val_println( sexpr.first->next->value );
    // printf( "Count is %i\n", sexpr.count );

    mvfl_val_t* val = mvfl_val_from_sexpr( sexpr2 );

    mvfl_val_println( val );
    //
    sexpr1 = mvfl_sexpr_append( sexpr1, val );
    //
    // printf( "Count is %i\n", sexpr.count );

    mvfl_sexpr_print( sexpr1, '(', ')' );
    // mvfl_val_println( val );
    //
    //
    //
    mvfl_sexpr_delete( sexpr1 );
    mvfl_sexpr_delete( sexpr2 );

/*
    mpc_parser_t* Integer = mpc_new("Integer");
    mpc_parser_t* Float = mpc_new("Float");
    mpc_parser_t* Number = mpc_new("Number");
    mpc_parser_t* PrefixOperator = mpc_new("PrefixOperator");
    mpc_parser_t* PrefixExpression = mpc_new("PrefixExpression");
    mpc_parser_t* InfixExpression = mpc_new("InfixExpression");
    mpc_parser_t* Factor = mpc_new("Factor");
    mpc_parser_t* Term = mpc_new("Term");
    mpc_parser_t* Base = mpc_new("Base");
    mpc_parser_t* Expression = mpc_new("Expression");
    mpc_parser_t* Sexpr = mpc_new("Sexpr");
    mpc_parser_t* Mvfl = mpc_new("Mvfl");

    mpca_lang_contents( MPCA_LANG_DEFAULT, GRAMMAR_FILE,
        Integer, Float, Number, PrefixOperator, PrefixExpression, InfixExpression, Factor, Term, Base,
        Expression, Sexpr,
        Mvfl
    );

    // Finish initializing grammar.

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

            // mvfl_val_t result = eval_arithmetic_expr( parsed.output );
            mvfl_val_t* val = mvfl_val_read( parsed.output );
            // mvfl_val_println( val );

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

    mpc_cleanup( 12,
        Integer, Float, Number, PrefixOperator, PrefixExpression, InfixExpression, Factor, Term, Base,
        Expression, Sexpr,
        Mvfl
    );
*/
    return 0;

}
