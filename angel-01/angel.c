#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <regex.h>
#include <errno.h>
#include <unistd.h>

#define VECTOR_IMPLEMENTATION
#include "vector.h"

#define DICT_FILE "dict.txt"

#define STRING_MAX 512

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
Vector *dicts = NULL;

char line[STRING_MAX];

ssize_t dictIndex = 0;
ssize_t dictStart = 0;

int match(char *r, char *s) {
	int matched;

	regex_t regex;
	int reti;

	char msgbuf[100];

	/* Compile regular expression */
	reti = regcomp(&regex, r, 0);
	if (reti) {
		fprintf(stderr, "Could not compile regex\n");
		exit(1);
	}

	/* Execute regular expression */
	reti = regexec(&regex, s, 0, NULL, 0);
	if (!reti) {
		matched = 1;
	} else if (reti == REG_NOMATCH) {
		matched = 0;
	} else {
		regerror(reti, &regex, msgbuf, sizeof(msgbuf));
		fprintf(stderr, "Regex match failed: %s\n", msgbuf);
		exit(1);
	}

	/* Free memory allocated to the pattern buffer by regcomp() */
	regfree(&regex);

	return matched;
}

int *wrapInt(int x) {
	int *integer = malloc(sizeof(*integer));
	if (integer) {
		*integer = x;
	}
	return integer;
}

int unwrapInt(int *x) {
	return *x;
}

void IntDestroy(void **data) {
	free(*data);
	(*data) = NULL;
}

void strrpc(char *src, char *str, char *rep) {
	char *p = strstr(src, str);
	if (p) {
		int len = strlen(src) + strlen(rep) - strlen(str);
		char r[len];
		memset(r, 0, len);
		if (p >= src) {
			strncpy(r, src, p - src);
			r[p - src] = '\0';
			strncat(r, rep, strlen(rep));
			strncat(r, p + strlen(str), p + strlen(str) - src + strlen(src));
			strcpy(src, r);
			strrpc(p + strlen(rep), str, rep);
		}
	}
}

char *strtrm(char *a) {
	char *p = a, *q = a;
	while (isspace(*q))
		++q;
	while (*q)
		*p++ = *q++;
	*p = '\0';
	while (p > a && isspace(*--p))
		*p = '\0';
	return a;
}

char *strupr(char *s) {
	while (*s++)
		*s = toupper(*s);
	return s;
}

char *strlwr(char *s) {
	while (*s++)
		*s = tolower(*s);
	return s;
}

void varsDestroy(void *data) {
	free(((Var *)data)->key);
	((Var *)data)->key = NULL;

	free(((Var *)data)->value);
	((Var *)data)->value = NULL;

	free(data);
	data = NULL;
}

void varsPrint(void *data) {
	printf(
		"key: %s value: %s\n",
		((Var *)data)->key,
		((Var *)data)->value);
}

void dictDestroy(void *data) {
	free(((Dict *)data)->key);
	((Dict *)data)->key = NULL;

	Vector_Destroy(((Dict *)data)->value);

	free(data);
	data = NULL;
}

void dictPrint(void *data) {
	printf(
		"key: %s\n",
		((Dict *)data)->key);

	Vector_Print(((Dict *)data)->value);
}

void strDestroy(void *data) {
	free(data);
	data = NULL;
}

void strPrint(void *data) {
	printf("value: %s\n", (char *)data);
}

int load(char *filename) {
	FILE *fin;

	char *line = NULL;
	size_t llen = 0;
	ssize_t rlen = 0;

	vars = Vector_Create(0, varsPrint, varsDestroy);
	dicts = Vector_Create(0, dictPrint, dictDestroy);

	if ((fin = fopen(filename, "rt")) == NULL) {
		perror(filename);
		return 1;
	}

	while ((rlen = getline(&line, &llen, fin)) != -1) {
		char *p = strchr(line, '\n');
		if (p)
			*p = '\0';

		char key[STRING_MAX];
		char value[STRING_MAX];

		if (line[0] == '%' && sscanf(line, "%[^=]=%[^\n]\n", key, value) == 2) {
			Var *var = malloc(sizeof(*var));

			strtrm(key);
			strtrm(value);

			var->key = strdup(key);
			var->value = strdup(value);

			Vector_Append(vars, var);
		} else if (sscanf(line, "?%[^\n]\n", key) == 1) {
			Dict *d = malloc(sizeof(*dicts));

			d->key = strdup(key);
			d->value = Vector_Create(0, strPrint, strDestroy);

			while ((rlen = getline(&line, &llen, fin)) != -1) {
				char *p = strchr(line, '\n');
				if (p)
					*p = '\0';

				if (line[0] == '\t') {
					if (strlen(line + 1) > 0) {
						Vector_Append(d->value, strdup(strtrm(line + 1)));
					}
				} else
					break;
			}

			Vector_Append(dicts, d);
		}

		free(line);
		line = NULL;
		llen = 0;
	}

	return 0;
}

