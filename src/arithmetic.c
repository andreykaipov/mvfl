
#include "mvfl.h"

mvfl_val_t* mvfl_val_from_error( mvfl_error_t msg );

void mvfl_arithmetic_neg_intern( mvfl_val_t* firstArg ) {

    switch ( firstArg->type ) {
        case MVFL_INTEGER:
            firstArg->manifestation.num.as_int = - firstArg->manifestation.num.as_int;
            break;
        case MVFL_FLOAT:
            firstArg->manifestation.num.as_float = - firstArg->manifestation.num.as_float;
            break;
        default:
            break;
    }

}

void mvfl_arithmetic_add_intern( mvfl_val_t* firstArg, mvfl_val_t* nextArg ) {

    switch ( firstArg->type ) {
        case MVFL_INTEGER:
            switch( nextArg->type ) {
                case MVFL_INTEGER:
                    firstArg->manifestation.num.as_int += nextArg->manifestation.num.as_int;
                    break;
                case MVFL_FLOAT:
                    firstArg->type = MVFL_FLOAT;
                    firstArg->manifestation.num.as_float = firstArg->manifestation.num.as_int;
                    firstArg->manifestation.num.as_float += nextArg->manifestation.num.as_float;
                    break;
                default:
                    break;
            }
            break;
        case MVFL_FLOAT:
            switch( nextArg->type ) {
                case MVFL_INTEGER:
                    firstArg->manifestation.num.as_float += nextArg->manifestation.num.as_int;
                    break;
                case MVFL_FLOAT:
                    firstArg->manifestation.num.as_float += nextArg->manifestation.num.as_float;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    /*
    if ( firstArg->type == MVFL_INTEGER ) {
        if ( nextArg->type == MVFL_INTEGER ) {
            firstArg->manifestation.num.as_int += nextArg->manifestation.num.as_int;
        }
        else if ( nextArg->type == MVFL_FLOAT ) {
            firstArg->type = MVFL_FLOAT;
            firstArg->manifestation.num.as_float = firstArg->manifestation.num.as_int;
            firstArg->manifestation.num.as_float += nextArg->manifestation.num.as_float;
        }
    }
    else if ( firstArg->type == MVFL_FLOAT ) {
        if ( nextArg->type == MVFL_INTEGER ) {
            firstArg->manifestation.num.as_float += nextArg->manifestation.num.as_int;
        }
        else if ( nextArg->type == MVFL_FLOAT ) {
            firstArg->manifestation.num.as_float += nextArg->manifestation.num.as_float;
        }
    }
    */
}

void mvfl_arithmetic_sub_intern( mvfl_val_t* firstArg, mvfl_val_t* nextArg ) {

    switch ( firstArg->type ) {
        case MVFL_INTEGER:
            switch( nextArg->type ) {
                case MVFL_INTEGER:
                    firstArg->manifestation.num.as_int -= nextArg->manifestation.num.as_int;
                    break;
                case MVFL_FLOAT:
                    firstArg->type = MVFL_FLOAT;
                    firstArg->manifestation.num.as_float = firstArg->manifestation.num.as_int;
                    firstArg->manifestation.num.as_float -= nextArg->manifestation.num.as_float;
                    break;
                default:
                    break;
            }
            break;
        case MVFL_FLOAT:
            switch( nextArg->type ) {
                case MVFL_INTEGER:
                    firstArg->manifestation.num.as_float -= nextArg->manifestation.num.as_int;
                    break;
                case MVFL_FLOAT:
                    firstArg->manifestation.num.as_float -= nextArg->manifestation.num.as_float;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

}

void mvfl_arithmetic_mul_intern( mvfl_val_t* firstArg, mvfl_val_t* nextArg ) {

    switch ( firstArg->type ) {
        case MVFL_INTEGER:
            switch( nextArg->type ) {
                case MVFL_INTEGER:
                    firstArg->manifestation.num.as_int *= nextArg->manifestation.num.as_int;
                    break;
                case MVFL_FLOAT:
                    firstArg->type = MVFL_FLOAT;
                    firstArg->manifestation.num.as_float = firstArg->manifestation.num.as_int;
                    firstArg->manifestation.num.as_float *= nextArg->manifestation.num.as_float;
                    break;
                default:
                    break;
            }
            break;
        case MVFL_FLOAT:
            switch( nextArg->type ) {
                case MVFL_INTEGER:
                    firstArg->manifestation.num.as_float *= nextArg->manifestation.num.as_int;
                    break;
                case MVFL_FLOAT:
                    firstArg->manifestation.num.as_float *= nextArg->manifestation.num.as_float;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

}

void mvfl_arithmetic_div_intern( mvfl_val_t* firstArg, mvfl_val_t* nextArg ) {

    switch ( nextArg->type ) {
        case MVFL_INTEGER:
            if ( nextArg->manifestation.num.as_int == 0 ) {
                firstArg->type = MVFL_ERROR;
                firstArg->manifestation.error = "Cannot divide by zero!";
            }
            else {
                switch( firstArg->type ) {
                    case MVFL_INTEGER:
                        firstArg->manifestation.num.as_int /= nextArg->manifestation.num.as_int;
                        break;
                    case MVFL_FLOAT:
                        firstArg->manifestation.num.as_float /= nextArg->manifestation.num.as_int;
                        break;
                    default:
                        break;
                }
            }
            break;
        case MVFL_FLOAT:
            if ( nextArg->manifestation.num.as_float == 0 ) {
                firstArg->type = MVFL_ERROR;
                firstArg->manifestation.error = "Cannot divide by zero!";
            }
            else {
                switch( firstArg->type ) {
                    case MVFL_INTEGER:
                        firstArg->type = MVFL_FLOAT;
                        firstArg->manifestation.num.as_float = firstArg->manifestation.num.as_int;
                        firstArg->manifestation.num.as_float /= nextArg->manifestation.num.as_float;
                        break;
                    case MVFL_FLOAT:
                        firstArg->manifestation.num.as_float /= nextArg->manifestation.num.as_float;
                        break;
                    default:
                        break;
                }
            }
            break;
        default:
            break;
    }

}
/*
mvfl_val_t mvfl_mod_intern( mvfl_val_t v, mvfl_val_t w ) {

    mvfl_int_t i;
    mvfl_int_t j;

    switch ( w.type ) {
        case MVFL_INTEGER:
            if ( w.manifestation.num.as_int == 0 )
                return mvfl_val_error( MVFL_ERROR_DIV_ZERO );
            else
                switch ( v.type ) {
                    case MVFL_INTEGER:
                        i = v.manifestation.num.as_int;
                        j = w.manifestation.num.as_int;
                        if ( i >= 0 )
                            return mvfl_val_from_int( i % j );
                        else
                            return mvfl_val_from_int( i % j + j );
                    case MVFL_FLOAT:
                        return mvfl_val_error( MVFL_ERROR_MOD_ON_FLOATS );
                    default: // Error - propogate it up.
                        return w;
                }
        case MVFL_FLOAT:
            return mvfl_val_error( MVFL_ERROR_MOD_ON_FLOATS );
        default: // Error - propogate it up.
            return w;
    }

}
*/

void mvfl_arithmetic_min_intern( mvfl_val_t* firstArg, mvfl_val_t* nextArg ) {

    switch ( firstArg->type ) {
        case MVFL_INTEGER:
            switch ( nextArg->type ) {
                case MVFL_INTEGER:
                    if ( firstArg->manifestation.num.as_int < nextArg->manifestation.num.as_int ) {
                        // do nothing
                    }
                    else {
                        firstArg->manifestation.num.as_int = nextArg->manifestation.num.as_int;
                    }
                    break;
                case MVFL_FLOAT:
                    if ( firstArg->manifestation.num.as_int < nextArg->manifestation.num.as_float ) {
                        // do nothing
                    }
                    else {
                        firstArg->type = MVFL_FLOAT;
                        firstArg->manifestation.num.as_float = nextArg->manifestation.num.as_float;
                    }
                    break;
                default:
                    break;
            }
            break;
        case MVFL_FLOAT:
            switch ( nextArg->type ) {
                case MVFL_INTEGER:
                    if ( firstArg->manifestation.num.as_float < nextArg->manifestation.num.as_int ) {
                        // do nothing
                    }
                    else {
                        firstArg->type = MVFL_INTEGER;
                        firstArg->manifestation.num.as_int = nextArg->manifestation.num.as_int;
                    }
                    break;
                case MVFL_FLOAT:
                    if ( firstArg->manifestation.num.as_float < nextArg->manifestation.num.as_float ) {
                        // do nothing
                    }
                    else {
                        firstArg->manifestation.num.as_float = nextArg->manifestation.num.as_float;
                    }
                    break;
                default:
                    break;
            }
        default:
            break;
    }

}

void mvfl_arithmetic_max_intern( mvfl_val_t* firstArg, mvfl_val_t* nextArg ) {

    switch ( firstArg->type ) {
        case MVFL_INTEGER:
            switch ( nextArg->type ) {
                case MVFL_INTEGER:
                    if ( firstArg->manifestation.num.as_int > nextArg->manifestation.num.as_int ) {
                        // do nothing
                    }
                    else {
                        firstArg->manifestation.num.as_int = nextArg->manifestation.num.as_int;
                    }
                    break;
                case MVFL_FLOAT:
                    if ( firstArg->manifestation.num.as_int > nextArg->manifestation.num.as_float ) {
                        // do nothing
                    }
                    else {
                        firstArg->type = MVFL_FLOAT;
                        firstArg->manifestation.num.as_float = nextArg->manifestation.num.as_float;
                    }
                    break;
                default:
                    break;
            }
            break;
        case MVFL_FLOAT:
            switch ( nextArg->type ) {
                case MVFL_INTEGER:
                    if ( firstArg->manifestation.num.as_float > nextArg->manifestation.num.as_int ) {
                        // do nothing
                    }
                    else {
                        firstArg->type = MVFL_INTEGER;
                        firstArg->manifestation.num.as_int = nextArg->manifestation.num.as_int;
                    }
                    break;
                case MVFL_FLOAT:
                    if ( firstArg->manifestation.num.as_float > nextArg->manifestation.num.as_float ) {
                        // do nothing
                    }
                    else {
                        firstArg->manifestation.num.as_float = nextArg->manifestation.num.as_float;
                    }
                    break;
                default:
                    break;
            }
        default:
            break;
    }

}
