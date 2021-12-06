/*
 * Copyright (c) 2019 Starblaze Data, Inc. <qiang.hu@Starblaze-tech.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef STAR_DEF_H
#define STAR_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "starkv.h"

#define STAR__packed

#ifdef TSKEY32
#define TSKEY int32_t;
#else
#define TSKEY int64_t
#endif

#define TSWINDOW_INITIALIZER ((STimeWindow) {INT64_MIN, INT64_MAX})
#define TSKEY_INITIAL_VAL    INT64_MIN

// ----------------- For variable data types such as STAR_DATA_TYPE_BINARY and STAR_DATA_TYPE_NCHAR
typedef int32_t VarDataOffsetT;
typedef int16_t VarDataLenT;

typedef struct tstr {
  VarDataLenT len;
  char        data[];
} tstr;

#define VARSTR_HEADER_SIZE  sizeof(VarDataLenT)

#define varDataLen(v)       ((VarDataLenT *)(v))[0]
#define varDataTLen(v)      (sizeof(VarDataLenT) + varDataLen(v))
#define varDataVal(v)       ((void *)((char *)v + VARSTR_HEADER_SIZE))
#define varDataCopy(dst, v) memcpy((dst), (void*) (v), varDataTLen(v))
#define varDataLenByData(v) (*(VarDataLenT *)(((char*)(v)) - VARSTR_HEADER_SIZE))
#define varDataSetLen(v, _len) (((VarDataLenT *)(v))[0] = (VarDataLenT) (_len))
#define IS_VAR_DATA_TYPE(t) (((t) == STAR_DATA_TYPE_BINARY) || ((t) == STAR_DATA_TYPE_NCHAR))

// this data type is internally used only in 'in' query to hold the values
#define STAR_DATA_TYPE_ARRAY      (STAR_DATA_TYPE_NCHAR + 1)


// Bytes for each type.
extern const int32_t TYPE_BYTES[11];
// TODO: replace and remove code below
#define CHAR_BYTES   sizeof(char)
#define SHORT_BYTES  sizeof(short)
#define INT_BYTES    sizeof(int)
#define LONG_BYTES   sizeof(int64_t)
#define FLOAT_BYTES  sizeof(float)
#define DOUBLE_BYTES sizeof(double)

// NULL definition
#define STAR_DATA_BOOL_NULL             0x02
#define STAR_DATA_TINYINT_NULL          0x80
#define STAR_DATA_SMALLINT_NULL         0x8000
#define STAR_DATA_INT_NULL              0x80000000
#define STAR_DATA_BIGINT_NULL           0x8000000000000000L

#define STAR_DATA_FLOAT_NULL            0x7FF00000              // it is an NAN
#define STAR_DATA_DOUBLE_NULL           0x7FFFFF0000000000L     // an NAN
#define STAR_DATA_NCHAR_NULL            0xFFFFFFFF
#define STAR_DATA_BINARY_NULL           0xFF

#define STAR_DATA_NULL_STR              "NULL"
#define STAR_DATA_NULL_STR_L            "null"

#define STAR_DEFAULT_USER               "root"
#define STAR_DEFAULT_PASS               "starblaze"

#define STAR_TRUE   1
#define STAR_FALSE  0
#define STAR_OK     0
#define STAR_ERR   -1

#define TS_PATH_DELIMITER "."

#define STAR_TIME_PRECISION_MILLI 0
#define STAR_TIME_PRECISION_MICRO 1
#define STAR_TIME_PRECISION_NANO  2

#define STAR_TIME_PRECISION_MILLI_STR "ms"
#define STAR_TIME_PRECISION_MICRO_STR "us"

#define T_MEMBER_SIZE(type, member) sizeof(((type *)0)->member)
#define T_APPEND_MEMBER(dst, ptr, type, member) \
do {\
  memcpy((void *)(dst), (void *)(&((ptr)->member)), T_MEMBER_SIZE(type, member));\
  dst = (void *)((char *)(dst) + T_MEMBER_SIZE(type, member));\
} while(0)
#define T_READ_MEMBER(src, type, target) \
do { \
  (target) = *(type *)(src); \
  (src) = (void *)((char *)src + sizeof(type));\
} while(0)

#define STAR_KEYSIZE              sizeof(TSKEY)

#if LINUX
  #define STAR_NCHAR_SIZE         sizeof(wchar_t)
#else
  #define STAR_NCHAR_SIZE         4
#endif
//#define STAR_CHAR_TERMINATED_SPACE 1

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

typedef struct tDataTypeDescriptor {
  int16_t nType;
  int16_t nameLen;
  int32_t nSize;
  char *  aName;
  int (*compFunc)(const char *const input, int inputSize, const int nelements, char *const output, int outputSize,
                  char algorithm, char *const buffer, int bufferSize);
  int (*decompFunc)(const char *const input, int compressedSize, const int nelements, char *const output,
                    int outputSize, char algorithm, char *const buffer, int bufferSize);
  void (*getStatisFunc)(const TSKEY *primaryKey, const void *pData, int32_t numofrow, int64_t *min, int64_t *max,
                         int64_t *sum, int16_t *minindex, int16_t *maxindex, int16_t *numofnull);
} tDataTypeDescriptor;

extern tDataTypeDescriptor tDataTypeDesc[11];
#define POINTER_BYTES sizeof(void *)  // 8 by default  assert(sizeof(ptrdiff_t) == sizseof(void*)

bool isValidDataType(int32_t type);
//bool isNull(const char *val, int32_t type);
static inline __attribute__((always_inline)) bool isNull(const char *val, int32_t type) {
  switch (type) {
    case STAR_DATA_TYPE_BOOL:
      return *(uint8_t *)val == STAR_DATA_BOOL_NULL;
    case STAR_DATA_TYPE_TINYINT:
      return *(uint8_t *)val == STAR_DATA_TINYINT_NULL;
    case STAR_DATA_TYPE_SMALLINT:
      return *(uint16_t *)val == STAR_DATA_SMALLINT_NULL;
    case STAR_DATA_TYPE_INT:
      return *(uint32_t *)val == STAR_DATA_INT_NULL;
    case STAR_DATA_TYPE_BIGINT:
    case STAR_DATA_TYPE_TIMESTAMP:
      return *(uint64_t *)val == STAR_DATA_BIGINT_NULL;
    case STAR_DATA_TYPE_FLOAT:
      return *(uint32_t *)val == STAR_DATA_FLOAT_NULL;
    case STAR_DATA_TYPE_DOUBLE:
      return *(uint64_t *)val == STAR_DATA_DOUBLE_NULL;
    case STAR_DATA_TYPE_NCHAR:
      return *(uint32_t*) varDataVal(val) == STAR_DATA_NCHAR_NULL;
    case STAR_DATA_TYPE_BINARY:
      return *(uint8_t *) varDataVal(val) == STAR_DATA_BINARY_NULL;
    default:
      return false;
  };
}

void setVardataNull(char* val, int32_t type);
void setNull(char *val, int32_t type, int32_t bytes);
void setNullN(char *val, int32_t type, int32_t bytes, int32_t numOfElems);
void* getNullValue(int32_t type);

void assignVal(char *val, const char *src, int32_t len, int32_t type);
void tsDataSwap(void *pLeft, void *pRight, int32_t type, int32_t size);

// TODO: check if below is necessary
#define STAR_RELATION_INVALID     0
#define STAR_RELATION_LESS        1
#define STAR_RELATION_GREATER     2
#define STAR_RELATION_EQUAL       3
#define STAR_RELATION_LESS_EQUAL  4
#define STAR_RELATION_GREATER_EQUAL 5
#define STAR_RELATION_NOT_EQUAL   6
#define STAR_RELATION_LIKE        7
#define STAR_RELATION_IN          8

#define STAR_RELATION_AND         9
#define STAR_RELATION_OR          10
#define STAR_RELATION_NOT         11

#define STAR_BINARY_OP_ADD        12
#define STAR_BINARY_OP_SUBTRACT   13
#define STAR_BINARY_OP_MULTIPLY   14
#define STAR_BINARY_OP_DIVIDE     15
#define STAR_BINARY_OP_REMAINDER  16
#define TS_PATH_DELIMITER_LEN     1

#define STAR_UNI_LEN              24
#define STAR_USER_LEN             STAR_UNI_LEN
// ACCOUNT is a 32 bit positive integer
// this is the length of its string representation
// including the terminator zero
#define STAR_ACCT_LEN             11
#define STAR_PASSWORD_LEN         STAR_UNI_LEN

#define STAR_MAX_COLUMNS          1024
#define STAR_MIN_COLUMNS          2       //PRIMARY COLUMN(timestamp) + other columns

#define STAR_NODE_NAME_LEN        64
#define STAR_TABLE_NAME_LEN       193     // it is a null-terminated string
#define STAR_DB_NAME_LEN          33
#define STAR_KV_KEY_LEN           32
#define STAR_TABLE_ID_LEN         (STAR_ACCT_LEN + STAR_DB_NAME_LEN + STAR_TABLE_NAME_LEN)
#define STAR_COL_NAME_LEN         65
#define STAR_MAX_SAVED_SQL_LEN    STAR_MAX_COLUMNS * 64
#define STAR_MAX_SQL_LEN          STAR_PAYLOAD_SIZE
#define STAR_MAX_SQL_SHOW_LEN     256
#define STAR_MAX_ALLOWED_SQL_LEN  (8*1024*1024U)          // sql length should be less than 8mb

#define STAR_MAX_BYTES_PER_ROW    16384
#define STAR_MAX_TAGS_LEN         16384
#define STAR_MAX_TAGS             128

#define STAR_AUTH_LEN             16
#define STAR_KEY_LEN              16
#define STAR_VERSION_LEN          12
#define STAR_STREET_LEN           64
#define STAR_CITY_LEN             20
#define STAR_STATE_LEN            20
#define STAR_COUNTRY_LEN          20
#define STAR_LOCALE_LEN           64
#define STAR_TIMEZONE_LEN         64
#define STAR_LABEL_LEN            8 

#define STAR_FQDN_LEN             128
#define STAR_EP_LEN               (STAR_FQDN_LEN+6)
#define STAR_IPv4ADDR_LEN      	  16
#define STAR_FILENAME_LEN         128
#define STAR_METER_VNODE_BITS     20
#define STAR_METER_SID_MASK       0xFFFFF
#define STAR_SHELL_VNODE_BITS     24
#define STAR_SHELL_SID_MASK       0xFF
#define STAR_HTTP_TOKEN_LEN       20
#define STAR_SHOW_SQL_LEN         64
#define STAR_SLOW_QUERY_SQL_LEN   512

#define STAR_METER_STATE_OFFLINE  0
#define STAR_METER_STATE_ONLLINE  1

#define STAR_DEFAULT_PKT_SIZE     65480  //same as RPC_MAX_UDP_SIZE

#define STAR_PAYLOAD_SIZE         STAR_DEFAULT_PKT_SIZE
#define STAR_DEFAULT_PAYLOAD_SIZE 5120   // default payload size, greater than PATH_MAX value
#define STAR_EXTRA_PAYLOAD_SIZE   128    // extra bytes for auth
#define STAR_CQ_SQL_SIZE          1024
#define STAR_MIN_VNODES           64
#define STAR_MAX_VNODES           2048

#define STAR_DNODE_ROLE_ANY       0
#define STAR_DNODE_ROLE_MGMT      1
#define STAR_DNODE_ROLE_VNODE     2

#define STAR_MAX_REPLICA          5

#define STAR_TBNAME_COLUMN_INDEX       (-1)
#define STAR_MULTI_METERMETA_MAX_NUM    100000  // maximum batch size allowed to load metermeta

#define STAR_MIN_CACHE_BLOCK_SIZE       1
#define STAR_MAX_CACHE_BLOCK_SIZE       128     // 128MB for each vnode
#define STAR_DEFAULT_CACHE_BLOCK_SIZE   16

#define STAR_MIN_TOTAL_BLOCKS           2
#define STAR_MAX_TOTAL_BLOCKS           10000
#define STAR_DEFAULT_TOTAL_BLOCKS       6

#define STAR_MIN_TABLES                 4
#define STAR_MAX_TABLES                 10000000
#define STAR_DEFAULT_TABLES             1000000
#define STAR_TABLES_STEP                1000

#define STAR_MIN_DAYS_PER_FILE          1
#define STAR_MAX_DAYS_PER_FILE          3650 
#define STAR_DEFAULT_DAYS_PER_FILE      10 

#define STAR_MIN_KEEP                   1        // data in db to be reserved.
#define STAR_MAX_KEEP                   365000   // data in db to be reserved.
#define STAR_DEFAULT_KEEP               3650     // ten years

#define STAR_DEFAULT_MIN_ROW_FBLOCK     100
#define STAR_MIN_MIN_ROW_FBLOCK         10
#define STAR_MAX_MIN_ROW_FBLOCK         1000

#define STAR_DEFAULT_MAX_ROW_FBLOCK     4096
#define STAR_MIN_MAX_ROW_FBLOCK         200
#define STAR_MAX_MAX_ROW_FBLOCK         10000

#define STAR_MIN_COMMIT_TIME            30
#define STAR_MAX_COMMIT_TIME            40960
#define STAR_DEFAULT_COMMIT_TIME        3600

#define STAR_MIN_PRECISION              STAR_TIME_PRECISION_MILLI
#define STAR_MAX_PRECISION              STAR_TIME_PRECISION_NANO
#define STAR_DEFAULT_PRECISION          STAR_TIME_PRECISION_MILLI

#define STAR_MIN_COMP_LEVEL             0
#define STAR_MAX_COMP_LEVEL             2
#define STAR_DEFAULT_COMP_LEVEL         0

#define STAR_MIN_WAL_LEVEL              1
#define STAR_MAX_WAL_LEVEL              2
#define STAR_DEFAULT_WAL_LEVEL          1

#define STAR_MIN_FSYNC_PERIOD           0
#define STAR_MAX_FSYNC_PERIOD           180000   // millisecond
#define STAR_DEFAULT_FSYNC_PERIOD       3000     // three second

#define STAR_MIN_DB_REPLICA_OPTION      1
#define STAR_MAX_DB_REPLICA_OPTION      3
#define STAR_DEFAULT_DB_REPLICA_OPTION  1

#define STAR_MAX_JOIN_TABLE_NUM         5
#define STAR_MAX_UNION_CLAUSE           5

#define STAR_MAX_BINARY_LEN            (STAR_MAX_BYTES_PER_ROW-STAR_KEYSIZE)
#define STAR_MAX_NCHAR_LEN             (STAR_MAX_BYTES_PER_ROW-STAR_KEYSIZE)
#define PRIMARYKEY_TIMESTAMP_COL_INDEX  0

#define STAR_MAX_RPC_THREADS            5

#define STAR_QUERY_TYPE_NON_TYPE                       0x00u     // none type
#define STAR_QUERY_TYPE_FREE_RESOURCE                  0x01u     // free qhandle at vnode

/*
 * 1. ordinary sub query for select * from super_table
 * 2. all sqlobj generated by createSubqueryObj with this flag
 */
