#include "../lib/mpc.h"

#include "mvfl.h"
#include "arithmetic.h"

mvfl_val_t eval_arithmetic_expr( mpc_ast_t* ast );

mvfl_val_t* mvfl_val_from_int( mvfl_int_t k );
mvfl_val_t* mvfl_val_from_float( mvfl_float_t x );
mvfl_val_t* mvfl_val_from_symbol( mvfl_symbol_t sym );
mvfl_val_t* mvfl_val_from_sexpr( mvfl_sexpr_t* sexpr );
mvfl_val_t* mvfl_val_from_qexpr( mvfl_sexpr_t* qexpr );

void mvfl_val_delete( mvfl_val_t* val );

void mvfl_val_print( mvfl_val_t* value );
void mvfl_val_println( mvfl_val_t* value );

mvfl_sexpr_t* mvfl_sexpr_init( void );
void mvfl_sexpr_append( mvfl_sexpr_t* sexpr, mvfl_val_t* val );
mvfl_val_t* mvfl_sexpr_pop( mvfl_sexpr_t* sexpr, int index );
mvfl_sexpr_t* mvfl_sexpr_clone( mvfl_sexpr_t* sexpr );
void mvfl_sexpr_delete( mvfl_sexpr_t* sexpr );
void mvfl_sexpr_print( mvfl_sexpr_t* sexpr, char open, char close );

void mvfl_read_ast( mpc_ast_t* tree, mvfl_sexpr_t* sexpr );
void mvfl_read_infix_into_sexpr( mpc_ast_t* tree, mvfl_sexpr_t* sexpr );
mvfl_sexpr_t* mvfl_prefix_to_sexpr( mpc_ast_t* tree );
mvfl_val_t* mvfl_eval_val( mvfl_val_t* value ); 
mvfl_val_t* mvfl_eval_sexpr( mvfl_sexpr_t* sexpr );

mvfl_val_t* mvfl_eval_intern_op( char* op, mvfl_sexpr_t* sexpr );
mvfl_val_t* mvfl_eval_qexpr_op( char* op, mvfl_sexpr_t* sexpr );
mvfl_val_t* mvfl_eval_arithmetic_op( char* op, mvfl_sexpr_t* sexpr );
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

