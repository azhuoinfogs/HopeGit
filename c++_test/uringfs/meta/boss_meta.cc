#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <algorithm>
#include <string.h>
#include <rocksdb/db.h>
#include <string>
#include <vector>
#include <sys/types.h>
#include <ctime>
#include <stdexcept>
#include <functional>
#include "boss_util.h"
#include "boss_meta.h"
#include "log.h"
using namespace std;
using namespace boss_meta;

// kv store prefixes
const string PREFIX_SUPER = "S";       // field -> value
const string PREFIX_STAT = "T";        // field -> value(int64 array)
const string PREFIX_COLL = "C";        // collection name -> cnode_t
const string PREFIX_OBJ = "O";         // object name -> onode_t
const string PREFIX_OMAP = "M";        // u64 + keyname -> value
const string PREFIX_PGMETA_OMAP = "P"; // u64 + keyname -> value(for meta coll)
const string PREFIX_PERPOOL_OMAP = "m"; // s64 + u64 + keyname -> value
const string PREFIX_PERPG_OMAP = "p";   // u64(pool) + u32(hash) + u64(id) + keyname -> value
const string PREFIX_DEFERRED = "L";    // id -> deferred_transaction_t
const string PREFIX_ALLOC = "B";       // u64 offset -> u64 length (freelist)
const string PREFIX_ALLOC_BITMAP = "b";// (see BitmapFreelistManager)
const string PREFIX_SHARED_BLOB = "X"; // u64 SB id -> shared_blob_t

const string META_PREFIX = "m/"; // u64 SB id -> shared_blob_t
const string METAID_PREFIX = "i/"; // u64 SB id -> shared_blob_t
const string METAPATH_PREFIX = "p/"; // u64 SB id -> shared_blob_t

const string METADATA_OWNER = "o/"; // u64 SB id -> shared_blob_t