#define STAR_QUERY_TYPE_SUBQUERY                       0x02u
#define STAR_QUERY_TYPE_STABLE_SUBQUERY                0x04u     // two-stage subquery for super table

#define STAR_QUERY_TYPE_TABLE_QUERY                    0x08u    // query ordinary table; below only apply to client side
#define STAR_QUERY_TYPE_STABLE_QUERY                   0x10u    // query on super table
#define STAR_QUERY_TYPE_JOIN_QUERY                     0x20u    // join query
#define STAR_QUERY_TYPE_PROJECTION_QUERY               0x40u    // select *,columns... query
#define STAR_QUERY_TYPE_JOIN_SEC_STAGE                 0x80u    // join sub query at the second stage

#define STAR_QUERY_TYPE_TAG_FILTER_QUERY              0x400u
#define STAR_QUERY_TYPE_INSERT                        0x100u    // insert type
#define STAR_QUERY_TYPE_MULTITABLE_QUERY              0x200u
#define STAR_QUERY_TYPE_STMT_INSERT                   0x800u    // stmt insert type

#define STAR_QUERY_HAS_TYPE(x, _type)         (((x) & (_type)) != 0)
#define STAR_QUERY_SET_TYPE(x, _type)         ((x) |= (_type))
#define STAR_QUERY_CLEAR_TYPE(x, _type)       ((x) &= (~_type))
#define STAR_QUERY_RESET_TYPE(x)              ((x) = STAR_QUERY_TYPE_NON_TYPE)

