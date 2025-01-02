#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



static inline void print_binary_format(char num) {
#define size 2 

    char binary[size + 1];
    binary[size] = '\0';
    for (int i = size - 1; i >= 0; i--) {
        binary[i] = (num & 1) ? '1' : '0';
        num >>= 1;
    }

    printf("%s\n", binary);
#undef size
}

const void* multiply10(void* x){
    void* out = malloc(sizeof(int));
    memcpy(out, (void*)&(int){*(int*)x * 10}, sizeof(int));
    return out;
}

const CmpState cmp_int(const void* const a, const void* const b){
    int a_d = *(int*)a;
    int b_d = *(int*)b;
    if ( a_d < b_d ) return inf;
    if ( a_d == b_d ) return eq;
    return sup;
}

int main(){
    vec_err err = no_err;
    Vector v = vec_init(sizeof(int) ,0 , &err);
    vec_dbg(v);
    vec_push(v, (void*)&(int){10}, &err);
    if ( err != no_err ) return 1;
    vec_dbg(v);
    vec_push(v, (void*)&(int){20}, &err);
    if ( err != no_err ) return 1;
    vec_dbg(v);
    vec_push(v, (void*)&(int){30}, &err);
    if ( err != no_err ) return 1;
    vec_dbg(v);
    printf("\nlength: %lu\ncapacity: %lu\n",
            vec_len(v, &err),
            vec_cap(v, &err)
    );
    printf("\ndebugging insert:\n");
    vec_insert(v, (void*)&(int){40}, 3, &err);
    vec_dbg(v);
    printf("\n");
    printf("\ndebugging remove:\n");
    int * x = vec_remove(v, 1, &err);
    vec_dbg(v);
    printf("\npopped value: %d\n", *x);
    printf("\ndebugging get:\n");
    x = vec_last(v, &err);
    printf("gotten element: %d\n", *x);
    vec_dbg(v);
    free(x);
    printf("\ndebugging map:\n");\
    Vector mapped = vec_map(v, multiply10, &err);
    printf("vec: "); vec_dbg(v);
    printf("\nmapped: "); vec_dbg(mapped);
    printf("\ndebugging reverse:\n");
    Vector reversed = vec_reverse(v, &err);
    printf("\nvec:"); vec_dbg(v);
    printf("\nreversed:"); vec_dbg(reversed);
    printf("\ndebugging subvec:\n");
    Vector sub = vec_subvec(v, 2, 3, &err);
    vec_panic(err);
    vec_dbg(sub);
    printf("\n\e[32m%d\e[m\n", err);
    printf("\ndebugging sort:\n");
    vec_push(v, (void*)&(int){2314}, &err);
    vec_push(v, (void*)&(int){-213}, &err);
    vec_push(v, (void*)&(int){-34}, &err);
    Vector sorted = vec_sort(v, cmp_int, &err);
    vec_dbg(sorted);
    vec_destroy(sorted, &err);
    vec_destroy(sub, &err);
    vec_destroy(reversed, &err);
    vec_destroy(mapped, &err);
    vec_destroy(v, &err);
}
