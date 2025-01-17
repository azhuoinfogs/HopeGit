#include "metadata_utils.h"

// 将C++对象转换为protobuf消息
boss_meta::Metadata toProtobuf(const Metadata& metadata) {
    boss_meta::Metadata protoMetadata;
    protoMetadata.set_parent_inode_id(metadata.parent_inode_id);
    protoMetadata.set_inode_id(metadata.inode_id);
    protoMetadata.set_name(metadata.name);
    protoMetadata.set_is_directory(metadata.is_directory);
    protoMetadata.set_file_size(metadata.file_size);
    protoMetadata.set_atime(metadata.atime);
    protoMetadata.set_mtime(metadata.mtime);
    protoMetadata.set_permissions(metadata.permissions);
    protoMetadata.set_owner_id(metadata.owner_id);
    protoMetadata.set_group_id(metadata.group_id);
    protoMetadata.set_nlink(metadata.nlink);
    protoMetadata.set_block_size(metadata.block_size);
    protoMetadata.set_blocks(metadata.blocks);

    for (const auto& [key, value] : metadata.xattrs) {
        (*protoMetadata.mutable_xattrs())[key] = value;
    }

    return protoMetadata;
}

// 将protobuf消息转换为C++对象
Metadata fromProtobuf(const boss_meta::Metadata& protoMetadata) {
    Metadata metadata;
    metadata.parent_inode_id = protoMetadata.parent_inode_id();
    metadata.inode_id = protoMetadata.inode_id();
    metadata.name = protoMetadata.name();
    metadata.is_directory = protoMetadata.is_directory();
    metadata.file_size = protoMetadata.file_size();
    metadata.atime = protoMetadata.atime();
    metadata.mtime = protoMetadata.mtime();
    metadata.permissions = protoMetadata.permissions();
    metadata.owner_id = protoMetadata.owner_id();
    metadata.group_id = protoMetadata.group_id();
    metadata.nlink = protoMetadata.nlink();
    metadata.block_size = protoMetadata.block_size();
    metadata.blocks = protoMetadata.blocks();

    for (const auto& [key, value] : protoMetadata.xattrs()) {
        metadata.xattrs[key] = value;
    }

    return metadata;
}

// 序列化元数据为字符串
std::string serializeMetadata(const boss_meta::Metadata& protoMetadata) {
    std::string serializedData;
    protoMetadata.SerializeToString(&serializedData);
    return serializedData;
}

// 反序列化字符串为元数据
bool deserializeMetadata(const std::string& data, Metadata* metadata) {
    boss_meta::Metadata protoMetadata;
    if (!protoMetadata.ParseFromString(data)) {
        return false;
    }
    *metadata = fromProtobuf(protoMetadata);
    return true;
}