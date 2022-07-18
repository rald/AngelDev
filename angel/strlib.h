#ifndef STRLIB_H
#define STRLIB_H



#include <string.h>
#include <ctype.h>



void strrpc(char *src, char *str, char *rep);
char *strtrm(char *a);
char *strupr(char *s);
char *strlwr(char *s);
char *strrnl(char *s);



#ifdef STRLIB_IMPLEMENTATION



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



char *strrnl(char *s) {
	char *p=strchr(s,'\0');
	if(p) *p='\0';
	return s;
}



char *strinp(char *m) {

	char *line=NULL;
	size_t llen=0;
	ssize_t rlen=0;
	

	printf("%s",m);

	if((rlen=getline(&line,&llen,stdin))==-1) {
		perror("error reading string");
		return NULL;
	}

	return line;
	
}



#endif /* STRLIB_IMPLEMENTATION */



#endif /* STRLIB_H */


