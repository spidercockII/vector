
#include "vector.h"



#include <stdlib.h>
#include <string.h>
#include <stdio.h>



struct vector{
    void*  array;
    u64    length;
    u64    capacity;
    u64    element_size;
};

enum{
    VECSIZE = sizeof(struct vector),
    VOIDPTRSIZE = sizeof(void*),
};

Vector vec_init_(
        u64 element_size,
        u64 def_capa,
        vec_err* __restrict const err
){
    Vector v = (Vector)malloc(VECSIZE);
    if ( v == NULL )
        goto exit_failure_outer;
    v -> capacity = def_capa != 0 ? def_capa : 10;
    v -> element_size = element_size;
    v -> array = malloc(element_size * v -> capacity);
    if ( v -> array == NULL )
        goto exit_failure_inner;
    *err = no_err;
    return v;
exit_failure_inner:
    free(v);
exit_failure_outer:
    *err = alloc_err;
    return NULL;
}


static inline Vector in_vec_init(
        u64 element_size,
        u64 def_capa,
        vec_err* __restrict const err
){
    Vector v = (Vector)malloc(VECSIZE);
    if ( v == NULL )
        goto in_exit_failure_outer;
    v -> capacity = def_capa != 0 ? def_capa : 10;
    v -> element_size = element_size;
    v -> array = malloc(element_size * v -> capacity);
    if ( v -> array == NULL )
        goto in_exit_failure_inner;
    *err = no_err;
    return v;
in_exit_failure_inner:
    free(v);
in_exit_failure_outer:
    *err = alloc_err;
    return NULL;
}



void vec_destroy(Vector v, vec_err* __restrict const err ){
    if ( v == NULL ){
        *err = null_vec_err;
        return;
    }
    if( v -> array == NULL ){
        free(v);
        *err = null_vec_err;
        return;
    }
    free(v->array);
    free(v);
    *err = no_err;
}


static inline void in_vec_destroy(Vector v, vec_err* __restrict const err ){
    if ( v == NULL ){
        *err = null_vec_err;
        return;
    }
    if( v -> array == NULL ){
        free(v);
        *err = null_vec_err;
        return;
    }
    free(v->array);
    free(v);
    *err = no_err;
}

void vec_push_(
    Vector v,
    const void* const element,
    vec_err* __restrict const err
){
    if ( v -> length == v -> capacity ){
        v -> capacity *= 2;
        v -> array = realloc(
                v -> array,
                v -> capacity * v -> element_size
        );
        if ( v -> array == NULL ){
            *err = realloc_err;
            return;
        }
    }
    memcpy( 
        v -> array + ( v -> length * v -> element_size ),
        element,
        v -> element_size
    );
    v -> length ++;
    *err = no_err; 
}

static inline void in_vec_push(
    Vector v,
    const void* const element,
    vec_err* __restrict const err
){
    if ( v -> length == v -> capacity ){
        v -> capacity *= 2;
        v -> array = realloc(
                v -> array,
                v -> capacity * v -> element_size
        );
        if ( v -> array == NULL ){
            *err = realloc_err;
            return;
        }
    }
    memcpy( 
        v -> array + ( v -> length * v -> element_size ),
        element,
        v -> element_size
    );
    v -> length ++;
    *err = no_err; 
}



void vec_dbg(
        const cVector __restrict v
        ){
    if ( v == NULL ) {
        fprintf(stdout, "(nullvec)\n");
        return;
    }
    if ( v -> array == NULL ){
        fprintf(stderr, "(error)\n");
        return;
    }
    if ( v -> length == 0 ) {
        printf("<>\n");
        return;
    }
    printf("<");
    int* content = (int*)v->array;
    for ( u64 i = 0; i < v -> length-1; i++ ){
        printf("%d, ", content[i]);
    }
    printf("%d>", content[v->length-1]);
}

u64 vec_len(__restrict const cVector v, vec_err* __restrict const err){
    if ( v == NULL ){
        *err = null_vec_err;
        return 0;
    }
    *err = no_err;
    return v -> length; 
}
u64 vec_cap(__restrict const cVector v, vec_err* __restrict const err){
    if ( v == NULL ){
        *err = null_vec_err;
        return 0;
    }
    *err = no_err;
    return v -> capacity;
}