#define STAR_ORDER_ASC   1
#define STAR_ORDER_DESC  2

#define STAR_DEFAULT_MNODES_HASH_SIZE   5
#define STAR_DEFAULT_DNODES_HASH_SIZE   10
#define STAR_DEFAULT_ACCOUNTS_HASH_SIZE 10
#define STAR_DEFAULT_USERS_HASH_SIZE    20
#define STAR_DEFAULT_DBS_HASH_SIZE      100
#define STAR_DEFAULT_VGROUPS_HASH_SIZE  100
#define STAR_DEFAULT_STABLES_HASH_SIZE  100
#define STAR_DEFAULT_CTABLES_HASH_SIZE  20000

#define STAR_PORT_DNODESHELL 0 
#define STAR_PORT_DNODEDNODE 5 
#define STAR_PORT_SYNC       10 

#define STAR_QTYPE_RPC      0
#define STAR_QTYPE_FWD      1
#define STAR_QTYPE_WAL      2 
#define STAR_QTYPE_CQ       3
#define STAR_QTYPE_QUERY    4

typedef enum {
  STAR_SUPER_TABLE        = 0,  // super table
  STAR_CHILD_TABLE        = 1,  // table created from super table
  STAR_NORMAL_TABLE       = 2,  // ordinary table
  STAR_STREAM_TABLE       = 3,  // table created from stream computing
  STAR_TABLE_MAX          = 4
} ETableType;

typedef enum {
  STAR_MOD_MNODE,
  STAR_MOD_HTTP,
  STAR_MOD_MONITOR,
  STAR_MOD_MQTT,
  STAR_MOD_MAX
} EModuleType;

#ifdef __cplusplus
}
#endif

#endif
