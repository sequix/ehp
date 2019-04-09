#include <ctype.h>
#include <string.h>

char *ehp_util_trim(char *s)
{
    if (s == NULL) {
        return NULL;
    }

    int i = 0, left = 0, right = 0;

    while (isspace(s[left])) {
        ++left;
    }

    for (right = strlen(s) - 1; isspace(s[right]); --right) {
        ;
    }

    while (left <= right) {
        s[i++] = s[left++];
    }
    s[i] = '\0';
    return s;
}

char *ehp_util_trim_left(char *s)
{
    if (s == NULL) {
        return NULL;
    }

    int i = 0, left = 0;

    while (isspace(s[left])) {
        ++left;
    }

    while (s[left]) {
        s[i++] = s[left++];
    }
    s[i] = '\0';
    return s;
}

char *ehp_util_trim_right(char *s)
{
    if (s == NULL) {
        return NULL;
    }

    int right = 0;
    for (right = strlen(s) - 1; isspace(s[right]); --right) {
        ;
    }
    s[right+1] = '\0';
    return s;
}
