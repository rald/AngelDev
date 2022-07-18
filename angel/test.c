#define EZXML_IMPLEMENTATION
#include "ezxml.h"



int main(int argc, char **argv)
{
    ezxml_t xml;
    char *s;
    int i;

    if (argc != 2) return fprintf(stderr, "usage: %s xmlfile\n", argv[0]);

    xml = ezxml_parse_file(argv[1]);
    printf("%s\n", (s = ezxml_toxml(xml)));
    free(s);
    i = fprintf(stderr, "%s", ezxml_error(xml));
    ezxml_free(xml);
    return (i) ? 1 : 0;
}
