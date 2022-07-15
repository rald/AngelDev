#include <stdio.h>

#define ARRAY_IMPLEMENTATION
#include "array.h"


int* wrapInt(int x) {
  int* integer=malloc(sizeof(*integer));
  if(integer) {
    *integer=x;
  }
  return integer;
}

int unwrapInt(int *x) {
  return *x;
}

void IntDestroy(void **data) {
  free(*data);
  (*data)=NULL;
}

int main(void) {

  Array *array=Array_New(5,IntDestroy);

  Array_Append(&array,wrapInt(143));

  for(size_t i=0;i<array->size;i++) {
    printf("%d",((int*)array->data)[i]);
  }

  Array_Destroy(&array);


  return 0;
}
