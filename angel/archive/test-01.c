#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <stddef.h>
#include <string.h>


#define STRING_MAX 256

int match(char *r,char *s) {

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
        matched=1;
    } else if (reti == REG_NOMATCH) {
        matched=0;
    } else {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        exit(1);
    }

    /* Free memory allocated to the pattern buffer by regcomp() */
    regfree(&regex);

    return matched;
}



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