std::string makeMetaKey(uint64_t inode_id) {
    return META_PREFIX + std::to_string(inode_id);
}
std::string makeMetaKey(uint64_t parentInodeId, const std::string& entryName) {
    return META_PREFIX + std::to_string(parentInodeId) + "/" + entryName;
}
std::string makeMetaIDKey(uint64_t inode_id) {
    return METAID_PREFIX + std::to_string(inode_id);
}
std::string makeMetaPathKey(uint64_t inode_id) {
    return METAPATH_PREFIX + std::to_string(inode_id);
}
std::string extractEntryNameFromKey(const std::string& key) {
    size_t pos = key.find_last_of('/');
    return pos != std::string::npos ? key.substr(pos + 1) : "";
}
bool starts_with(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

// std::string makePermissionIndexKey(mode_t permissions, uid_t owner_id, gid_t group_id) {
//     return METADATA_PREMISSION + std::to_string(permissions) + ":" + std::to_string(owner_id) + ":" + std::to_string(group_id);
// }

// std::string makeOwnerIndexKey(uid_t owner_id, const std::string& inode) {
//     return METADATA_OWNER + std::to_string(owner_id) + ":" + inode;
// }
// 辅助方法：解析路径以获取所有目录名
std::vector<std::string> splitPath(const std::string& path) {
    std::istringstream iss(path);
    std::vector<std::string> parts;
    std::string part;
    while (getline(iss, part, '/')) {
        if (!part.empty()) {
            parts.push_back(part);
        }
    }
    return parts;
}
// 辅助方法：解析路径以获取父目录和文件名
void parsePath(const std::string& path, std::string* parentPath, std::string* entryName) {
    size_t last_slash_pos = path.find_last_of('/');
    if (last_slash_pos == std::string::npos || last_slash_pos == path.length() - 1) {
        *parentPath = "/";
        *entryName = path;
    } else {
        *parentPath = path.substr(0, last_slash_pos);
        *entryName = path.substr(last_slash_pos + 1);
    }
}
// // 添加索引条目
// void addIndexEntry(rocksdb::WriteBatch* batch,
//                    const std::string& path, const std::string& inode) {
//     // 正向索引: path -> inode
//     batch->Put(makeme(path), inode);

//     // 反向索引: inode -> path (可选)
//     batch->Put(makeInodeToPathKey(inode, path), path);
// }

// // 删除索引条目
// void removeIndexEntry(rocksdb::WriteBatch* batch, 
//                       const std::string& path, const std::string& inode) {
//     // 删除正向索引
//     batch->Delete(makePathToInodeKey(path));

//     // 删除反向索引 (如果存在)
//     batch->Delete(makeInodeToPathKey(inode, path));
// }

bool BossMeta::getMetadataByInode(uint64_t inode_id,  metadata_t* metadata) {
    std::string entryIDKey = makeMetaIDKey(inode_id);
    std::string value;
    rocksdb::Status status = db_->Get(rocksdb::ReadOptions(), entryIDCF, entryIDKey, &value);
    if (!status.ok()) {
        throw std::runtime_error("Failed to get metadata for inode: " + inode_id);
    }
    return deserializeMetadata(value, metadata);
}
bool BossMeta::getEntryNameByInode(uint64_t parent_inode_id, uint64_t inode_id, std::string* entryName) {
    bool ret = false;
    if (inode_id == ROOT_INODE) {
        *entryName = "/";
        return true;
    }
    rocksdb::Iterator* it = db_->NewIterator(rocksdb::ReadOptions(), entryMetaCF);
    std::string prefix = makeMetaKey(parent_inode_id, ""); // 构建前缀，不包括具体entryName
    for (it->Seek(prefix); it->Valid() && it->key().starts_with(prefix); it->Next()) {
        // 将字符串转换为 uint64_t
        uint64_t cInodeId = std::stoull(it->value().ToString());
        // 读取现有的元数据
        if (cInodeId == inode_id) {
            std::string gentryName = extractEntryNameFromKey(it->key().ToString());
            *entryName = gentryName;
            ret = true;
        }
    }
    delete it;
    return ret;
}
bool BossMeta::getMetadataWithNameByInode(uint64_t inode_id,  metadata_t* metadata, std::string* entryName) {
    bool ret = false;
    std::string value;
    std::string entryIDKey = makeMetaIDKey(inode_id);
    rocksdb::Status status = db_->Get(rocksdb::ReadOptions(), entryIDCF, entryIDKey, &value);
    if (!status.ok()) {
        LOG_CRITICAL("Failed to get metadata for inode:%ld entryIDKey:%s", inode_id, entryIDKey.c_str());
        return false;
    }
    ret = deserializeMetadata(value, metadata);
    if (!ret) {
        LOG_CRITICAL("Failed to deserializeMetadata for inode:%ld entryIDKey:%s", inode_id, entryIDKey.c_str());
        return false;
    }
    ret = getEntryNameByInode(metadata->parent_inode_id, inode_id, entryName);
    if (!ret) {
        LOG_CRITICAL("Failed to getEntryNameByInode for parent_inode_id:%ld inode:%ld entryIDKey:%s", metadata->parent_inode_id, inode_id, entryIDKey.c_str());
        return false;
    }
    LOG_INFO_P("getMetadataByInode inode_id:%ld entryName:%s", inode_id, entryName->c_str());
    return true;
}

bool BossMeta::getInodeIDByPath(const std::string& path, uint64_t *inode_id) {
    if (path == "/") {
        *inode_id = ROOT_INODE;
        return true; // 根目录的 inode_id
    }
    std::vector<std::string> parts = splitPath(path);
    uint64_t currentInode = ROOT_INODE;
    for (const auto& part : parts) {
        if (part.empty()) continue; // 忽略空的部分

        // 查找当前目录下的条目键
        std::string entryKey = makeMetaKey(currentInode, part);
        std::string nextInodeStr;

        rocksdb::Status status = db_->Get(rocksdb::ReadOptions(), entryMetaCF, entryKey, &nextInodeStr);
        if (!status.ok()) {
            return false;
        }
        // metadata_t metadata;
        uint64_t nextInode = std::stoull(nextInodeStr);
        // getMetadataByInode(nextInode, &metadata);
        // if (!metadata.is_directory) {
        //     // 如果找到的条目是一个文件，则返回该文件的 inode_id
        //     currentInode = nextInode;
        //     break;
        // }
        currentInode = nextInode;
    }
    *inode_id = currentInode;
    return true;
}

// 实现方法
uint64_t BossMeta::createEntry(uint64_t parentInodeId, const std::string& entryName, bool isDirectory, size_t fileSize /*= 0*/, mode_t permissions /*= 0644*/, uid_t owner_id /*= 0*/, gid_t group_id /*= 0*/) {
    try {
        WriteBatch batch;
        std::string entryMetaKey = makeMetaKey(parentInodeId, entryName);
        uint64_t inode_id = generateInode();
        std::string entryIDKey = makeMetaIDKey(inode_id);

        metadata_t metadata{parent_inode_id: parentInodeId, is_directory: isDirectory, file_size: fileSize,
                          atime:util::GetTimeStampMS(), mtime:util::GetTimeStampMS(), permissions: permissions,
                          owner_id: owner_id, group_id: group_id};

        boss_meta::Metadata protoMetadata = toProtobuf(metadata);
        // std::string inode_id = makeMetaDataKey(generateInode());//暂时用不到
        std::string inode_id_str = std::to_string(inode_id);
        if (!isDirectory) {
            // 如果是文件，则同时创建meta和id对象, 后面可能需要添加其他属性
            batch.Put(entryMetaCF, entryMetaKey, inode_id_str);
            batch.Put(entryIDCF, entryIDKey, protoMetadata.SerializeAsString());
        } else {
            // 如果是目录，则只创建meta对象
            batch.Put(entryMetaCF, entryMetaKey, inode_id_str);
            batch.Put(entryIDCF, entryIDKey, protoMetadata.SerializeAsString());
        }
        rocksdb::Status s = CommitBatch(&batch);
        if (!s.ok()) {
            throw std::runtime_error("Failed to commit transaction: " + s.ToString());
        }
        return inode_id;
    } catch (const std::exception& e) {
        std::cerr << "Error creating entry: " << e.what() << "\n";
        return false;
    }
}
bool BossMeta::deleteEntry(uint64_t parentInodeID, const std::string& entryName) {
    try {
        rocksdb::WriteBatch batch;
        // 构建键值对
        std::string entryMetaKey = makeMetaKey(parentInodeID, entryName);
        // 读取元数据以获取inode_id
        std::string inode_id_str;
        rocksdb::Status s = db_->Get(rocksdb::ReadOptions(), entryMetaCF, entryMetaKey, &inode_id_str);
        if (!s.ok() || inode_id_str.empty()) {
            throw std::runtime_error("Failed to get inode ID for entry: " + s.ToString());
        }
        uint64_t inode_id = std::stoull(inode_id_str);
        std::string entryIDKey = makeMetaIDKey(inode_id);

        // 添加删除操作到batch
        batch.Delete(entryMetaCF, entryMetaKey); // 删除entry_meta列族中的条目
        batch.Delete(entryIDCF, entryIDKey);   // 删除entry_id列族中的条目

        // 如果是目录，可能还需要删除与该目录相关的所有子项
        // 这里假设有一个方法可以列出并删除所有子项
        // listAndDeleteSubEntries(batch, inode_id);
        // 提交batch
        s = CommitBatch(&batch);
        if (!s.ok()) {
            throw std::runtime_error("Failed to commit transaction: " + s.ToString());
        }
        LOG_INFO_P("Entry deleted successfully: %s", entryName.c_str());
        return true;
    } catch (const std::exception& e) {
        LOG_CRITICAL("Error deleting entry: %s", e.what());
        return false;
    }
}
bool BossMeta::createFile(const std::string& path, int flags, mode_t mode,
                          size_t fileSize, mode_t permissions, uid_t owner_id, gid_t group_id) {
    try {
        rocksdb::WriteBatch batch;
        // 解析路径以获取父目录和文件名
        std::string parentPath, entryName;
        parsePath(path, &parentPath, &entryName); // 需要实现 parsePath 方法

        // 获取父目录的 inode_id
        uint64_t parentInodeId;
        if (!getInodeIDByPath(parentPath, &parentInodeId)) {
            throw std::runtime_error("Failed to get parent directory inode ID");
        }
        // 插入新的元数据到数据库
        if (!createEntry(parentInodeId, entryName, false, fileSize, permissions, owner_id, group_id)) {
            throw std::runtime_error("Failed to insert metadata for new file");
        }

        // 提交batch
        rocksdb::Status s = CommitBatch(&batch);
        if (!s.ok()) {
            throw std::runtime_error("Failed to commit transaction: " + s.ToString());
        }
        LOG_INFO_P("File created successfully: %s", path.c_str());
        return true;
    } catch (const std::exception& e) {
        LOG_CRITICAL("Error creating file: %s", e.what());
        return false;
    }
}
void BossMeta::listEntry(uint64_t inodeId) {
    rocksdb::Iterator* it = db_->NewIterator(rocksdb::ReadOptions(), entryMetaCF);
    std::string prefix = makeMetaKey(inodeId, ""); // 构建前缀，不包括具体entryName
    for (it->Seek(prefix); it->Valid() && it->key().starts_with(prefix); it->Next()) {
        std::string entryName = it->key().ToString().substr(prefix.length()); // 提取entryName部分
        std::cout << "Entry: " << entryName << "\n";
    }
    delete it;
}
bool BossMeta::updateEntryMeta(uint64_t parentInodeId, const std::string& entryName,
                               const metadata_t& updatedMetadata) {
    try {
        rocksdb::WriteBatch batch;
        // 构建键值对
        std::string entryMetaKey = makeMetaKey(parentInodeId, entryName);
        std::string inode_id_str;

        // 读取现有的 inode_id
        rocksdb::Status s = db_->Get(rocksdb::ReadOptions(), entryMetaCF, entryMetaKey, &inode_id_str);
        if (!s.ok() || inode_id_str.empty()) {
            throw std::runtime_error("Failed to get inode ID for entry: " + s.ToString());
        }

        // 将字符串转换为 uint64_t
        uint64_t nextInode = std::stoull(inode_id_str);
        std::string entryIDKey = makeMetaIDKey(nextInode);

        // 读取现有的元数据
        metadata_t currentMetadata;
        getMetadataByInode(nextInode, &currentMetadata);

        // 更新元数据
        currentMetadata.file_size = updatedMetadata.file_size;      // 更新文件大小
        currentMetadata.atime = updatedMetadata.atime;              // 更新访问时间
        currentMetadata.mtime = updatedMetadata.mtime;              // 更新修改时间
        currentMetadata.permissions = updatedMetadata.permissions;  // 更新权限
        currentMetadata.owner_id = updatedMetadata.owner_id;        // 更新所有者ID
        currentMetadata.group_id = updatedMetadata.group_id;        // 更新组ID

        // 转换为protobuf格式
        boss_meta::Metadata newProtoMetadata = toProtobuf(currentMetadata);

        // 添加更新操作到batch
        batch.Put(entryIDCF, entryIDKey, newProtoMetadata.SerializeAsString());

        // 提交batch
        s = CommitBatch(&batch);
        if (!s.ok()) {
            throw std::runtime_error("Failed to commit transaction: " + s.ToString());
        }

        LOG_INFO_P("Entry metadata updated successfully: %s", entryName.c_str());
        return true;
    } catch (const std::exception& e) {
        LOG_CRITICAL("Error updating entry metadata: %s", e.what());
        return false;
    }
}
bool BossMeta::getMetadataByPath(const std::string& path, metadata_t* metadata) {
    try {
        uint64_t inodeId;
        // 根据路径获取 inode_id
        if (!getInodeIDByPath(path, &inodeId)) {
            // throw std::runtime_error("Failed to get inode ID by path");
            return false;
        }

        // 使用 inode_id 获取元数据
        if (!getMetadataByInode(inodeId, metadata)) {
            return false;
            // throw std::runtime_error("Failed to get metadata by inode ID");
        }
        return true;
    } catch (const std::exception& e) {
        LOG_CRITICAL("Error getting metadata by path: %s", e.what());
        return false;
    }
}

std::vector<std::pair<std::string, metadata_t>> BossMeta::ListDirectory(const std::string& path) {
    std::vector<std::pair<std::string, metadata_t>> entries;
    uint64_t pInodeId;
    // 根据路径获取 inode_id
    if (!getInodeIDByPath(path, &pInodeId)) {
        LOG_CRITICAL("Failed to get inode ID by path");
        return entries;
    }
    rocksdb::Iterator* it = db_->NewIterator(rocksdb::ReadOptions(), entryMetaCF);
    std::string prefix = makeMetaKey(pInodeId, ""); // 构建前缀，不包括具体entryName
    for (it->Seek(prefix); it->Valid() && it->key().starts_with(prefix); it->Next()) {
        std::string entryName = extractEntryNameFromKey(it->key().ToString());
        // 将字符串转换为 uint64_t
        uint64_t cInodeId = std::stoull(it->value().ToString());
        // 读取现有的元数据
        metadata_t currentMetadata;
        if (!getMetadataByInode(cInodeId, &currentMetadata)) {
            LOG_CRITICAL("Failed to get metadata by inode ID");
            continue;
        }
        entries.emplace_back(entryName, currentMetadata);
    }
    delete it;
    return entries;
}
void BossMeta::initializeRootDirectory(rocksdb::DB* db) {
    std::string entryMetaKey = makeMetaKey(ROOT_INODE);
    std::string entryIDKey   = makeMetaIDKey(ROOT_INODE);

    std::string value;
    rocksdb::Status status = db->Get(rocksdb::ReadOptions(), entryMetaCF, entryMetaKey, &value);

    if (!status.ok() || value.empty()) {
        // 根目录不存在，创建新的根目录
        metadata_t rootMetadata{.parent_inode_id = ROOT_INODE,  // 根目录没有父目录
                              .inode_id = ROOT_INODE,
                              .name = "/",
                              .is_directory = true,
                              .file_size = 0,
                              .atime = util::GetTimeStampMS(),
                              .mtime = util::GetTimeStampMS(),
                              .permissions = 0755,  // 默认权限
                              .owner_id = getuid(),
                              .group_id = getgid(),
                              .nlink = 2,  // 包括 "." 和 ".." 链接
                              .block_size = 4096,
                              .blocks = 0};

        // 插入根目录的元数据
        boss_meta::Metadata protoMetadata = toProtobuf(rootMetadata);
        db->Put(rocksdb::WriteOptions(), entryMetaCF, entryMetaKey, std::to_string(ROOT_INODE));
        db->Put(rocksdb::WriteOptions(), entryIDCF, entryIDKey, serializeMetadata(protoMetadata));
        // db->Put(rocksdb::WriteOptions(), entryPathCF, entryPathKey, ROOT_INODE);
        LOG_INFO_P("Initialized root directory with inode: !!!");
    } else {
        LOG_INFO_P("Root directory already exists.");
    }
}

void BossMeta::CreateDB(const std::string& db_path) {
    rocksdb::Options options;
    rocksdb::ColumnFamilyOptions cf_options;
    options.create_if_missing = true;
    // 如果是数据库不存在错误，则尝试创建数据库
    rocksdb::Status status = rocksdb::DB::Open(options, db_path, &db_);
    if (status.ok()) {
        LOG_INFO_P("open database successfully.");
    } else {
        LOG_CRITICAL("Failed to repair database: %s", status.ToString().c_str());
        return;
    }
    // 创建列族
    rocksdb::Status create_status = db_->CreateColumnFamilies(column_families_desc, &columnFamilyHandles);
    if (!create_status.ok()) {
        LOG_CRITICAL("Failed to create missing column families: %s",
                    create_status.ToString().c_str());
        return;
    }
    for (auto* cf : columnFamilyHandles) {
        if (cf->GetName() == kEntryMetaCFName) {
            entryMetaCF = cf;
        } else if (cf->GetName() == kEntryIDCFName) {
            entryIDCF = cf;
        } else if (cf->GetName() == kEntryPathCFName) {
            entryPathCF = cf;
        } else if (cf->GetName() == rocksdb::kDefaultColumnFamilyName) {
            defaultCF = cf;
        } else {
            LOG_CRITICAL("Unknown column family: %s", cf->GetName().c_str());
        }
    }
    return;
}
void BossMeta::initializeDB(const std::string& db_path) {
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::ColumnFamilyOptions cf_options;
    std::vector<std::string> required_cfs_name = {kEntryMetaCFName, kEntryIDCFName,
                                           kEntryPathCFName};
    //存在defaultcf
    // 添加现有的列族到描述符中
    for (const auto& cf_name : required_cfs_name) {
        if (cf_name == kEntryMetaCFName || cf_name == kEntryIDCFName || cf_name == kEntryPathCFName) {
            column_families_desc.emplace_back(cf_name, cf_options);
        }
    }
    // 尝试打开现有的数据库
    std::vector<std::string> existing_cfs;
    rocksdb::Status status = rocksdb::DB::ListColumnFamilies(options, db_path, &existing_cfs);
    if (!status.ok()) {
        if (status.IsPathNotFound()) {
            return CreateDB(db_path);
        }else {
            LOG_CRITICAL("Failed to list column families: %s", status.ToString().c_str());
            return;
        }
    }
    column_families_desc.emplace_back(rocksdb::kDefaultColumnFamilyName, cf_options);
    // 打开或创建数据库及列族
    status = rocksdb::DB::Open(options, db_path, column_families_desc, &columnFamilyHandles, &db_);
    if (!status.ok()) {
        LOG_CRITICAL("Failed to open/create DB with column families: %s", status.ToString().c_str());
        return;
    }
    for (auto* cf : columnFamilyHandles) {
        if (cf->GetName() == kEntryMetaCFName) {
            entryMetaCF = cf;
        } else if (cf->GetName() == kEntryIDCFName) {
            entryIDCF = cf;
        } else if (cf->GetName() == kEntryPathCFName) {
            entryPathCF = cf;
        } else if (cf->GetName() == rocksdb::kDefaultColumnFamilyName) {
            defaultCF = cf;
        } else {
            LOG_CRITICAL("Unknown column family: %s", cf->GetName().c_str());
        }
    }
    LOG_INFO_P("Database initialized successfully with column families.");
}