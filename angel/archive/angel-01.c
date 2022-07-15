#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define DICT_FILE "dict.txt"

#define STRING_MAX 256



typedef struct Dictionary Dictionary;

struct Dictionary {
	char *key;
	char *value;
};



Dictionary *dict = NULL;
ssize_t dictSize = 0;
ssize_t dictIndex = 0;
ssize_t dictStart = 0;
ssize_t dictDir = 0;



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



void add(Dictionary **dict, ssize_t *dictSize, char *key, char *value) {
	(*dict) = realloc(*dict, sizeof(**dict) * ((*dictSize) + 1));

	(*dict)[*dictSize].key = strdup(key);
	(*dict)[*dictSize].value = strdup(value);

	(*dictSize)++;
}



int load(char *filename,Dictionary **dict,ssize_t *dictSize) {

	FILE *fin=NULL;
	char line1[STRING_MAX];
	char line2[STRING_MAX];

	char *p;

	if((fin=fopen(filename,"rt"))==NULL) {
		printf("Error: opening %s\n",filename);
		return 1;
	}

	for(;;) {

		if(!fgets(line1,STRING_MAX-2,fin)) {
			break;
		}

		p = strchr(line1, '\n');
		if (p) {
			*p = '\0';
		}

		if(!fgets(line2,STRING_MAX-2,fin)) {
			break;
		}

		p = strchr(line2, '\n');
		if (p) {
			*p = '\0';
		}

		add(dict,dictSize,line1,line2);

	}

	fclose(fin);

	return 0;

}


int save(char *filename,Dictionary *dict,ssize_t dictSize) {

	FILE *fout=NULL;

	if((fout=fopen(filename,"wt"))==NULL) {
		printf("Error: opening %s",filename);
		return 1;
	}

	for(ssize_t i=0; i<dictSize; i++) {
		fprintf(fout,"%s\n",dict[i].key);
		fprintf(fout,"%s\n",dict[i].value);
	}

	fclose(fout);

	return 0;
}


char *ask() {
	char *line=malloc(sizeof(*line)*STRING_MAX);

	printf("Huh? ");

	fgets(line, STRING_MAX - 2, stdin);
	fflush(stdin);

	char *p = strchr(line, '\n');
	if (p) {
		*p = '\0';
	}

	return line;
}



int main(void) {
	char line[STRING_MAX];

	srand(time(NULL));

	load(DICT_FILE,&dict,&dictSize);

	dictIndex = rand() % dictSize;

	for (;;) {
		printf("> ");

		fgets(line, STRING_MAX - 2, stdin);

		fflush(stdin);

		char *p = strchr(line, '\n');
		if (p) {
			*p = '\0';
		}

		if (strcasestr(line, ".quit")) {
			break;
		} else if (strcasestr(line, ".save")) {
			save(DICT_FILE,dict,dictSize);
			continue;
		}

		dictStart = dictIndex;

		dictDir = rand() % 2 ? 1 : -1;

		for (;;) {
			if(line[0]!='?' && dictSize>0) {
				dictIndex += dictDir;

				if (dictIndex < 0)
					dictIndex = dictSize - 1;

				if (dictIndex > dictSize - 1)
					dictIndex = 0;
			}

			if (line[0]=='?' || dictSize==0 || dictIndex == dictStart) {
				char *ans=ask();
				if(strlen(ans)>0) {
					add(&dict, &dictSize, line[0]=='?'?line+1:line, ans);
				}
				free(ans);
				ans=NULL;
				break;
			}

			if (strcasestr(line, dict[dictIndex].key))            {
				puts(dict[dictIndex].value);
				break;
			}
		}
	}

	return 0;
}


