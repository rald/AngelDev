#ifndef VAR_H
#define VAR_H



typedef struct Var Var;

struct Var {
    char *name;
    char *value;
};



#ifdef VAR_IMPLEMENTATION



Var *Var_Create(char *name,char *value) {
    Var *var=malloc(sizeof(*var));
    if(var) {
        var->name=strdup(name);
        var->value=strdup(value);
    }
    return var;
}



void Var_Destroy(void *data) {

    free(((Var*)data)->name);
    ((Var*)data)->name=NULL;

    free(((Var*)data)->value);
    ((Var*)data)->value=NULL;

    free(data);
    data=NULL;

}



void Var_Print(void *data) {
    printf("name: %s value: %s\n",
           ((Var*)data)->name,
           ((Var*)data)->value
          );
}



#endif /* VAR_IMPLEMENTATION */



#endif /* VAR_H */



