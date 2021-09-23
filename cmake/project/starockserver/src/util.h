#ifndef _STROCKS_UTIL_H
#define _STROCKS_UTIL_H


void elogInit(void);
int64_t starGetTimestampMs();
int regmatch(const char *pattern, const char *buf);
void del_char(char str[], char c);

#endif