void* vec_pop_(cVector const v, vec_err* __restrict err){
    if ( v == NULL ){
        *err = null_vec_err;
        return NULL;
    }
    if ( v -> array == NULL ){
        *err = null_vec_err;
        return NULL;
    }
    if ( v -> length == 0 ){
        *err = illegal_del_err;
        return NULL;
    }
    void* out = malloc(v -> element_size);
    if ( out == NULL ){
        *err = alloc_err;
        return NULL;
    }
    memcpy( 
            out,
            v -> array + (v -> element_size * ( -- v -> length )),
            v -> element_size
    );
    *err = no_err;
    return out;
}

void vec_insert_(
        Vector v,
        const void* const element,
        u64 index,
        vec_err* __restrict const err
        ){
    if ( v == NULL ){
        *err = null_vec_err;
        return;
    }
    if ( v -> array == NULL ){
        *err = null_vec_err;
        return;
    }
    if ( index > v -> length ){
        *err = index_out_of_bounds_err;
        return;
    }
    if ( v -> length == v -> capacity ){
        v -> capacity *= 2;
        v -> array = realloc(
            v -> array,
            v -> capacity * v -> element_size
        );
        if ( v -> array == NULL ){
            *err = realloc_err;
            return ;
        }
    }
    memmove(
        v -> array + (index+1) * v -> element_size,
        v -> array + index * v -> element_size,
        v -> element_size * (v -> length - index)
    );
    memcpy(
        v -> array + index * v -> element_size,
        element,
        v -> element_size
    );  
    v -> length ++;
    *err = no_err;
}

void* vec_remove_(
        cVector const v,
        const u64 index,
        vec_err* __restrict err
        ){
    if ( v == NULL ){
        *err = null_vec_err;
        return NULL;
    }
    if ( v -> array == NULL ){
        *err = null_vec_err;
        return NULL;
    }
    if ( v -> length == 0 ){
        *err = illegal_del_err;
        return NULL;
    }
    if ( index >= v -> length ){
        *err = index_out_of_bounds_err;
        return NULL;
    }
    void* out = malloc(v -> element_size);
    if ( out == NULL ){
        *err = alloc_err;
        return NULL;
    }
    memcpy(
            out,
            v -> array + (index * v -> element_size),
            v -> element_size
    );
    memmove(
        v -> array + (index * v -> element_size),
        v -> array + (index + 1) * v -> element_size,
        v -> element_size * ( v -> length - index )
    );
    v -> length --;
    *err = no_err;
    return out;
}

void* vec_get_(
        __restrict const cVector v,
        const u64 index,
        vec_err* __restrict const err
        ){
    if ( v == NULL ){
        *err = null_vec_err;
        return NULL;
    }
    if ( v -> array == NULL ){
        *err = null_vec_err;
        return NULL;
    }
    if ( v -> length == 0 ){
        *err = illegal_acces_err;
        return NULL;
    }
    if ( index >= v -> length ){
        *err = index_out_of_bounds_err;
        return NULL;
    }
    void* out = malloc(v -> element_size);
    if ( out == NULL ){
        *err = alloc_err;
        return NULL;
    }
    memcpy(
        out,
        v -> array + v -> element_size * index,
        v -> element_size
    );
    *err = no_err;
    return out;
}


static inline void* in_vec_get(
        __restrict const cVector v,
        const u64 index,
        vec_err* __restrict const err
        ){
    if ( v == NULL ){
        *err = null_vec_err;
        return NULL;
    }
    if ( v -> array == NULL ){
        *err = null_vec_err;
        return NULL;
    }
    if ( v -> length == 0 ){
        *err = illegal_acces_err;
        return NULL;
    }
    if ( index >= v -> length || index < 0){
        *err = index_out_of_bounds_err;
        return NULL;
    }
    void* out = malloc(v -> element_size);
    if ( out == NULL ){
        *err = alloc_err;
        return NULL;
    }
    memcpy(
        out,
        v -> array + v -> element_size * index,
        v -> element_size
    );
    *err = no_err;
    return out;
}

