#ifndef _LIB_UTIL_H
#define _LIB_UTIL_H
#include "os.h"
#pragma once
#define CMD_BUF_LENGTH ((128)*(1024))
#define ALL_BUF_LENGTH ((256)*(1024))			//128k is enough
#define ALIGN(num,ali) (((num)+(ali)-(1))/(ali)*(ali))
#define GM_HASH_BIT_NUM (128)
#define GM_HASH_BYTE (GM_HASH_BIT_NUM / 8)   // 16-Byte
#define GM_HASH_DWORD (GM_HASH_BIT_NUM / 32) // 4-Dw
#define GM_RK_DWORD  (8)
typedef unsigned char           uint8_t;  
typedef unsigned short int      uint16_t;
typedef unsigned int            uint32_t;
typedef int                     int32_t;  
#if __WORDSIZE == 64  
typedef unsigned long int       uint64_t;  
#else  
__extension__  
typedef unsigned long long int  uint64_t;  
#endif 
#if defined(__GNUC__)
#define FORCE_INLINE inline __attribute__((always_inline))
#else
#define FORCE_INLINE
#endif

typedef uint32_t UINT32;
typedef uint16_t quint16;
typedef uint8_t quint8;
typedef uint32_t quint32;
typedef int32_t qint32;
typedef uint64_t quint64;
#define STAR_ORDER_ASC   1
#define STAR_ORDER_DESC  2

#define CHAR_BYTES   sizeof(char)
#define SHORT_BYTES  sizeof(short)
#define INT_BYTES    sizeof(int)
#define LONG_BYTES   sizeof(int64_t)
#define FLOAT_BYTES  sizeof(float)
#define DOUBLE_BYTES sizeof(double)

#define POINTER_BYTES sizeof(void *)  // 8 by default  assert(sizeof(ptrdiff_t) == sizseof(void*)

#define STAR_DATA_TYPE_NULL       0     // 1 bytes
#define STAR_DATA_TYPE_BOOL       1     // 1 bytes
#define STAR_DATA_TYPE_TINYINT    2     // 1 byte
#define STAR_DATA_TYPE_SMALLINT   3     // 2 bytes
#define STAR_DATA_TYPE_INT        4     // 4 bytes
#define STAR_DATA_TYPE_BIGINT     5     // 8 bytes
#define STAR_DATA_TYPE_FLOAT      6     // 4 bytes
#define STAR_DATA_TYPE_DOUBLE     7     // 8 bytes
#define STAR_DATA_TYPE_BINARY     8     // string
#define STAR_DATA_TYPE_TIMESTAMP  9     // 8 bytes
#define STAR_DATA_TYPE_NCHAR      10    // unicode string
#define tfree(x)         \
  {                      \
    if (x) {             \
      free((void *)(x)); \
      x = 0;             \
    }                    \
  }

#define tstrncpy(dst, src, size)   \
  do {                             \
    strncpy((dst), (src), (size)); \
    (dst)[(size)-1] = 0;           \
  } while (0);

#define GET_INT8_VAL(x)   (*(int8_t *)(x))
#define GET_INT16_VAL(x)  (*(int16_t *)(x))
#define GET_INT32_VAL(x)  (*(int32_t *)(x))
#define GET_INT64_VAL(x)  (*(int64_t *)(x))
#ifdef _TD_ARM_32_
  #define GET_FLOAT_VAL(x)  taos_align_get_float(x)
  #define GET_DOUBLE_VAL(x) taos_align_get_double(x)

  float  taos_align_get_float(const char* pBuf);
  double taos_align_get_double(const char* pBuf);

  //#define __float_align_declear()  float __underlyFloat = 0.0;
  //#define __float_align_declear()
  //#define GET_FLOAT_VAL_ALIGN(x) (*(int32_t*)&(__underlyFloat) = *(int32_t*)(x); __underlyFloat);
  // notes: src must be float or double type variable !!!
  #define SET_FLOAT_VAL_ALIGN(dst, src) (*(int32_t*) dst = *(int32_t*)src);
  #define SET_DOUBLE_VAL_ALIGN(dst, src) (*(int64_t*) dst = *(int64_t*)src);
#else
  #define GET_FLOAT_VAL(x)  (*(float *)(x))
  #define GET_DOUBLE_VAL(x) (*(double *)(x))
#endif

#define DEFAULT_UNICODE_ENCODEC "UCS-4LE"
  
#define DEFAULT_COMP(x, y)       \
  do {                           \
    if ((x) == (y)) {            \
      return 0;                  \
    } else {                     \
      return (x) < (y) ? -1 : 1; \
    }                            \
  } while (0)

#define ALIGN_NUM(n, align) (((n) + ((align)-1)) & (~((align)-1)))

// align to 8bytes
#define ALIGN8(n) ALIGN_NUM(n, 8)
void d_raw(unsigned char *buf, unsigned len);
void nvme_free(void *p, bool huge);
void *nvme_alloc(size_t len, int basesize, bool *huge);
int tSystem(const char * cmd);
int create_ns(char *devname, size_t lba_count);
#endif /* _LIB_NVMED_H */