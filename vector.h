#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>

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

Vector   vec_init(u64 element_size, u64 def_capa, vec_err* __restrict const err);
void     vec_destroy(Vector v, vec_err* __restrict const err);
void     vec_push(Vector v, const void* const element, vec_err* const err);
void*    vec_pop(cVector const v, vec_err* __restrict const err);
void     vec_insert(Vector v, const void* const element, const u64 index, vec_err* const err);
void*    vec_remove(cVector v, const u64 index, vec_err* __restrict const err);
void     vec_dbg(const cVector __restrict v);
u64      vec_len(__restrict const cVector v, vec_err* __restrict const err);
u64      vec_cap(__restrict const cVector v, vec_err* __restrict const err);
void*    vec_get(__restrict const cVector v, const u64 index, vec_err* __restrict const err);
void*    vec_first(__restrict const cVector v, vec_err* __restrict const err);
void*    vec_last(__restrict const cVector v, vec_err* __restrict const err);
Vector   vec_map(__restrict const cVector v, const void*(* const function)(void*), vec_err* __restrict const err);
Vector   vec_sort(__restrict const cVector v, const CmpState(*const cmp)(const void* const, const void* const), vec_err* __restrict const err);
Vector   vec_reverse(__restrict const cVector v, vec_err* __restrict const err);
Vector   vec_subvec(__restrict const cVector v, const u64 b, const u64 e, vec_err* __restrict const err);
void     vec_panic(const vec_err);

#endif



//TODO:
//-vec_subvec: fix a bug in which adress boundary is violated without proper error catching in both reversed and actual cases 
//-vec_sort
//-more rigorous tests
