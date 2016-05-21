

// Typdefs

// Our ints our C's longs and our floats is C's doubles.
typedef long mvfl_int_t;
typedef double mvfl_float_t;

// MVFL errors and symbols are strings.
typedef char* mvfl_error_t;
typedef char* mvfl_symbol_t;

typedef struct mvfl_val_t mvfl_val_t;
typedef union mvfl_manifestation_t mvfl_manifestation_t;
typedef union mvfl_number_t mvfl_number_t;
typedef struct mvfl_cons_cell_t mvfl_cons_cell_t;
typedef struct mvfl_sexpr_t mvfl_sexpr_t;

// Our types will be stored in enums.
typedef enum mvfl_type_t mvfl_type_t;

enum mvfl_type_t {
    MVFL_INTEGER,
    MVFL_FLOAT,
    MVFL_SYMBOL,
    MVFL_SEXPR,
    MVFL_ERROR
};

// A cons cell has two pointers:
// One is to a value, and the other is to another cons cell.
struct mvfl_cons_cell_t {
    mvfl_val_t * value;
    mvfl_cons_cell_t * next;
    mvfl_cons_cell_t * prev;
};

// A sequence of cons cells form a list, or in our case an S-Expression.
// An S-Expression knows its length, and its first cons cell.
struct mvfl_sexpr_t {
    int count;
    mvfl_cons_cell_t * first;
    mvfl_cons_cell_t * last;
};

// Our number can either be an integer or floating-point decimals.
union mvfl_number_t {
    mvfl_int_t as_int;
    mvfl_float_t as_float;
};

// The manifestation of our values are either numbers, symbols or
// errors, or a pointer to an S-Expression.
union mvfl_manifestation_t {
    mvfl_number_t num;
    mvfl_symbol_t symbol;
    mvfl_error_t error;
    mvfl_sexpr_t * sexpr;
};

// Our values have a type, and a manifestation.
struct mvfl_val_t {
    mvfl_type_t type;
    mvfl_manifestation_t manifestation;
};

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
mvfl_sexpr_t* mvfl_sexpr_clone( mvfl_sexpr_t* sexpr );
void mvfl_sexpr_delete( mvfl_sexpr_t* sexpr );
void mvfl_sexpr_print( mvfl_sexpr_t* sexpr, char open, char close );
