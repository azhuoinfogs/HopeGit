
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
     __u64 watermark;
    int record_type;
    int record_len;
    size_t data_len;
    size_t val_len;
    size_t val_offset;
    char data[];
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

#define STAR_DBINFO_BLOCKID_START 4220
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
	uint32_t table_id;
	uint32_t db_id;
	uint32_t row_count;
	uint32_t max_row_size;
	uint32_t header_size;
	int64_t max_time;
	int64_t min_time;
	uint32_t offset;
	char data[];
}DataBlock;

#define MAX_ROW_SIZE 1024

typedef Bits* BITS;