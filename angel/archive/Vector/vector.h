#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Vector Vector;

typedef void Vector_PrintDataCallback(void *data);
typedef void Vector_DestroyDataCallback(void *data);

struct Vector {
  void **data;
  size_t capacity;
  size_t length;
  Vector_PrintDataCallback *print; 
  Vector_DestroyDataCallback *destroy; 
}; 

Vector *Vector_Create(size_t capacity,Vector_PrintDataCallback *print,Vector_DestroyDataCallback *destroy);
void Vector_Destroy(Vector *vector);
void Vector_Set(Vector *vector,size_t index,void *value);
void *Vector_Get(Vector *vector,size_t index);
void Vector_Append(Vector *vector,void *value);
void Vector_Prepend(Vector *vector,void *value);
void Vector_Insert(Vector *vector,size_t index,void *value);
void Vector_Remove(Vector *vector,size_t index);
void Vector_Print(Vector *vector);
void Vector_DestroyData(void *vector);
void Vector_PrintData(void *vector);

#endif /* VECTOR_H  */
