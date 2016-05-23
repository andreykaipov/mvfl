#include "../lib/mpc.h"

#include "mvfl.h"
#include "arithmetic.h"

mvfl_val_t eval_arithmetic_expr( mpc_ast_t* ast );
mvfl_val_t* mvfl_val_read( mpc_ast_t* tree );

mvfl_val_t* mvfl_val_from_int( mvfl_int_t k );
mvfl_val_t* mvfl_val_from_float( mvfl_float_t x );
mvfl_val_t* mvfl_val_from_symbol( mvfl_symbol_t sym );
mvfl_val_t* mvfl_val_from_sexpr( mvfl_sexpr_t* sexpr );
void mvfl_val_delete( mvfl_val_t* val );

void mvfl_val_print( mvfl_val_t* value );
void mvfl_val_println( mvfl_val_t* value );

mvfl_sexpr_t* mvfl_sexpr_init( void );
void mvfl_sexpr_append( mvfl_sexpr_t* sexpr, mvfl_val_t* val );
mvfl_val_t* mvfl_sexpr_pop( mvfl_sexpr_t* sexpr, int index );
mvfl_sexpr_t* mvfl_sexpr_clone( mvfl_sexpr_t* sexpr );
void mvfl_sexpr_delete( mvfl_sexpr_t* sexpr );
void mvfl_sexpr_print( mvfl_sexpr_t* sexpr, char open, char close );

mvfl_val_t* mvfl_eval_op( char* op, mvfl_sexpr_t* sexpr );
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~ MVFL constructors for each MVFL type ~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Creates an MVFL value pointer from an MVFL integer.
mvfl_val_t* mvfl_val_from_int( mvfl_int_t k ) {

    mvfl_val_t* value = malloc( sizeof(mvfl_val_t) );
    value->type = MVFL_INTEGER;
    value->manifestation.num.as_int = k;
    return value;

}

// Creates an MVFL value pointer from an MVFL float.
mvfl_val_t* mvfl_val_from_float( mvfl_float_t x ) {

    mvfl_val_t* value = malloc( sizeof(mvfl_val_t) );
    value->type = MVFL_FLOAT;
    value->manifestation.num.as_float = x;
    return value;

}

// Creates an MVFL value pointer from an MVFL symbol.
mvfl_val_t* mvfl_val_from_symbol( mvfl_symbol_t sym ) {

    mvfl_val_t* value = malloc( sizeof(mvfl_val_t) );
    value->type = MVFL_SYMBOL;
    value->manifestation.symbol = sym;
    return value;

}

// Creates an MVFL value pointer from an MVFL error message.
mvfl_val_t* mvfl_val_from_error( mvfl_error_t msg ) {

    mvfl_val_t* value = malloc( sizeof(mvfl_val_t) );
    value->type = MVFL_ERROR;
    value->manifestation.error = msg;
    return value;

}

// Creates an MVFL value pointer from an MVFL S-Expression.
mvfl_val_t* mvfl_val_from_sexpr( mvfl_sexpr_t* sexpr ) {

    mvfl_val_t* value = malloc( sizeof(mvfl_val_t) );
    value->type = MVFL_SEXPR;
    value->manifestation.sexpr = sexpr;
    return value;

}

mvfl_val_t* mvfl_val_clone( mvfl_val_t* value ) {

    switch ( value->type ) {
        case MVFL_INTEGER:
            return mvfl_val_from_int( value->manifestation.num.as_int );
        case MVFL_FLOAT:
            return mvfl_val_from_float( value->manifestation.num.as_float );
        case MVFL_SYMBOL:
            return mvfl_val_from_symbol( value->manifestation.symbol );
        case MVFL_ERROR:
            return mvfl_val_from_error( value->manifestation.error );
        case MVFL_SEXPR:
            return mvfl_val_from_sexpr( value->manifestation.sexpr );
        default:
            fprintf( stderr, "WOAH WHAT AM I CLONING\n" );
            exit(1);
            break;
    }

}


