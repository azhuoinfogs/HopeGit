#pragma once
#include <iostream>
#include <sstream>
#include "rocksdb/utilities/optimistic_transaction_db.h"
#include "rocksdb/utilities/transaction.h"
#include "rocksdb/db.h"
#include "rocksdb/env.h"
#include "metadata_utils.h"
using namespace rocksdb;
// 定义列族名称
const std::string kEntryMetaCFName = "entry_meta";
const std::string kEntryIDCFName = "entry_id";
const std::string kEntryPathCFName = "entry_path";
//根目录
static const uint64_t ROOT_INODE = 1;
static const std::string BOSS_META_PATH = "/opt/boss_meta";
// 定义 Metadata 结构体
// 假设 Metadata 结构体定义如下：
typedef struct Metadata {
    uint64_t parent_inode_id;
    uint64_t inode_id;
    std::string name;
    bool is_directory;
    size_t file_size;
    uint64_t atime; // 访问时间戳
    uint64_t mtime; // 修改时间戳
    uint32_t permissions; // 权限位
    uint32_t owner_id;   // 所有者 ID
    uint32_t group_id;   // 组 ID
    uint32_t nlink;     // 链接数
    uint32_t block_size; // 数据块大小
    uint64_t blocks;    // 占用的数据块总数
    std::unordered_map<std::string, std::string> xattrs; // 扩展属性
}metadata_t;

class BossMeta {
public:
    BossMeta(const std::string& db_path): //rnd_(0xdeadbeef),
      env_(rocksdb::Env::Default()) {
        // logger_ = std::make_shared<rocksdb::StderrLogger>(rocksdb::InfoLogLevel::DEBUG_LEVEL);
        // env_->NewLogger(db_path + "/rc_LOG",
        //                             &logger_);
        //                             db_ =tram.getbasedb();
        //初始化数据库
        initializeDB(db_path);
        //初始化根目录
        initializeRootDirectory(db_);
        next_ino = 2;
    }

    ~BossMeta() {
        for (auto cf : columnFamilyHandles) {
            delete cf;
        }
        delete db_;
    }
    static BossMeta& Instance()
    {
        static BossMeta meta_(BOSS_META_PATH);
        return meta_;
    }

    // 创建元数据记录
    uint64_t createEntry(uint64_t parentInodeId, const std::string& entryName, bool isDirectory, size_t fileSize = 0, mode_t permissions = 0644, uid_t owner_id = 0, gid_t group_id = 0);
    // list元数据
    void listEntry(uint64_t InodeId);
    // 删除元数据
    bool deleteEntry(uint64_t parentInodeID, const std::string& entryName);
    // 更新元数据
    bool updateEntryMeta(uint64_t parentInodeId, const std::string& entryName,
                               const metadata_t& updatedMetadata);
    //根据路径获取inodeID
    bool getInodeIDByPath(const std::string& path, uint64_t *inode_id);
    //根据inodeID 获取path
    bool getEntryNameByInode(uint64_t parent_inode_id, uint64_t inode_id, std::string* entryName);

    //根据inodeID 获取meta属性
    bool getMetadataByInode(uint64_t inode_id,  metadata_t* metadata);
    //根据inodeID 获取meta属性和entryname
    bool getMetadataWithNameByInode(uint64_t inode_id,  metadata_t* metadata, std::string* entryName);
    //根据路径path 获取meta属性
    bool getMetadataByPath(const std::string& path,  metadata_t* metadata);
    //根据路径创建文件
    bool createFile(const std::string& path, int flags, mode_t mode,  size_t fileSize = 0, mode_t permissions = 0644, uid_t owner_id = 0, gid_t group_id = 0);
    // 列出目录内容
    std::vector<std::pair<std::string, metadata_t>> ListDirectory(const std::string& path);
private:
    rocksdb::DB* db_;
    // rocksdb::Random64 rnd_;
    std::atomic<uint64_t> next_ino;
    rocksdb::Env *env_;
    std::mutex db_mutex;
    std::shared_ptr<rocksdb::Logger> logger_;

    std::vector<rocksdb::ColumnFamilyDescriptor> column_families_desc;
    std::vector<rocksdb::ColumnFamilyHandle*> columnFamilyHandles;
    rocksdb::ColumnFamilyHandle* entryMetaCF; // meta索引列族句柄
    rocksdb::ColumnFamilyHandle* entryIDCF; // id索引列族句柄
    rocksdb::ColumnFamilyHandle* entryPathCF; // path索引列族句柄， 暂时没用到
    rocksdb::ColumnFamilyHandle* defaultCF; // 默认索引列族句柄
    // 模拟生成唯一inode的方法
    uint64_t generateInode() {
      // return rnd_.Next();
      return  next_ino.fetch_add(1, std::memory_order_relaxed);;
    }
    //数据库相关
    //初始化数据库
    void initializeDB(const std::string& db_path);
    //创建相关列族
    void CreateDB(const std::string& db_path);

    Status  CommitBatch(rocksdb::WriteBatch* batch) {
      rocksdb::WriteOptions write_options;
      // 设置同步选项（根据需求）
      write_options.sync = true;  // 或者 false
      rocksdb::Status status = db_->Write(write_options, batch);
      if (!status.ok()) {
        // 处理错误...
        // ROCKS_LOG_ERROR(logger_, "Error writing to DB: %s", status.ToString().c_str());
      } else {
        // ROCKS_LOG_INFO(logger_, "Batch committed successfully.");
      }
      return status;
    }

    //元数据相关
    //初始化根目录
    void initializeRootDirectory(rocksdb::DB* db);
    // 从meta键中提取inode号
    std::string extractInodeFromMetaKey(const std::string& metaKey);

    // 递归删除目录下的所有子项
    void listAndDeleteSubEntries(const std::string& parentInode, const std::string& entryName, rocksdb::Transaction& txn);

};