int save(char *filename) {
	FILE *fout;

	if ((fout = fopen(filename, "wt")) == NULL) {
		perror(filename);
		return 1;
	}

	for (size_t i = 0; i < vars->length; i++) {
		Var *var = Vector_Get(vars, i);
		fprintf(fout, "%s = %s\n", var->key, var->value);
	}

	fprintf(fout, "\n\n\n");

	for (size_t i = 0; i < dicts->length; i++) {
		Dict *dict = Vector_Get(dicts, i);
		fprintf(fout, "?%s\n", dict->key);

		for (size_t j = 0; j < Vector_Length(dict->value); j++) {
			fprintf(fout, "\t%s\n", (char *)Vector_Get(dict->value, j));
		}

		fprintf(fout, "\n");
	}

	fprintf(fout, "\n");

	fclose(fout);

	return 0;
}

char *varrpc(char *str) {
	for (size_t i = 0; i < vars->length; i++) {
		Var *var = Vector_Get(vars, i);
		strrpc(str, var->key, var->value);
	}
	return str;
}

void type(char *s) {
	while (*s) {
		usleep(rand() % 1000 * 500);
		putchar(*s);
		fflush(stdout);
		s++;
	}
	putchar('\n');
}

int main() {
	char key[STRING_MAX];
	char value[STRING_MAX];

	srand(time(NULL));

	load("dict.txt");

	for (;;) {
		printf("> ");

		fgets(line, STRING_MAX - 2, stdin);
		fflush(stdin);

		char *p = strchr(line, '\n');
		if (p)
			*p = '\0';

		if (!strcasecmp(line, ".quit")) {
			break;
		} else if (!strcasecmp(line, ".save")) {
			save(DICT_FILE);

			continue;
		} else if (line[0] == '%' && sscanf(line, "%[^=]=%[^\n]\n", key, value) == 2) {
			Var *var = malloc(sizeof(*var));

			var->key = strdup(strtrm(key));
			var->value = strdup(strtrm(value));

			Vector_Append(vars, var);

			continue;
		} else if (sscanf(line, "?%511[^\n]\n", key) == 1) {
			Dict *dict = malloc(sizeof(*dict));

			dict->key = strdup(strtrm(key));
			dict->value = Vector_Create(0, strPrint, strDestroy);

			printf("Huh?\n");

			for (;;) {
				printf("? ");

				fgets(value, STRING_MAX - 2, stdin);

				fflush(stdin);

				char *p = strchr(value, '\n');
				if (p)
					*p = '\0';

				strtrm(value);

				if (strlen(value) == 0)
					break;

				Vector_Append(dict->value, strdup(strtrm(value)));
			}

			Vector_Append(dicts, dict);

			continue;
		}

		varrpc(line);

		dictIndex = rand() % Vector_Length(dicts);

		dictStart = dictIndex;

		for (;;) {
		
			Dict *dict = Vector_Get(dicts, dictIndex);

			strcpy(key, dict->key);

			varrpc(key);



			if (strcasestr(line, key) || match(key, line)){ 
				Vector* vec=dict->value;

				char *str=Vector_Get(vec,rand()%vec->length);
				
				strcpy(value,str);

				varrpc(value);

				type(value);
				
				break;
			}

			dictIndex = (dictIndex + 1) % dicts->length;

			if (dictIndex == dictStart) {
				break;
			}
		}
	}

	Vector_Destroy(dicts);
	Vector_Destroy(vars);

	return 0;
}