// Deletes an MVFL value accordingly.
void mvfl_val_delete( mvfl_val_t* value ) {

    switch ( value->type ) {
        case MVFL_INTEGER:
            break;
        case MVFL_FLOAT:
            break;
        case MVFL_SYMBOL:
            break;
        case MVFL_ERROR:
            break;
        case MVFL_SEXPR:
            mvfl_sexpr_delete( value->manifestation.sexpr );
            break;
        default:
            break;
    }
    free( value );

}

//
void mvfl_val_print( mvfl_val_t* value ) {
    switch ( value->type ) {
        case MVFL_INTEGER:
            printf( "%li", value->manifestation.num.as_int );
            break;
        case MVFL_FLOAT:
            if ( value->manifestation.num.as_float == (int) value->manifestation.num.as_float )
                printf( "%g.0", value->manifestation.num.as_float );
            else
                printf( "%.16g", value->manifestation.num.as_float );
            break;
        case MVFL_SYMBOL:
            printf( "%s", value->manifestation.symbol );
            break;
        case MVFL_ERROR:
            printf( "Error! %s", value->manifestation.error );
            break;
        case MVFL_SEXPR:
            mvfl_sexpr_print( value->manifestation.sexpr, '(', ')' );
            break;
        default:
            printf( "Lol what am I even printing." );
            break;

    }
}

void mvfl_val_println( mvfl_val_t* value ) {

    mvfl_val_print( value );
    putchar( '\n' );

}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~ S-Expressions and Cons Cells ~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Creates an empty S-Expression.
mvfl_sexpr_t* mvfl_sexpr_init( void ) {

    mvfl_sexpr_t* sexpr = malloc( sizeof(mvfl_sexpr_t) );
    sexpr->count = 0;
    sexpr->first = NULL;
    sexpr->last = NULL;
    return sexpr;

}

// Creates a lonely cons cell with a pointer to a value, and no next nor previous.
mvfl_cons_cell_t* mvfl_cons_cell( mvfl_val_t* value ) {

    mvfl_cons_cell_t* cons_cell = malloc( sizeof(mvfl_cons_cell_t) );
    cons_cell->value = value;
    cons_cell->next = NULL;
    cons_cell->prev = NULL;
    return cons_cell;

}

// Appends an MVFL value pointer to the end of an existing S-Expression.
// Be careful to NOT append the same value pointer to an S-Expression twice!
// Doing so will cause a double free error upon deletion of the S-Expr!
// Also, do NOT do the following...
// sexpr = mvfl_sexpr_append( sexpr, mvfl_val_from_sexpr(sexpr) );
// fromIS S-Expr.// Doing this will cause an infinite S-Expression.
void mvfl_sexpr_append( mvfl_sexpr_t* sexpr, mvfl_val_t* val ) {

    if ( sexpr->count == 0 ) {
        sexpr->first = sexpr->last = mvfl_cons_cell( val );
    }
    else {
        mvfl_cons_cell_t* cell = mvfl_cons_cell( val );
        cell->prev = sexpr->last;
        sexpr->last->next = cell;
        sexpr->last = cell;
    }
    sexpr->count += 1;

}

mvfl_sexpr_t* mvfl_sexpr_clone( mvfl_sexpr_t* sexpr ) {

    mvfl_sexpr_t* cloned = mvfl_sexpr_init();

    mvfl_cons_cell_t* cell = sexpr->first;
    while ( cell != NULL ) {
        mvfl_sexpr_append( cloned, mvfl_val_clone(cell->value) );
        cell = cell->next;
    }

    return cloned;

}

// Inserts an MVFL value into the index-th position of an S-Expression.
// In other words, finds the index-th element in an S-Expression, and inserts
// our new element BEFORE it, shifting subsequent elements to the right by one.
void mvfl_sexpr_insert( mvfl_sexpr_t* sexpr, mvfl_val_t* val, int index ) {
    
    mvfl_cons_cell_t* cell = mvfl_cons_cell( val );

    if ( index == 0 ) {
        sexpr->first->prev = cell;
        cell->next = sexpr->first;
        sexpr->first = cell;
    }
    else if ( index == sexpr->count - 1 ) {
        mvfl_sexpr_append( sexpr, val );
    }
    else {
        mvfl_cons_cell_t* current = sexpr->first;
        for ( int i = 0; i < index; i += 1 ) { current = current->next; }

        // Connect new cell in-between two existing cells.
        cell->prev = current->prev;
        cell->next = current;

        // Make the surrounding cells point to the new cell.
        current->prev->next = cell;
        current->prev = cell;
    }

    sexpr->count += 1;

}

