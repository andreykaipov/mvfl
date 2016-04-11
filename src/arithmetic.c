
/*
#include "mvfl.h"

mvfl_val_t mvfl_val_from_int( mvfl_int_t );
mvfl_val_t mvfl_val_from_float( mvfl_float_t );
mvfl_val_t mvfl_val_error( mvfl_err_code_t );

mvfl_val_t mvfl_add_intern( mvfl_val_t v, mvfl_val_t w ) {

    switch ( v.type ) {
        case MVFL_INTEGER:
            switch ( w.type ) {
                case MVFL_INTEGER:
                    return mvfl_val_from_int( v.manifestation.num.as_int + w.manifestation.num.as_int );
                case MVFL_FLOAT:
                    return mvfl_val_from_float( v.manifestation.num.as_int + w.manifestation.num.as_float );
                default: // Error - propogate it up.
                    return w;
            }
        case MVFL_FLOAT:
            switch ( w.type ) {
                case MVFL_INTEGER:
                    return mvfl_val_from_float( v.manifestation.num.as_float + w.manifestation.num.as_int );
                case MVFL_FLOAT:
                    return mvfl_val_from_float( v.manifestation.num.as_float + w.manifestation.num.as_float );
                default: // Error - propogate it up.
                    return w;
            }
        default: // Error - propogate it up.
            return v;
    }

}

mvfl_val_t mvfl_sub_intern( mvfl_val_t v, mvfl_val_t w ) {

    switch ( v.type ) {
        case MVFL_INTEGER:
            switch ( w.type ) {
                case MVFL_INTEGER:
                    return mvfl_val_from_int( v.manifestation.num.as_int - w.manifestation.num.as_int );
                case MVFL_FLOAT:
                    return mvfl_val_from_float( v.manifestation.num.as_int - w.manifestation.num.as_float );
                default: // Error - propogate it up.
                    return w;
            }
        case MVFL_FLOAT:
            switch ( w.type ) {
                case MVFL_INTEGER:
                    return mvfl_val_from_float( v.manifestation.num.as_float - w.manifestation.num.as_int );
                case MVFL_FLOAT:
                    return mvfl_val_from_float( v.manifestation.num.as_float - w.manifestation.num.as_float );
                default: // Error - propogate it up.
                    return w;
            }
        default: // Error - propogate it up.
            return v;
    }

}

mvfl_val_t mvfl_mul_intern( mvfl_val_t v, mvfl_val_t w ) {

    switch ( v.type ) {
        case MVFL_INTEGER:
            switch ( w.type ) {
                case MVFL_INTEGER:
                    return mvfl_val_from_int( v.manifestation.num.as_int * w.manifestation.num.as_int );
                case MVFL_FLOAT:
                    return mvfl_val_from_float( v.manifestation.num.as_int * w.manifestation.num.as_float );
                default: // Error - propogate it up.
                    return w;
            }
        case MVFL_FLOAT:
            switch ( w.type ) {
                case MVFL_INTEGER:
                    return mvfl_val_from_float( v.manifestation.num.as_float * w.manifestation.num.as_int );
                case MVFL_FLOAT:
                    return mvfl_val_from_float( v.manifestation.num.as_float * w.manifestation.num.as_float );
                default: // Error - propogate it up.
                    return w;
            }
        default: // Error - propogate it up.
            return v;
    }

}

mvfl_val_t mvfl_div_intern( mvfl_val_t v, mvfl_val_t w ) {

    switch ( w.type ) {
        case MVFL_INTEGER:
            if ( w.manifestation.num.as_int == 0 )
                return mvfl_val_error( MVFL_ERROR_DIV_ZERO );
            else
                switch ( v.type ) {
                    case MVFL_INTEGER:
                        return mvfl_val_from_int( v.manifestation.num.as_int / w.manifestation.num.as_int );
                    case MVFL_FLOAT:
                        return mvfl_val_from_float( v.manifestation.num.as_float / w.manifestation.num.as_int );
                    default: // Error - propogate it up.
                        return w;
                }
        case MVFL_FLOAT:
            if ( w.manifestation.num.as_float == 0 )
                return mvfl_val_error( MVFL_ERROR_DIV_ZERO );
            else
                switch ( v.type ) {
                    case MVFL_INTEGER:
                        return mvfl_val_from_float( v.manifestation.num.as_int / w.manifestation.num.as_float );
                    case MVFL_FLOAT:
                        return mvfl_val_from_float( v.manifestation.num.as_float / w.manifestation.num.as_float );
                    default: // Error - propogate it up.
                        return w;
                }
        default: // Error - propogate it up.
            return w;
    }

}

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

mvfl_val_t mvfl_min_intern( mvfl_val_t v, mvfl_val_t w ) {

    switch ( v.type ) {
        case MVFL_INTEGER:
            switch ( w.type ) {
                case MVFL_INTEGER:
                    return ( v.manifestation.num.as_int < w.manifestation.num.as_int )
                        ? mvfl_val_from_int( v.manifestation.num.as_int )
                        : mvfl_val_from_int( w.manifestation.num.as_int );
                case MVFL_FLOAT:
                    return ( v.manifestation.num.as_int < w.manifestation.num.as_float )
                        ? mvfl_val_from_int( v.manifestation.num.as_int )
                        : mvfl_val_from_float( w.manifestation.num.as_float );
                default: // Error - propogate it up.
                    return w;
            }
        case MVFL_FLOAT:
            switch ( w.type ) {
                case MVFL_INTEGER:
                    return ( v.manifestation.num.as_float < w.manifestation.num.as_int )
                        ? mvfl_val_from_float( v.manifestation.num.as_float )
                        : mvfl_val_from_int( w.manifestation.num.as_int );
                case MVFL_FLOAT:
                    return ( v.manifestation.num.as_float < w.manifestation.num.as_float )
                        ? mvfl_val_from_float( v.manifestation.num.as_float )
                        : mvfl_val_from_float( w.manifestation.num.as_float );
                default: // Error - propogate it up.
                    return w;
            }
        default: // Error - propogate it up.
            return v;
    }

}

mvfl_val_t mvfl_max_intern( mvfl_val_t v, mvfl_val_t w ) {

    switch ( v.type ) {
        case MVFL_INTEGER:
            switch ( w.type ) {
                case MVFL_INTEGER:
                    return ( v.manifestation.num.as_int > w.manifestation.num.as_int )
                        ? mvfl_val_from_int( v.manifestation.num.as_int )
                        : mvfl_val_from_int( w.manifestation.num.as_int );
                case MVFL_FLOAT:
                    return ( v.manifestation.num.as_int > w.manifestation.num.as_float )
                        ? mvfl_val_from_int( v.manifestation.num.as_int )
                        : mvfl_val_from_float( w.manifestation.num.as_float );
                default: // Error - propogate it up.
                    return w;
            }
        case MVFL_FLOAT:
            switch ( w.type ) {
                case MVFL_INTEGER:
                    return ( v.manifestation.num.as_float > w.manifestation.num.as_int )
                        ? mvfl_val_from_float( v.manifestation.num.as_float )
                        : mvfl_val_from_int( w.manifestation.num.as_int );
                case MVFL_FLOAT:
                    return ( v.manifestation.num.as_float > w.manifestation.num.as_float )
                        ? mvfl_val_from_float( v.manifestation.num.as_float )
                        : mvfl_val_from_float( w.manifestation.num.as_float );
                default: // Error - propogate it up.
                    return w;
            }
        default: // Error - propogate it up.
            return v;
    }

}
*/