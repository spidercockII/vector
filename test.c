#include "vector.h"
#include <stdio.h>

void print_int(const int x){ fprintf(stdout, "%d", x); }
void print_float(const float x){ fprintf(stdout, "%f", x); }

const CmpState cmp_int(int a, int b){
    if ( a < b ) return inf;
    if ( a == b ) return eq;
    return sup;
}
const int multiply10(int x){ return x*10; }

const float div10(int x){ return ((float)x)/10.0; }

#define vpi(v) do { vec_print_int(v, print_int); } while(0);
#define vpf(v) do { vec_print_float(v, print_float); } while(0);

int main(){
    GENERIC_VEC(int)
    GENERIC_VEC(float)
    vec_err err = no_err;
    Vector v_int = vec_init_int(0, &err);
    vpi(v_int);
    vec_push_int(v_int, 1234, &err);
    vec_push_int(v_int, 5132, &err);
    vec_push_int(v_int, 9604, &err);
    vpi(v_int);
    GENERIC_VEC_MAPPER(int, float)
    Vector v_float = vec_map_int_float(v_int, div10, &err);
    vpf(v_float);
    vpi(v_int);
    vec_destroy(v_float, &err);
    vec_destroy(v_int, &err);
    return 0; 
}