// Pops the index-th MVFL value out of the S-Expression,
// shifting all subsequent elements to the left by one.
mvfl_val_t* mvfl_sexpr_pop( mvfl_sexpr_t* sexpr, int index ) {

    // Find the i-th cons cell and adjust its pointers.
    mvfl_cons_cell_t* current = sexpr->first;
    for ( int i = 0; i < index; i += 1 ) { current = current->next; }
    if ( current->prev != NULL ) { current->prev->next = current->next; }
    if ( current->next != NULL ) { current->next->prev = current->prev; }

    // Fix the ptrs of the sexpr for the edge-cases.
    if ( index == 0 ) { sexpr->first = sexpr->first->next; }
    if ( index == sexpr->count - 1 ) { sexpr->last = sexpr->last->prev; }

    // We're removing a cell, so adjust the count.
    sexpr->count -= 1;
   
    // Store the value in a variable to free the cell before returning.
    mvfl_val_t* popped = current->value;
    free( current );

    return popped;

}


// Prints the constituent cells of an S-Expression, delimited by spaces.
// The entire S-Expression is enclosed between open and close characters.
void mvfl_sexpr_print( mvfl_sexpr_t* sexpr, char open, char close ) {

    putchar( open );
    mvfl_cons_cell_t* cell = sexpr->first;
    while ( cell != NULL ) {
        mvfl_val_print( cell->value );
        cell = cell->next;
        // If we're at the last cell, don't print the trailing space.
        if ( cell != NULL ) { putchar( ' ' ); }
    }
    putchar( close );

}

// Deletes the constituent cons cells in an S-Expression.
void mvfl_sexpr_delete( mvfl_sexpr_t* sexpr ) {

    mvfl_cons_cell_t* cell;
    while ( sexpr->first != NULL ) {
        cell = sexpr->first;
        sexpr->first = sexpr->first->next;
        mvfl_val_delete( cell->value );
        free( cell );
    }
    free( sexpr );

}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~ Read and Evaluate ~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

mvfl_val_t* mvfl_val_read_int( mpc_ast_t* tree ) {
    errno = 0;
    mvfl_int_t i = strtol( tree->contents, NULL, 10 );
    // printf("%li\n", i);
    return errno != ERANGE ? mvfl_val_from_int( i )
                           : mvfl_val_from_error( "Invalid number." );
}

mvfl_val_t* mvfl_val_read_float( mpc_ast_t* tree ) {
    errno = 0;
    mvfl_float_t x = strtod( tree->contents, NULL );
    // printf("%f\n",x);
    return errno != ERANGE ? mvfl_val_from_float( x )
                           : mvfl_val_from_error( "Invalid number." );
}

mvfl_val_t* mvfl_val_read_sym( mpc_ast_t* tree ) {
    return mvfl_val_from_symbol( tree->contents );
}

mvfl_val_t* mvfl_val_read( mpc_ast_t* tree );
mvfl_sexpr_t* mvfl_infix_to_sexpr( mpc_ast_t* tree );
mvfl_sexpr_t* mvfl_prefix_to_sexpr( mpc_ast_t* tree );

