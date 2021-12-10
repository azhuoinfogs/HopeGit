/*
 * Copyright (c) 2019 Starblaze, Inc. <qiang.hu@starblaze-tech.com>
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

#ifndef STARDB_PLATFORM_LINUX_H
#define STARDB_PLATFORM_LINUX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#include <argp.h>
#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <endian.h>
#include <errno.h>
#include <float.h>
#include <ifaddrs.h>
#include <libgen.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <pthread.h>
#include <pwd.h>
#include <regex.h>
#include <semaphore.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <syslog.h>
#include <termios.h>
#include <unistd.h>
#include <wchar.h>
#include <wordexp.h>
#include <wctype.h>
#include <inttypes.h>
#include <fcntl.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <error.h>
#include <linux/fs.h>
#define atomic_load_8(ptr) __atomic_load_n((ptr), __ATOMIC_SEQ_CST)
#define atomic_load_16(ptr) __atomic_load_n((ptr), __ATOMIC_SEQ_CST)
#define atomic_load_32(ptr) __atomic_load_n((ptr), __ATOMIC_SEQ_CST)
#define atomic_load_64(ptr) __atomic_load_n((ptr), __ATOMIC_SEQ_CST)
#define atomic_load_ptr(ptr) __atomic_load_n((ptr), __ATOMIC_SEQ_CST)

#define atomic_store_8(ptr, val) __atomic_store_n((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_store_16(ptr, val) __atomic_store_n((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_store_32(ptr, val) __atomic_store_n((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_store_64(ptr, val) __atomic_store_n((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_store_ptr(ptr, val) __atomic_store_n((ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_exchange_8(ptr, val) __atomic_exchange_n((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_exchange_16(ptr, val) __atomic_exchange_n((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_exchange_32(ptr, val) __atomic_exchange_n((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_exchange_64(ptr, val) __atomic_exchange_n((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_exchange_ptr(ptr, val) __atomic_exchange_n((ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_val_compare_exchange_8 __sync_val_compare_and_swap
#define atomic_val_compare_exchange_16 __sync_val_compare_and_swap
#define atomic_val_compare_exchange_32 __sync_val_compare_and_swap
#define atomic_val_compare_exchange_64 __sync_val_compare_and_swap
#define atomic_val_compare_exchange_ptr __sync_val_compare_and_swap

#define atomic_add_fetch_8(ptr, val) __atomic_add_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_add_fetch_16(ptr, val) __atomic_add_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_add_fetch_32(ptr, val) __atomic_add_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_add_fetch_64(ptr, val) __atomic_add_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_add_fetch_ptr(ptr, val) __atomic_add_fetch((ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_fetch_add_8(ptr, val) __atomic_fetch_add((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_add_16(ptr, val) __atomic_fetch_add((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_add_32(ptr, val) __atomic_fetch_add((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_add_64(ptr, val) __atomic_fetch_add((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_add_ptr(ptr, val) __atomic_fetch_add((ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_sub_fetch_8(ptr, val) __atomic_sub_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_sub_fetch_16(ptr, val) __atomic_sub_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_sub_fetch_32(ptr, val) __atomic_sub_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_sub_fetch_64(ptr, val) __atomic_sub_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_sub_fetch_ptr(ptr, val) __atomic_sub_fetch((ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_fetch_sub_8(ptr, val) __atomic_fetch_sub((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_sub_16(ptr, val) __atomic_fetch_sub((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_sub_32(ptr, val) __atomic_fetch_sub((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_sub_64(ptr, val) __atomic_fetch_sub((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_sub_ptr(ptr, val) __atomic_fetch_sub((ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_and_fetch_8(ptr, val) __atomic_and_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_and_fetch_16(ptr, val) __atomic_and_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_and_fetch_32(ptr, val) __atomic_and_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_and_fetch_64(ptr, val) __atomic_and_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_and_fetch_ptr(ptr, val) __atomic_and_fetch((ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_fetch_and_8(ptr, val) __atomic_fetch_and((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_and_16(ptr, val) __atomic_fetch_and((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_and_32(ptr, val) __atomic_fetch_and((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_and_64(ptr, val) __atomic_fetch_and((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_and_ptr(ptr, val) __atomic_fetch_and((ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_or_fetch_8(ptr, val) __atomic_or_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_or_fetch_16(ptr, val) __atomic_or_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_or_fetch_32(ptr, val) __atomic_or_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_or_fetch_64(ptr, val) __atomic_or_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_or_fetch_ptr(ptr, val) __atomic_or_fetch((ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_fetch_or_8(ptr, val) __atomic_fetch_or((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_or_16(ptr, val) __atomic_fetch_or((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_or_32(ptr, val) __atomic_fetch_or((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_or_64(ptr, val) __atomic_fetch_or((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_or_ptr(ptr, val) __atomic_fetch_or((ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_xor_fetch_8(ptr, val) __atomic_xor_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_xor_fetch_16(ptr, val) __atomic_xor_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_xor_fetch_32(ptr, val) __atomic_xor_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_xor_fetch_64(ptr, val) __atomic_xor_fetch((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_xor_fetch_ptr(ptr, val) __atomic_xor_fetch((ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_fetch_xor_8(ptr, val) __atomic_fetch_xor((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_xor_16(ptr, val) __atomic_fetch_xor((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_xor_32(ptr, val) __atomic_fetch_xor((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_xor_64(ptr, val) __atomic_fetch_xor((ptr), (val), __ATOMIC_SEQ_CST)
#define atomic_fetch_xor_ptr(ptr, val) __atomic_fetch_xor((ptr), (val), __ATOMIC_SEQ_CST)

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

#define STR_TO_VARSTR(x, str)             \
  do {                                    \
    VarDataLenT __len = strlen(str);      \
    *(VarDataLenT *)(x) = __len;          \
    memcpy(varDataVal(x), (str), __len); \
  } while (0);

#if LINUX
  #define STAR_NCHAR_SIZE         sizeof(wchar_t)
#else
  #define STAR_NCHAR_SIZE         4
#endif
//#define STAR_CHAR_TERMINATED_SPACE 1
typedef void *SDataRow;
#define POINTER_SHIFT(p, b) ((void *)((char *)(p) + (b)))
#define POINTER_DISTANCE(p1, p2) ((char *)(p1) - (char *)(p2)) 
// ----------------- Data row structure

/* A data row, the format is like below:
 * |<--------------------+--------------------------- len ---------------------------------->|
 * |<--     Head      -->|<---------   flen -------------->|                                 |
 * +---------------------+---------------------------------+---------------------------------+
 * | uint16_t |  int16_t |                                 |                                 |
 * +----------+----------+---------------------------------+---------------------------------+
 * |   len    | sversion |           First part            |             Second part         |
 * +----------+----------+---------------------------------+---------------------------------+
 */
