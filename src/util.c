#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

char *str_trim(char *s)
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

char *str_trim_left(char *s)
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

char *str_trim_right(char *s)
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

// copy string to heap, return pointer to the copied string
char *str_copy(const char *s)
{
    size_t len = strlen(s) + 1;
    char *sp = (char*)malloc(len);
    if (sp == NULL) {
        return NULL;
    }
    strncpy(sp, s, len);
    return sp;
}

// copy most n chars to heap, return pointer to the copied string
char *str_copy_n(const char *s, size_t n)
{
    char *sp = (char*)malloc(n+1);
    if (sp == NULL) {
        return NULL;
    }
    strncpy(sp, s, n);
    sp[n] = '\0';
    return sp;
}