mvfl_sexpr_t* mvfl_infix_to_sexpr( mpc_ast_t* tree ) {

    mvfl_sexpr_t* sexpr = mvfl_sexpr_init();
    mvfl_sexpr_append( sexpr, mvfl_val_read( tree->children[1] ) ); //symbol
    mvfl_sexpr_append( sexpr, mvfl_val_read( tree->children[0] ) );
    mvfl_sexpr_append( sexpr, mvfl_val_read( tree->children[2] ) );

    for ( int i = 3; i < tree->children_num; i += 2 ) {

        mvfl_sexpr_t* sexpr2 = mvfl_sexpr_init();
        mvfl_sexpr_append( sexpr2, mvfl_val_read( tree->children[i] ) ); //symbol
        mvfl_sexpr_append( sexpr2, mvfl_val_from_sexpr( sexpr ) );
        mvfl_sexpr_append( sexpr2, mvfl_val_read( tree->children[i+1] ) );
        sexpr = sexpr2;

    }

    return sexpr;

}

/*
mvfl_sexpr_t* mvfl_prefix_to_sexpr( mpc_ast_t* tree ) {

    mvfl_sexpr_t* sexpr = mvfl_sexpr_init();

    mvfl_sexpr_append( sexpr, mvfl_val_read( tree->children[0] ) );
    */



mvfl_val_t* mvfl_val_read( mpc_ast_t* tree ) {

    char* tag = tree->tag;

    if ( strstr(tag, "Integer") ) { return mvfl_val_read_int( tree ); }
    if ( strstr(tag, "Float") ) { return mvfl_val_read_float( tree ); }
    if ( strstr(tag, "Symbol") ) { return mvfl_val_read_sym( tree ); }
    if ( strstr(tag, "PlusOp") ) { return mvfl_val_read_sym( tree ); }
    if ( strstr(tag, "MultOp") ) { return mvfl_val_read_sym( tree ); }
    if ( strstr(tag, "ExpnOp") ) { return mvfl_val_read_sym( tree ); }

    if ( strstr(tag, "InfixExpr") ) { return mvfl_val_from_sexpr( mvfl_infix_to_sexpr(tree) ); }
    if ( strstr(tag, "Factor") ) { return mvfl_val_from_sexpr( mvfl_infix_to_sexpr(tree) ); }
    if ( strstr(tag, "Term") ) { return mvfl_val_from_sexpr( mvfl_infix_to_sexpr(tree) ); }

    // Create an empty sexpr for one of the following nonterminals.
    // We use an else-if to prevent the potential memory leak!
    // e.g. Whenever a tree branch looks like Sexpr|...|Base|...
    mvfl_sexpr_t* sexpr;
    if ( strcmp(tag, ">") == 0 ) { sexpr = mvfl_sexpr_init(); }
    else if ( strstr(tag, "Sexpr") ) { sexpr = mvfl_sexpr_init(); }
    else if ( strstr(tag, "PrefixExpr") ) { sexpr = mvfl_sexpr_init(); }
    else if ( strstr(tag, "Base") ) { sexpr = mvfl_sexpr_init(); }

    // Fill the S-Expression with the children of the nonterminal.
    for ( int i = 0; i < tree->children_num; i++ ) {

        mpc_ast_t* child = tree->children[i];

        if ( strcmp(child->contents, "(") == 0 ) { continue; }
        if ( strcmp(child->contents, ")") == 0 ) { continue; }
        if ( strcmp(child->contents, "{") == 0 ) { continue; }
        if ( strcmp(child->contents, "}") == 0 ) { continue; }
        if ( strcmp(child->tag, "regex") == 0 ) { continue; }

        fprintf(stderr,"HELLO\n");
        mvfl_sexpr_append( sexpr, mvfl_val_read( child ) );

    }

    fprintf(stderr,"BYE\n");
    //return mvfl_val_from_sexpr( sexpr )->manifestation.sexpr->first->value;
    return mvfl_val_from_sexpr( sexpr );
    

}

mvfl_val_t* mvfl_eval_val( mvfl_val_t* value ); 
mvfl_val_t* mvfl_eval_sexpr( mvfl_sexpr_t* sexpr );

mvfl_val_t* mvfl_eval_val( mvfl_val_t* value ) {
    
    switch ( value->type ) {
        case MVFL_SEXPR:
            return mvfl_eval_sexpr( value->manifestation.sexpr );
        default:
            return mvfl_val_clone( value );
    }

}

