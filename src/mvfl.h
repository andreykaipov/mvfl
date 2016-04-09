
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
    MVFL_ERROR_BAD_NUM,
    MVFL_ERROR_MOD_ON_FLOATS
};

// Our number can either be an integer or floating-point decimals.
typedef union mvfl_number_t mvfl_number_t;

union mvfl_number_t {
    mvfl_int_t as_int;
    mvfl_float_t as_float;
};

// The manifestation of our values can either be error codes or numbers.
typedef union mvfl_manifestation_t mvfl_manifestation_t;

union mvfl_manifestation_t {
    mvfl_err_code_t err_code;
    mvfl_number_t num;
};

// Our values have a type, and a manifestation.
typedef struct mvfl_val_t mvfl_val_t;

struct mvfl_val_t {
    mvfl_type_t type;
    mvfl_manifestation_t manifestation;
};
