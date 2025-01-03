/*
 *  vector.h is a library that provides an interface to interact with an implementation of a generic vector
 *  a vector in this case is a dynamically sized array
 *  the implementation in the background deals with the memory management of a generic vector
 *  the proposed method of error handling here is the enum vec_err which captures the error, later the user can choose
 *  to panic at the error with the implemented vec_panic or handle the error by himself, or ignore it at all ( which is not 
 *  recommanded )
 *  
 *  apart of a constructor and destructor, methods are implemented which are:
 *  basic:
 *  - access: get, first, last ...
 *  - writing: insertions, deletions, pushing, popping
 *  - status: len, capa ...
 *  more abstract:
 *  - a generic mapper from Vector<T> to Vector<U>
 *  - sorting
 *  - slicing
 *  - filtering
 *  - copying
 *  - printing
 *
 *  aside from the generic interface presented thanks to the use of void pointers, we have just below it an other interface
 *  a more user friendly one which manages genericity gracefully with the appending of the generic typing to the method's name
 *  ( replace <T> or ::<T> with _T in the method name ) to further abstract away the memory management for 
 *  memory leaks prevention and the simple and complex memory conversions required to use the methods
 *
 *
 *
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>
#include <string.h>


typedef uint64_t u64;
typedef struct vector* Vector;
typedef struct vector* const cVector;

typedef enum{
    no_err,
    alloc_err,
    realloc_err,
    null_vec_err,
    illegal_del_err,
    illegal_acces_err,
    index_out_of_bounds_err,
} vec_err;

typedef enum{
    inf = -1,
    eq  =  0,
    sup =  1
} CmpState;

Vector   vec_init_(u64 element_size, u64 def_capa, vec_err* __restrict const err);
void     vec_destroy(Vector v, vec_err* __restrict const err);
void     vec_push_(Vector v, const void* const element, vec_err* const err);
void*    vec_pop_(cVector const v, vec_err* __restrict const err);
void     vec_insert_(Vector v, const void* const element, const u64 index, vec_err* const err);
void*    vec_remove_(cVector v, const u64 index, vec_err* __restrict const err);
u64      vec_len(__restrict const cVector v, vec_err* __restrict const err);
u64      vec_cap(__restrict const cVector v, vec_err* __restrict const err);
void*    vec_get_(__restrict const cVector v, const u64 index, vec_err* __restrict const err);
void*    vec_first_(__restrict const cVector v, vec_err* __restrict const err);
void*    vec_last_(__restrict const cVector v, vec_err* __restrict const err);
Vector   vec_map_(const u64 out_element_size, __restrict const cVector v, const void*(* const function)(void*), vec_err* __restrict const err);
Vector   vec_sort_(__restrict const cVector v, const CmpState(*const cmp)(const void* const, const void* const), vec_err* __restrict const err);
Vector   vec_reverse(__restrict const cVector v, vec_err* __restrict const err);
Vector   vec_subvec(__restrict const cVector v, const u64 b, const u64 e, vec_err* __restrict const err);
void     vec_panic(const vec_err);

void     vec_print_(const __restrict cVector v, void (* const printer )(const void* const));

// a front end for the ease of use

#define GENERIC_VEC(T)                                                                                              \
    inline Vector vec_init_##T(u64 def_capa, vec_err* __restrict const err){                                        \
        return vec_init_(sizeof(T), def_capa, err);                                                             \
    }                                                                                                               \
    inline void   vec_push_##T(Vector v, const T element, vec_err* const err){                                      \
        vec_push_(v, (void*)&(T){element}, err);                                                                    \
    }                                                                                                               \
    inline T      vec_pop_##T(cVector v, vec_err* const err){                                                        \
        void* input = vec_pop_(v, err);                                                                             \
        if ( input == NULL) return (T)0;                                                                            \
        T output = *(T*)input;                                                                                      \
        free(input);                                                                                                \
        return output;                                                                                              \
    }                                                                                                               \
    inline void   vec_insert_##T(Vector v, const T element, const u64 index, vec_err* __restrict const err){        \
        vec_insert_(v, (void*)&(T){element}, index, err);                                                           \
    }                                                                                                               \
    inline T      vec_remove_##T(cVector v, const u64 index, vec_err* __restrict const err){                        \
        void* input = vec_remove_(v, index, err);                                                                   \
        if ( input == NULL ) return (T)0;                                                                           \
        T output = *(T*)input;                                                                                      \
        free(input);                                                                                                \
        return output;                                                                                              \
    }                                                                                                               \
    inline T       vec_get_##T(const __restrict cVector v, const u64 index, vec_err* __restrict const err ){        \
        void* input = vec_get_(v, index,err);                                                                       \
        if ( input == NULL ) return (T)0;                                                                           \
        T output = *(T*)input;                                                                                      \
        return output;                                                                                              \
    }                                                                                                               \
    inline T       vec_first_##T(const __restrict cVector v, vec_err* __restrict const err){                        \
        void* input = vec_first_(v, err);                                                                           \
        if ( input == NULL ) return (T)0;                                                                           \
        T output = *(T*)input;                                                                                      \
        return output;                                                                                              \
    }                                                                                                               \
    inline T       vec_last_##T(const __restrict cVector v, vec_err* __restrict const err){                         \
        void* input = vec_last_(v, err);                                                                            \
        if ( input == NULL ) return (T)0;                                                                           \
        T output = *(T*)input;                                                                                      \
        return output;                                                                                              \
    }                                                                                                               \
    Vector   vec_sort_##T(__restrict const cVector v,                                                               \
            const CmpState(*const cmp)(const T, const T),                                                           \
            vec_err* __restrict const err){                                                                         \
        const CmpState inner_function(const void* const a, const void* const b){ return cmp(*(T*)a, *(T*)b);  }     \
        return vec_sort_(v, inner_function, err);                                                                   \
    }                                                                                                               \
    void   vec_print_##T(const __restrict cVector v, void (* const printer)(const T)){                        \
        void inner_printer(const void* const x){                                                              \
            printer(*(T*)x);                                                                                    \
        }                                                                                                           \
        vec_print_(v, inner_printer);                                                                               \
    } 

#define GENERIC_VEC_MAPPER(T, U)                                                                                    \
    Vector vec_map_##T##_##U(__restrict const cVector v, const U (* const mapper)(T), vec_err* __restrict const err)\
    {                                                                                                               \
        const void* inner_mapper(void* x){                                                                          \
            void* out = malloc(sizeof(U));                                                                          \
            memcpy(out, (void*)&(U){mapper(*(T*)x)}, sizeof(U));                                                    \
            return out;                                                                                             \
        }                                                                                                           \
        return vec_map_(sizeof(U), v, inner_mapper, err);                                                           \
    }

#endif



//TODO:
//-filter method
//-deep copy
//-more rigorous tests