mvfl_val_t* mvfl_val_from_qexpr( mvfl_qexpr_t* qexpr ) {

    mvfl_val_t* value = malloc( sizeof(mvfl_val_t) );
    value->type = MVFL_QEXPR;
    value->manifestation.qexpr = qexpr;
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
            return mvfl_val_from_sexpr( mvfl_sexpr_clone(value->manifestation.sexpr) );
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
        case MVFL_QEXPR:
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
        case MVFL_QEXPR:
            mvfl_sexpr_print( value->manifestation.qexpr, '{', '}' );
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

void mvfl_sexpr_prepend( mvfl_sexpr_t* sexpr, mvfl_val_t* val ) {

    if ( sexpr->count == 0 ) {
        sexpr->first = sexpr->last = mvfl_cons_cell( val );
    }
    else {
        mvfl_cons_cell_t* cell = mvfl_cons_cell( val );
        cell->next = sexpr->first;
        sexpr->first->prev = cell;
        sexpr->first = cell;
    }
    sexpr->count += 1;

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

// Inserts an MVFL value into the index-th position of an S-Expression.
// In other words, finds the index-th element in an S-Expression, and inserts
// our new element BEFORE it, shifting subsequent elements to the right by one.
void mvfl_sexpr_insert( mvfl_sexpr_t* sexpr, mvfl_val_t* val, int index ) {
    
    if ( index == 0 ) {
        mvfl_sexpr_prepend( sexpr, val );
    }
    else if ( index == sexpr->count - 1 ) {
        mvfl_sexpr_append( sexpr, val );
    }
    else if ( 0 < index && index < sexpr->count - 1 ) {
        // Find the index-th cell.
        mvfl_cons_cell_t* current = sexpr->first;
        for ( int i = 0; i < index; i += 1 ) { current = current->next; }

        mvfl_cons_cell_t* cell = mvfl_cons_cell( val );
        // Connect new cell in-between two existing cells.
        cell->prev = current->prev;
        cell->next = current;
        // Make the surrounding cells point to the new cell.
        current->prev->next = cell;
        current->prev = cell;
    }
    else {
        fprintf( stderr, "You're doing it wrong.\n" );
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

mvfl_val_t* mvfl_sexpr_head( mvfl_sexpr_t* sexpr ) {

    mvfl_val_t* val = mvfl_sexpr_pop( sexpr, 0 );
    mvfl_sexpr_delete( sexpr );
    return val;

}

mvfl_sexpr_t* mvfl_sexpr_tail( mvfl_sexpr_t* sexpr ) {

    mvfl_val_delete( mvfl_sexpr_pop(sexpr, 0) );
    return sexpr;

}

// Concatenates sexpr2 onto the end of sexpr1.
void mvfl_sexpr_concat( mvfl_sexpr_t* sexpr1, mvfl_sexpr_t* sexpr2 ) {

    if ( sexpr1->count == 0 ) {
        sexpr1->first = sexpr2->first;
        sexpr1->last = sexpr2->last;
        sexpr1->count = sexpr2->count;
    }
    else if ( sexpr2->count == 0 ) {
        free( sexpr2 );
    }
    else {
        sexpr1->last->next = sexpr2->first;
        sexpr2->first->prev = sexpr1->last;
        sexpr1->last = sexpr2->last;
        sexpr1->count += sexpr2->count;
        free( sexpr2 );
    }

}

void mvfl_sexpr_clear( mvfl_sexpr_t* sexpr );

// Takes a pointer to an sexpr S and points it to the sexpr (S).
void mvfl_sexpr_enclose( mvfl_sexpr_t* sexpr ) {

    mvfl_sexpr_t* cloned = mvfl_sexpr_clone( sexpr );
    mvfl_sexpr_t* enclosed = mvfl_sexpr_init();
    mvfl_sexpr_append( enclosed, mvfl_val_from_sexpr(cloned) );

    mvfl_sexpr_clear( sexpr );
    *sexpr = *enclosed;
    free( enclosed );

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

void mvfl_sexpr_println( mvfl_sexpr_t* sexpr ) {
    mvfl_sexpr_print( sexpr, '(', ')' );
}

void mvfl_sexpr_clear( mvfl_sexpr_t* sexpr ) {

    mvfl_cons_cell_t* cell;
    while ( sexpr->first != NULL ) {
        cell = sexpr->first;
        sexpr->first = sexpr->first->next;
        mvfl_val_delete( cell->value );
        free( cell );
    }

}

// Deletes the constituent cons cells in an S-Expression.
void mvfl_sexpr_delete( mvfl_sexpr_t* sexpr ) {

    mvfl_sexpr_clear( sexpr );
    free( sexpr );

}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~ Q-Expressions ~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

mvfl_qexpr_t* mvfl_qexpr_init( void ) {
    return mvfl_sexpr_init();
}

void mvfl_qexpr_append( mvfl_qexpr_t* qexpr, mvfl_val_t* val ) {
    mvfl_sexpr_append( qexpr, val );
}

mvfl_val_t* mvfl_qexpr_pop( mvfl_qexpr_t* qexpr, int index ) {
    return mvfl_sexpr_pop( qexpr, index );
}

void mvfl_qexpr_delete( mvfl_qexpr_t* qexpr ) {
    mvfl_sexpr_delete( qexpr );
}

mvfl_val_t* mvfl_qexpr_head( mvfl_qexpr_t* qexpr ) {
    return mvfl_sexpr_head( qexpr );
}

mvfl_qexpr_t* mvfl_qexpr_tail( mvfl_qexpr_t* qexpr ) {
    mvfl_val_delete( mvfl_qexpr_pop(qexpr, 0) );
    return qexpr;
}

void mvfl_qexpr_concat( mvfl_qexpr_t* qexpr1, mvfl_qexpr_t* qexpr2 ) {
    mvfl_sexpr_concat( qexpr1, qexpr2 );
}

// Changes an MVFL value whose manifestation is a Sexpr to an Qexpr.

mvfl_val_t* mvfl_qexpr_list( mvfl_val_t* val ) {
    val->type = MVFL_QEXPR;
    val->manifestation.qexpr = val->manifestation.sexpr;
    return val;
}

mvfl_val_t* mvfl_qexpr_eval( mvfl_qexpr_t* qexpr ) {
    return mvfl_eval_sexpr( qexpr ); 
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

mvfl_val_t* mvfl_val_read_number( mpc_ast_t* tree ) {
    
    if ( tree->children_num == 0 ) {
        if ( strstr(tree->tag, "Integer") ) {
            return mvfl_val_read_int( tree );
        }
        else if ( strstr(tree->tag, "Float") ) {
            return mvfl_val_read_float( tree );
        }
        else {
            exit(24);
        }
    }
    else if ( tree->children_num == 2 ) {
        if ( strcmp(tree->children[0]->contents, "+") == 0 ) {
            return mvfl_val_read_number( tree->children[1] );
        }
        else if ( strcmp(tree->children[0]->contents, "-") == 0 ) {
            mvfl_val_t* number = mvfl_val_read_number( tree->children[1] );
            if ( number->type == MVFL_INTEGER ) {
                number->manifestation.num.as_int = - number->manifestation.num.as_int;
            }
            if ( number->type == MVFL_FLOAT ) {
                number->manifestation.num.as_float = - number->manifestation.num.as_float;
            }
            return number;
        }
        else {
            exit(23);
        }
    }
    else {
        exit(22);
    }

}

mvfl_val_t* mvfl_val_read_sym( mpc_ast_t* tree ) {
    return mvfl_val_from_symbol( tree->contents );
}

/* Takes an abstract syntax tree representing an arithmetic Infix Expression
   and creates an equivalent prefix S-Expression out of it.
   Since the parser creates a tree adhering to the order of operations,
   this function isn't actually all that hard than I made it out to be... :-) */
void mvfl_read_infix_into_sexpr( mpc_ast_t* tree, mvfl_sexpr_t* sexpr ) {
    
    // First remove the spaces from the tree.
    for ( int i = 0; i < tree->children_num; i += 1 ) {
        if ( *tree->children[i]->contents == ' ' ) {
            mpc_ast_delete( tree->children[i] );
            memmove( &tree->children[i],
                     &tree->children[i+1],
                     sizeof(mpc_ast_t*) * (tree->children_num - i - 1) );
            tree->children_num -= 1;
            i -= 1;
        }
    } 

    // If the current passed in Sexpr is empty...
    if ( sexpr->count == 0 ) {
        mvfl_read_ast( tree->children[1], sexpr ); // symbol
        mvfl_read_ast( tree->children[0], sexpr ); // first arg
        mvfl_read_ast( tree->children[2], sexpr ); // second arg
    }
    // Otherwise, append a new Sexpr to the end of the current one.
    else if ( sexpr->count > 0 ) {
        mvfl_sexpr_t* sexpr2 = mvfl_sexpr_init();
        mvfl_read_ast( tree->children[1], sexpr2 ); // symbol
        mvfl_read_ast( tree->children[0], sexpr2 ); // first arg
        mvfl_read_ast( tree->children[2], sexpr2 ); // second arg
        mvfl_sexpr_append( sexpr, mvfl_val_from_sexpr(sexpr2) );
        sexpr = sexpr2;
    }

    for ( int i = 3; i < tree->children_num; i += 2 ) {

        mpc_ast_t* prevOp = tree->children[i-2];
        mpc_ast_t* currOp = tree->children[i];
        mpc_ast_t* nextArg = tree->children[i+1];

        // Verify that ops with same precedence are on the same level of the tree.
        if ( strcmp( prevOp->tag,currOp->tag ) == 0 ) {
            // If ops are equal, just append the arg.
            // Otherwise, enclose and prepend op, and then append the next arg.
            if ( strcmp( prevOp->contents,currOp->contents ) != 0 ) {
                mvfl_sexpr_enclose( sexpr );
                mvfl_sexpr_prepend( sexpr, mvfl_val_from_symbol(currOp->contents) );
            }
            mvfl_read_ast( nextArg, sexpr );
        }
        else {
            fprintf(stderr,"Your grammar is messed up Andrey!\n");
            exit(111);
        }

    }

}

// Reads an MPC abstract syntax tree by reading it into an MVFL S-Expression.
void mvfl_read_ast( mpc_ast_t* tree, mvfl_sexpr_t* sexpr ) {

    // Sexpr and Qexpr are like the same thing, so 
    mvfl_sexpr_t* anothaSexpr = NULL;
    mvfl_qexpr_t* qexpr = NULL;

    char* tag = tree->tag;

    if ( strstr(tag,"Number") ) {
        mvfl_sexpr_append( sexpr, mvfl_val_read_number(tree) );
    }
    else if ( strstr(tag,"Symbol") ||
              strstr(tag,"PlusOp") || strstr(tag,"MultOp") || strstr(tag,"ExpnOp") ) {
        mvfl_sexpr_append( sexpr, mvfl_val_read_sym(tree) );
    }
    else if ( strstr(tag,"InfixExpr") || strstr(tag,"Factor") || strstr(tag,"Term") ) {
        mvfl_read_infix_into_sexpr( tree, sexpr );
    }
    // We do not create anotha sexpr for these tags because it would be redundant.
    // They exist mostly in the grammar for neatness and proper parsing.
    else if ( strcmp(tag,">") == 0 || strstr(tag,"PrefixExpr") || strstr(tag,"Base")
              || strstr(tag,"Expr") ) {
        anothaSexpr = sexpr;
    }
    else if ( strstr(tag,"Sexpr") ) {
        anothaSexpr = mvfl_sexpr_init();
        mvfl_sexpr_append( sexpr, mvfl_val_from_sexpr( anothaSexpr ) );
    }
    else if ( strstr(tag,"Qexpr") ) {
        qexpr = mvfl_qexpr_init();
        mvfl_sexpr_append( sexpr, mvfl_val_from_qexpr( qexpr ) );
    }
    /*
    else if ( strstr(tag,"Space") == 0 ) {
        //return;
    }
    */
    else {
        fprintf(stderr,"Failed to read AST. The tag %s was not handled.\n", tag);
        exit(21);
    }

    // Fill the S-Expression with the children of the nonterminal.
    for ( int i = 0; i < tree->children_num; i += 1 ) {

        mpc_ast_t* childTree = tree->children[i];

        if ( strcmp(childTree->tag, "regex") == 0 ) { continue; }
        if ( strcmp(childTree->contents, "(") == 0 ) { continue; }
        else if ( strcmp(childTree->contents, ")") == 0 ) { continue; }
        else if ( strcmp(childTree->contents, "{") == 0 ) { continue; }
        else if ( strcmp(childTree->contents, "}") == 0 ) { continue; }
        else if ( strcmp(childTree->contents, " ") == 0 ) { continue; }

        if ( anothaSexpr != NULL ) { mvfl_read_ast(childTree, anothaSexpr); }
        else if ( qexpr != NULL ) { fprintf(stderr,"HERE I AM\n"); mvfl_read_ast(childTree, qexpr); }

    }

}

mvfl_val_t* mvfl_eval_val( mvfl_val_t* value ) {
    
    if ( value->type == MVFL_SEXPR ) {
        mvfl_sexpr_t* sexpr = value->manifestation.sexpr;
        free( value );
        return mvfl_eval_sexpr( sexpr );
    }
    else {
        return value;
    }

}

mvfl_val_t* mvfl_eval_sexpr( mvfl_sexpr_t* sexpr ) {
    
    mvfl_val_t* first = sexpr->first->value;
    if ( first->type != MVFL_SYMBOL || // short-circuit or
         strcmp(first->manifestation.symbol,"list") != 0 ) {
        mvfl_cons_cell_t* cell = sexpr->first;
        while ( cell != NULL ) {
            mvfl_val_t* evaluated = mvfl_eval_val( cell->value );
            // mvfl_val_delete( cell->value );
            cell->value = evaluated;
            cell = cell->next;
        }
    }

    if ( sexpr->count == 0 ) {
        mvfl_sexpr_delete( sexpr );
        return mvfl_val_from_sexpr( mvfl_sexpr_init() );
    }
    
    // We look at the first value of an Sexpr to decide what to do.
    if ( sexpr->count == 1 ) {
        mvfl_val_t* first = mvfl_sexpr_pop( sexpr, 0 );
        mvfl_sexpr_delete( sexpr );
        return mvfl_eval_val( first );
    }
    else {
        fprintf(stderr,"SHSHHSS\n");
        mvfl_val_t* first = mvfl_sexpr_pop( sexpr, 0 );
        // Otherwise, ensure first value is a symbol.
        if ( first->type != MVFL_SYMBOL ) {
            mvfl_val_delete( first );
            mvfl_sexpr_delete( sexpr );
            return mvfl_val_from_error("No operator found");
        }
        else {
            char* operand = first->manifestation.symbol;
            mvfl_val_t* result = mvfl_eval_intern_op( operand, sexpr );
            mvfl_val_delete( first );

 //           mvfl_sexpr_delete( sexpr );

            return result;
        }
    }

}

mvfl_val_t* mvfl_qexpr_head_intern( mvfl_sexpr_t* args ) {

    if ( args->first->value->type != MVFL_QEXPR ) {
        mvfl_sexpr_delete( args );
        return mvfl_val_from_error("Function `head` requires a Q-Expr.");
    }
    if ( args->count != 1 ) {
        mvfl_sexpr_delete( args );
        return mvfl_val_from_error("Function 'head' passed more than 1 argument.");
    }
    mvfl_val_t* val = mvfl_sexpr_pop( args, 0 );
    mvfl_qexpr_t* qexpr = val->manifestation.qexpr;
    free( val );
    mvfl_sexpr_delete( args );
    return mvfl_qexpr_head( qexpr );
}

mvfl_val_t* mvfl_qexpr_tail_intern( mvfl_sexpr_t* args ) {
    if ( args->first->value->type != MVFL_QEXPR ) {
        mvfl_sexpr_delete( args );
        return mvfl_val_from_error("Function tail` requires a Q-Expr.");
    }
    if ( args->count != 1 ) {
        mvfl_sexpr_delete( args );
        return mvfl_val_from_error("Function 'tail' passed more than 1 argument.");
    }
    mvfl_val_t* val = mvfl_sexpr_pop( args, 0 );
    mvfl_qexpr_t* qexpr = val->manifestation.qexpr;
    free( val );
    mvfl_sexpr_delete( args );
    mvfl_qexpr_t* tail = mvfl_qexpr_tail( qexpr );
    return mvfl_val_from_qexpr( tail );
}

mvfl_val_t* mvfl_qexpr_join_intern( mvfl_sexpr_t* args ) {
    mvfl_cons_cell_t* cell = args->first;
    while ( cell != NULL ) {
        if ( cell->value->type != MVFL_QEXPR ) {
            mvfl_sexpr_delete( args );
            return mvfl_val_from_error("Function 'join' expects arguments all \
of which are Q-Expressions.");
        }
        cell = cell->next;
    }

    mvfl_val_t* val = mvfl_sexpr_pop( args, 0 );
    mvfl_qexpr_t* firstQexpr = val->manifestation.qexpr;
    free( val );
    while ( args->count > 0 ) {
        mvfl_val_t* val = mvfl_sexpr_pop( args, 0 );
        mvfl_qexpr_t* nextQexpr = val->manifestation.qexpr;
        free( val );
        mvfl_qexpr_concat( firstQexpr, nextQexpr );
    }
    mvfl_sexpr_delete( args );
    return mvfl_val_from_qexpr( firstQexpr );
}

mvfl_val_t* mvfl_qexpr_list_intern( mvfl_sexpr_t* args ) {
    return mvfl_val_from_qexpr( args );
}

mvfl_val_t* mvfl_qexpr_eval_intern( mvfl_sexpr_t* args ) {
    /*
    if ( args->first->value->type != MVFL_QEXPR &&
         args->first->value->type != MVFL_SEXPR ) {
        mvfl_sexpr_delete( args );
        return mvfl_val_from_error("Function 'eval' requires a Q-Expr or a S-Expr.");
    }
    */
    if ( args->count != 1 ) {
        mvfl_sexpr_delete( args );
        return mvfl_val_from_error("Function 'eval' passed more than 1 argument.");
    }
    /*
    mvfl_val_t* val = mvfl_qexpr_pop( args, 0 );
    mvfl_qexpr_t* qexpr = val->manifestation.qexpr;
    free( val );
    mvfl_sexpr_delete( args );
    return mvfl_qexpr_eval( qexpr );
    */
    if ( args->first->value->type == MVFL_QEXPR ) {
        mvfl_val_t* val = mvfl_qexpr_pop( args, 0 );
        mvfl_qexpr_t* qexpr = val->manifestation.qexpr;
        free( val );
        mvfl_sexpr_delete( args );
        return mvfl_qexpr_eval( qexpr );
    }
    else {
        return mvfl_eval_sexpr( args );
    }
}

mvfl_val_t* mvfl_eval_intern_op( char* op, mvfl_sexpr_t* sexpr ) {

    if ( strcmp("head",op) == 0 || strcmp("tail",op) == 0 || strcmp("join",op) == 0
      || strcmp("list",op) == 0 || strcmp("eval",op) == 0 ) {
        return mvfl_eval_qexpr_op( op, sexpr );
    }
    else if ( strcmp("add",op) == 0 || strcmp("sum",op) == 0 || strcmp("sub",op) == 0
           || strcmp("diff",op) == 0 || strcmp("mul",op) == 0 || strcmp("prod",op) == 0
           || strcmp("div",op) == 0 || strcmp("min",op) == 0 || strcmp("max",op) == 0 
           || strstr("+-/*",op) ) {
        return mvfl_eval_arithmetic_op( op, sexpr );
    }
    else {
        mvfl_sexpr_delete( sexpr );
        return mvfl_val_from_error( "Unknown function!" );
    }

}


mvfl_val_t* mvfl_eval_qexpr_op( char* op, mvfl_sexpr_t* args ) {

    if ( strcmp(op,"head") == 0 ) { return mvfl_qexpr_head_intern( args ); }
    if ( strcmp(op,"tail") == 0 ) { return mvfl_qexpr_tail_intern( args ); }
    if ( strcmp(op,"join") == 0 ) { return mvfl_qexpr_join_intern( args ); }
    if ( strcmp(op,"list") == 0 ) { return mvfl_qexpr_list_intern( args ); }
    if ( strcmp(op,"eval") == 0 ) { return mvfl_qexpr_eval_intern( args ); }
    /*
    if ( strcmp(op,"tail") == 0 ) {
        mvfl_sexpr_delete( sexpr );
        return mvfl_qexpr_tail( qexpr );
    }
    if ( strcmp(op,"join") == 0 ) {
        while ( sexpr->count > 0 ) {
            mvfl_val_t* val = mvfl_sexpr_pop( sexpr, 0 );
            mvfl_qexpr_t* nextQexpr = val->manifestation.qexpr;
            free( val );
            mvfl_qexpr_cat( qexpr, nextQexpr );
        }
        mvfl_sexpr_delete( sexpr );
        return mvfl_val_from_qexpr( qexpr );
    }
    */

}
    


mvfl_val_t* mvfl_eval_arithmetic_op( char* op, mvfl_sexpr_t* sexpr ) {
    
    // first check everything in the sexpr is a number
    mvfl_cons_cell_t* cell = sexpr->first;
    while ( cell != NULL ) {
        mvfl_type_t type = cell->value->type;
        if ( type != MVFL_INTEGER && type != MVFL_FLOAT ) {
            // If it's an error, then return that one.
            if  ( type == MVFL_ERROR ) {
                mvfl_val_t* error = mvfl_val_clone( cell->value ); 
                mvfl_sexpr_delete( sexpr );
                return error;
            }
            else {
                mvfl_sexpr_delete( sexpr );
                return mvfl_val_from_error( "Cannot operate on non-number!" );
            }
        }
        cell = cell->next;
    }

    // Get the first argument.
    mvfl_val_t* firstArg = mvfl_sexpr_pop( sexpr, 0 );

    // Check unary operators.
    if ( strcmp(op, "+") == 0 && sexpr->count == 0 ) {
        // Do nothing.
    }
    if ( strcmp(op, "-") == 0 && sexpr->count == 0 ) {
        mvfl_arithmetic_neg_intern( firstArg );
    }

    while ( sexpr->count > 0 ) {

        mvfl_val_t* nextArg = mvfl_sexpr_pop( sexpr, 0 );
        
        if ( strcmp(op,"add") == 0 || strcmp(op,"sum") == 0 || strcmp(op,"+") == 0 ) {
            mvfl_arithmetic_add_intern( firstArg, nextArg );
        }
        if ( strcmp(op,"sub") == 0 || strcmp(op,"diff") == 0 || strcmp(op,"-") == 0 ) {
            mvfl_arithmetic_sub_intern( firstArg, nextArg );
        }
        if ( strcmp(op,"mul") == 0 || strcmp(op,"prod") == 0 || strcmp(op,"*") == 0 ) {
            mvfl_arithmetic_mul_intern( firstArg, nextArg );
        }
        if ( strcmp(op,"div") == 0 || strcmp(op,"/") == 0 ) {
            mvfl_arithmetic_div_intern( firstArg, nextArg );
        } 
        if ( strcmp(op,"min") == 0 ) {
            mvfl_arithmetic_min_intern( firstArg, nextArg );
        }
        if ( strcmp(op,"max") == 0 ) {
            mvfl_arithmetic_max_intern( firstArg, nextArg );
        }

        mvfl_val_delete( nextArg );

    }

    mvfl_sexpr_delete( sexpr );

    return firstArg; 

}

