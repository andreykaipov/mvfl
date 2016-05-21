#include "../lib/mpc.h"
#include "../lib/linenoise.h"

#include "mvfl.h"

#define GRAMMAR_FILE "src/mvfl-grammar.gr"

static char* prompt = "mvfl> ";
static char* prompt_exit = ":exit";

mvfl_val_t eval_arithmetic_expr( mpc_ast_t* ast );
mvfl_val_t* mvfl_val_read( mpc_ast_t* tree );

int main( int argc, char** argv ) {
/*
    mvfl_val_t* a = mvfl_val_from_symbol( "T" );
    mvfl_val_t* b = mvfl_val_from_symbol( "5" );
    mvfl_val_t* c1 = mvfl_val_from_int( 10 );
    mvfl_val_t* c2 = mvfl_val_from_int( 10 );
    mvfl_val_t* d = mvfl_val_from_float( 13.3 );

    mvfl_sexpr_t* sexpr1 = mvfl_sexpr_init();
    mvfl_sexpr_t* sexpr2 = mvfl_sexpr_init();

    mvfl_sexpr_print( sexpr1, '(', ')' );
    putchar('\n');
    mvfl_sexpr_print( sexpr2, '(', ')' );
    putchar('\n');
   
    mvfl_sexpr_append( sexpr1, a );
    mvfl_sexpr_append( sexpr1, b );
    mvfl_sexpr_append( sexpr2, c1 );
    mvfl_sexpr_append( sexpr2, c2 );
    mvfl_sexpr_append( sexpr2, d );

    mvfl_sexpr_print( sexpr1, '(', ')' );
    putchar('\n');
    mvfl_sexpr_print( sexpr2, '(', ')' );
    putchar('\n');

    printf( "2nd element of first sexpr is " );
    mvfl_val_println( sexpr1->first->next->value );
    printf( "2nd element of second sexpr is " );
    mvfl_val_println( sexpr2->first->next->value );

    printf( "Count of first sexpr is %i\n", sexpr1->count );
    printf( "Count of second sexpr is %i\n", sexpr2->count );

    mvfl_sexpr_t* cloned = mvfl_sexpr_clone( sexpr1 );
    mvfl_val_t* val = mvfl_val_from_sexpr( cloned );

    mvfl_sexpr_append( sexpr1, val );

    mvfl_sexpr_print( sexpr1, '(', ')' );
    putchar('\n');

    printf( "Count of first sexpr is now %i\n", sexpr1->count );

    a->manifestation.symbol = "@";
    sexpr1->first->next->next->value->manifestation.sexpr->first->value->manifestation.symbol = "FFF";
    sexpr1->last->value->manifestation.sexpr->first->value->manifestation.symbol = "PPP";

    mvfl_sexpr_print( sexpr1, '(', ')' );

    mvfl_sexpr_delete( cloned );
    mvfl_sexpr_delete( sexpr1 );
    mvfl_sexpr_delete( sexpr2 );
    */

    mpc_parser_t* Integer = mpc_new("Integer");
    mpc_parser_t* Float = mpc_new("Float");
    mpc_parser_t* Number = mpc_new("Number");
    mpc_parser_t* Symbol = mpc_new("Symbol");
    mpc_parser_t* PlusOp = mpc_new("PlusOp");
    mpc_parser_t* MultOp = mpc_new("MultOp");
    mpc_parser_t* ExpnOp = mpc_new("ExpnOp");
    mpc_parser_t* InfixExpr = mpc_new("InfixExpr");
    mpc_parser_t* PrefixExpr = mpc_new("PrefixExpr");
    mpc_parser_t* Factor = mpc_new("Factor");
    mpc_parser_t* Term = mpc_new("Term");
    mpc_parser_t* Base = mpc_new("Base");
    mpc_parser_t* Expr = mpc_new("Expr");
    mpc_parser_t* Sexpr = mpc_new("Sexpr");
    mpc_parser_t* Mvfl = mpc_new("Mvfl");

    mpca_lang_contents( MPCA_LANG_DEFAULT, GRAMMAR_FILE,
        Integer, Float, Number, PlusOp, MultOp, ExpnOp, Symbol,
        InfixExpr, PrefixExpr, Factor, Term, Base, Expr, Sexpr,
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

            printf("Parse tree:\n");
            mpc_ast_print( parsed.output );

             // mvfl_val_t result = eval_arithmetic_expr( parsed.output );
            mvfl_val_t* val = mvfl_val_read( parsed.output );
            mvfl_val_println( val );

            mvfl_val_delete( val );

            mpc_ast_delete( parsed.output );

        }
        else {

            mpc_err_print( parsed.error );
            mpc_err_delete( parsed.error );

        }

        free( line );

    }

    mpc_cleanup( 15,
        Integer, Float, Number, PlusOp, MultOp, ExpnOp, Symbol,
        InfixExpr, PrefixExpr, Factor, Term, Base, Expr, Sexpr,
        Mvfl
    );

    return 0;

}

