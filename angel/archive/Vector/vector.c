#include "vector.h"

Vector *Vector_Create(size_t capacity,Vector_PrintDataCallback *print,Vector_DestroyDataCallback *destroy) {
  Vector *vector=malloc(sizeof(*vector));
  if(vector!=NULL) {
    vector->data=malloc(sizeof(*(vector->data))*capacity);
    vector->capacity=capacity;
    vector->length=0;
    vector->print=print;
    vector->destroy=destroy;
  }
  return vector;
}

void Vector_Destroy(Vector *vector) {
  for(size_t i=0;i<vector->length;i++) {
    vector->destroy(vector->data[i]);
  }
  free(vector->data);
  free(vector);
}

void Vector_Set(Vector *vector,size_t index,void *value) {
  vector->destroy(vector->data[index]);
  vector->data[index]=value;
}

void *Vector_Get(Vector *vector,size_t index) {
  return vector->data[index];
}

void Vector_Append(Vector *vector,void *value) {
  if(vector->capacity==vector->length) {
    vector->capacity++;
    vector->data=realloc(vector->data,sizeof(vector->data)*(vector->capacity));
  }
  vector->data[vector->length++]=value;
}

void Vector_Prepend(Vector *vector,void *value) {
  if(vector->capacity==vector->length) {
    vector->capacity++;
    vector->data=realloc(vector->data,sizeof(vector->data)*(vector->capacity));
  }
  for(size_t i=vector->length;i>0;i--) {
    vector->data[i]=vector->data[i-1];
  }
  vector->data[0]=value;
  vector->length++;
}

void Vector_Insert(Vector *vector,size_t index,void *value) {
  assert(vector!=NULL);
  if(vector->capacity==vector->length) {
    vector->capacity++;
    vector->data=realloc(vector->data,sizeof(*(vector->data))*vector->capacity);
  }
  vector->length++;
  for(size_t i=vector->length-1;i>index;i--) {
    vector->data[i]=vector->data[i-1];
  }
  vector->data[index]=value;
}

void Vector_Remove(Vector *vector,size_t index) {
  vector->destroy(vector->data[index]);
  for(size_t i=index;i<vector->length-1;i++) {
    vector->data[i]=vector->data[i+1];
  }
  vector->length--;  
}

void Vector_Print(Vector *vector) {
  for(size_t i=0;i<vector->length;i++) {
    vector->print(vector->data[i]);
  }
  printf("\n");
}

void Vector_PrintData(void *vector) {
  for(size_t i=0;i<((Vector*)vector)->length;i++) {
    ((Vector*)vector)->print(((Vector*)vector)->data[i]);
  }
  printf("\n");
}

void Vector_DestroyData(void *vector) {
  for(size_t i=0;i<((Vector*)vector)->length;i++) {
    ((Vector*)vector)->destroy(((Vector*)vector)->data[i]);
  }
  free((Vector*)((Vector*)vector)->data);
  free((Vector*)vector);
}