void* vec_first_(
        __restrict const cVector v,
        vec_err* __restrict const err
        ){
    if ( v == NULL || v -> array == NULL ){
        *err = null_vec_err;
        return NULL;
    }
    return in_vec_get(v, 0, err);
}

void* vec_last_(
        __restrict const cVector v,
        vec_err* __restrict const err
        ){
    if ( v == NULL || v -> array == NULL ){
        *err = null_vec_err;
        return NULL;
    }
    return in_vec_get(v, v->length-1, err);
}

Vector vec_map_(
        const u64 out_element_size,
        __restrict const cVector v,
        const void*(* const function)(void*),
        vec_err* __restrict const err
        ){
    if ( v == NULL || v -> array == NULL ){
        *err = null_vec_err;
        return NULL;
    }
    Vector out = in_vec_init(out_element_size, v -> length, err);
    if ( *err != no_err ){
        return NULL;
    }
    void *curr, *curr_morphed;
    for ( u64 i = 0; i < v -> length; i++ ){
        curr = in_vec_get(v, i, err);
        if ( *err != no_err ) {
            in_vec_destroy(out, err);
            free(curr);
            return NULL;
        }
        curr_morphed = (void*)function(curr);
        in_vec_push(out, curr_morphed, err);
        if( *err != no_err ){
            in_vec_destroy(out, err);
            free(curr_morphed);
            free(curr);
            return NULL;
        }
        free(curr_morphed);
        free(curr);
    }
    *err = no_err;
    return out;

}

Vector vec_reverse(
        __restrict const cVector v,
        vec_err* __restrict const err
        ){
    if ( v == NULL || v -> array == NULL){
        *err = null_vec_err;
        return NULL;
    }
    if ( v -> length == 0 ){
        Vector out = in_vec_init( v -> element_size, v -> length, err );
        return *err != no_err ? NULL : out;
    }
    Vector out = in_vec_init(
            v -> element_size,
            v -> length,
            err
            );
    if ( *err != no_err ) return NULL;
    void* current;
    for ( u64 i = v -> length-1; i > 0; i-- ){
        current = in_vec_get(v, i, err);
        if ( *err != no_err ){
            in_vec_destroy(out, err);
            return NULL;
        }
        in_vec_push(out, current, err);
        if ( *err != no_err){
            in_vec_destroy(out, err);
            free(current);
            return NULL;
        }
        free(current);
    }
    current = in_vec_get(v, 0, err);
    if ( *err != no_err ){
        in_vec_destroy(out, err);
        free(current);
        return NULL;
    }
    in_vec_push(out, current, err);
    if ( *err != no_err ){
        in_vec_destroy(out, err);
        free(current);
        return NULL;
    }
    free(current);
    *err = no_err;
    return out;
}

Vector vec_subvec(
        __restrict const cVector v,
        const u64 b,
        const u64 e,
        vec_err* __restrict const err
        ){
    if ( v == NULL || v -> array == NULL ){
        * err = null_vec_err;
        return NULL;
    }
    if ( e > v -> length || b >= v -> length ){
        *err = illegal_acces_err;
        return NULL;
    }
    Vector out = in_vec_init(
            v -> element_size,
            e > b ? e - b : b - e,
            err
    );
    if ( *err != no_err ) return NULL;
    void * current;
    if ( b > e && e == 0 )  goto special_reversed_subvec;
    if ( b > e )            goto reversed_subvec;
    else                    goto actual_subvec;

special_reversed_subvec:
    for (u64 i = b; i > e; i--){
        current = in_vec_get(v, i, err);
        if ( *err != no_err ){
            in_vec_destroy(out, err);
            return NULL;
        }
        in_vec_push(out, current, err);
        if( *err != no_err ){
            in_vec_destroy(out, err);
            free(current);
            return NULL;
        }
        free(current);
    }
    current = in_vec_get(v, 0, err);
    if ( *err != no_err ){
        in_vec_destroy(out, err);
        return NULL;
    }
    in_vec_push(out, current, err);
    if ( *err != no_err) {
        in_vec_destroy(out, err);
        free(current);
        return NULL;
    }
    free(current);
    return out;


    
reversed_subvec:
    for (u64 i = b; i >= e; i--){
        current = in_vec_get(v, i, err);
        if ( *err != no_err ){
            if ( out != NULL ){
                if ( out -> array != NULL ) free(out -> array);
                free(out);
            }       
            return NULL;
        }
        in_vec_push(out, current, err);
        if( *err != no_err ){
            in_vec_destroy(out, err);
            free(current);
            return NULL;
        }
        free(current);
    }
    return out;


actual_subvec:
    for ( u64 i = b; i < e; i++ ){
        current = in_vec_get(v, i, err);
        if ( *err != no_err ){
            if ( out != NULL ){
                if ( out -> array != NULL ) free(out -> array);
                free(out);
            } 
            return NULL;
        }
        in_vec_push(out, current, err);
        if( *err != no_err ){
            in_vec_destroy(out, err);
            free(current);
            return NULL;
        }
        free(current);
    }
    return out;
}


