#include "vector.h"

int *Int_Box(int x) {
  int *data=malloc(sizeof(*data));
  *data=x;
  return data;
}

int Int_Unbox(void *x) {
  return *(int*)x;
}

void Int_PrintData(void *data) {
  printf("%2d ",Int_Unbox(data));
}

void Int_DestroyData(void *data) {
  free(data);
  data=NULL;
}

int main(int argc, char **argv) {

  size_t n=3;

/*
  Vector *a=Vector_Create(n,Int_Print,Int_Destroy);
  for(size_t i=0;i<n;i++) {
    Vector_Append(a,Int_Box(i));
  }

//*/

  Vector *a=Vector_Create(n,Vector_PrintData,Vector_DestroyData);
  for(size_t k=0;k<n;k++) {
    Vector_Append(a,Vector_Create(n,Vector_PrintData,Vector_DestroyData));
    for(size_t j=0;j<n;j++) {
      Vector_Append(Vector_Get(a,k),Vector_Create(n,Int_PrintData,Int_DestroyData));
      for(size_t i=0;i<n;i++) {
        Vector_Append(Vector_Get(Vector_Get(a,k),j),Int_Box(i+j*n+k*n*n));
      }
    }
  }

  Vector_Print(a);

  Vector_Destroy(a);

  return 0;
}
