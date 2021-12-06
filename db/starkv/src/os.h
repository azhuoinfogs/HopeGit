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

#define starCloseSocket(x) \
  {                        \
    if (FD_VALID(x)) {     \
      close(x);            \
      x = FD_INITIALIZER;  \
    }                      \
  }
  
#ifdef STAR_RANDOM_NETWORK_FAIL

ssize_t star_send_random_fail(int sockfd, const void *buf, size_t len, int flags);

ssize_t star_sendto_random_fail(int sockfd, const void *buf, size_t len, int flags,
                      const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t star_read_random_fail(int fd, void *buf, size_t count);
ssize_t star_write_random_fail(int fd, const void *buf, size_t count);

#define send(sockfd, buf, len, flags) star_send_random_fail(sockfd, buf, len, flags)
#define sendto(sockfd, buf, len, flags, dest_addr, addrlen) \
          star_sendto_random_fail(sockfd, buf, len, flags, dest_addr, addrlen)
#define starWriteSocket(fd, buf, len) star_write_random_fail(fd, buf, len)
#define starReadSocket(fd, buf, len) star_read_random_fail(fd, buf, len)

#else

#define starWriteSocket(fd, buf, len) write(fd, buf, len)
#define starReadSocket(fd, buf, len) read(fd, buf, len)

#endif  /* STAR_RANDOM_NETWORK_FAIL */

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

#define SWAP(a, b, c)      \
  do {                     \
    typeof(a) __tmp = (a); \
    (a) = (b);             \
    (b) = __tmp;           \
  } while (0)

#define MAX(a, b)            \
  ({                         \
    typeof(a) __a = (a);     \
    typeof(b) __b = (b);     \
    (__a > __b) ? __a : __b; \
  })

#define MIN(a, b)            \
  ({                         \
    typeof(a) __a = (a);     \
    typeof(b) __b = (b);     \
    (__a < __b) ? __a : __b; \
  })

#define MILLISECOND_PER_SECOND ((int64_t)1000L)

#define tsem_t sem_t
#define tsem_init sem_init
#define tsem_wait sem_wait
#define tsem_post sem_post
#define tsem_destroy sem_destroy

void osInit();

ssize_t tsendfile(int dfd, int sfd, off_t *offset, size_t size);

ssize_t twrite(int fd, void *buf, size_t n);

ssize_t tread(int fd, void *buf, size_t count);

ssize_t starpwrite(int __fd, const void *__buf, size_t __n, __off_t __offset);

ssize_t starpread(int __fd, void *__buf, size_t __nbytes, __off64_t __offset);

bool starCheckPthreadValid(pthread_t thread);

void starResetPthread(pthread_t *thread);

int64_t starGetPthreadId();

int starSetNonblocking(int sock, int on);

int starSetSockOpt(int socketfd, int level, int optname, void *optval, int optlen);

void starPrintOsInfo();

char *starCharsetReplace(char *charsetstr);

void starGetSystemInfo();

void starKillSystem();

bool starSkipSocketCheck();

int64_t str2int64(char *str);

void starSetCoreDump();

void starBlockSIGPIPE();

int tSystem(const char * cmd) ;

#define MAX_ROW_SIZE 1024
#define MAX_SHORT_KEY_LEN 65535
#define MAX_SHORT_VAL_LEN 16777215
#define ZS_KEY_BASE_REC_SIZE 24
#define ZS_VAL_BASE_REC_SIZE 16
#define MAX_KEY_LEN 512
#define SECTOR_SIZE 512
enum record_type_t {
        REC_TYPE_UNUSED              =  0,
        REC_TYPE_KEY                 =  1,
        REC_TYPE_VALUE               =  2,
        REC_TYPE_COMMIT              =  4,
        REC_TYPE_2ND_HALF_COMMIT     =  8,
        REC_TYPE_FINAL               = 16,
        REC_TYPE_LONG                = 32,
        REC_TYPE_DELETED             = 64,
        REC_TYPE_LONG_KEY            = REC_TYPE_KEY | REC_TYPE_LONG,
        REC_TYPE_LONG_VALUE          = REC_TYPE_VALUE | REC_TYPE_LONG,
        REC_TYPE_LONG_COMMIT         = REC_TYPE_COMMIT | REC_TYPE_LONG,
        REC_TYPE_LONG_FINAL          = REC_TYPE_FINAL | REC_TYPE_LONG,
};
#define START_MARK  0x1234567890abcdef
typedef struct record{
    uint64_t watermark;
    int record_type;
    int record_len;
    size_t key_len;
    size_t data_len;
	  uint32_t data_block_id;
    size_t data_offset;
    char key[MAX_ROW_SIZE];
}record_t;

#define BITMAP_BLOCK_SIZE 128*1024
typedef struct _Bits {
	uint32_t total_blocks; // 100
	uint32_t free_blocks;
	uint32_t used_blocks;
	uint32_t num_of_bits;
	uint32_t num_of_bytes;
	pthread_mutex_t mutex;
	char bits[];
} Bits;
enum BlockType{
	STAR_BLOCK_TYPE_BITMAP = 1,
	STAR_BLOCK_TYPE_WAL,
	STAR_BLOCK_TYPE_DBINFO,
	STAR_BLOCK_TYPE_TBINFO,
	STAR_BLOCK_TYPE_DATA,
	STAR_BLOCK_TYPE_MAX
};
#define DEV_NAME "/dev/nvme0n1"
#define STAR_BITMAP_BLOCKID_START 0
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

#define MAX_TABLE_NAME_LEN 128
#define MAX_FIELD_NAME_LEN 32
#define MAX_DEVICE_COUNT 10
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
typedef struct DataBlockInfo {
	Block block;
	uint32_t row_count;
	uint32_t row_size;
	uint32_t data_size;
	int64_t max_time;
	int64_t min_time;
}DataBlockInfo;

typedef struct BitBlock {
	struct Block block;
	uint32_t bitLength;
	char data[];
} BitBlock;

typedef struct DataBlock {
	Block block;
	uint32_t row_count;
	uint32_t max_row_size;
	uint32_t header_size;
	int64_t max_time;
	int64_t min_time;
	uint32_t offset;
  char max_key[MAX_ROW_SIZE];
  char min_key[MAX_ROW_SIZE];
	char data[];
}DataBlock;

typedef Bits* BITS;

#define MAX_ROW_SIZE 1024
#define DATA_BLOCK_SIZE (128 * 1024)
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

#ifdef _ALPINE
  typedef int(*__compar_fn_t)(const void *, const void *);
  void  error (int, int, const char *);
  #ifndef PTHREAD_MUTEX_RECURSIVE_NP
    #define  PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE
  #endif
#endif

#ifndef _TD_ARM_32_
#define BUILDIN_CLZL(val) __builtin_clzl(val)
#define BUILDIN_CTZL(val) __builtin_ctzl(val)
#else
#define BUILDIN_CLZL(val) __builtin_clzll(val)
#define BUILDIN_CTZL(val) __builtin_ctzll(val)
#endif
#define BUILDIN_CLZ(val) __builtin_clz(val)
#define BUILDIN_CTZ(val) __builtin_ctz(val)

#undef threadlocal
#ifdef _ISOC11_SOURCE
  #define threadlocal _Thread_local
#elif defined(__APPLE__)
  #define threadlocal
#elif defined(__GNUC__) && !defined(threadlocal)
  #define threadlocal __thread
#else
  #define threadlocal
#endif

#ifdef __cplusplus
}
#endif

#endif
