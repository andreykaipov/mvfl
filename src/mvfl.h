

// Typdefs

// Our ints our C's longs and our floats is C's doubles.
typedef long mvfl_int_t;
typedef double mvfl_float_t;

// The errors and symbols are strings.
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
};

// A sequence of cons cells form a list, or in our case an S-Expression.
// An S-Expression knows its length, and its first cons cell.
struct mvfl_sexpr_t {
    int count;
    mvfl_cons_cell_t * first;
};

// Our number can either be an integer or floating-point decimals.
union mvfl_number_t {
    mvfl_int_t as_int;
    mvfl_float_t as_float;
};

// The manifestation of our values can either be error codes or numbers.
union mvfl_manifestation_t {
    mvfl_number_t num;
    mvfl_sexpr_t sexpr;
    mvfl_symbol_t symbol;
    mvfl_error_t error;
};

// Our values have a type, and a manifestation.
struct mvfl_val_t {
    mvfl_type_t type;
    mvfl_manifestation_t manifestation;
};

void mvfl_sexpr_print( mvfl_sexpr_t sexp, char open, char close );
void mvfl_val_print( mvfl_val_t* value );
