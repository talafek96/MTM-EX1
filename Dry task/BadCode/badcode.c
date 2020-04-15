#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

char *stringDuplicator(char *s, int times);

int main()
{
    char *tmp = stringDuplicator("hello", 3);
    printf("%s", tmp);
    return 0;
}

char *stringDuplicator(char *s, int times) { //Convention error - unconventional "s" shortcut for string/str
                                             //Convention error - bad function name - it is not a verb.
                                             //Coding error - s must be of type const char*
    assert(!s);
    assert(times > 0);
    int LEN = strlen(s); //Convention error - variable name is in capital letters - "LEN"
    char *out = malloc(LEN * times);
    assert(out); //Coding error - memory allocation must be checked during runtime and not as an assert
    for (int i = 0; i < times; i++) {
    out = out + LEN; //Coding error - does not start at the initial 0th index, leaving behind empty memory cells
    strcpy(out, s); //Coding error - at the last loop, will encounter a segmentation error for leaving the bounds of the malloc
                    //Convention error - no indentation!
    }
    return out; //Coding error - returns wrong address to string, only returns the last duplication (outside the malloc allocation...)
}

/*char *duplicateString(const char *str, int times) 
{
    assert(!str);
    assert(times > 0);
    int len = strlen(str);
    char *out = malloc(len * times);
    if (out == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < times; i++)
    {
        strcpy(out +(i*len), str);
    }
    return out;
}*/