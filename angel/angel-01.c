#include <stdio.h>
#include <stdlib.h>



#define VECTOR_IMPLEMENTATION
#include "vector.h"

#define STRLIB_IMPLEMENTATION
#include "strlib.h"

#define VAR_IMPLEMENTATION
#include "var.h"



int main() {

    Vector *vars = Vector_Create(0,Var_Print,Var_Destroy);

    Vector_Append(vars,Var_Create("v1","Hello"));
    Vector_Append(vars,Var_Create("v2","World"));

    Vector_Print(vars);

    Vector_Destroy(vars);

    return 0;
}