mvfl_val_t* mvfl_eval_sexpr( mvfl_sexpr_t* sexpr ) {
    
    mvfl_cons_cell_t* cell = sexpr->first;
    while ( cell != NULL ) {
        mvfl_val_t* evaluated = mvfl_eval_val( cell->value );
        mvfl_val_delete( cell->value );
        cell->value = evaluated;
        cell = cell->next;
    }
    
    if ( sexpr->count == 1 ) {
        return sexpr->first->value;
    }
    else {
        mvfl_val_t* first = mvfl_sexpr_pop( sexpr, 0 );
        if ( first->type != MVFL_SYMBOL ) {
            mvfl_val_delete( first );
            //mvfl_sexpr_delete( sexpr );
            return mvfl_val_from_error("No operator found");
        }
        else {
            char* operand = first->manifestation.symbol;
            mvfl_val_delete( first );

            mvfl_val_t* result = mvfl_eval_op( operand, sexpr );

            //mvfl_sexpr_delete( sexpr );

            return result;
        }
    }

}

mvfl_val_t* mvfl_eval_op( char* op, mvfl_sexpr_t* sexpr ) {
    
    mvfl_cons_cell_t* cell = sexpr->first;
    while ( cell != NULL ) {
        mvfl_type_t type = cell->value->type;
        if ( type != MVFL_INTEGER && type != MVFL_FLOAT ) {
            mvfl_sexpr_delete( sexpr );
            return mvfl_val_from_error( "Cannot operate on non-number!" );
        }
        cell = cell->next;
    }

    // Get the first argument.
    mvfl_val_t* value = mvfl_sexpr_pop( sexpr, 0 );

    if ( strcmp(op, "-") == 0 && sexpr->count == 0 ) {
        value->manifestation.num.as_int = - value->manifestation.num.as_int;
    }

    while ( sexpr->count > 0 ) {

            fprintf(stderr,":dshsdhs\n");
        mvfl_val_t* arg = mvfl_sexpr_pop( sexpr, 0 );
        
        if ( strcmp(op, "add") == 0 ) {
            value->manifestation.num.as_int += arg->manifestation.num.as_int;
        }

        mvfl_val_delete( arg );

    }

 //   mvfl_sexpr_delete( sexpr );

    return value; 
   /* 
    if ( strcmp(op,"+") == 0 || strcmp(op,"add") == 0 ) { return mvfl_add_intern( v, w ); }
    if ( strcmp(op,"-") == 0 || strcmp(op,"sub") == 0 ) { return mvfl_sub_intern( v, w ); }
    if ( strcmp(op,"*") == 0 || strcmp(op,"mul") == 0 ) { return mvfl_mul_intern( v, w ); }
    if ( strcmp(op,"/") == 0 || strcmp(op,"div") == 0 ) { return mvfl_div_intern( v, w ); }
    if ( strcmp(op,"%") == 0 ) { return mvfl_mod_intern( v, w ); }
    if ( strcmp(op,"min") == 0 ) { return mvfl_min_intern( v, w ); }
    if ( strcmp(op,"max") == 0 ) { return mvfl_max_intern( v, w ); }

    return mvfl_val_from_error( MVFL_ERROR_BAD_OP );
    */

}

/*
mvfl_val_t eval_arithmetic_expr( mpc_ast_t* ast ) {

    // If tag contains integer, get the value of its contents.
    if ( strstr( ast->tag, "number" ) ) {

        if ( strstr( ast->tag, "integer" ) ) {

            errno = 0;
            mvfl_int_t contents = strtol( ast->contents, NULL, 10 );

            return errno != ERANGE ? mvfl_val_from_int( contents )
                                   : mvfl_val_from_error( MVFL_ERROR_BAD_NUM );

        }
        else if ( strstr( ast->tag, "float" ) ) {

            errno = 0;
            mvfl_float_t contents = strtod( ast->contents, NULL );

            return errno != ERANGE ? mvfl_val_from_float( contents )
                                   : mvfl_val_from_error( MVFL_ERROR_BAD_NUM );

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

*/
