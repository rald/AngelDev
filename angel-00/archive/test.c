#define MATCH_IMPLEMENTATION
#include "match.h"

#define STRING_MAX 256

char *remnl(char *s) {
	char *p=strchr(s,'\n');
	if(p) *p='\0';
	return s;
}

int main() {

	char r[STRING_MAX];
	char s[STRING_MAX];

	for(;;) {
 
		printf("reg: ");

		fgets(r,STRING_MAX,stdin);

		fflush(stdin);

		remnl(r);

		if(strlen(r)==0) break;


		for(;;) {

			printf("str: ");

			fgets(s,STRING_MAX,stdin);

			fflush(stdin);

			remnl(s);

			if(strlen(s)==0) break;
			
			printf("%d\n",match(r,s));

		}

	}

	return 0;

}
