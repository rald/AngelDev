#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main() {

	char str[256]="Hello $name";

	strrpc(str,"$name","World");

	puts(str);

	return 0;
}
