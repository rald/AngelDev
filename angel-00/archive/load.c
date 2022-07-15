#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>



#define VECTOR_IMPLEMENTATION
#include "vector.h"

#define MATH_IMPLEMENTATION
#include "match.h"



#define STRING_MAX 256



typedef struct Var Var;

struct Var {
	char *key;
	char *value;
};



typedef struct Dict Dict;

struct Dict {
	char *key;
	void *value;
};



Vector *vars = NULL;
Vector *dict = NULL;



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



void strrpc(char *src, char *str, char *rep) {
	char *p = strstr(src, str);
	if (p) {
		int len = strlen(src)+strlen(rep)-strlen(str);
		char r[len];
		memset(r, 0, len);
		if ( p >= src ) {
			strncpy(r, src, p-src);
			r[p-src]='\0';
			strncat(r, rep, strlen(rep));
			strncat(r, p+strlen(str), p+strlen(str)-src+strlen(src));
			strcpy(src, r);
			strrpc(p+strlen(rep), str, rep);
		}
	}
}



char *strtrm(char *a) {
	char *p = a, *q = a;
	while (isspace(*q)) ++q;
	while (*q) *p++ = *q++;
	*p = '\0';
	while (p > a && isspace(*--p)) *p = '\0';
	return a;
}



void varsDestroy(void *data) {
	free(((Var*)data)->key);
	((Var*)data)->key=NULL;
	
	free(((Var*)data)->value);  
	((Var*)data)->value=NULL;
		
	free(data);
	data=NULL;
}



void varsPrint(void *data) {
	printf(
		"key: %s value: %s\n",
		((Var*)data)->key,
		((Var*)data)->value
	);
}



void dictDestroy(void *data) {
	free(((Dict*)data)->key);
	((Dict*)data)->key=NULL;
	
	Vector_Destroy(((Dict*)data)->value);

	free(((Dict*)data)->value);  
	((Dict*)data)->value=NULL;
		
	free(data);
	data=NULL;
}



void dictPrint(void *data) {
	printf(
		"key: %s\n",
		((Dict*)data)->key
	);

	Vector_Print(((Dict*)data)->value);
}




void strDestroy(void *data) {
	free(data);
	data=NULL;
}



void strPrint(void *data) {
	printf("value: %s\n",(char*)data);
}





int load(char *filename) {

	FILE *fin;

	char *line=NULL;
	size_t llen=0;
	ssize_t rlen=0;


	vars=Vector_Create(0,varsPrint,varsDestroy);
	dict=Vector_Create(0,dictPrint,dictDestroy);


	if((fin=fopen(filename,"rt"))==NULL) {
		perror(filename);
		return 1;
	}

	while((rlen=getline(&line,&llen,fin))!=-1) {

		char *p=strchr(line,'\n');
		if(p) *p='\0';

		char key[STRING_MAX];
		char value[STRING_MAX];

		strtrm(line);
		
		if(line[0]=='$' && sscanf(line,"%[^=]=%[^\n]\n",key,value)==2) {

			Var *var=malloc(sizeof(*var));

			strtrm(key);
			strtrm(value);

			var->key=strdup(key);
			var->value=strdup(value);

			Vector_Append(vars,var);

		} else if(sscanf(line,"?%[^\n]\n",key)==1) {

			Dict *d=malloc(sizeof(*dict));

			d->key=strdup(key);
			d->value=Vector_Create(0,strPrint,strDestroy);

			while((rlen=getline(&line,&llen,fin))!=-1) {

				char *p=strchr(line,'\n');
				if(p) *p='\0';

				if(line[0]=='\t') {
					if(strlen(line+1)>0) {
						Vector_Append(d->value,strdup(strtrm(line+1)));
					}
				} else {
					break;
				}
			
			}  

			Vector_Append(dict,d);
		} 

		free(line);
		line=NULL;
		llen=0;
	}

	return 0;
}



int main() {

	load("dict.txt");

	Vector_Print(vars);
	Vector_Print(dict);

	return 0;
}