static void quick_sort(
        void* const dest,
        const u64 length,
        u64  element_size,
        const CmpState(* const cmp)(
            const void* const,
            const void* const
        )
        ){
    if (length < 2) return;

    // Choose the pivot (using the last element in this case)
    void* pivot = dest + (length - 1) * element_size;
    u64 i, j;

    for (i = 0, j = 0; j < length - 1; j++) {
        if (cmp(dest + j * element_size, pivot) <= 0) {
            if (i != j) {
                // Swap elements if necessary
                void* temp = malloc(element_size);
                memcpy(temp, dest + i * element_size, element_size);
                memcpy(dest + i * element_size, dest + j * element_size, element_size);
                memcpy(dest + j * element_size, temp, element_size);
                free(temp);
            }
            i++;
        }
    }

    // Swap the pivot into the correct place
    if (i != length - 1) {
        void* temp = malloc(element_size);
        memcpy(temp, (char*)dest + i * element_size, element_size);
        memcpy((char*)dest + i * element_size, pivot, element_size);
        memcpy(pivot, temp, element_size);
        free(temp);
    }

    // Recursively sort the partitions
    quick_sort(dest, i, element_size, cmp);
    quick_sort(dest + (i + 1) * element_size, length - i - 1, element_size, cmp);
}


Vector vec_sort_(
        __restrict const cVector v,
        const CmpState(* const cmp)(
            const void* const,
            const void* const
        ),
        vec_err* __restrict const err
        ){
    if ( v == NULL || v -> array == NULL ){
        *err = null_vec_err;
        return NULL;
    }

    Vector out = in_vec_init(v -> element_size, v -> length, err);
    if ( *err != no_err ) return NULL;
    
    if ( v -> length == 0 )
        return out;

    memcpy(out->array, v->array, v->length * v->element_size);
    out->length = v->length;

    // Perform quick sort on the copied array
    quick_sort(out->array, out->length, out->element_size, cmp);

    *err = no_err;
    return out; 


}

void vec_panic(const vec_err err){
#define handle_err(x)                           \
    do{                                         \
        fprintf(stdout, "\n\e[31m%s\e[m\n", x); \
        exit(EXIT_FAILURE);                     \
    }while(0);
    
    switch(err){
        case no_err:
            return;
        case alloc_err:
            handle_err("allocation Error!");
        case realloc_err:
            handle_err("reallocation Error!");
        case null_vec_err:
            handle_err("null vector Error!");
        case illegal_del_err:
            handle_err("illegal deletion Error!");
        case illegal_acces_err:
            handle_err("illegal access Error!");
        case index_out_of_bounds_err:
            handle_err("Index out of Bounds Error!");
        default:
            handle_err("Unkown Error!");
    }
#undef handle_err
}


void vec_print_(
        const cVector __restrict v,
        void (* const printer)(const void* const) 
        ){
    if ( v == NULL ) {
        fprintf(stdout, "(nullvec)\n");
        return;
    }
    if ( v -> array == NULL ){
        fprintf(stderr, "(error)\n");
        return;
    }
    if ( v -> length == 0 ) {
        printf("< >\n");
        return;
    }
    printf("<");
    for ( u64 i = 0; i < v -> length-1; i++ ){
        printer(v -> array + i * v -> element_size);
        printf(", ");
    }
    printer(v -> array + v -> element_size * ( v -> length - 1 ));
    printf(">");
}