#define SD_DATA_ROW_HEAD_SIZE (sizeof(uint16_t) + sizeof(int16_t))

#define dataRowLen(r) (*(uint16_t *)(r))
#define dataRowVersion(r) *(int16_t *)POINTER_SHIFT(r, sizeof(int16_t))

#define dataRowTuple(r) POINTER_SHIFT(r, SD_DATA_ROW_HEAD_SIZE)
#define dataRowKey(r) (*(TSKEY *)(dataRowTuple(r)))

#define dataRowSetKey(r, l) (dataRowKey(r) = (l))
#define dataRowSetLen(r, l) (dataRowLen(r) = (l))
#define dataRowSetVersion(r, v) (dataRowVersion(r) = (v))
#define dataRowCpy(dst, r) memcpy((dst), (r), dataRowLen(r))
#define dataRowMaxBytesFromSchema(s) (schemaTLen(s) + SD_DATA_ROW_HEAD_SIZE)

enum BlockType{
	STAR_BLOCK_TYPE_BITMAP = 1,
	STAR_BLOCK_TYPE_MEM,
	STAR_BLOCK_TYPE_DATA,
	STAR_BLOCK_TYPE_MAX
};
#define DEV_NAME "/dev/nvme0"
#define MAX_NAME_LEN 32
#define MAX_DEVICE_COUNT 8
#define STAR_DBINFO_BLOCKID 0
#define BLOCK_START 1
#define STAR_BITMAP_BLOCKID_START 1
#define STAR_BITMAP_BLOCKID_END   99

#define STAR_WAL_BLOCKID_START 100
#define STAR_WAL_BLOCKID_END   4199

#define STAR_DNODE_CFG_BLOCKID    	  4200
#define STAR_MNODE_IP_LIST_BLOCKID    4201
#define STAR_VNODE_CFG_BLOCKID_START  4203
#define STAR_VNODE_CFG_BLOCKID_END    4219

#define STAR_MEM_START 4220
#define STAR_MEM_END   4299
#define STAR_DBINFO_BLOCKID_START   4224
#define STAR_DBINFO_BLOCKID_END   4299

#define STAR_TBINFO_BLOCKID_START 4300
#define STAR_TBINFO_BLOCKID_END   5199

#define STAR_DATA_BLOCKID_START   5200
#define STAR_DATA_BLOCKID_END     0xFFFFFFFF
typedef struct Block {
	uint32_t is_used;
	uint32_t id;
	uint32_t size;
	uint32_t type;
	uint32_t used_size;
	uint32_t free_size;
	uint64_t start_address;
	uint64_t end_address;
} Block;
#ifdef __cplusplus
}
#endif

#endif
