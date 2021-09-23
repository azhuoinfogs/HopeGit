#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<stdbool.h>
#define TRUE 1
#define FALSE 0
#define UINT8 int8_t
#define DIGITAL_TYPE 1
#define LETTER_TYPE  2
#define BOTTOM_TYPE  3
typedef struct _Interpret
{
    int type;
    void* (*process)(void* pData, int* type, int* result);
 
}Interpret;
 
void* digital_process(void* pData, int* type, int* result)
{
    UINT8* str;
    assert(NULL != pData && NULL != type && NULL != result);
 
    str = (UINT8*)pData;
    while (*str >= '0' && *str <= '9')
    {
        str ++;
    } 
 
    if(*str == '\0')
    {
        *result = TRUE;
        return NULL;
    }   
 
    if(*str == '_')
    {
        *result = TRUE;
        *type = BOTTOM_TYPE;
        return str;
    }
 
    if(*str >= 'a' && *str <= 'z' || *str >= 'A' && *str <= 'Z')
    {
        *result = TRUE;
        *type = LETTER_TYPE;
        return str;
    }
 
    *result = FALSE;
    return NULL;            
}    
 
void* letter_process(void* pData, int* type, int* result)
{
    UINT8* str;
    assert(NULL != pData && NULL != type && NULL != result);
 
    str = (UINT8*)pData;
    while (*str >= 'a' && *str <= 'z' || *str >= 'A' && *str <= 'Z')
    {
        str ++;
    } 
 
    if(*str == '\0')
    {
        *result = TRUE;
        return NULL;
    }   
 
    if(*str == '_')
    {
        *result = TRUE;
        *type = BOTTOM_TYPE;
        return str;
    }
 
    if(*str >= '0' && *str <= '9')
    {
        *result = TRUE;
        *type = DIGITAL_TYPE;
        return str;
    }
 
    *result = FALSE;
    return NULL;            
}          
 
void* bottom_process(void* pData, int* type, int* result)
{
    UINT8* str;
    assert(NULL != pData && NULL != type && NULL != result);
 
    str = (UINT8*)pData;
    while ('_' == *str )
    {
        str ++;
    } 
 
    if(*str == '\0')
    {
        *result = TRUE;
        return NULL;
    }   
 
    if(*str >= 'a' && *str <= 'z' || *str >= 'A' && *str <= 'Z')
    {
        *result = TRUE;
        *type = LETTER_TYPE;
        return str;
    }
 
    if(*str >= '0' && *str <= '9')
    {
        *result = TRUE;
        *type = DIGITAL_TYPE;
        return str;
    }
    *result = FALSE;
    return NULL;            
}
Interpret *InitInterpret(int type) {
    Interpret *i = malloc (sizeof (Interpret));
    memset(i, 0, sizeof(Interpret));
    switch (type)
    {
    case DIGITAL_TYPE:
        i->process = digital_process;
        /* code */
        break;
    case LETTER_TYPE:
        i->process = letter_process;
        /* code */
        break;
    case BOTTOM_TYPE:
        i->process = bottom_process;
        /* code */
        break;
    default:
        break;
    }
    return i;
}
int main () {
    Interpret *i = InitInterpret(LETTER_TYPE);
    int type;
    int result;
    i->process("12", &type, &result);
    printf("%d--%d\n", type, result);
}